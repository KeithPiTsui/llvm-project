//===- LCCInstrInfo.td - Target Description for LCC Target -*- tablegen -*-=//
// This file contains the LCC implementation of the TargetInstrInfo class.
//===----------------------------------------------------------------------===//
//===----------------------------------------------------------------------===//
// Instruction format superclass
//===----------------------------------------------------------------------===//
// Format specifies the encoding used by the instruction.  This is part of the
// ad-hoc solution used to emit machine instruction encodings by our machine
// code emitter.
class Format<bits<4> val> {
  bits<4> Value = val;
}

// Generic LCC Format
class LCCInst<dag outs, dag ins, string asmstr, list<dag> pattern,
               InstrItinClass itin, Format f>: Instruction {
  field bits<16> Inst;
  Format Form = f;
  let Namespace = "LCC";
  let Size = 2;
  bits<4> Opcode = 0;
  let Inst{15-12} = Opcode;
  let Inst{11-0} = 0;
  let OutOperandList = outs;
  let InOperandList  = ins;
  let AsmString   = asmstr;
  let Pattern     = pattern;
  let Itinerary   = itin;
  bits<4> FormBits = Form.Value;
  let TSFlags{3-0}   = FormBits; // Target-specific flags
  let DecoderNamespace = "LCC";
  field bits<32> SoftFail = 0;
}

def Pseudo    : Format<0>; // LCC Pseudo Instructions Format
class LCCPseudo<dag outs, dag ins, string asmstr, list<dag> pattern>:
      LCCInst<outs, ins, asmstr, pattern, IIPseudo, Pseudo> {
  let isCodeGenOnly = 1;
  let isPseudo = 1;
}

def FrmBr     : Format<1>; // format for Branch, <opcode> <condition> <imm9>
class FBr <bits<4> op, bits<3> cond, dag outs, dag ins, string asmstr,
         list<dag> pattern, InstrItinClass itin>:
      LCCInst<outs, ins, asmstr, pattern, itin, FrmBr> {
  bits<9> imm9;
  let Opcode = op;
  let Inst{11-9} = cond;
  let Inst{8-0}  = imm9;
}


def FrmTrap   : Format<2>; // format for Trap, <opcode> '0000' <trapVector>
class FTrap <bits<4> op, bits<8> vector, dag outs, dag ins, string asmstr,
         list<dag> pattern, InstrItinClass itin>:
      LCCInst<outs, ins, asmstr, pattern, itin, FrmTrap> {
  let Opcode = op;
  let Inst{11-8} = 0;
  let Inst{7-0}  = vector;
}

def FrmA      : Format<3>; // format for <opcode> <ra> <rb> '000' <rc>
class FA <bits<4> op, dag outs, dag ins, string asmstr,
         list<dag> pattern, InstrItinClass itin>:
      LCCInst<outs, ins, asmstr, pattern, itin, FrmA> {
  bits<3> ra;
  bits<3> rb;
  bits<3> rc;
  let Opcode = op;
  let Inst{11-9} = ra;
  let Inst{8-6} = rb;
  let Inst{5-3} = 0;
  let Inst{2-0} = rc;
}

def FrmB      : Format<4>; // format for <opcode> <ra> <rb> '1' <imm5>
class FB <bits<4> op, dag outs, dag ins, string asmstr,
         list<dag> pattern, InstrItinClass itin>:
      LCCInst<outs, ins, asmstr, pattern, itin, FrmB> {
  bits<3> ra;
  bits<3> rb;
  bits<5> imm5;
  let Opcode = op;
  let Inst{11-9} = ra;
  let Inst{8-6} = rb;
  let Inst{5} = 1;
  let Inst{4-0} = imm5;
}

def FrmC      : Format<5>; // format for <opcode> <ra> <imm9>
class FC <bits<4> op, dag outs, dag ins, string asmstr,
         list<dag> pattern, InstrItinClass itin>:
      LCCInst<outs, ins, asmstr, pattern, itin, FrmC> {
  bits<3> ra; // dr/sr for load/store
  bits<9> imm9;
  let Opcode = op;
  let Inst{11-9} = ra;
  let Inst{8-0} = imm9;
}

def FrmD      : Format<6>; // format for <opcode> '1' <imm11>
class FD <bits<4> op, dag outs, dag ins, string asmstr,
         list<dag> pattern, InstrItinClass itin>:
      LCCInst<outs, ins, asmstr, pattern, itin, FrmD> {
  bits<11> imm11;
  let Opcode = op;
  let Inst{11} = 1;
  let Inst{10-0} = imm11;
}

def FrmE      : Format<7>; // format for <opcode> '000' <ra> '000000'
class FE <bits<4> op,dag outs, dag ins, string asmstr,
         list<dag> pattern, InstrItinClass itin>:
      LCCInst<outs, ins, asmstr, pattern, itin, FrmE> {
  bits<3> ra;
  let Opcode = op;
  let Inst{11-9} = 0;
  let Inst{8-6} = ra;
  let Inst{5-0} = 0;
}

