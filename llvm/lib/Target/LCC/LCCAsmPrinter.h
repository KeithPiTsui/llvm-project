//===-- LCCAsmPrinter.h - LCC LLVM Assembly Printer ----------*- C++ -*--===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// LCC Assembly printer class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_LCC_LCCASMPRINTER_H
#define LLVM_LIB_TARGET_LCC_LCCASMPRINTER_H



#include "LCCMCInstLower.h"
#include "LCCMachineFunction.h"
#include "LCCSubtarget.h"
#include "LCCTargetMachine.h"
#include "llvm/CodeGen/AsmPrinter.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/Support/Compiler.h"
#include "llvm/Target/TargetMachine.h"

namespace llvm {
class MCStreamer;
class MachineInstr;
class MachineBasicBlock;
class Module;
class raw_ostream;

class LLVM_LIBRARY_VISIBILITY LCCAsmPrinter : public AsmPrinter {

  void emitInstrWithMacroNoAT(const MachineInstr *MI);

private:
  // lowerOperand - Convert a MachineOperand into the equivalent MCOperand.
  bool lowerOperand(const MachineOperand &MO, MCOperand &MCOp);

public:
  const LCCSubtarget *Subtarget;
  const LCCFunctionInfo *LCCFI;
  LCCMCInstLower MCInstLowering;

  explicit LCCAsmPrinter(TargetMachine &TM,
                          std::unique_ptr<MCStreamer> Streamer)
      : AsmPrinter(TM, std::move(Streamer)), MCInstLowering(*this) {
    Subtarget = static_cast<LCCTargetMachine &>(TM).getSubtargetImpl();
  }

  virtual llvm::StringRef getPassName() const override {
    return "LCC Assembly Printer";
  }

  virtual bool runOnMachineFunction(MachineFunction &MF) override;

  //- EmitInstruction() must exists or will have run time error.
  void emitInstruction(const MachineInstr *MI) override;
  void printSavedRegsBitmask(raw_ostream &O);
  void printHex32(unsigned int Value, raw_ostream &O);
  void emitFrameDirective();
  const char *getCurrentABIString() const;
  void emitFunctionEntryLabel() override;
  void emitFunctionBodyStart() override;
  void emitFunctionBodyEnd() override;
  void emitStartOfAsmFile(Module &M) override;
  void printDebugValueComment(const MachineInstr *MI, raw_ostream &OS);
};
} // namespace llvm

#endif
