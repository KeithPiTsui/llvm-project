//===-- LCCMCTargetDesc.h - LCC Target Descriptions -----------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file provides LCC specific target descriptions.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_LCC_MCTARGETDESC_LCCMCTARGETDESC_H
#define LLVM_LIB_TARGET_LCC_MCTARGETDESC_LCCMCTARGETDESC_H


#include "llvm/Support/DataTypes.h"

namespace llvm {
class MCAsmBackend;
class MCCodeEmitter;
class MCContext;
class MCInstrInfo;
class MCObjectWriter;
class MCRegisterInfo;
class MCSubtargetInfo;
class StringRef;
class Target;
class Triple;
class raw_ostream;
class raw_pwrite_stream;

extern Target TheLCCTarget;
extern Target TheLCCelTarget;

} // End llvm namespace

// Defines symbolic names for LCC registers.  This defines a mapping from
// register name to register number.
#define GET_REGINFO_ENUM
#include "LCCGenRegisterInfo.inc"

// Defines symbolic names for the LCC instructions.
#define GET_INSTRINFO_ENUM
#include "LCCGenInstrInfo.inc"

#define GET_SUBTARGETINFO_ENUM
#include "LCCGenSubtargetInfo.inc"

#endif

