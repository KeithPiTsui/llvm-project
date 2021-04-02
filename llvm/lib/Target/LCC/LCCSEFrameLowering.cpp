//===-- LCCSEFrameLowering.cpp - LCC Frame Information ------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the LCC implementation of TargetFrameLowering class.
//
//===----------------------------------------------------------------------===//

#include "LCCSEFrameLowering.h"

#include "LCCMachineFunction.h"
#include "LCCSEInstrInfo.h"
#include "LCCSubtarget.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineModuleInfo.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/RegisterScavenging.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Target/TargetOptions.h"

using namespace llvm;

LCCSEFrameLowering::LCCSEFrameLowering(const LCCSubtarget &STI)
    : LCCFrameLowering(STI, STI.stackAlignment()) {}

//@emitPrologue {
void LCCSEFrameLowering::emitPrologue(MachineFunction &MF,
                                       MachineBasicBlock &MBB) const {}
//}

//@emitEpilogue {
void LCCSEFrameLowering::emitEpilogue(MachineFunction &MF,
                                       MachineBasicBlock &MBB) const {}
//}

const LCCFrameLowering *
llvm::createLCCSEFrameLowering(const LCCSubtarget &ST) {
  return new LCCSEFrameLowering(ST);
}
