//===- Cpu0InstrInfo.td - Target Description for Cpu0 Target -*- tablegen -*-=//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the Cpu0 implementation of the TargetInstrInfo class.
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// Cpu0 profiles and nodes
//===----------------------------------------------------------------------===//

// Reference for SDTypeProfile here:
// https://github.com/llvm-mirror/llvm/blob/master/include/llvm/Target/TargetSelectionDAG.td
// DAG is composed of SDNodes. SDNode, PatLeaf and SDNodeXForm are in the same link above.

def SDT_Cpu0Ret          : SDTypeProfile<0, 1, [SDTCisInt<0>]>;

// Return
def Cpu0Ret : SDNode<"Cpu0ISD::Ret", SDTNone,
                     [SDNPHasChain, SDNPOptInGlue, SDNPVariadic]>;

//===----------------------------------------------------------------------===//
// Cpu0 Instruction Predicate Definitions.
//===----------------------------------------------------------------------===//

def Ch3_1       :     Predicate<"Subtarget->hasChapter3_1()">,
                      AssemblerPredicate<(all_of FeatureChapter3_1)>;
def Ch3_2       :     Predicate<"Subtarget->hasChapter3_2()">,
                      AssemblerPredicate<(all_of FeatureChapter3_2)>;
def Ch3_3       :     Predicate<"Subtarget->hasChapter3_3()">,
                      AssemblerPredicate<(all_of FeatureChapter3_3)>;
def Ch3_4       :     Predicate<"Subtarget->hasChapter3_4()">,
                      AssemblerPredicate<(all_of FeatureChapter3_4)>;
def Ch3_5       :     Predicate<"Subtarget->hasChapter3_5()">,
                      AssemblerPredicate<(all_of FeatureChapter3_5)>;
def Ch4_1       :     Predicate<"Subtarget->hasChapter4_1()">,
                      AssemblerPredicate<(all_of FeatureChapter4_1)>;
def Ch4_2       :     Predicate<"Subtarget->hasChapter4_2()">,
                      AssemblerPredicate<(all_of FeatureChapter4_2)>;
def Ch5_1       :     Predicate<"Subtarget->hasChapter5_1()">,
                      AssemblerPredicate<(all_of FeatureChapter5_1)>;
def Ch6_1       :     Predicate<"Subtarget->hasChapter6_1()">,
                      AssemblerPredicate<(all_of FeatureChapter6_1)>;
def Ch7_1       :     Predicate<"Subtarget->hasChapter7_1()">,
                      AssemblerPredicate<(all_of FeatureChapter7_1)>;
def Ch8_1       :     Predicate<"Subtarget->hasChapter8_1()">,
                      AssemblerPredicate<(all_of FeatureChapter8_1)>;
def Ch8_2       :     Predicate<"Subtarget->hasChapter8_2()">,
                      AssemblerPredicate<(all_of FeatureChapter8_2)>;
def Ch9_1       :     Predicate<"Subtarget->hasChapter9_1()">,
                      AssemblerPredicate<(all_of FeatureChapter9_1)>;
def Ch9_2       :     Predicate<"Subtarget->hasChapter9_2()">,
                      AssemblerPredicate<(all_of FeatureChapter9_2)>;
def Ch9_3       :     Predicate<"Subtarget->hasChapter9_3()">,
                      AssemblerPredicate<(all_of FeatureChapter9_3)>;
def Ch10_1      :     Predicate<"Subtarget->hasChapter10_1()">,
                      AssemblerPredicate<(all_of FeatureChapter10_1)>;
def Ch11_1      :     Predicate<"Subtarget->hasChapter11_1()">,
                      AssemblerPredicate<(all_of FeatureChapter11_1)>;
def Ch11_2      :     Predicate<"Subtarget->hasChapter11_2()">,
                      AssemblerPredicate<(all_of FeatureChapter11_2)>;
