//===-- LCCSEISelDAGToDAG.h - A Dag to Dag Inst Selector for LCCSE -----===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// Subclass of LCCDAGToDAGISel specialized for LCC32.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_LCC_LCCSEISELDAGTODAG_H
#define LLVM_LIB_TARGET_LCC_LCCSEISELDAGTODAG_H



#include "LCCISelDAGToDAG.h"

namespace llvm {

class LCCSEDAGToDAGISel : public LCCDAGToDAGISel {

public:
  explicit LCCSEDAGToDAGISel(LCCTargetMachine &TM, CodeGenOpt::Level OL)
      : LCCDAGToDAGISel(TM, OL) {}

private:
  bool runOnMachineFunction(MachineFunction &MF) override;

  bool trySelect(SDNode *Node) override;

  void processFunctionAfterISel(MachineFunction &MF) override;

  // Insert instructions to initialize the global base register in the
  // first MBB of the function.
  //  void initGlobalBaseReg(MachineFunction &MF);
};

FunctionPass *createLCCSEISelDag(LCCTargetMachine &TM,
                                  CodeGenOpt::Level OptLevel);

} // namespace llvm

#endif
