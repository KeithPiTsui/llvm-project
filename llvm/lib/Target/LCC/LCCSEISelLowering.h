//===-- LCCISEISelLowering.h - LCCISE DAG Lowering Interface ----*- C++
//-*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// Subclass of LCCITargetLowering specialized for LCC32/64.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_LCC_LCCSEISELLOWERING_H
#define LLVM_LIB_TARGET_LCC_LCCSEISELLOWERING_H

#include "LCCConfig.h"

#include "LCCISelLowering.h"
#include "LCCRegisterInfo.h"

namespace llvm {
class LCCSETargetLowering : public LCCTargetLowering {
public:
  explicit LCCSETargetLowering(const LCCTargetMachine &TM,
                                const LCCSubtarget &STI);

  SDValue LowerOperation(SDValue Op, SelectionDAG &DAG) const override;

private:
};
} // namespace llvm

#endif // LCCISEISELLOWERING_H
