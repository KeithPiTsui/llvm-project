//===- BBOpts.cpp - Example code from "Writing an LLVM Pass"
//---------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements two versions of the LLVM "BBOpts World" pass described
// in docs/WritingAnLLVMPass.html
//
//===----------------------------------------------------------------------===//

#include "llvm/ADT/Statistic.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/LegacyPassManagers.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/TableGen/Record.h"

using namespace llvm;

#define DEBUG_TYPE "BBOpts"
namespace {
struct DumpPasses : public ModulePass {
  static char ID; // Pass identification, replacement for typeid
  DumpPasses() : ModulePass(ID) {}
  bool runOnModule(Module &) override {
    this->getResolver()->getPMDataManager().getTopLevelManager()->dumpPasses();
    return false;
  }
};
} // namespace
char DumpPasses::ID = 0;
static RegisterPass<DumpPasses> Z("DumpPasses", "DumpPasses");

namespace {
struct LVN : public FunctionPass {
  static char ID; // Pass identification, replacement for typeid
  LVN() : FunctionPass(ID) {}

  bool runOnFunction(Function &F) override {
    errs() << "BBOpts: ";
    errs().write_escaped(F.getName()) << '\n';
    return false;
  }

  // We don't modify the program, so we preserve all analyses.
  void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.setPreservesAll();
  }
};
} // namespace

char LVN::ID = 0;
static RegisterPass<LVN> A("LVN", "Local Value Numbering");
