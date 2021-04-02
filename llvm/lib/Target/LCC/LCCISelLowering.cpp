//===-- LCCISelLowering.cpp - LCC DAG Lowering Implementation -----------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file defines the interfaces that LCC uses to lower LLVM code into a
// selection DAG.
//
//===----------------------------------------------------------------------===//
#include "LCCISelLowering.h"

#include "LCCMachineFunction.h"
#include "LCCSubtarget.h"
#include "LCCTargetMachine.h"
#include "LCCTargetObjectFile.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/CodeGen/CallingConvLower.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/SelectionDAG.h"
#include "llvm/CodeGen/ValueTypes.h"
#include "llvm/IR/CallingConv.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

#define DEBUG_TYPE "LCC-lower"

//@3_1 1 {
const char *LCCTargetLowering::getTargetNodeName(unsigned Opcode) const {
  switch (Opcode) {
  case LCCISD::JmpLink:
    return "LCCISD::JmpLink";
  case LCCISD::TailCall:
    return "LCCISD::TailCall";
  case LCCISD::Hi:
    return "LCCISD::Hi";
  case LCCISD::Lo:
    return "LCCISD::Lo";
  case LCCISD::GPRel:
    return "LCCISD::GPRel";
  case LCCISD::Ret:
    return "LCCISD::Ret";
  case LCCISD::EH_RETURN:
    return "LCCISD::EH_RETURN";
  case LCCISD::DivRem:
    return "LCCISD::DivRem";
  case LCCISD::DivRemU:
    return "LCCISD::DivRemU";
  case LCCISD::Wrapper:
    return "LCCISD::Wrapper";
  default:
    return NULL;
  }
}
//@3_1 1 }

//@LCCTargetLowering {
LCCTargetLowering::LCCTargetLowering(const LCCTargetMachine &TM,
                                       const LCCSubtarget &STI)
    : TargetLowering(TM), Subtarget(STI), ABI(TM.getABI()) {}

const LCCTargetLowering *
LCCTargetLowering::create(const LCCTargetMachine &TM,
                           const LCCSubtarget &STI) {
  return llvm::createLCCSETargetLowering(TM, STI);
}

//===----------------------------------------------------------------------===//
//  Lower helper functions
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
//  Misc Lower Operation implementation
//===----------------------------------------------------------------------===//

#include "LCCGenCallingConv.inc"

//===----------------------------------------------------------------------===//
//@            Formal Arguments Calling Convention Implementation
//===----------------------------------------------------------------------===//

//@LowerFormalArguments {
/// LowerFormalArguments - transform physical registers into virtual registers
/// and generate load operations for arguments places on the stack.
SDValue LCCTargetLowering::LowerFormalArguments(
    SDValue Chain, CallingConv::ID CallConv, bool IsVarArg,
    const SmallVectorImpl<ISD::InputArg> &Ins, const SDLoc &DL,
    SelectionDAG &DAG, SmallVectorImpl<SDValue> &InVals) const {

  return Chain;
}
// @LowerFormalArguments }

//===----------------------------------------------------------------------===//
//@              Return Value Calling Convention Implementation
//===----------------------------------------------------------------------===//

SDValue
LCCTargetLowering::LowerReturn(SDValue Chain, CallingConv::ID CallConv,
                                bool IsVarArg,
                                const SmallVectorImpl<ISD::OutputArg> &Outs,
                                const SmallVectorImpl<SDValue> &OutVals,
                                const SDLoc &DL, SelectionDAG &DAG) const {
  return DAG.getNode(LCCISD::Ret, DL, MVT::Other, Chain,
                     DAG.getRegister(LCC::LR, MVT::i32));
}
