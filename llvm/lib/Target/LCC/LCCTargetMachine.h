//===-- LCCTargetMachine.h - Define TargetMachine for LCC -----*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file declares the LCC specific subclass of TargetMachine.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_LCC_LCCTARGETMACHINE_H
#define LLVM_LIB_TARGET_LCC_LCCTARGETMACHINE_H



#include "LCCSubtarget.h"
#include "MCTargetDesc/LCCABIInfo.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/CodeGen/SelectionDAGISel.h"
#include "llvm/CodeGen/TargetFrameLowering.h"
#include "llvm/Target/TargetMachine.h"

namespace llvm {
class formatted_raw_ostream;
class LCCRegisterInfo;

class LCCTargetMachine : public LLVMTargetMachine {
  bool isLittle;
  std::unique_ptr<TargetLoweringObjectFile> TLOF;
  LCCABIInfo ABI;
  LCCSubtarget DefaultSubtarget;
  mutable StringMap<std::unique_ptr<LCCSubtarget>> SubtargetMap;

public:
  LCCTargetMachine(const Target &T, const Triple &TT, StringRef CPU,
                    StringRef FS, const TargetOptions &Options,
                    Optional<Reloc::Model> RM, Optional<CodeModel::Model> CM,
                    CodeGenOpt::Level OL, bool JIT, bool isLittle);
  ~LCCTargetMachine() override;

  const LCCSubtarget *getSubtargetImpl() const { return &DefaultSubtarget; }
  const LCCSubtarget *getSubtargetImpl(const Function &F) const override;
  // Pass Pipeline Configuration
  TargetPassConfig *createPassConfig(PassManagerBase &PM) override;

  TargetLoweringObjectFile *getObjFileLowering() const override {
    return TLOF.get();
  }
  bool isLittleEndian() const { return isLittle; }
  const LCCABIInfo &getABI() const { return ABI; }
};

/// LCCebTargetMachine - LCC32 big endian target machine.
///
class LCCebTargetMachine : public LCCTargetMachine {
  virtual void anchor();
public:
  LCCebTargetMachine(const Target &T, const Triple &TT, StringRef CPU,
                      StringRef FS, const TargetOptions &Options,
                      Optional<Reloc::Model> RM, Optional<CodeModel::Model> CM,
                      CodeGenOpt::Level OL, bool JIT);
};

/// LCCelTargetMachine - LCC32 little endian target machine.
///
class LCCelTargetMachine : public LCCTargetMachine {
  virtual void anchor();
public:
  LCCelTargetMachine(const Target &T, const Triple &TT, StringRef CPU,
                      StringRef FS, const TargetOptions &Options,
                      Optional<Reloc::Model> RM, Optional<CodeModel::Model> CM,
                      CodeGenOpt::Level OL, bool JIT);
};
} // namespace llvm

#endif
