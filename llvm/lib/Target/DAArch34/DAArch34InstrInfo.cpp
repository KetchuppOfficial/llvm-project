#include "DAArch34InstrInfo.h"

#define GET_INSTRINFO_CTOR_DTOR
#include "DAArch34GenInstrInfo.inc"

namespace llvm {

DAArch34InstrInfo::DAArch34InstrInfo() : DAArch34GenInstrInfo() {}

} // end namespace llvm