def Ch12_1      :     Predicate<"Subtarget->hasChapter12_1()">,
                      AssemblerPredicate<(all_of FeatureChapter12_1)>;
def Ch_all      :     Predicate<"Subtarget->hasChapterAll()">,
                      AssemblerPredicate<(all_of FeatureChapterAll)>;

def EnableOverflow  : Predicate<"Subtarget->enableOverflow()">;
def DisableOverflow : Predicate<"Subtarget->disableOverflow()">;

def HasCmp      :     Predicate<"Subtarget->hasCmp()">;
def HasSlt      :     Predicate<"Subtarget->hasSlt()">;

//===----------------------------------------------------------------------===//
// Instruction format superclass
//===----------------------------------------------------------------------===//

include "Cpu0InstrFormats.td"

//===----------------------------------------------------------------------===//
// Cpu0 Operand, Complex Patterns and Transformations Definitions.
//===----------------------------------------------------------------------===//
// Instruction operand types

// Signed Operand
def simm16      : Operand<i32> {
// LLVM supply the “let DecoderMethod” keyword to allow programmers implement 
// their decode function.
// LLVM will call these DecodeMethod when user uses Disassembler tools, such as
// llvm-objdump -d.
  let DecoderMethod= "DecodeSimm16";
}

// Address operand
def mem : Operand<iPTR> {
// Making llvm call function Cpu0InstPrinter::printMemOperand()
  let PrintMethod = "printMemOperand";
// For example, ld $2, 4($sp), the mem operand is 4($sp):(ops CPURegs, simm16). 
  let MIOperandInfo = (ops CPURegs, simm16);
// Making llvm call function getMemEncoding() when either ld or st instruction is 
// issued in elf obj since these two instructions use mem Operand.
  let EncoderMethod = "getMemEncoding";
}

// Node immediate fits as 16-bit sign extended on target immediate.
// e.g. addi, andi
def immSExt16  : PatLeaf<(imm), [{ return isInt<16>(N->getSExtValue()); }]>;

// As TableGen is unable to generate selection code for complex addressing modes,
// selection of a MEMbol operand must be done manually. 
// Reference https://opus4.kobv.de/opus4-fau/files/1108/tricore_llvm.pdf
// Cpu0 Address Mode! SDNode frameindex could possibily be a match
// since load and store instructions from stack used it.
def addr : 
  ComplexPattern<iPTR, 2, "SelectAddr", [frameindex], [SDNPWantParent]>;

//===----------------------------------------------------------------------===//
// Pattern fragment for load/store
//===----------------------------------------------------------------------===//

class AlignedLoad<PatFrag Node> :
  PatFrag<(ops node:$ptr), (Node node:$ptr), [{
  LoadSDNode *LD = cast<LoadSDNode>(N);
  return LD->getMemoryVT().getSizeInBits()/8 <= LD->getAlignment();
}]>;

class AlignedStore<PatFrag Node> :
  PatFrag<(ops node:$val, node:$ptr), (Node node:$val, node:$ptr), [{
  StoreSDNode *SD = cast<StoreSDNode>(N);
  return SD->getMemoryVT().getSizeInBits()/8 <= SD->getAlignment();
}]>;

// Load/Store PatFrags.
def load_a          : AlignedLoad<load>;
def store_a         : AlignedStore<store>;

//===----------------------------------------------------------------------===//
// Instructions specific format
//===----------------------------------------------------------------------===//

// Arithmetic and logical instructions with 2 register operands.
class ArithLogicI<bits<8> op, string instr_asm, SDNode OpNode,
                  Operand Od, PatLeaf imm_type, RegisterClass RC> :
  FL<op, (outs GPROut:$ra), (ins RC:$rb, Od:$imm16),
     !strconcat(instr_asm, "\t$ra, $rb, $imm16"),
     [(set GPROut:$ra, (OpNode RC:$rb, imm_type:$imm16))], IIAlu> {
  let isReMaterializable = 1;
}

