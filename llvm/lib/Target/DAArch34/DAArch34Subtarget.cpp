#include "llvm/CodeGen/TargetSubtargetInfo.h" // needed by DAArch34GenSubtargetInfo ctor
#include "llvm/Target/TargetMachine.h"

#include "DAArch34Subtarget.h"

#define GET_SUBTARGETINFO_CTOR
#include "DAArch34GenSubtargetInfo.inc"

namespace llvm {

DAArch34Subtarget::DAArch34Subtarget(const TargetMachine &TM, const Triple &TT,
                                     const std::string &CPU,
                                     const std::string &FS)
    : DAArch34GenSubtargetInfo(TT, CPU, CPU /* TunCPU */, FS),
      TLInfo(TM, *this), FrameLowering(*this) {}


} // end namespace llvm
