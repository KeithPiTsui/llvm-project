set(LLVM_TARGET_DEFINITIONS Tablegen/LCC.td)

tablegen(LLVM LCCGenRegisterInfo.inc -gen-register-info)
tablegen(LLVM LCCGenInstrInfo.inc -gen-instr-info)
tablegen(LLVM LCCGenCodeEmitter.inc -gen-emitter)
tablegen(LLVM LCCGenMCCodeEmitter.inc -gen-emitter)

tablegen(LLVM LCCGenAsmWriter.inc -gen-asm-writer)
tablegen(LLVM LCCGenDAGISel.inc -gen-dag-isel)
tablegen(LLVM LCCGenCallingConv.inc -gen-callingconv)
tablegen(LLVM LCCGenSubtargetInfo.inc -gen-subtarget)
tablegen(LLVM LCCGenMCPseudoLowering.inc -gen-pseudo-lowering)

# LCCCommonTableGen must be defined
add_public_tablegen_target(LCCCommonTableGen)

# LCCCodeGen should match with LLVMBuild.txt LCCCodeGen
add_llvm_target(LCCCodeGen
  LCCAsmPrinter.cpp
  LCCMCInstLower.cpp
  LCCFrameLowering.cpp
  LCCInstrInfo.cpp
  LCCISelLowering.cpp
  LCCMachineFunction.cpp
  LCCRegisterInfo.cpp
  LCCSEFrameLowering.cpp
  LCCSEInstrInfo.cpp
  LCCSEISelLowering.cpp
  LCCSERegisterInfo.cpp
  LCCSubtarget.cpp
  LCCTargetObjectFile.cpp
  LCCTargetMachine.cpp

  LCCISelDAGToDAG.cpp
  LCCSEISelDAGToDAG.cpp

  LCCAnalyzeImmediate.cpp
  )

# Should match with "subdirectories =  MCTargetDesc TargetInfo" in LLVMBuild.txt
#add_subdirectory(InstPrinter)
add_subdirectory(TargetInfo)
add_subdirectory(MCTargetDesc)

