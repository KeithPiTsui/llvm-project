//===-- NTTargetMachine.cpp - Define TargetMachine for NT ---*- C++ -*-===//
#include "NTTargetMachine.h"
#include "NT.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/CodeGen/TargetPassConfig.h"
#include "llvm/Support/TargetRegistry.h"
using namespace llvm;
#define DEBUG_TYPE "NT"
extern "C" void LLVMInitializeNTTarget() {
}
