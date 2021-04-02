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
#include "LCCSubtarget.h"
#include "LCCTargetObjectFile.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/CodeGen/TargetPassConfig.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Support/TargetRegistry.h"
using namespace llvm;

#define DEBUG_TYPE "LCC"

extern "C" void LLVMInitializeLCCTarget() {
  RegisterTargetMachine<LCCebTargetMachine> X(TheLCCTarget);
  RegisterTargetMachine<LCCelTargetMachine> Y(TheLCCelTarget);
}

static std::string computeDataLayout(const Triple &TT, StringRef CPU,
                                     const TargetOptions &Options,
                                     bool isLittle) {
  std::string Ret = "";
  // There are both little and big endian LCC.
  if (isLittle)
    Ret += "e";
  else
    Ret += "E";

  Ret += "-m:m";

  // Pointers are 32 bit on some ABIs.
  Ret += "-p:32:32";

  // 8 and 16 bit integers only need to have natural alignment, but try to
  // align them to 32 bits. 64 bit integers have natural alignment.
  Ret += "-i8:8:32-i16:16:32-i64:64";

  // 32 bit registers are always available and the stack is at least 64 bit
  // aligned.
  Ret += "-n32-S64";

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
                                   CodeGenOpt::Level OL, bool JIT,
                                   bool isLittle)
    : LLVMTargetMachine(T, computeDataLayout(TT, CPU, Options, isLittle), TT,
                        CPU, FS, Options, getEffectiveRelocModel(JIT, RM),
                        getEffectiveCodeModel(CM, CodeModel::Small), OL),
      isLittle(isLittle), TLOF(std::make_unique<LCCTargetObjectFile>()),
      ABI(LCCABIInfo::computeTargetABI()),
      DefaultSubtarget(TT, CPU.str(), FS.str(), isLittle, *this) {
  initAsmInfo();
}

LCCTargetMachine::~LCCTargetMachine() {}

void LCCebTargetMachine::anchor() {}

LCCebTargetMachine::LCCebTargetMachine(const Target &T, const Triple &TT,
                                       StringRef CPU, StringRef FS,
                                       const TargetOptions &Options,
                                       Optional<Reloc::Model> RM,
                                       Optional<CodeModel::Model> CM,
                                       CodeGenOpt::Level OL, bool JIT)
    : LCCTargetMachine(T, TT, CPU, FS, Options, RM, CM, OL, JIT, false) {}

void LCCelTargetMachine::anchor() {}

LCCelTargetMachine::LCCelTargetMachine(const Target &T, const Triple &TT,
                                       StringRef CPU, StringRef FS,
                                       const TargetOptions &Options,
                                       Optional<Reloc::Model> RM,
                                       Optional<CodeModel::Model> CM,
                                       CodeGenOpt::Level OL, bool JIT)
    : LCCTargetMachine(T, TT, CPU, FS, Options, RM, CM, OL, JIT, true) {}

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
};
} // namespace

TargetPassConfig *LCCTargetMachine::createPassConfig(PassManagerBase &PM) {
  return new LCCPassConfig(this, PM);
}
