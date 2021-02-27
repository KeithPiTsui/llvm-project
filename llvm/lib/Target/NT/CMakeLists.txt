# llvm/lib/Target/NT/CMakeLists.txt
# NTCodeGen should match with LLVMBuild.txt NTCodeGen
add_llvm_target(NTCodeGen
  NTTargetMachine.cpp
  )
# Should match with "subdirectories =  MCTargetDesc TargetInfo" in LLVMBuild.txt
add_subdirectory(TargetInfo)
add_subdirectory(MCTargetDesc)
