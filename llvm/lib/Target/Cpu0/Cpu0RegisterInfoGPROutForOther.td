//===----------------------------------------------------------------------===//
// Register Classes
//===----------------------------------------------------------------------===//
def GPROut : RegisterClass<"Cpu0", [i32], 32, (add (sub CPURegs, SW))>;

