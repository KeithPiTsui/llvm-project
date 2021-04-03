//===-- LCCInstrInfo.cpp - LCC Instruction Information ------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the LCC implementation of the TargetInstrInfo class.
//
//===----------------------------------------------------------------------===//

#include "LCCInstrInfo.h"

#include "LCCMachineFunction.h"
#include "LCCTargetMachine.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/TargetRegistry.h"

using namespace llvm;

#define GET_INSTRINFO_CTOR_DTOR
#include "LCCGenInstrInfo.inc"

// Pin the vtable to this file.
void LCCInstrInfo::anchor() {}

//@LCCInstrInfo {
LCCInstrInfo::LCCInstrInfo(const LCCSubtarget &STI) : Subtarget(STI) {}

const LCCInstrInfo *LCCInstrInfo::create(LCCSubtarget &STI) {
  return llvm::createLCCSEInstrInfo(STI);
}

//@GetInstSizeInBytes {
/// Return the number of bytes of code the specified instruction may be.
unsigned LCCInstrInfo::GetInstSizeInBytes(const MachineInstr &MI) const {
  //@GetInstSizeInBytes - body
  return MI.getDesc().getSize();
}

MachineMemOperand *
LCCInstrInfo::GetMemOperand(MachineBasicBlock &MBB, int FI,
                             MachineMemOperand::Flags Flags) const {
  MachineFunction &MF = *MBB.getParent();
  MachineFrameInfo &MFI = MF.getFrameInfo();
  unsigned align = MFI.getObjectAlignment(FI);
  return MF.getMachineMemOperand(MachinePointerInfo::getFixedStack(MF, FI),
                                 Flags, MFI.getObjectSize(FI), Align(align));
}
