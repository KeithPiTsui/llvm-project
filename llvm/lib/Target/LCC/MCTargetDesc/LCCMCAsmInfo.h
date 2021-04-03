//===-- LCCMCAsmInfo.h - LCC Asm Info ------------------------*- C++ -*--===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the declaration of the LCCMCAsmInfo class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_LCC_MCTARGETDESC_LCCMCASMINFO_H
#define LLVM_LIB_TARGET_LCC_MCTARGETDESC_LCCMCASMINFO_H



#include "llvm/MC/MCAsmInfoELF.h"

namespace llvm {
  class Triple;

  class LCCMCAsmInfo : public MCAsmInfoELF {
    void anchor() override;
  public:
    explicit LCCMCAsmInfo(const Triple &TheTriple);
  };

} // namespace llvm

#endif

