
//===----------------------------------------------------------------------===//
// Register Classes
//===----------------------------------------------------------------------===//
// General Purpose Register for Output
def GPROut : RegisterClass<"Cpu0", [i32], 32, (add (sub CPURegs, SW))>;

