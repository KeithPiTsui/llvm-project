//===-- LCCSEInstrInfo.cpp - LCC32/64 Instruction Information -----------===//
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

#include "LCCSEInstrInfo.h"

#include "LCCMachineFunction.h"
#include "LCCTargetMachine.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/TargetRegistry.h"

using namespace llvm;

LCCSEInstrInfo::LCCSEInstrInfo(const LCCSubtarget &STI)
    : LCCInstrInfo(STI), RI(STI) {}

const LCCRegisterInfo &LCCSEInstrInfo::getRegisterInfo() const { return RI; }

const LCCInstrInfo *llvm::createLCCSEInstrInfo(const LCCSubtarget &STI) {
  return new LCCSEInstrInfo(STI);
}