def FrmF      : Format<8>; // format for <opcode> <ra> <rb> <imm6>
class FF <bits<4> op,dag outs, dag ins, string asmstr,
         list<dag> pattern, InstrItinClass itin>:
      LCCInst<outs, ins, asmstr, pattern, itin, FrmF> {
  bits<3> ra;
  bits<3> rb;
  bits<6> imm6;
  let Opcode = op;
  let Inst{11-9} = ra;
  let Inst{8-6} = rb;
  let Inst{5-0} = imm6;
}

def FrmG      : Format<8>; // format for <opcode> '000' <ra> '000' <rb>
class FG <bits<4> op,dag outs, dag ins, string asmstr,
         list<dag> pattern, InstrItinClass itin>:
      LCCInst<outs, ins, asmstr, pattern, itin, FrmG> {
  bits<3> ra;
  bits<3> rb;
  let Opcode = op;
  let Inst{11-9} = 0;
  let Inst{8-6} = ra;
  let Inst{5-3} = 0;
  let Inst{2-0} = rb;
}

def FrmH      : Format<8>; // format for <opcode> <ra> <rb> '111111'
class FH <bits<4> op,dag outs, dag ins, string asmstr,
         list<dag> pattern, InstrItinClass itin>:
      LCCInst<outs, ins, asmstr, pattern, itin, FrmH> {
  bits<3> ra;
  bits<3> rb;
  let Opcode = op;
  let Inst{11-9} = ra;
  let Inst{8-6} = rb;
  let Inst{5-0} = 0b111111;
}

def FrmI      : Format<8>; // format for <opcode> <ra> <vector>
class FI <bits<4> op, bits<9> vector, dag outs, dag ins, string asmstr,
         list<dag> pattern, InstrItinClass itin>:
      LCCInst<outs, ins, asmstr, pattern, itin, FrmI> {
  bits<3> ra;
  let Opcode = op;
  let Inst{11-9} = ra;
  let Inst{8-0} = vector;
}

def FrmJ      : Format<8>; // format for <opcode> '111' '000000'
class FJ <bits<4> op, dag outs, dag ins, string asmstr,
         list<dag> pattern, InstrItinClass itin>:
      LCCInst<outs, ins, asmstr, pattern, itin, FrmJ> {
  let Opcode = op;
  let Inst{11-9} = 0b111;
  let Inst{8-0} = 0;
}


// Arithmetic and logical instructions with 2 register operands.
class ArithLogic2Regs<bits<4> op,
                     string instr_asm,
                     SDNode OpNode,
                     Operand Od,
                     PatLeaf imm_type,
                     RegisterClass RC> :
  FB<op,
  (outs GPROut:$ra),
  (ins RC:$rb, Od:$imm5),
  !strconcat(instr_asm, "\t$ra, $rb, $imm5"),
  [(set GPROut:$ra, (OpNode RC:$rb, imm_type:$imm5))],
  IIAlu> {
  let isReMaterializable = 1;
}

// Arithmetic and logical instructions with 3 register operands.
class ArithLogic3Regs<bits<4> op, string instr_asm, SDNode OpNode,
                  InstrItinClass itin, RegisterClass RC, bit isComm = 0>:
  FA<op, (outs GPROut:$ra), (ins RC:$rb, RC:$rc),
     !strconcat(instr_asm, "\t$ra, $rb, $rc"),
     [(set GPROut:$ra, (OpNode RC:$rb, RC:$rc))], itin> {
  let isCommutable = isComm;	// e.g. add rb rc =  add rc rb
  let isReMaterializable = 1;
}


// Signed Operand
def simm16      : Operand<i16> {
// LLVM supply the “let DecoderMethod” keyword to allow programmers implement 
// their decode function.
// LLVM will call these DecodeMethod when user uses Disassembler tools, such as
// llvm-objdump -d.
  let DecoderMethod= "DecodeSimm16";
}

class ImmSEPL<int size>: PatLeaf<(imm), [{ return isInt<size>(N->getSExtValue()); }]>;
def imm5PL: ImmSEPL<5>;
def imm9PL: ImmSEPL<9>;

def ADDi: ArithLogic2Regs<0x01, "add", add, simm16, imm5PL, CPURegs>;
def ADD: ArithLogic3Regs<0x01, "add", add, IIAlu, CPURegs, 1>;

def ANDi: ArithLogic2Regs<0x01, "and", and, simm16, imm5PL, CPURegs>;
def AND: ArithLogic3Regs<0x01, "and", and, IIAlu, CPURegs, 1>;

// def CMPi: ArithLogic2Regs<0x01, "cmp", cmp, simm16, imm5PL, CPURegs>;
// def CMP: ArithLogic3Regs<0x01, "cmp", cmp, IIAlu, CPURegs, 1>;

def SUBi: ArithLogic2Regs<0x01, "sub", sub, simm16, imm5PL, CPURegs>;
def SUB: ArithLogic3Regs<0x01, "sub", sub, IIAlu, CPURegs, 1>;


//===----------------------------------------------------------------------===//
// Instruction aliasesna
//===----------------------------------------------------------------------===//
// def : LCCInstAlias<"mov $dst, $src",
//                     (ADDi GPROut:$dst, GPROut:$src,ZERO), 1>;
// }


