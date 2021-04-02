//===-- LCCMachineFunctionInfo.h - Private data used for LCC ----*- C++ -*-=//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file declares the LCC specific subclass of MachineFunctionInfo.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_LCC_LCCMACHINEFUNCTION_H
#define LLVM_LIB_TARGET_LCC_LCCMACHINEFUNCTION_H

#include "LCCConfig.h"

#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineMemOperand.h"
#include "llvm/CodeGen/PseudoSourceValue.h"
#include "llvm/CodeGen/TargetFrameLowering.h"
#include "llvm/Target/TargetMachine.h"
#include <map>

namespace llvm {

//@1 {
/// LCCFunctionInfo - This class is derived from MachineFunction private
/// LCC target-specific information for each MachineFunction.
class LCCFunctionInfo : public MachineFunctionInfo {
public:
  LCCFunctionInfo(MachineFunction &MF)
      : MF(MF), VarArgsFrameIndex(0), MaxCallFrameSize(0) {}

  ~LCCFunctionInfo();

  int getVarArgsFrameIndex() const { return VarArgsFrameIndex; }
  void setVarArgsFrameIndex(int Index) { VarArgsFrameIndex = Index; }

private:
  virtual void anchor();

  MachineFunction &MF;

  /// VarArgsFrameIndex - FrameIndex for start of varargs area.
  int VarArgsFrameIndex;

  unsigned MaxCallFrameSize;
};
//@1 }

} // end of namespace llvm

#endif // LCC_MACHINE_FUNCTION_INFO_H
