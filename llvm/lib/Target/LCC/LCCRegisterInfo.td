//===-- LCCRegisterInfo.td - LCC Register defs -----------*- tablegen -*-===//
//===----------------------------------------------------------------------===//
//  Declarations that describe the CPU0 register file
//===----------------------------------------------------------------------===//
// We have banks of 8 registers each.
class LCCReg<bits<16> Enc, string n> : Register<n> {
  let HWEncoding = Enc;
  let Namespace = "LCC";
}
// LCC CPU Registers
class LCCGPRReg<bits<16> Enc, string n> : LCCReg<Enc, n>;

//===----------------------------------------------------------------------===//
//@Registers
//===----------------------------------------------------------------------===//
// The register string, such as "9" or "gp" will show on "llvm-objdump -d"
//@ All registers definition
let Namespace = "LCC" in {
  //@ General Purpose Registers
  // DwarfRegNum used by gcc, gdb, or a debug information
  // ref. Target.td
  //      https://llvm.org/docs/WritingAnLLVMBackend.html#defining-a-register (obsolete)
  def R0   : LCCGPRReg<0,  "0">, DwarfRegNum<[0]>;
  def R1   : LCCGPRReg<1,  "1">, DwarfRegNum<[1]>;
  def R2   : LCCGPRReg<2,  "2">, DwarfRegNum<[2]>;
  def R3   : LCCGPRReg<3,  "3">, DwarfRegNum<[3]>;
  def R4   : LCCGPRReg<4,  "4">, DwarfRegNum<[4]>;
  def FP   : LCCGPRReg<5, "fp">, DwarfRegNum<[5]>;
  def SP   : LCCGPRReg<6, "sp">, DwarfRegNum<[6]>;
  def LR   : LCCGPRReg<7, "lr">, DwarfRegNum<[7]>;
}

//===----------------------------------------------------------------------===//
//@Register Classes
//===----------------------------------------------------------------------===//
def LCCRetValRegs: RegisterClass<"LCC", [i16], 16, (add
  R0)>;

def LCCReserved2Regs: RegisterClass<"LCC", [i16], 16, (add
  FP, SP, LR)>;

def CPURegs : RegisterClass<"LCC", [i16], 16, (add
  // Return Values and Arguments
  LCCRetValRegs,
  // Reserved
  LCCReserved2Regs
)>;

def GPROut : RegisterClass<"LCC", [i16], 16, (add R0, R1, R2, R3, R4)>;
