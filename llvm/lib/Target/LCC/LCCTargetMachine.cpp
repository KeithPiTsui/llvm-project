//===-- LCCTargetMachine.cpp - Define TargetMachine for LCC ---*- C++ -*-===//
#include "LCCTargetMachine.h"
#include "LCC.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/CodeGen/TargetPassConfig.h"
#include "llvm/Support/TargetRegistry.h"
using namespace llvm;
#define DEBUG_TYPE "LCC"
extern "C" void LLVMInitializeLCCTarget() {
}
