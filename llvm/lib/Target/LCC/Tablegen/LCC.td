//===-- LCC.td - Describe the LCC Target Machine ---------*- tablegen -*-===//
//===----------------------------------------------------------------------===//
// Target-independent interfaces
//===----------------------------------------------------------------------===//
include "llvm/Target/Target.td"
//===----------------------------------------------------------------------===//
// Target-dependent interfaces
//===----------------------------------------------------------------------===//
include "Tablegen/LCCRegisterInfo.td"
//===----------------------------------------------------------------------===//
// Calling Conv, Instruction Descriptions
//===----------------------------------------------------------------------===//
include "Tablegen/LCCSchedule.td"
include "Tablegen/LCCInstrInfo.td"
include "Tablegen/LCCCallingConv.td"
//===----------------------------------------------------------------------===//
// LCC processors supported.
//===----------------------------------------------------------------------===//
class Proc<string Name, list<SubtargetFeature> Features>
 : Processor<Name, LCCGenericItineraries, Features>;

def : Proc<"LCC", []>;
def LCCInstrInfo : InstrInfo;
def LCC : Target {
  let InstructionSet = LCCInstrInfo;
}

