//===-- LLVMTargetMachine.cpp - Implement the LLVMTargetMachine class -----===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements the LLVMTargetMachine class.
//
//===----------------------------------------------------------------------===//

#include "llvm/Analysis/Passes.h"
#include "llvm/CodeGen/AsmPrinter.h"
#include "llvm/CodeGen/BasicTTIImpl.h"
#include "llvm/CodeGen/MachineModuleInfo.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/CodeGen/TargetPassConfig.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/MC/MCAsmBackend.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCCodeEmitter.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCObjectWriter.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/FormattedStream.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Target/TargetLoweringObjectFile.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"
using namespace llvm;

static cl::opt<bool>
    EnableTrapUnreachable("trap-unreachable", cl::Hidden, cl::ZeroOrMore,
                          cl::init(false),
                          cl::desc("Enable generating trap for unreachable"));

void LLVMTargetMachine::initAsmInfo() {
  MRI.reset(TheTarget.createMCRegInfo(getTargetTriple().str()));
  MII.reset(TheTarget.createMCInstrInfo());
  // FIXME: Having an MCSubtargetInfo on the target machine is a hack due
  // to some backends having subtarget feature dependent module level
  // code generation. This is similar to the hack in the AsmPrinter for
  // module level assembly etc.
  STI.reset(TheTarget.createMCSubtargetInfo(
      getTargetTriple().str(), getTargetCPU(), getTargetFeatureString()));

  MCAsmInfo *TmpAsmInfo = TheTarget.createMCAsmInfo(
      *MRI, getTargetTriple().str(), Options.MCOptions);
  // TargetSelect.h moved to a different directory between LLVM 2.9 and 3.0,
  // and if the old one gets included then MCAsmInfo will be NULL and
  // we'll crash later.
  // Provide the user with a useful error message about what's wrong.
  assert(TmpAsmInfo && "MCAsmInfo not initialized. "
                       "Make sure you include the correct TargetSelect.h"
                       "and that InitializeAllTargetMCs() is being invoked!");

  if (Options.DisableIntegratedAS)
    TmpAsmInfo->setUseIntegratedAssembler(false);

  TmpAsmInfo->setPreserveAsmComments(Options.MCOptions.PreserveAsmComments);

  TmpAsmInfo->setCompressDebugSections(Options.CompressDebugSections);

  TmpAsmInfo->setRelaxELFRelocations(Options.RelaxELFRelocations);

  if (Options.ExceptionModel != ExceptionHandling::None)
    TmpAsmInfo->setExceptionsType(Options.ExceptionModel);

  AsmInfo.reset(TmpAsmInfo);
}

LLVMTargetMachine::LLVMTargetMachine(const Target &T,
                                     StringRef DataLayoutString,
                                     const Triple &TT, StringRef CPU,
                                     StringRef FS, const TargetOptions &Options,
                                     Reloc::Model RM, CodeModel::Model CM,
                                     CodeGenOpt::Level OL)
    : TargetMachine(T, DataLayoutString, TT, CPU, FS, Options) {
  this->RM = RM;
  this->CMModel = CM;
  this->OptLevel = OL;

  if (EnableTrapUnreachable)
    this->Options.TrapUnreachable = true;
}

TargetTransformInfo
LLVMTargetMachine::getTargetTransformInfo(const Function &F) {
  return TargetTransformInfo(BasicTTIImpl(this, F));
}

/// addPassesToX helper drives creation and initialization of TargetPassConfig.
static TargetPassConfig *
addPassesToGenerateCode(LLVMTargetMachine &TM, PassManagerBase &PM,
                        bool DisableVerify,
                        MachineModuleInfoWrapperPass &MMIWP) {
  // Targets may override createPassConfig to provide a target-specific
  // subclass.
  TargetPassConfig *PassConfig = TM.createPassConfig(PM);
  // Set PassConfig options provided by TargetMachine.
  PassConfig->setDisableVerify(DisableVerify);
  PM.add(PassConfig);
  PM.add(&MMIWP);

  if (PassConfig->addISelPasses())
    return nullptr;
  PassConfig->addMachinePasses();
  PassConfig->setInitialized();
  return PassConfig;
}

