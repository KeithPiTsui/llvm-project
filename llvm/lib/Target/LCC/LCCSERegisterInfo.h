//===-- LCCSERegisterInfo.h - LCC32 Register Information ------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the LCC32/64 implementation of the TargetRegisterInfo
// class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_LCC_LCCSEREGISTERINFO_H
#define LLVM_LIB_TARGET_LCC_LCCSEREGISTERINFO_H

#include "LCCRegisterInfo.h"

namespace llvm {
class LCCSEInstrInfo;

class LCCSERegisterInfo : public LCCRegisterInfo {
public:
  LCCSERegisterInfo(const LCCSubtarget &Subtarget);

  const TargetRegisterClass *intRegClass(unsigned Size) const override;
};

} // end namespace llvm

#endif