class FMem<bits<8> op, dag outs, dag ins, string asmstr, list<dag> pattern,
          InstrItinClass itin>: FL<op, outs, ins, asmstr, pattern, itin> {
  bits<20> addr;
  let Inst{19-16} = addr{19-16};
  let Inst{15-0}  = addr{15-0};
  let DecoderMethod = "DecodeMem";
}

// Memory Load/Store
let canFoldAsLoad = 1 in
class LoadM<bits<8> op, string instr_asm, PatFrag OpNode, RegisterClass RC,
            Operand MemOpnd, bit Pseudo>:
  FMem<op, (outs RC:$ra), (ins MemOpnd:$addr),
     !strconcat(instr_asm, "\t$ra, $addr"),
     [(set RC:$ra, (OpNode addr:$addr))], IILoad> {
  let isPseudo = Pseudo;
}

class StoreM<bits<8> op, string instr_asm, PatFrag OpNode, RegisterClass RC,
             Operand MemOpnd, bit Pseudo>:
  FMem<op, (outs), (ins RC:$ra, MemOpnd:$addr),
     !strconcat(instr_asm, "\t$ra, $addr"),
     [(OpNode RC:$ra, addr:$addr)], IIStore> {
  let isPseudo = Pseudo;
}

//@ 32-bit load.
multiclass LoadM32<bits<8> op, string instr_asm, PatFrag OpNode,
                   bit Pseudo = 0> {
// When defm LD : LoadM32<...>, then #NAME# is LD
// When defm LB : LoadM32<...>, then #NAME# is LB
  def  : LoadM<op, instr_asm, OpNode, GPROut, mem, Pseudo>;
}

// 32-bit store.
multiclass StoreM32<bits<8> op, string instr_asm, PatFrag OpNode,
                    bit Pseudo = 0> {
  def  : StoreM<op, instr_asm, OpNode, CPURegs, mem, Pseudo>;
}

//@JumpFR {
let isBranch=1, isTerminator=1, isBarrier=1, imm16=0, hasDelaySlot = 1,
    isIndirectBranch = 1 in
class JumpFR<bits<8> op, string instr_asm, RegisterClass RC>:
  FL<op, (outs), (ins RC:$ra),
     !strconcat(instr_asm, "\t$ra"), [(brind RC:$ra)], IIBranch> {
  let rb = 0;
  let imm16 = 0;
}
//@JumpFR }

// Return instruction
class RetBase<RegisterClass RC>: JumpFR<0x3c, "ret", RC> {
  let isReturn = 1;
  let isCodeGenOnly = 1;
  let hasCtrlDep = 1;
  let hasExtraSrcRegAllocReq = 1;
}

  
//===----------------------------------------------------------------------===//
// Instruction definition
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// Cpu0 Instructions
//===----------------------------------------------------------------------===//

/// Load and Store Instructions
///  aligned
defm LD     : LoadM32<0x01,  "ld",  load_a>;
defm ST     : StoreM32<0x02, "st",  store_a>;

/// Arithmetic Instructions (ALU Immediate)
// IR "add" defined in include/llvm/Target/TargetSelectionDAG.td, line 315 (def add).
def ADDiu   : ArithLogicI<0x09, "addiu", add, simm16, immSExt16, CPURegs>;

/// Arithmetic Instructions (3-Operand, R-Type)

/// Shift Instructions

def JR      : JumpFR<0x3c, "jr", GPROut>;

def RET     : RetBase<GPROut>;

/// No operation
let addr=0 in
  def NOP   : FJ<0, (outs), (ins), "nop", [], IIAlu>;

//===----------------------------------------------------------------------===//
//  Arbitrary patterns that map to one or more instructions
//===----------------------------------------------------------------------===//

// Small immediates
def : Pat<(i32 immSExt16:$in),
          (ADDiu ZERO, imm:$in)>;

