//===-- LCCSERegisterInfo.cpp - LCC Register Information ------== -------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the LCC implementation of the TargetRegisterInfo
// class.
//
//===----------------------------------------------------------------------===//

#include "LCCSERegisterInfo.h"

using namespace llvm;

#define DEBUG_TYPE "LCC-reg-info"

LCCSERegisterInfo::LCCSERegisterInfo(const LCCSubtarget &ST)
  : LCCRegisterInfo(ST) {}

const TargetRegisterClass *
LCCSERegisterInfo::intRegClass(unsigned Size) const {
  return &LCC::CPURegsRegClass;
}

