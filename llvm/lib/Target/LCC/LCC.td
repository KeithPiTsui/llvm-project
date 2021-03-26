//===-- Cpu0.td - Describe the LCC Target Machine ---------*- tablegen -*-===//
//===----------------------------------------------------------------------===//
// Calling Conv, Instruction Descriptions
//===----------------------------------------------------------------------===//
include "LCCSchedule.td"
include "LCCInstrInfo.td"
include "LCCCallingConv.td"

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

