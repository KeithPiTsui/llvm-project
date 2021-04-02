//===-- LCCSEISelLowering.cpp - LCCSE DAG Lowering Interface --*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// Subclass of LCCTargetLowering specialized for LCC32.
//
//===----------------------------------------------------------------------===//
#include "LCCSEISelLowering.h"
#include "LCCMachineFunction.h"

#include "LCCRegisterInfo.h"
#include "LCCTargetMachine.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/TargetInstrInfo.h"
#include "llvm/IR/Intrinsics.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

#define DEBUG_TYPE "LCC-isel"

static cl::opt<bool> EnableLCCTailCalls("enable-LCC-tail-calls", cl::Hidden,
                                         cl::desc("LCC: Enable tail calls."),
                                         cl::init(false));

//@LCCSETargetLowering {
LCCSETargetLowering::LCCSETargetLowering(const LCCTargetMachine &TM,
                                           const LCCSubtarget &STI)
    : LCCTargetLowering(TM, STI) {
  //@LCCSETargetLowering body {
  // Set up the register classes
  addRegisterClass(MVT::i32, &LCC::CPURegsRegClass);

  // must, computeRegisterProperties - Once all of the register classes are
  //  added, this allows us to compute derived properties we expose.
  computeRegisterProperties(Subtarget.getRegisterInfo());
}

SDValue LCCSETargetLowering::LowerOperation(SDValue Op,
                                             SelectionDAG &DAG) const {

  return LCCTargetLowering::LowerOperation(Op, DAG);
}

const LCCTargetLowering *
llvm::createLCCSETargetLowering(const LCCTargetMachine &TM,
                                 const LCCSubtarget &STI) {
  return new LCCSETargetLowering(TM, STI);
}
