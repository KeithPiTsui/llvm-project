//===-- LCCFrameLowering.h - Define frame lowering for LCC ----*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
//
//
//===----------------------------------------------------------------------===//
#ifndef LLVM_LIB_TARGET_LCC_LCCFRAMELOWERING_H
#define LLVM_LIB_TARGET_LCC_LCCFRAMELOWERING_H

#include "LCCConfig.h"

#include "LCC.h"
#include "llvm/CodeGen/TargetFrameLowering.h"
#include "llvm/Support/Alignment.h"

namespace llvm {
  class LCCSubtarget;

class LCCFrameLowering : public TargetFrameLowering {
protected:
  const LCCSubtarget &STI;

public:
  explicit LCCFrameLowering(const LCCSubtarget &sti, Align Alignment)
    : TargetFrameLowering(StackGrowsDown, Alignment, 0, Alignment),
      STI(sti) {
  }

  static const LCCFrameLowering *create(const LCCSubtarget &ST);

  bool hasFP(const MachineFunction &MF) const override;

};

/// Create LCCFrameLowering objects.
const LCCFrameLowering *createLCCSEFrameLowering(const LCCSubtarget &ST);

} // End llvm namespace

#endif

