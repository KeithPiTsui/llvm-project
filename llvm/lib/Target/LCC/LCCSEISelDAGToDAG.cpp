//===-- LCCSEISelDAGToDAG.cpp - A Dag to Dag Inst Selector for LCCSE ----===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// Subclass of LCCDAGToDAGISel specialized for LCC32.
//
//===----------------------------------------------------------------------===//

#include "LCCSEISelDAGToDAG.h"

#include "LCC.h"
#include "LCCMachineFunction.h"
#include "LCCRegisterInfo.h"
#include "MCTargetDesc/LCCBaseInfo.h"
#include "llvm/CodeGen/MachineConstantPool.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/SelectionDAGNodes.h"
#include "llvm/IR/CFG.h"
#include "llvm/IR/GlobalValue.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Intrinsics.h"
#include "llvm/IR/Type.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetMachine.h"
using namespace llvm;

#define DEBUG_TYPE "LCC-isel"

bool LCCSEDAGToDAGISel::runOnMachineFunction(MachineFunction &MF) {
  Subtarget = &static_cast<const LCCSubtarget &>(MF.getSubtarget());
  return LCCDAGToDAGISel::runOnMachineFunction(MF);
}

void LCCSEDAGToDAGISel::processFunctionAfterISel(MachineFunction &MF) {}

//@selectNode
bool LCCSEDAGToDAGISel::trySelect(SDNode *Node) {
  unsigned Opcode = Node->getOpcode();
  SDLoc DL(Node);

  ///
  // Instruction Selection not handled by the auto-generated
  // tablegen selection should be handled here.
  ///

  ///
  // Instruction Selection not handled by the auto-generated
  // tablegen selection should be handled here.
  ///
  EVT NodeTy = Node->getValueType(0);
  unsigned MultOpc;

  switch (Opcode) {
  default:
    break;
  }

  return false;
}

FunctionPass *llvm::createLCCSEISelDag(LCCTargetMachine &TM,
                                        CodeGenOpt::Level OptLevel) {
  return new LCCSEDAGToDAGISel(TM, OptLevel);
}