bool LLVMTargetMachine::addAsmPrinter(PassManagerBase &PM,
                                      raw_pwrite_stream &Out,
                                      raw_pwrite_stream *DwoOut,
                                      CodeGenFileType FileType,
                                      MCContext &Context) {
  if (Options.MCOptions.MCSaveTempLabels)
    Context.setAllowTemporaryLabels(false);

  const MCSubtargetInfo &STI = *getMCSubtargetInfo();
  const MCAsmInfo &MAI = *getMCAsmInfo();
  const MCRegisterInfo &MRI = *getMCRegisterInfo();
  const MCInstrInfo &MII = *getMCInstrInfo();

  std::unique_ptr<MCStreamer> AsmStreamer;

  switch (FileType) {
  case CGFT_AssemblyFile: {
    MCInstPrinter *InstPrinter = getTarget().createMCInstPrinter(
        getTargetTriple(), MAI.getAssemblerDialect(), MAI, MII, MRI);

    // Create a code emitter if asked to show the encoding.
    std::unique_ptr<MCCodeEmitter> MCE;
    if (Options.MCOptions.ShowMCEncoding)
      MCE.reset(getTarget().createMCCodeEmitter(MII, MRI, Context));

    std::unique_ptr<MCAsmBackend> MAB(
        getTarget().createMCAsmBackend(STI, MRI, Options.MCOptions));
    auto FOut = std::make_unique<formatted_raw_ostream>(Out);
    MCStreamer *S = getTarget().createAsmStreamer(
        Context, std::move(FOut), Options.MCOptions.AsmVerbose,
        Options.MCOptions.MCUseDwarfDirectory, InstPrinter, std::move(MCE),
        std::move(MAB), Options.MCOptions.ShowMCInst);
    AsmStreamer.reset(S);
    break;
  }
  case CGFT_ObjectFile: {
    // Create the code emitter for the target if it exists.  If not, .o file
    // emission fails.
    MCCodeEmitter *MCE = getTarget().createMCCodeEmitter(MII, MRI, Context);
    MCAsmBackend *MAB =
        getTarget().createMCAsmBackend(STI, MRI, Options.MCOptions);
    if (!MCE || !MAB)
      return true;

    Triple T(getTargetTriple().str());
    AsmStreamer.reset(getTarget().createMCObjectStreamer(
        T, Context, std::unique_ptr<MCAsmBackend>(MAB),
        DwoOut ? MAB->createDwoObjectWriter(Out, *DwoOut)
               : MAB->createObjectWriter(Out),
        std::unique_ptr<MCCodeEmitter>(MCE), STI, Options.MCOptions.MCRelaxAll,
        Options.MCOptions.MCIncrementalLinkerCompatible,
        /*DWARFMustBeAtTheEnd*/ true));
    break;
  }
  case CGFT_Null:
    // The Null output is intended for use for performance analysis and testing,
    // not real users.
    AsmStreamer.reset(getTarget().createNullStreamer(Context));
    break;
  }

  // Create the AsmPrinter, which takes ownership of AsmStreamer if successful.
  FunctionPass *Printer =
      getTarget().createAsmPrinter(*this, std::move(AsmStreamer));
  if (!Printer)
    return true;

  PM.add(Printer);
  return false;
}

bool LLVMTargetMachine::addPassesToEmitFile(
    PassManagerBase &PM, raw_pwrite_stream &Out, raw_pwrite_stream *DwoOut,
    CodeGenFileType FileType, bool DisableVerify,
    MachineModuleInfoWrapperPass *MMIWP) {
  // Add common CodeGen passes.
  if (!MMIWP)
    MMIWP = new MachineModuleInfoWrapperPass(this);
  TargetPassConfig *PassConfig =
      addPassesToGenerateCode(*this, PM, DisableVerify, *MMIWP);
  if (!PassConfig)
    return true;

  if (!TargetPassConfig::willCompleteCodeGenPipeline()) {
    if (this->getTargetTriple().isOSAIX()) {
      // On AIX, we might manifest MCSymbols during SDAG lowering. For MIR
      // testing to be meaningful, we need to ensure that the symbols created
      // are MCSymbolXCOFF variants, which requires that
      // the TargetLoweringObjectFile instance has been initialized.
      MCContext &Ctx = MMIWP->getMMI().getContext();
      const_cast<TargetLoweringObjectFile &>(*this->getObjFileLowering())
          .Initialize(Ctx, *this);
    }
    PM.add(createPrintMIRPass(Out));
  } else if (addAsmPrinter(PM, Out, DwoOut, FileType,
                           MMIWP->getMMI().getContext()))
    return true;

  PM.add(createFreeMachineFunctionPass());
  return false;
}

/// addPassesToEmitMC - Add passes to the specified pass manager to get
/// machine code emitted with the MCJIT. This method returns true if machine
/// code is not supported. It fills the MCContext Ctx pointer which can be
/// used to build custom MCStreamer.
///
bool LLVMTargetMachine::addPassesToEmitMC(PassManagerBase &PM, MCContext *&Ctx,
                                          raw_pwrite_stream &Out,
                                          bool DisableVerify) {
  // Add common CodeGen passes.
  MachineModuleInfoWrapperPass *MMIWP = new MachineModuleInfoWrapperPass(this);
  TargetPassConfig *PassConfig =
      addPassesToGenerateCode(*this, PM, DisableVerify, *MMIWP);
  if (!PassConfig)
    return true;
  assert(TargetPassConfig::willCompleteCodeGenPipeline() &&
         "Cannot emit MC with limited codegen pipeline");

  Ctx = &MMIWP->getMMI().getContext();
  if (Options.MCOptions.MCSaveTempLabels)
    Ctx->setAllowTemporaryLabels(false);

  // Create the code emitter for the target if it exists.  If not, .o file
  // emission fails.
  const MCSubtargetInfo &STI = *getMCSubtargetInfo();
  const MCRegisterInfo &MRI = *getMCRegisterInfo();
  MCCodeEmitter *MCE =
      getTarget().createMCCodeEmitter(*getMCInstrInfo(), MRI, *Ctx);
  MCAsmBackend *MAB =
      getTarget().createMCAsmBackend(STI, MRI, Options.MCOptions);
  if (!MCE || !MAB)
    return true;

  const Triple &T = getTargetTriple();
  std::unique_ptr<MCStreamer> AsmStreamer(getTarget().createMCObjectStreamer(
      T, *Ctx, std::unique_ptr<MCAsmBackend>(MAB), MAB->createObjectWriter(Out),
      std::unique_ptr<MCCodeEmitter>(MCE), STI, Options.MCOptions.MCRelaxAll,
      Options.MCOptions.MCIncrementalLinkerCompatible,
      /*DWARFMustBeAtTheEnd*/ true));

  // Create the AsmPrinter, which takes ownership of AsmStreamer if successful.
  FunctionPass *Printer =
      getTarget().createAsmPrinter(*this, std::move(AsmStreamer));
  if (!Printer)
    return true;

  PM.add(Printer);
  PM.add(createFreeMachineFunctionPass());

  return false; // success!
}
