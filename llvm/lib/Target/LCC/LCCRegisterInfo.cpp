//===-- LCCRegisterInfo.cpp - LCC Register Information -== --------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the LCC implementation of the TargetRegisterInfo class.
//
//===----------------------------------------------------------------------===//

#include "llvm/CodeGen/Register.h"
#define DEBUG_TYPE "LCC-reg-info"
#include "LCC.h"
#include "LCCMachineFunction.h"
#include "LCCRegisterInfo.h"
#include "LCCSubtarget.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Type.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"

#define GET_REGINFO_TARGET_DESC
#include "LCCGenRegisterInfo.inc"

using namespace llvm;

LCCRegisterInfo::LCCRegisterInfo(const LCCSubtarget &ST)
    : LCCGenRegisterInfo(LCC::LR), Subtarget(ST) {}

//===----------------------------------------------------------------------===//
// Callee Saved Registers methods
//===----------------------------------------------------------------------===//
/// LCC Callee Saved Registers
// In LCCCallConv.td,
// def CSR_O32 : CalleeSavedRegs<(add LR, FP,
//                                   (sequence "S%u", 2, 0))>;
// llc create CSR_O32_SaveList and CSR_O32_RegMask from above defined.
const MCPhysReg *
LCCRegisterInfo::getCalleeSavedRegs(const MachineFunction *MF) const {
  return CSR_O16_SaveList;
}

const uint32_t *LCCRegisterInfo::getCallPreservedMask(const MachineFunction &MF,
                                                      CallingConv::ID) const {
  return CSR_O16_RegMask;
}

// pure virtual method
//@getReservedRegs {
BitVector LCCRegisterInfo::getReservedRegs(const MachineFunction &MF) const {
  //@getReservedRegs body {
  static const uint16_t ReservedCPURegs[] = {LCC::SP, LCC::LR};
  BitVector Reserved(getNumRegs());
  for (unsigned I = 0; I < array_lengthof(ReservedCPURegs); ++I)
    Reserved.set(ReservedCPURegs[I]);
  return Reserved;
}

//@eliminateFrameIndex {
//- If no eliminateFrameIndex(), it will hang on run.
// pure virtual method
// FrameIndex represent objects inside a abstract stack.
// We must replace FrameIndex with an stack/frame pointer
// direct reference.
void LCCRegisterInfo::eliminateFrameIndex(MachineBasicBlock::iterator II,
                                          int SPAdj, unsigned FIOperandNum,
                                          RegScavenger *RS) const {}
//}

bool LCCRegisterInfo::requiresRegisterScavenging(
    const MachineFunction &MF) const {
  return true;
}

bool LCCRegisterInfo::trackLivenessAfterRegAlloc(
    const MachineFunction &MF) const {
  return true;
}

// pure virtual method
Register LCCRegisterInfo::getFrameRegister(const MachineFunction &MF) const {
  const TargetFrameLowering *TFI = MF.getSubtarget().getFrameLowering();
  return TFI->hasFP(MF) ? (LCC::FP) : (LCC::SP);
}
