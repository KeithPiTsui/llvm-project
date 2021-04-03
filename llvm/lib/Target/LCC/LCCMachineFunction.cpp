//===-- LCCMachineFunctionInfo.cpp - Private data used for LCC ----------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "LCCMachineFunction.h"

#include "LCCInstrInfo.h"
#include "LCCSubtarget.h"
#include "MCTargetDesc/LCCBaseInfo.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/IR/Function.h"

using namespace llvm;

bool FixGlobalBaseReg;

LCCFunctionInfo::~LCCFunctionInfo() {}

void LCCFunctionInfo::anchor() {}
