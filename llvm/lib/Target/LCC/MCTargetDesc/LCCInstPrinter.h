//=== LCCInstPrinter.h - Convert LCC MCInst to assembly syntax -*- C++ -*-==//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This class prints a LCC MCInst to a .s file.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_LCC_INSTPRINTER_LCCINSTPRINTER_H
#define LLVM_LIB_TARGET_LCC_INSTPRINTER_LCCINSTPRINTER_H



#include "llvm/MC/MCInstPrinter.h"

namespace llvm {
// These enumeration declarations were orignally in LCCInstrInfo.h but
// had to be moved here to avoid circular dependencies between
// LLVMLCCCodeGen and LLVMLCCAsmPrinter.

class TargetMachine;

class LCCInstPrinter : public MCInstPrinter {
public:
  LCCInstPrinter(const MCAsmInfo &MAI, const MCInstrInfo &MII,
                  const MCRegisterInfo &MRI)
      : MCInstPrinter(MAI, MII, MRI) {}

  // Autogenerated by tblgen.
  std::pair<const char *, uint64_t> getMnemonic(const MCInst *MI) override;
  void printInstruction(const MCInst *MI, uint64_t Address, raw_ostream &O);
  static const char *getRegisterName(unsigned RegNo);

  void printRegName(raw_ostream &OS, unsigned RegNo) const override;

  /// Print the specified MCInst to the specified raw_ostream.
  ///
  /// \p Address the address of current instruction on most targets, used to
  /// print a PC relative immediate as the target address. On targets where a PC
  /// relative immediate is relative to the next instruction and the length of a
  /// MCInst is difficult to measure (e.g. x86), this is the address of the next
  /// instruction. If Address is 0, the immediate will be printed.
  void printInst(const MCInst *MI, uint64_t Address, StringRef Annot,
                 const MCSubtargetInfo &STI, raw_ostream &OS) override;

  // void printInst(const MCInst *MI, raw_ostream &O, StringRef Annot,
  //                const MCSubtargetInfo &STI) override;

  bool printAliasInstr(const MCInst *MI, uint64_t Address, raw_ostream &OS);
  void printCustomAliasOperand(const MCInst *MI, uint64_t Address,
                               unsigned OpIdx, unsigned PrintMethodIdx,
                               raw_ostream &OS);
  // void printCustomAliasOperand(const MCInst *MI,
  //                                                   unsigned OpIdx,
  //                                                   unsigned PrintMethodIdx,
  //                                                   raw_ostream &O);

private:
  void printOperand(const MCInst *MI, unsigned OpNo, raw_ostream &O);
  void printOperand(const MCInst *MI, uint64_t /*Address*/, unsigned OpNum,
                    raw_ostream &O) {
    printOperand(MI, OpNum, O);
  }
  void printUnsignedImm(const MCInst *MI, int opNum, raw_ostream &O);
  void printMemOperand(const MCInst *MI, int opNum, raw_ostream &O);
  //#if CH >= CH7_1
  void printMemOperandEA(const MCInst *MI, int opNum, raw_ostream &O);
  //#endif
};
} // end namespace llvm

#endif
