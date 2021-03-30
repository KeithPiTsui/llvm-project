//===-- LCCInstrFormats.td - LCC Instruction Formats -----*- tablegen -*-===//
//===----------------------------------------------------------------------===//
//  Describe CPU0 instructions format
//  CPU INSTRUCTION FORMATS
//  opcode  - operation code.
//  ra      - dst reg, only used on 3 regs instr.
//  rb      - src reg.
//  rc      - src reg (on a 3 reg instr).
//  cx      - immediate
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

