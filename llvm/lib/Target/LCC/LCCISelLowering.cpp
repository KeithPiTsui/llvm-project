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
LCCTargetLowering::LCCTargetLowering(const LCCTargetMachine &TM,
                                     const LCCSubtarget &STI)
    : TargetLowering(TM), Subtarget(STI), ABI(TM.getABI()) {
  setMinFunctionAlignment(Align(2));
}

const LCCTargetLowering *LCCTargetLowering::create(const LCCTargetMachine &TM,
                                                   const LCCSubtarget &STI) {
  return llvm::createLCCSETargetLowering(TM, STI);
}

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
  // CCValAssign - represent the assignment of
  // the return value to a location
  SmallVector<CCValAssign, 16> RVLocs;
  MachineFunction &MF = DAG.getMachineFunction();

  // CCState - Info about the registers and stack slot.
  CCState CCInfo(CallConv, IsVarArg, MF, RVLocs, *DAG.getContext());
  LCCCC LCCCCInfo(CallConv, ABI.IsO32(), CCInfo);

  // Analyze return values.
  LCCCCInfo.analyzeReturn(Outs, Subtarget.abiUsesSoftFloat(),
                          MF.getFunction().getReturnType());

  SDValue Flag;
  SmallVector<SDValue, 4> RetOps(1, Chain);

  // Copy the result values into the output registers.
  for (unsigned i = 0; i != RVLocs.size(); ++i) {
    SDValue Val = OutVals[i];
    CCValAssign &VA = RVLocs[i];
    assert(VA.isRegLoc() && "Can only return in registers!");

    if (RVLocs[i].getValVT() != RVLocs[i].getLocVT())
      Val = DAG.getNode(ISD::BITCAST, DL, RVLocs[i].getLocVT(), Val);

    Chain = DAG.getCopyToReg(Chain, DL, VA.getLocReg(), Val, Flag);

    // Guarantee that all emitted copies are stuck together with flags.
    Flag = Chain.getValue(1);
    RetOps.push_back(DAG.getRegister(VA.getLocReg(), VA.getLocVT()));
  }

  //@Ordinary struct type: 2 {
  // The LCC ABIs for returning structs by value requires that we copy
  // the sret argument into $v0 for the return. We saved the argument into
  // a virtual register in the entry block, so now we copy the value out
  // and into $v0.
  if (MF.getFunction().hasStructRetAttr()) {
    LCCFunctionInfo *LCCFI = MF.getInfo<LCCFunctionInfo>();
    unsigned Reg = LCCFI->getSRetReturnReg();

    if (!Reg)
      llvm_unreachable("sret virtual register not created in the entry block");
    SDValue Val =
        DAG.getCopyFromReg(Chain, DL, Reg, getPointerTy(DAG.getDataLayout()));

    // fixme
    unsigned V0 = LCC::R0;

    Chain = DAG.getCopyToReg(Chain, DL, V0, Val, Flag);
    Flag = Chain.getValue(1);
    RetOps.push_back(DAG.getRegister(V0, getPointerTy(DAG.getDataLayout())));
  }
  //@Ordinary struct type: 2 }

  RetOps[0] = Chain; // Update chain.

  // Add the flag if we have it.
  if (Flag.getNode())
    RetOps.push_back(Flag);

  // Return on LCC is always a "ret $lr"
  return DAG.getNode(LCCISD::Ret, DL, MVT::Other, RetOps);
}

LCCTargetLowering::LCCCC::LCCCC(
    CallingConv::ID CC, bool IsO32_, CCState &Info,
    LCCCC::SpecialCallingConvType SpecialCallingConv_)
    : CCInfo(Info), CallConv(CC), IsO32(IsO32_) {
  // Pre-allocate reserved argument area.
  CCInfo.AllocateStack(reservedArgArea(), 1);
}

template <typename Ty>
void LCCTargetLowering::LCCCC::analyzeReturn(const SmallVectorImpl<Ty> &RetVals,
                                             bool IsSoftFloat,
                                             const SDNode *CallNode,
                                             const Type *RetTy) const {
  CCAssignFn *Fn;

  Fn = RetCC_LCC;

  for (unsigned I = 0, E = RetVals.size(); I < E; ++I) {
    MVT VT = RetVals[I].VT;
    ISD::ArgFlagsTy Flags = RetVals[I].Flags;
    MVT RegVT = this->getRegVT(VT, RetTy, CallNode, IsSoftFloat);
    if (Fn(I, VT, RegVT, CCValAssign::Full, Flags, this->CCInfo)) {
#ifndef NDEBUG
      dbgs() << "Call result #" << I << " has unhandled type "
             << EVT(VT).getEVTString() << '\n';
#endif
      llvm_unreachable(nullptr);
    }
  }
}

void LCCTargetLowering::LCCCC::analyzeCallResult(
    const SmallVectorImpl<ISD::InputArg> &Ins, bool IsSoftFloat,
    const SDNode *CallNode, const Type *RetTy) const {
  analyzeReturn(Ins, IsSoftFloat, CallNode, RetTy);
}

void LCCTargetLowering::LCCCC::analyzeReturn(
    const SmallVectorImpl<ISD::OutputArg> &Outs, bool IsSoftFloat,
    const Type *RetTy) const {
  analyzeReturn(Outs, IsSoftFloat, nullptr, RetTy);
}

unsigned LCCTargetLowering::LCCCC::reservedArgArea() const {
  return (IsO32 && (CallConv != CallingConv::Fast)) ? 8 : 0;
}

MVT LCCTargetLowering::LCCCC::getRegVT(MVT VT, const Type *OrigTy,
                                       const SDNode *CallNode,
                                       bool IsSoftFloat) const {
  if (IsSoftFloat || IsO32)
    return VT;

  return VT;
}