// Address operand
def mem : Operand<iPTR> {
// Making llvm call function LCCInstPrinter::printMemOperand()
  let PrintMethod = "printMemOperand";
// For example, ld $2, 4($sp), the mem operand is 4($sp):(ops CPURegs, simm16). 
  let MIOperandInfo = (ops CPURegs, simm16);
// Making llvm call function getMemEncoding() when either ld or st instruction is 
// issued in elf obj since these two instructions use mem Operand.
  let EncoderMethod = "getMemEncoding";
}
// As TableGen is unable to generate selection code for complex addressing modes,
// selection of a MEMbol operand must be done manually. 
// Reference https://opus4.kobv.de/opus4-fau/files/1108/tricore_llvm.pdf
// LCC Address Mode! SDNode frameindex could possibily be a match
// since load and store instructions from stack used it.
def addr : 
  ComplexPattern<iPTR, 2, "SelectAddr", [frameindex], [SDNPWantParent]>;

//===----------------------------------------------------------------------===//
// Pattern fragment for load/store
//===----------------------------------------------------------------------===//
class AlignedLoad<PatFrag Node> :
  PatFrag<(ops node:$ptr), (Node node:$ptr), [{
  LoadSDNode *LD = cast<LoadSDNode>(N);
  return LD->getMemoryVT().getSizeInBits()/16 <= LD->getAlignment();
}]>;

class AlignedStore<PatFrag Node> :
  PatFrag<(ops node:$val, node:$ptr), (Node node:$val, node:$ptr), [{
  StoreSDNode *SD = cast<StoreSDNode>(N);
  return SD->getMemoryVT().getSizeInBits()/16 <= SD->getAlignment();
}]>;

// Load/Store PatFrags.
def load_a          : AlignedLoad<load>;
def store_a         : AlignedStore<store>;

class FMem<bits<4> op, dag outs, dag ins, string asmstr, list<dag> pattern,
          InstrItinClass itin>: FC<op, outs, ins, asmstr, pattern, itin> {
  bits<9> addr;
  let imm9 = addr;
  let DecoderMethod = "DecodeMem";
}

let canFoldAsLoad = 1 in
class LoadM<bits<4> op, string instr_asm, PatFrag OpNode, RegisterClass RC,
            Operand MemOpnd, bit Pseudo>:
  FMem<op, (outs RC:$ra), (ins MemOpnd:$addr),
     !strconcat(instr_asm, "\t$ra, $addr"),
     [(set RC:$ra, (OpNode addr:$addr))], IILoad> {
  let isPseudo = Pseudo;
}

class StoreM<bits<4> op, string instr_asm, PatFrag OpNode, RegisterClass RC,
             Operand MemOpnd, bit Pseudo>:
  FMem<op, (outs), (ins RC:$ra, MemOpnd:$addr),
     !strconcat(instr_asm, "\t$ra, $addr"),
     [(OpNode RC:$ra, addr:$addr)], IIStore> {
  let isPseudo = Pseudo;
}

//@ 16-bit load.
multiclass LoadM16<bits<4> op, string instr_asm, PatFrag OpNode,
                   bit Pseudo = 0> {
  def NAME : LoadM<op, instr_asm, OpNode, GPROut, mem, Pseudo>;
}

// 16-bit store.
multiclass StoreM16<bits<4> op, string instr_asm, PatFrag OpNode,
                    bit Pseudo = 0> {
  def NAME : StoreM<op, instr_asm, OpNode, CPURegs, mem, Pseudo>;
}



/// Load and Store Instructions
///  aligned
defm LD     : LoadM16<0b0010,  "ld",  load_a>;
defm ST     : StoreM16<0b0011, "st",  store_a>;



def imm1 : ImmLeaf<i16, [{return Imm == 1;}]>;
class PPS<bits<9> vector, string instr_asm, SDNode opNode>:
          FI<0b1010, vector, (outs GPROut:$ra), (ins CPURegs:$rb),
          !strconcat(instr_asm, "\t$ra"),
          [(set GPROut:$ra, (opNode CPURegs:$rb, imm1))], IIAlu>;

def SRL: PPS<0b000000100, "srl", srl>;
def SRA: PPS<0b000001000, "sra", sra>;
def SLL: PPS<0b000010000, "sll", shl>;




// Return Instruction
// Return
def LCCRet : SDNode<"LCCISD::Ret", SDTNone,
                     [SDNPHasChain, SDNPOptInGlue, SDNPVariadic]>;

let isReturn=1, isTerminator=1, hasDelaySlot=1, isBarrier=1, hasCtrlDep=1 in
  def RetLR : LCCPseudo<(outs), (ins), "", [(LCCRet)]>;

def RET: FJ<0b1100, (outs), (ins CPURegs: $ra), "ret", [(brind CPURegs: $ra)], IIBranch> {
  let isReturn = 1;
  let isCodeGenOnly = 1;
  let hasCtrlDep = 1;
  let hasExtraSrcRegAllocReq = 1;
  let isBranch=1;
  let isTerminator=1;
  let isBarrier=1;
  let hasDelaySlot = 1;
  let isIndirectBranch = 1;
}
