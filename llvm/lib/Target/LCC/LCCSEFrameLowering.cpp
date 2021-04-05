//===-- LCCSEFrameLowering.cpp - LCC Frame Information ------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the LCC implementation of TargetFrameLowering class.
//
//===----------------------------------------------------------------------===//

#include "LCCSEFrameLowering.h"
#include "LCCMachineFunction.h"
#include "LCCSEInstrInfo.h"
#include "LCCSubtarget.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineModuleInfo.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/RegisterScavenging.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/Function.h"
#include "llvm/MC/MCDwarf.h"
#include "llvm/MC/MachineLocation.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Target/TargetOptions.h"
#include <cstddef>

using namespace llvm;

LCCSEFrameLowering::LCCSEFrameLowering(const LCCSubtarget &STI)
    : LCCFrameLowering(STI, STI.stackAlignment()) {}

//@emitPrologue {
void LCCSEFrameLowering::emitPrologue(MachineFunction &MF,
                                      MachineBasicBlock &MBB) const {
  assert(&MF.front() == &MBB && "Shrink-wrapping not yet supported");
  MachineFrameInfo &MFI = MF.getFrameInfo();
  LCCFunctionInfo *LCCFI = MF.getInfo<LCCFunctionInfo>();
  const LCCSEInstrInfo &TII =
      *static_cast<const LCCSEInstrInfo *>(STI.getInstrInfo());
  const LCCRegisterInfo &RegInfo =
      *static_cast<const LCCRegisterInfo *>(STI.getRegisterInfo());

  MachineBasicBlock::iterator MBBI = MBB.begin();
  DebugLoc dl = MBBI != MBB.end() ? MBBI->getDebugLoc() : DebugLoc();
  LCCABIInfo ABI = STI.getABI();
  unsigned SP = LCC::SP;
  const TargetRegisterClass *RC = &LCC::GPROutRegClass;

  // First, compute final stack size.
  uint64_t StackSize = MFI.getStackSize();

  // No need to allocate space on the stack.
  if (StackSize == 0 && !MFI.adjustsStack())
    return;

  MachineModuleInfo &MMI = MF.getMMI();
  const MCRegisterInfo *MRI = MMI.getContext().getRegisterInfo();
  MachineLocation DstML, SrcML;

  // Adjust stack.
  TII.adjustStackPtr(SP, -StackSize, MBB, MBBI);

  // emit ".cfi_def_cfa_offset StackSize"  //FIXME:
  unsigned CFIIndex = MF.addFrameInst(
      MCCFIInstruction::createAdjustCfaOffset(nullptr, -StackSize));
  BuildMI(MBB, MBBI, dl, TII.get(TargetOpcode::CFI_INSTRUCTION))
      .addCFIIndex(CFIIndex);

  const std::vector<CalleeSavedInfo> &CSI = MFI.getCalleeSavedInfo();

  if (CSI.size()) {
    // Find the instruction past the last instruction that saves a callee-saved
    // register to the stack.
    for (unsigned i = 0; i < CSI.size(); ++i)
      ++MBBI;

    // Iterate over list of callee-saved registers and emit .cfi_offset
    // directives.
    for (std::vector<CalleeSavedInfo>::const_iterator I = CSI.begin(),
                                                      E = CSI.end();
         I != E; ++I) {
      int64_t Offset = MFI.getObjectOffset(I->getFrameIdx());
      unsigned Reg = I->getReg();
      {
        // Reg is in CPURegs.
        unsigned CFIIndex = MF.addFrameInst(MCCFIInstruction::createOffset(
            nullptr, MRI->getDwarfRegNum(Reg, 1), Offset)); // FIXME
        BuildMI(MBB, MBBI, dl, TII.get(TargetOpcode::CFI_INSTRUCTION))
            .addCFIIndex(CFIIndex);
      }
    }
  }
}
//}

//@emitEpilogue {
void LCCSEFrameLowering::emitEpilogue(MachineFunction &MF,
                                      MachineBasicBlock &MBB) const {
  MachineBasicBlock::iterator MBBI = MBB.getLastNonDebugInstr();
  MachineFrameInfo &MFI = MF.getFrameInfo();
  LCCFunctionInfo *LCCFI = MF.getInfo<LCCFunctionInfo>();

  const LCCSEInstrInfo &TII =
      *static_cast<const LCCSEInstrInfo *>(STI.getInstrInfo());
  const LCCRegisterInfo &RegInfo =
      *static_cast<const LCCRegisterInfo *>(STI.getRegisterInfo());

  DebugLoc dl = MBBI->getDebugLoc();
  LCCABIInfo ABI = STI.getABI();
  unsigned SP = LCC::SP;

  // Get the number of bytes from FrameInfo
  uint64_t StackSize = MFI.getStackSize();

  if (!StackSize)
    return;

  // Adjust stack.
  TII.adjustStackPtr(SP, StackSize, MBB, MBBI);
}
//}

bool LCCSEFrameLowering::hasReservedCallFrame(const MachineFunction &MF) const {
  const MachineFrameInfo &MFI = MF.getFrameInfo();

  // Reserve call frame if the size of the maximum call frame fits into 16-bit
  // immediate field and there are no variable sized objects on the stack.
  // Make sure the second register scavenger spill slot can be accessed with one
  // instruction.
  return isInt<16>(MFI.getMaxCallFrameSize() + getStackAlignment()) &&
         !MFI.hasVarSizedObjects();
}

const LCCFrameLowering *llvm::createLCCSEFrameLowering(const LCCSubtarget &ST) {
  return new LCCSEFrameLowering(ST);
}

// This method is called immediately before PrologEpilogInserter scans the
//  physical registers used to determine what callee saved registers should be
//  spilled. This method is optional.
void LCCSEFrameLowering::determineCalleeSaves(MachineFunction &MF,
                                              BitVector &SavedRegs,
                                              RegScavenger *RS) const {
  //@determineCalleeSaves-body
  TargetFrameLowering::determineCalleeSaves(MF, SavedRegs, RS);
  LCCFunctionInfo *LCCFI = MF.getInfo<LCCFunctionInfo>();
  MachineRegisterInfo &MRI = MF.getRegInfo();
  if (MF.getFrameInfo().hasCalls())
    ;
  // setAliasRegs(MF, SavedRegs, LCC::LR); // FIXME
  return;
}
