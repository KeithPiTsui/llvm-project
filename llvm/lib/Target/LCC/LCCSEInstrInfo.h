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
  //@expandPostRAPseudo
  bool expandPostRAPseudo(MachineInstr &MI) const override;

  /// Adjust SP by Amount bytes. FIXME: it is an override method.
  void adjustStackPtr(unsigned SP, int64_t Amount, MachineBasicBlock &MBB,
                      MachineBasicBlock::iterator I) const;

  /// Emit a series of instructions to load an immediate. If NewImm is a
  /// non-NULL parameter, the last instruction is not emitted, but instead
  /// its immediate operand is returned in NewImm.
  unsigned loadImmediate(int64_t Imm, MachineBasicBlock &MBB,
                         MachineBasicBlock::iterator II, const DebugLoc &DL,
                         unsigned *NewImm) const;

  void storeRegToStack(MachineBasicBlock &MBB, MachineBasicBlock::iterator MI,
                       unsigned SrcReg, bool isKill, int FrameIndex,
                       const TargetRegisterClass *RC,
                       const TargetRegisterInfo *TRI,
                       int64_t Offset) const override;

  void loadRegFromStack(MachineBasicBlock &MBB, MachineBasicBlock::iterator MI,
                        unsigned DestReg, int FrameIndex,
                        const TargetRegisterClass *RC,
                        const TargetRegisterInfo *TRI,
                        int64_t Offset) const override;

private:
  void expandRetLR(MachineBasicBlock &MBB, MachineBasicBlock::iterator I) const;
};

} // namespace llvm

#endif
