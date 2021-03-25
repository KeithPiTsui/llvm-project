# llvm/lib/Target/LCC/CMakeLists.txt
# LCCCodeGen should match with LLVMBuild.txt NTCodeGen
add_llvm_target(LCCCodeGen
  LCCTargetMachine.cpp
  )
# Should match with "subdirectories =  MCTargetDesc TargetInfo" in LLVMBuild.txt
add_subdirectory(TargetInfo)
add_subdirectory(MCTargetDesc)
