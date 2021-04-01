//===---- LCCABIInfo.cpp - Information about CPU0 ABI's ------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "LCCABIInfo.h"
#include "LCCRegisterInfo.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/ADT/StringSwitch.h"
#include "llvm/MC/MCTargetOptions.h"
#include "llvm/Support/CommandLine.h"

using namespace llvm;

static cl::opt<bool>
EnableLCCS32Calls("cpu0-s32-calls", cl::Hidden,
                    cl::desc("CPU0 S32 call: use stack only to pass arguments.\
                    "), cl::init(false));

namespace {
static const MCPhysReg O32IntRegs[4] = {LCC::A0, LCC::A1};
static const MCPhysReg S32IntRegs = {};
}

const ArrayRef<MCPhysReg> LCCABIInfo::GetByValArgRegs() const {
  if (IsO32())
    return makeArrayRef(O32IntRegs);
  if (IsS32())
    return makeArrayRef(S32IntRegs);
  llvm_unreachable("Unhandled ABI");
}

const ArrayRef<MCPhysReg> LCCABIInfo::GetVarArgRegs() const {
  if (IsO32())
    return makeArrayRef(O32IntRegs);
  if (IsS32())
    return makeArrayRef(S32IntRegs);
  llvm_unreachable("Unhandled ABI");
}

unsigned LCCABIInfo::GetCalleeAllocdArgSizeInBytes(CallingConv::ID CC) const {
  if (IsO32())
    return CC != 0;
  if (IsS32())
    return 0;
  llvm_unreachable("Unhandled ABI");
}

LCCABIInfo LCCABIInfo::computeTargetABI() {
  LCCABIInfo abi(ABI::Unknown);

  if (EnableLCCS32Calls)
    abi = ABI::S32;
  else
    abi = ABI::O32;
  // Assert exactly one ABI was chosen.
  assert(abi.ThisABI != ABI::Unknown);

  return abi;
}

unsigned LCCABIInfo::GetStackPtr() const {
  return LCC::SP;
}

unsigned LCCABIInfo::GetFramePtr() const {
  return LCC::FP;
}

unsigned LCCABIInfo::GetNullPtr() const {
  return LCC::ZERO;
}

unsigned LCCABIInfo::GetEhDataReg(unsigned I) const {
  static const unsigned EhDataReg[] = {
    LCC::A0, LCC::A1
  };

  return EhDataReg[I];
}

int LCCABIInfo::EhDataRegSize() const {
  if (ThisABI == ABI::S32)
    return 0;
  else
    return 2;
}

