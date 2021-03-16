//===-- Cpu0Schedule.td - Cpu0 Scheduling Definitions ------*- tablegen -*-===//
//===----------------------------------------------------------------------===//
// Functional units across Cpu0 chips sets. Based on GCC/Cpu0 backend files.
//===----------------------------------------------------------------------===//
def ALU     : FuncUnit;
def IMULDIV : FuncUnit;

//===----------------------------------------------------------------------===//
// Instruction Itinerary classes used for Cpu0
//===----------------------------------------------------------------------===//
def IIAlu              : InstrItinClass;
def II_CLO             : InstrItinClass;
def II_CLZ             : InstrItinClass;
def IILoad             : InstrItinClass;
def IIStore            : InstrItinClass;

def IIHiLo             : InstrItinClass;
def IIImul             : InstrItinClass;
def IIIdiv             : InstrItinClass;

def IIBranch           : InstrItinClass;
def IIPseudo           : InstrItinClass;

//===----------------------------------------------------------------------===//
// Cpu0 Generic instruction itineraries.
//===----------------------------------------------------------------------===//
def Cpu0GenericItineraries : ProcessorItineraries<[ALU, IMULDIV], [], [
  InstrItinData<IIAlu              , [InstrStage<1,  [ALU]>]>,
  InstrItinData<II_CLO             , [InstrStage<1,  [ALU]>]>,
  InstrItinData<II_CLZ             , [InstrStage<1,  [ALU]>]>,
  InstrItinData<IILoad             , [InstrStage<3,  [ALU]>]>,
  InstrItinData<IIStore            , [InstrStage<1,  [ALU]>]>,
  InstrItinData<IIHiLo             , [InstrStage<1,  [IMULDIV]>]>,
  InstrItinData<IIImul             , [InstrStage<17, [IMULDIV]>]>,
  InstrItinData<IIIdiv             , [InstrStage<38, [IMULDIV]>]>,
  InstrItinData<IIBranch           , [InstrStage<1,  [ALU]>]>
]>;
