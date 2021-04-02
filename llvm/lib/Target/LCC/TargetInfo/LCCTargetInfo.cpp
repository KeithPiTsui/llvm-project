//===-- LCCTargetInfo.cpp - LCC Target Implementation -------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "LCC.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/TargetRegistry.h"
using namespace llvm;

Target llvm::TheLCCTarget;

extern "C" void LLVMInitializeLCCTargetInfo() {
  RegisterTarget<Triple::LCC,
                 /*HasJIT=*/true> X(TheLCCTarget, "LCC", "LCC", "LCC");
}
