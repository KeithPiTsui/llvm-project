//===-- NTTargetInfo.cpp - NT Target Implementation -------------------===//
#include "NT.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/TargetRegistry.h"
using namespace llvm;
Target llvm::TheNTTarget;
extern "C" void LLVMInitializeNTTargetInfo() {
  RegisterTarget<Triple::NT,
                 /*HasJIT=*/true> X(TheNTTarget, "NT", "NT", "NT");
}
