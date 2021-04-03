//===-- LCCSEFrameLowering.h - LCC32/64 frame lowering --------*- C++ -*-===//
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

#ifndef LLVM_LIB_TARGET_LCC_LCCSEFRAMELOWERING_H
#define LLVM_LIB_TARGET_LCC_LCCSEFRAMELOWERING_H



#include "LCCFrameLowering.h"

namespace llvm {

class LCCSEFrameLowering : public LCCFrameLowering {
public:
  explicit LCCSEFrameLowering(const LCCSubtarget &STI);

  /// emitProlog/emitEpilog - These methods insert prolog and epilog code into
  /// the function.
  void emitPrologue(MachineFunction &MF, MachineBasicBlock &MBB) const override;
  void emitEpilogue(MachineFunction &MF, MachineBasicBlock &MBB) const override;

  void determineCalleeSaves(MachineFunction &MF, BitVector &SavedRegs,
                            RegScavenger *RS = nullptr) const override;

  bool hasReservedCallFrame(const MachineFunction &MF) const override;
};

} // namespace llvm

#endif
