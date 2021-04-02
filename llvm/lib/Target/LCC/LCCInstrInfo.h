//===-- LCCInstrInfo.h - LCC Instruction Information ----------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the LCC implementation of the TargetInstrInfo class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_LCC_LCCINSTRINFO_H
#define LLVM_LIB_TARGET_LCC_LCCINSTRINFO_H



#include "LCC.h"
#include "LCCRegisterInfo.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/TargetInstrInfo.h"

#define GET_INSTRINFO_HEADER
#include "LCCGenInstrInfo.inc"

namespace llvm {

class LCCInstrInfo : public LCCGenInstrInfo {
  virtual void anchor();
protected:
  const LCCSubtarget &Subtarget;
public:
  explicit LCCInstrInfo(const LCCSubtarget &STI);

  static const LCCInstrInfo *create(LCCSubtarget &STI);

  /// getRegisterInfo - TargetInstrInfo is a superset of MRegister info.  As
  /// such, whenever a client has an instance of instruction info, it should
  /// always be able to get register info as well (through this method).
  ///
  virtual const LCCRegisterInfo &getRegisterInfo() const = 0;

  /// Return the number of bytes of code the specified instruction may be.
  unsigned GetInstSizeInBytes(const MachineInstr &MI) const;

protected:
};
const LCCInstrInfo *createLCCSEInstrInfo(const LCCSubtarget &STI);
}

#endif

