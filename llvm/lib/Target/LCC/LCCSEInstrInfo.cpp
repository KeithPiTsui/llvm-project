//===-- LCCSEInstrInfo.cpp - LCC32/64 Instruction Information -----------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the LCC32/64 implementation of the TargetInstrInfo class.
//
//===----------------------------------------------------------------------===//

#include "LCCSEInstrInfo.h"

#include "LCCMachineFunction.h"
#include "LCCTargetMachine.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/TargetRegistry.h"

#include "LCCAnalyzeImmediate.h"
#include "MCTargetDesc/LCCMCTargetDesc.h"

using namespace llvm;

LCCSEInstrInfo::LCCSEInstrInfo(const LCCSubtarget &STI)
    : LCCInstrInfo(STI), RI(STI) {}

const LCCRegisterInfo &LCCSEInstrInfo::getRegisterInfo() const { return RI; }

bool LCCSEInstrInfo::expandPostRAPseudo(MachineInstr &MI) const {
  MachineBasicBlock &MBB = *MI.getParent();
  switch (MI.getDesc().getOpcode()) {
  default:
    return false;
  case LCC::RetLR:
    expandRetLR(MBB, MI);
    break;
  }
  MBB.erase(MI);
  return true;
}

void LCCSEInstrInfo::expandRetLR(MachineBasicBlock &MBB,
                                 MachineBasicBlock::iterator I) const {
  // expand Ret LR to ret.
  BuildMI(MBB, I, I->getDebugLoc(), get(LCC::RET));
}

const LCCInstrInfo *llvm::createLCCSEInstrInfo(const LCCSubtarget &STI) {
  return new LCCSEInstrInfo(STI);
}

/// Adjust SP by Amount bytes.
void LCCSEInstrInfo::adjustStackPtr(unsigned SP, int64_t Amount,
                                    MachineBasicBlock &MBB,
                                    MachineBasicBlock::iterator I) const {
  DebugLoc DL = I != MBB.end() ? I->getDebugLoc() : DebugLoc();
  // fixme
  unsigned ADDu = LCC::ADD;
  unsigned ADDiu = LCC::ADDi;

  if (isInt<5>(Amount)) {
    // addiu sp, sp, amount
    BuildMI(MBB, I, DL, get(ADDiu), SP).addReg(SP).addImm(Amount);
  } else { // Expand immediate that doesn't fit in 16-bit.
    assert(false &&
           "Not yet support adjsut sp by amount of byte larger than 32");
    unsigned Reg = loadImmediate(Amount, MBB, I, DL, nullptr);
    BuildMI(MBB, I, DL, get(ADDu), SP).addReg(SP).addReg(Reg, RegState::Kill);
  }
}

/// This function generates the sequence of instructions needed to get the
/// result of adding register REG and immediate IMM.
unsigned LCCSEInstrInfo::loadImmediate(int64_t Imm, MachineBasicBlock &MBB,
                                       MachineBasicBlock::iterator II,
                                       const DebugLoc &DL,
                                       unsigned *NewImm) const {
  LCCAnalyzeImmediate AnalyzeImm;
  unsigned Size = 32;
  // fixme
  unsigned LUi = LCC::LD;
  unsigned ZEROReg = LCC::ADD;
  unsigned ATReg = LCC::ADD;

  bool LastInstrIsADDiu = NewImm;

  const LCCAnalyzeImmediate::InstSeq &Seq =
      AnalyzeImm.Analyze(Imm, Size, LastInstrIsADDiu);
  LCCAnalyzeImmediate::InstSeq::const_iterator Inst = Seq.begin();

  assert(Seq.size() && (!LastInstrIsADDiu || (Seq.size() > 1)));

  // The first instruction can be a LUi, which is different from other
  // instructions (ADDiu, ORI and SLL) in that it does not have a register
  // operand.
  if (Inst->Opc == LUi)
    BuildMI(MBB, II, DL, get(LUi), ATReg)
        .addImm(SignExtend64<16>(Inst->ImmOpnd));
  else
    BuildMI(MBB, II, DL, get(Inst->Opc), ATReg)
        .addReg(ZEROReg)
        .addImm(SignExtend64<16>(Inst->ImmOpnd));

  // Build the remaining instructions in Seq.
  for (++Inst; Inst != Seq.end() - LastInstrIsADDiu; ++Inst)
    BuildMI(MBB, II, DL, get(Inst->Opc), ATReg)
        .addReg(ATReg)
        .addImm(SignExtend64<16>(Inst->ImmOpnd));

  if (LastInstrIsADDiu)
    *NewImm = Inst->ImmOpnd;

  return ATReg;
}

void LCCSEInstrInfo::storeRegToStack(MachineBasicBlock &MBB,
                                     MachineBasicBlock::iterator I,
                                     unsigned SrcReg, bool isKill, int FI,
                                     const TargetRegisterClass *RC,
                                     const TargetRegisterInfo *TRI,
                                     int64_t Offset) const {
  DebugLoc DL;
  MachineMemOperand *MMO = GetMemOperand(MBB, FI, MachineMemOperand::MOStore);
  unsigned Opc = 0;
  Opc = LCC::ST;
  assert(Opc && "Register class not handled!");
  BuildMI(MBB, I, DL, get(Opc))
      .addReg(SrcReg, getKillRegState(isKill))
      .addFrameIndex(FI)
      .addImm(Offset)
      .addMemOperand(MMO);
}

void LCCSEInstrInfo::loadRegFromStack(MachineBasicBlock &MBB,
                                      MachineBasicBlock::iterator I,
                                      unsigned DestReg, int FI,
                                      const TargetRegisterClass *RC,
                                      const TargetRegisterInfo *TRI,
                                      int64_t Offset) const {
  DebugLoc DL;
  if (I != MBB.end())
    DL = I->getDebugLoc();
  MachineMemOperand *MMO = GetMemOperand(MBB, FI, MachineMemOperand::MOLoad);
  unsigned Opc = 0;

  Opc = LCC::LD;
  assert(Opc && "Register class not handled!");
  BuildMI(MBB, I, DL, get(Opc), DestReg)
      .addFrameIndex(FI)
      .addImm(Offset)
      .addMemOperand(MMO);
}
