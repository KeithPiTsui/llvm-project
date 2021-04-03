//===-- LCCMCTargetDesc.cpp - LCC Target Descriptions -------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file provides LCC specific target descriptions.
//
//===----------------------------------------------------------------------===//

#include "LCCMCTargetDesc.h"
#include "LCCInstPrinter.h"
#include "LCCInstrInfo.h"
#include "LCCMCAsmInfo.h"
#include "llvm/MC/MCDwarf.h"
#include "llvm/MC/MCELFStreamer.h"
#include "llvm/MC/MCInstPrinter.h"
#include "llvm/MC/MCInstrAnalysis.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/MC/MachineLocation.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/FormattedStream.h"
#include "llvm/Support/TargetRegistry.h"

using namespace llvm;

#define GET_INSTRINFO_MC_DESC
#include "LCCGenInstrInfo.inc"

#define GET_SUBTARGETINFO_MC_DESC
#include "LCCGenSubtargetInfo.inc"

#define GET_REGINFO_MC_DESC
#include "LCCGenRegisterInfo.inc"
//@1 {
/// Select the LCC Architecture Feature for the given triple and cpu name.
/// The function will be called at command 'llvm-objdump -d' for LCC elf input.
static std::string selectLCCArchFeature(const Triple &TT, StringRef CPU) {
  std::string LCCArchFeature;
  if (CPU.empty() || CPU == "generic") {
    if (TT.getArch() == Triple::LCC) {
      if (CPU.empty() || CPU == "LCC32II") {
        LCCArchFeature = "+LCC32II";
      } else {
        if (CPU == "LCC32I") {
          LCCArchFeature = "+LCC32I";
        }
      }
    }
  }
  return LCCArchFeature;
}
//@1 }

static MCInstrInfo *createLCCMCInstrInfo() {
  MCInstrInfo *X = new MCInstrInfo();
  InitLCCMCInstrInfo(X); // defined in LCCGenInstrInfo.inc
  return X;
}

static MCRegisterInfo *createLCCMCRegisterInfo(const Triple &TT) {
  MCRegisterInfo *X = new MCRegisterInfo();
  // fixme
  InitLCCMCRegisterInfo(X, LCC::R0); // defined in LCCGenRegisterInfo.inc
  return X;
}

static MCSubtargetInfo *createLCCMCSubtargetInfo(const Triple &TT,
                                                 StringRef CPU, StringRef FS) {
  std::string ArchFS = selectLCCArchFeature(TT, CPU);
  if (!FS.empty()) {
    if (!ArchFS.empty())
      ArchFS = ArchFS + "," + FS.str();
    else
      ArchFS = FS.str();
  }
  return createLCCMCSubtargetInfoImpl(TT, CPU, ArchFS);
  // createLCCMCSubtargetInfoImpl defined in LCCGenSubtargetInfo.inc
}

static MCAsmInfo *createLCCMCAsmInfo(const MCRegisterInfo &MRI,
                                     const Triple &TT,
                                     const MCTargetOptions &Options) {
  MCAsmInfo *MAI = new LCCMCAsmInfo(TT);
  unsigned SP = MRI.getDwarfRegNum(LCC::SP, true);
  MCCFIInstruction Inst = MCCFIInstruction::createDefCfaRegister(nullptr, SP);
  MAI->addInitialFrameState(Inst);
  return MAI;
}

static MCInstPrinter *createLCCMCInstPrinter(const Triple &T,
                                             unsigned SyntaxVariant,
                                             const MCAsmInfo &MAI,
                                             const MCInstrInfo &MII,
                                             const MCRegisterInfo &MRI) {
  return new LCCInstPrinter(MAI, MII, MRI);
}

namespace {

class LCCMCInstrAnalysis : public MCInstrAnalysis {
public:
  LCCMCInstrAnalysis(const MCInstrInfo *Info) : MCInstrAnalysis(Info) {}
};
} // namespace

static MCInstrAnalysis *createLCCMCInstrAnalysis(const MCInstrInfo *Info) {
  return new LCCMCInstrAnalysis(Info);
}

//@2 {
extern "C" void LLVMInitializeLCCTargetMC() {
  for (Target *T : {&TheLCCTarget}) {
    // Register the MC asm info.
    TargetRegistry::RegisterMCAsmInfo(*T, createLCCMCAsmInfo);
    // Register the MC instruction info.
    TargetRegistry::RegisterMCInstrInfo(*T, createLCCMCInstrInfo);
    // Register the MC register info.
    TargetRegistry::RegisterMCRegInfo(*T, createLCCMCRegisterInfo);
    // Register the MC subtarget info.
    TargetRegistry::RegisterMCSubtargetInfo(*T, createLCCMCSubtargetInfo);
    // Register the MC instruction analyzer.
    TargetRegistry::RegisterMCInstrAnalysis(*T, createLCCMCInstrAnalysis);
    // Register the MCInstPrinter.
    TargetRegistry::RegisterMCInstPrinter(*T, createLCCMCInstPrinter);
  }
}
//@2 }
