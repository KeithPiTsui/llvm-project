//===-- LCC.h - Top-level interface for LCC representation ----*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the entry points for global functions defined in
// the LLVM LCC back-end.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_LCC_LCC_H
#define LLVM_LIB_TARGET_LCC_LCC_H

#include "LCCConfig.h"
#include "MCTargetDesc/LCCMCTargetDesc.h"
#include "llvm/Target/TargetMachine.h"

namespace llvm {
  class LCCTargetMachine;
  class FunctionPass;

} // end namespace llvm;

#endif

