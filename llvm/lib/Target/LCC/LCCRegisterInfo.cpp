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

#include "LCCRegisterInfo.h"

#include "LCC.h"
#include "LCCMachineFunction.h"
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
// void LCCRegisterInfo::eliminateFrameIndex(MachineBasicBlock::iterator II,
//                                            int SPAdj, unsigned FIOperandNum,
//                                            RegScavenger *RS) const {}
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

//- If no eliminateFrameIndex(), it will hang on run.
// pure virtual method
// FrameIndex represent objects inside a abstract stack.
// We must replace FrameIndex with an stack/frame pointer
// direct reference.
void LCCRegisterInfo::eliminateFrameIndex(MachineBasicBlock::iterator II,
                                          int SPAdj, unsigned FIOperandNum,
                                          RegScavenger *RS) const {
  MachineInstr &MI = *II;
  MachineFunction &MF = *MI.getParent()->getParent();
  MachineFrameInfo MFI = MF.getFrameInfo();
  LCCFunctionInfo *LCCFI = MF.getInfo<LCCFunctionInfo>();

  unsigned i = 0;
  while (!MI.getOperand(i).isFI()) {
    ++i;
    assert(i < MI.getNumOperands() && "Instr doesn't have FrameIndex operand!");
  }

  // DEBUG(errs() << "\nFunction : " << MF.getFunction()->getName() << "\n";
  //       errs() << "<--------->\n"
  //              << MI);

  int FrameIndex = MI.getOperand(i).getIndex();
  uint64_t stackSize = MF.getFrameInfo().getStackSize();
  int64_t spOffset = MF.getFrameInfo().getObjectOffset(FrameIndex);

  // DEBUG(errs() << "FrameIndex : " << FrameIndex << "\n"
  //              << "spOffset   : " << spOffset << "\n"
  //              << "stackSize  : " << stackSize << "\n");

  const std::vector<CalleeSavedInfo> &CSI = MFI.getCalleeSavedInfo();
  int MinCSFI = 0;
  int MaxCSFI = -1;

  if (CSI.size()) {
    MinCSFI = CSI[0].getFrameIdx();
    MaxCSFI = CSI[CSI.size() - 1].getFrameIdx();
  }

  // The following stack frame objects are always referenced relative to $sp:
  //  1. Outgoing arguments.
  //  2. Pointer to dynamically allocated stack space.
  //  3. Locations for callee-saved registers.
  // Everything else is referenced relative to whatever register
  // getFrameRegister() returns.
  unsigned FrameReg;

  FrameReg = LCC::SP;

  // Calculate final offset.
  // - There is no need to change the offset if the frame object is one of the
  //   following: an outgoing argument, pointer to a dynamically allocated
  //   stack space or a $gp restore location,
  // - If the frame object is any of the following, its offset must be adjusted
  //   by adding the size of the stack:
  //   incoming argument, callee-saved register location or local variable.
  int64_t Offset;
  Offset = spOffset + (int64_t)stackSize;

  Offset += MI.getOperand(i + 1).getImm();

  // DEBUG(errs() << "Offset     : " << Offset << "\n"
  //              << "<--------->\n");

  // If MI is not a debug value, make sure Offset fits in the 16-bit immediate
  // field.
  if (!MI.isDebugValue() && !isInt<16>(Offset)) {
    assert("(!MI.isDebugValue() && !isInt<16>(Offset))");
  }

  MI.getOperand(i).ChangeToRegister(FrameReg, false);
  MI.getOperand(i + 1).ChangeToImmediate(Offset);
}
