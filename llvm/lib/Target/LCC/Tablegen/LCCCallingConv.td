//===-- LCCCallingConv.td - Calling Conventions for LCC --*- tablegen -*-===//
//===----------------------------------------------------------------------===//
// This describes the calling conventions for LCC architecture.
//===----------------------------------------------------------------------===//
/// CCIfSubtarget - Match if the current subtarget has a feature F.
class CCIfSubtarget<string F, CCAction A>:
  CCIf<!strconcat("State.getTarget().getSubtarget<LCCSubtarget>().", F), A>;

def RetCC_LCCEABI : CallingConv<[
  // i16 are returned in registers R0
  CCIfType<[i16], CCAssignToReg<[R0]>>
]>;

def RetCC_LCC : CallingConv<[
  CCDelegateTo<RetCC_LCCEABI>
]>;

def CSR_O16 : CalleeSavedRegs<(add LR, FP, LR)>;

