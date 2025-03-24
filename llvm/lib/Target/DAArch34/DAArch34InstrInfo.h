#ifndef LLVM_LIB_TARGET_DAARCH34_DAARCH34INSTRINFO_H
#define LLVM_LIB_TARGET_DAARCH34_DAARCH34INSTRINFO_H

// for TargetInstrInfo which DAArch34GenInstrInfo inherits from
#include "llvm/CodeGen/TargetInstrInfo.h"

#define GET_INSTRINFO_HEADER // for DAArch34GenInstrInfo
#include "DAArch34GenInstrInfo.inc"

namespace llvm {

class DAArch34InstrInfo : public DAArch34GenInstrInfo {
public:
  DAArch34InstrInfo();
};

} // end namespace llvm

#endif // LLVM_LIB_TARGET_DAARCH34_DAARCH34INSTRINFO_H
