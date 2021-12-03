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
#include "llvm/IR/BasicBlock.h"
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
    bool changed = false;
    for(auto &BB: F) {
      changed |= runOnBasicBlock(BB);
    }
    return changed;
  }

  bool runOnBasicBlock(BasicBlock &BB) {
    std::map<LVNKey, unsigned> LVNValues{};
    unsigned valueCounter = 0;
    for (auto &instr: BB) {
      auto opcode = instr.getOpcode();
      if (opcode != Instruction::Add)
        continue;
      auto opSize = instr.getNumOperands();
      if (opSize != 2)
        continue;
      auto *left = instr.getOperand(0);
      auto *right = instr.getOperand(1);

    }
  }

private:
  struct LVNKey {
    unsigned lval, opcode, rval;
    friend bool operator<(const LVNKey &l, const LVNKey &r) {
      return std::tie(l.lval, l.opcode, l.rval) <
             std::tie(r.lval, r.opcode, r.rval); // keep the same order
    }
  };
};
} // namespace

char LVN::ID = 0;
static RegisterPass<LVN> A("LVN", "Local Value Numbering");
