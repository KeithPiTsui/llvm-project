//===-- LCCSEInstrInfo.h - LCC32/64 Instruction Information ---*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the LCC32/64 implementation of the TargetInstrInfo class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_LCC_LCCSEINSTRINFO_H
#define LLVM_LIB_TARGET_LCC_LCCSEINSTRINFO_H



#include "LCCInstrInfo.h"
#include "LCCMachineFunction.h"
#include "LCCSERegisterInfo.h"

namespace llvm {

class LCCSEInstrInfo : public LCCInstrInfo {
  const LCCSERegisterInfo RI;

public:
  explicit LCCSEInstrInfo(const LCCSubtarget &STI);

  const LCCRegisterInfo &getRegisterInfo() const override;
};

} // namespace llvm

#endif
