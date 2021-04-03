//===-- LCCSubtarget.cpp - LCC Subtarget Information --------------------===//
// This file implements the LCC specific subclass of TargetSubtargetInfo.
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
      // LCCGenSubtargetInfo will display features by llc -march=LCC
      // -mcpu=help
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
  if (TargetTriple.getArch() == Triple::LCC) {
    if (CPU.empty() || CPU == "generic") {
      CPU = "LCC32II";
    } else if (CPU == "help") {
      CPU = "";
      return *this;
    } else if (CPU != "LCC32I" && CPU != "LCC32II") {
      CPU = "LCC32II";
    }
  } else {
    errs() << "!!!Error, TargetTriple.getArch() = " << TargetTriple.getArch()
           << "CPU = " << CPU << "\n";
    exit(0);
  }

  if (CPU == "LCC32I")
    LCCArchVersion = LCC32I;
  else if (CPU == "LCC32II")
    LCCArchVersion = LCC32II;

  if (isLCC32I()) {
    HasCmp = true;
    HasSlt = false;
  } else if (isLCC32II()) {
    HasCmp = true;
    HasSlt = true;
  } else {
    errs() << "-mcpu must be empty(default:LCC32II), LCC32I or LCC32II"
           << "\n";
  }
  // Parse features string.
  ParseSubtargetFeatures(CPU, FS);
  // Initialize scheduling itinerary for the specified CPU.
  InstrItins = getInstrItineraryForCPU(CPU);
  return *this;
}

bool LCCSubtarget::abiUsesSoftFloat() const {
  //  return TM->Options.UseSoftFloat;
  return true;
}
const LCCABIInfo &LCCSubtarget::getABI() const { return TM.getABI(); }
