set(LLVM_TARGET_DEFINITIONS Cpu0Other.td)

# Generate Cpu0GenRegisterInfo.inc and Cpu0GenInstrInfo.inc which included by
#  your hand code C++ files.
# Cpu0GenRegisterInfo.inc came from Cpu0RegisterInfo.td, Cpu0GenInstrInfo.inc
#  came from Cpu0InstrInfo.td.
tablegen(LLVM Cpu0GenRegisterInfo.inc -gen-register-info)
tablegen(LLVM Cpu0GenInstrInfo.inc -gen-instr-info)
tablegen(LLVM Cpu0GenDAGISel.inc -gen-dag-isel)
tablegen(LLVM Cpu0GenCallingConv.inc -gen-callingconv)
tablegen(LLVM Cpu0GenSubtargetInfo.inc -gen-subtarget)
tablegen(LLVM Cpu0GenMCPseudoLowering.inc -gen-pseudo-lowering)

# Cpu0CommonTableGen must be defined
add_public_tablegen_target(Cpu0CommonTableGen)

# Cpu0CodeGen should match with LLVMBuild.txt Cpu0CodeGen
add_llvm_target(Cpu0CodeGen
  Cpu0TargetMachine.cpp     ## class Cpu0TargetMachine
  Cpu0Subtarget.cpp         ## class Cpu0Subtarget

  Cpu0ISelLowering.cpp      ## class Cpu0TargeLowering
  Cpu0InstrInfo.cpp         ## class Cpu0InstrInfo
  Cpu0RegisterInfo.cpp      ## class Cpu0RegisterInfo
  Cpu0FrameLowering.cpp     ## class Cpu0FrameLowering

  Cpu0SEISelLowering.cpp    ## class Cpu0SETargetLowering
  Cpu0SEInstrInfo.cpp       ## class Cpu0SEInstrInfo
  Cpu0SERegisterInfo.cpp    ## class Cpu0SERegisterInfo
  Cpu0SEFrameLowering.cpp   ## class Cpu0SEFrameLowering

  Cpu0TargetObjectFile.cpp  ## class Cpu0TargetObjectFile
  Cpu0MachineFunction.cpp   ## class Cpu0MachineFunction: MachineFunctionInfo

  )

# Should match with "subdirectories =  MCTargetDesc TargetInfo" in LLVMBuild.txt
add_subdirectory(TargetInfo)
add_subdirectory(MCTargetDesc)
