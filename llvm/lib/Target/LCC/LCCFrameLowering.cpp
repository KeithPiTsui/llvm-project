//===-- LCCFrameLowering.cpp - LCC Frame Information --------------------===//
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

#include "LCCFrameLowering.h"

#include "LCCInstrInfo.h"
#include "LCCMachineFunction.h"
#include "LCCSubtarget.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineModuleInfo.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Target/TargetOptions.h"

using namespace llvm;

const LCCFrameLowering *LCCFrameLowering::create(const LCCSubtarget &ST) {
  return llvm::createLCCSEFrameLowering(ST);
}
bool LCCFrameLowering::hasFP(const MachineFunction &MF) const { return true; }
