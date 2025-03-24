#include "llvm/CodeGen/TargetSubtargetInfo.h" // needed by DAArch34GenSubtargetInfo ctor
#include "llvm/Target/TargetMachine.h"

#include "DAArch34Subtarget.h"

#define GET_SUBTARGETINFO_CTOR
#include "DAArch34GenSubtargetInfo.inc"

namespace llvm {

DAArch34Subtarget::DAArch34Subtarget(const TargetMachine &TM, StringRef CPU,
                                     StringRef TuneCPU, StringRef FS)
    : DAArch34GenSubtargetInfo(TM.getTargetTriple(), CPU, TuneCPU, FS) {}

// No implementation of DAArch34Subtarget::ParseSubtargetFeatures for now

} // end namespace llvm
