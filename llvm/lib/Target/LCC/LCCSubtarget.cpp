//===-- LCCSubtarget.cpp - LCC Subtarget Information --------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements the LCC specific subclass of TargetSubtargetInfo.
//
//===----------------------------------------------------------------------===//

#include "LCCSubtarget.h"
#include "LCC.h"
#include "LCCMachineFunction.h"
#include "LCCRegisterInfo.h"
#include "LCCTargetMachine.h"
#include "llvm/IR/Attributes.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/TargetRegistry.h"

using namespace llvm;

#define DEBUG_TYPE "LCC-subtarget"

#define GET_SUBTARGETINFO_TARGET_DESC
#define GET_SUBTARGETINFO_CTOR
#include "LCCGenSubtargetInfo.inc"

extern bool FixGlobalBaseReg;

void LCCSubtarget::anchor() {}

//@1 {
LCCSubtarget::LCCSubtarget(const Triple &TT, const std::string &CPU,
                           const std::string &FS, bool little,
                           const LCCTargetMachine &_TM)
    : //@1 }
      LCCGenSubtargetInfo(TT, CPU, FS), IsLittle(little), TM(_TM),
      TargetTriple(TT), TSInfo(),
      InstrInfo(
          LCCInstrInfo::create(initializeSubtargetDependencies(CPU, FS, TM))),
      FrameLowering(LCCFrameLowering::create(*this)),
      TLInfo(LCCTargetLowering::create(TM, *this)) {}

bool LCCSubtarget::isPositionIndependent() const {
  return TM.isPositionIndependent();
}

LCCSubtarget &
LCCSubtarget::initializeSubtargetDependencies(StringRef CPU, StringRef FS,
                                              const TargetMachine &TM) {
  ParseSubtargetFeatures(CPU, FS);
  InstrItins = getInstrItineraryForCPU(CPU);
  return *this;
}

bool LCCSubtarget::abiUsesSoftFloat() const { return true; }

const LCCABIInfo &LCCSubtarget::getABI() const { return TM.getABI(); }
