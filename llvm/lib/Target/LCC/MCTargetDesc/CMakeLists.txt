# MCTargetDesc/CMakeLists.txt
add_llvm_library(LLVMLCCDesc
  LCCABIInfo.cpp
  LCCMCAsmInfo.cpp
  LCCMCTargetDesc.cpp
  LCCInstPrinter.cpp
  )
