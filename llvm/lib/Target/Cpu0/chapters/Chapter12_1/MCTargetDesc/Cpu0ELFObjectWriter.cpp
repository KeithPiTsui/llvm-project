//===-- Cpu0ELFObjectWriter.cpp - Cpu0 ELF Writer -------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "Cpu0Config.h"

#include "MCTargetDesc/Cpu0BaseInfo.h"
#include "MCTargetDesc/Cpu0FixupKinds.h"
#include "MCTargetDesc/Cpu0MCTargetDesc.h"
#include "llvm/MC/MCAssembler.h"
#include "llvm/MC/MCELFObjectWriter.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCSection.h"
#include "llvm/MC/MCValue.h"
#include "llvm/Support/ErrorHandling.h"
#include <list>

using namespace llvm;

namespace {
  class Cpu0ELFObjectWriter : public MCELFObjectTargetWriter {
  public:
    Cpu0ELFObjectWriter(uint8_t OSABI);

    ~Cpu0ELFObjectWriter() override;

    unsigned getRelocType(MCContext &Ctx, const MCValue &Target,
                        const MCFixup &Fixup, bool IsPCRel) const override;
    bool needsRelocateWithSymbol(const MCSymbol &Sym,
                                 unsigned Type) const override;
  };
}

Cpu0ELFObjectWriter::Cpu0ELFObjectWriter(uint8_t OSABI)
  : MCELFObjectTargetWriter(/*_is64Bit=false*/ false, OSABI, ELF::EM_CPU0,
                            /*HasRelocationAddend*/ false) {}

Cpu0ELFObjectWriter::~Cpu0ELFObjectWriter() {}

//@GetRelocType {
unsigned Cpu0ELFObjectWriter::getRelocType(MCContext &Ctx,
                                           const MCValue &Target,
                                           const MCFixup &Fixup,
                                           bool IsPCRel) const {
  // determine the type of the relocation
  unsigned Type = (unsigned)ELF::R_CPU0_NONE;
  unsigned Kind = (unsigned)Fixup.getKind();

  switch (Kind) {
  default:
    llvm_unreachable("invalid fixup kind!");
  case FK_Data_4:
    Type = ELF::R_CPU0_32;
    break;
  case FK_GPRel_4:
    Type = ELF::R_CPU0_GPREL32;
    break;
  case Cpu0::fixup_Cpu0_32:
    Type = ELF::R_CPU0_32;
    break;
  case Cpu0::fixup_Cpu0_GPREL16:
    Type = ELF::R_CPU0_GPREL16;
    break;
  case Cpu0::fixup_Cpu0_CALL16:
    Type = ELF::R_CPU0_CALL16;
    break;
  case Cpu0::fixup_Cpu0_GOT:
    Type = ELF::R_CPU0_GOT16;
    break;
  case Cpu0::fixup_Cpu0_HI16:
    Type = ELF::R_CPU0_HI16;
    break;
  case Cpu0::fixup_Cpu0_LO16:
    Type = ELF::R_CPU0_LO16;
    break;
  case Cpu0::fixup_Cpu0_TLSGD:
    Type = ELF::R_CPU0_TLS_GD;
    break;
  case Cpu0::fixup_Cpu0_GOTTPREL:
    Type = ELF::R_CPU0_TLS_GOTTPREL;
    break;
  case Cpu0::fixup_Cpu0_PC16:
    Type = ELF::R_CPU0_PC16;
    break;
  case Cpu0::fixup_Cpu0_PC24:
    Type = ELF::R_CPU0_PC24;
    break;
  case Cpu0::fixup_Cpu0_TP_HI:
    Type = ELF::R_CPU0_TLS_TP_HI16;
    break;
  case Cpu0::fixup_Cpu0_TP_LO:
    Type = ELF::R_CPU0_TLS_TP_LO16;
    break;
  case Cpu0::fixup_Cpu0_TLSLDM:
    Type = ELF::R_CPU0_TLS_LDM;
    break;
  case Cpu0::fixup_Cpu0_DTP_HI:
    Type = ELF::R_CPU0_TLS_DTP_HI16;
    break;
  case Cpu0::fixup_Cpu0_DTP_LO:
    Type = ELF::R_CPU0_TLS_DTP_LO16;
    break;
  case Cpu0::fixup_Cpu0_GOT_HI16:
    Type = ELF::R_CPU0_GOT_HI16;
    break;
  case Cpu0::fixup_Cpu0_GOT_LO16:
    Type = ELF::R_CPU0_GOT_LO16;
    break;
  }

  return Type;
}
//@GetRelocType }

bool
Cpu0ELFObjectWriter::needsRelocateWithSymbol(const MCSymbol &Sym,
                                             unsigned Type) const {
  // FIXME: This is extremelly conservative. This really needs to use a
  // whitelist with a clear explanation for why each realocation needs to
  // point to the symbol, not to the section.
  switch (Type) {
  default:
    return true;

  case ELF::R_CPU0_GOT16:
  // For Cpu0 pic mode, I think it's OK to return true but I didn't confirm.
  //  llvm_unreachable("Should have been handled already");
    return true;

  // These relocations might be paired with another relocation. The pairing is
  // done by the static linker by matching the symbol. Since we only see one
  // relocation at a time, we have to force them to relocate with a symbol to
  // avoid ending up with a pair where one points to a section and another
  // points to a symbol.
  case ELF::R_CPU0_HI16:
  case ELF::R_CPU0_LO16:
  // R_CPU0_32 should be a relocation record, I don't know why Mips set it to 
  // false.
  case ELF::R_CPU0_32:
    return true;

  case ELF::R_CPU0_GPREL16:
    return false;
  }
}

MCObjectWriter *llvm::createCpu0ELFObjectWriter(raw_pwrite_stream &OS,
                                                uint8_t OSABI,
                                                bool IsLittleEndian) {
  MCELFObjectTargetWriter *MOTW = new Cpu0ELFObjectWriter(OSABI);
  return createELFObjectWriter(MOTW, OS, IsLittleEndian);
}

