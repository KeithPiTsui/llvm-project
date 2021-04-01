set(LLVM_TARGET_DEFINITIONS LCCOther.td)

# Generate LCCGenRegisterInfo.inc and LCCGenInstrInfo.inc which included by
#  your hand code C++ files.
# LCCGenRegisterInfo.inc came from LCCRegisterInfo.td, LCCGenInstrInfo.inc
#  came from LCCInstrInfo.td.
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
  )

# Should match with "subdirectories =  MCTargetDesc TargetInfo" in LLVMBuild.txt
#add_subdirectory(InstPrinter)
add_subdirectory(TargetInfo)
add_subdirectory(MCTargetDesc)
