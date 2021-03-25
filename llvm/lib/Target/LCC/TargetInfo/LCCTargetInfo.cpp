//===-- NTTargetInfo.cpp - NT Target Implementation -------------------===//
#include "LCC.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/TargetRegistry.h"
using namespace llvm;
Target llvm::TheLCCTarget;
extern "C" void LLVMInitializeLCCTargetInfo() {
  RegisterTarget<Triple::NT,
                 /*HasJIT=*/true> X(TheLCCTarget, "LCC", "LCC", "LCC");
}
