#include "DAArch34RegisterInfo.h"

#define GET_REGINFO_ENUM // for enum with registers
#include "DAArch34GenRegisterInfo.inc"

namespace llvm {

DAArch34RegisterInfo::DAArch34RegisterInfo()
    : DAArch34GenRegisterInfo(DAArch34::D0) {}

} // end namespace llvm
