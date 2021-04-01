//===-- LCCTargetMachine.cpp - Define TargetMachine for LCC -------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// Implements the info about LCC target spec.
//
//===----------------------------------------------------------------------===//

#include "LCCTargetMachine.h"
#include "LCC.h"
#include "LCCSEISelDAGToDAG.h"
#include "LCCSubtarget.h"
#include "LCCTargetObjectFile.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/CodeGen/TargetPassConfig.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Support/TargetRegistry.h"
using namespace llvm;
#define DEBUG_TYPE "LCC"

extern "C" void LLVMInitializeLCCTarget() {
  RegisterTargetMachine<LCCTargetMachine> X(TheLCCTarget);
}

static std::string computeDataLayout(const Triple &TT, StringRef CPU,
                                     const TargetOptions &Options,
                                     bool isLittle) {
  std::string Ret = "";
  // Little endian
  Ret += "e";
  // Mangling Mode is ELF
  Ret += "-m:e";
  // Pointers are 16 bit on some ABIs.
  Ret += "-p16:16:16";
  // Integer and float point size
  Ret += "-i8:16:16-i16:16:16-i32:16-i64:16-f32:16-f64:16";
  // Aggregate size
  Ret += "-a:16";
  // Native Integer Types and Stack Natural Aligment
  Ret += "-n16-S16";
  return Ret;
}

static Reloc::Model getEffectiveRelocModel(bool JIT,
                                           Optional<Reloc::Model> RM) {
  if (!RM.hasValue() || JIT)
    return Reloc::Static;
  return *RM;
}

LCCTargetMachine::LCCTargetMachine(const Target &T, const Triple &TT,
                                   StringRef CPU, StringRef FS,
                                   const TargetOptions &Options,
                                   Optional<Reloc::Model> RM,
                                   Optional<CodeModel::Model> CM,
                                   CodeGenOpt::Level OL, bool JIT)
    : LLVMTargetMachine(T, computeDataLayout(TT, CPU, Options, true), TT, CPU,
                        FS, Options, getEffectiveRelocModel(JIT, RM),
                        getEffectiveCodeModel(CM, CodeModel::Small), OL),
      isLittle(true), TLOF(std::make_unique<LCCTargetObjectFile>()),
      ABI(LCCABIInfo::computeTargetABI()),
      DefaultSubtarget(TT, CPU.str(), FS.str(), true, *this) {
  initAsmInfo();
}

LCCTargetMachine::~LCCTargetMachine() {}

const LCCSubtarget *
LCCTargetMachine::getSubtargetImpl(const Function &F) const {
  Attribute CPUAttr = F.getFnAttribute("target-cpu");
  Attribute FSAttr = F.getFnAttribute("target-features");

  std::string CPU = !CPUAttr.hasAttribute(Attribute::None)
                        ? CPUAttr.getValueAsString().str()
                        : TargetCPU;
  std::string FS = !FSAttr.hasAttribute(Attribute::None)
                       ? FSAttr.getValueAsString().str()
                       : TargetFS;

  auto &I = SubtargetMap[CPU + FS];
  if (!I) {
    // This needs to be done before we create a new subtarget since any
    // creation will depend on the TM and the code generation flags on the
    // function that reside in TargetOptions.
    resetTargetOptions(F);
    I = std::make_unique<LCCSubtarget>(TargetTriple, CPU, FS, isLittle, *this);
  }
  return I.get();
}

namespace {
class LCCPassConfig : public TargetPassConfig {
public:
  LCCPassConfig(LCCTargetMachine *TM, PassManagerBase &PM)
      : TargetPassConfig(*TM, PM) {}
  LCCTargetMachine &getLCCTargetMachine() const {
    return getTM<LCCTargetMachine>();
  }
  const LCCSubtarget &getLCCSubtarget() const {
    return *getLCCTargetMachine().getSubtargetImpl();
  }
  bool addInstSelector() override;
};

// Install an instruction selector pass using
// the ISelDag to gen LCC code.
bool LCCPassConfig::addInstSelector() {
  addPass(createLCCSEISelDag(getLCCTargetMachine(), getOptLevel()));
  return false;
}

} // namespace

TargetPassConfig *LCCTargetMachine::createPassConfig(PassManagerBase &PM) {
  return new LCCPassConfig(this, PM);
}
