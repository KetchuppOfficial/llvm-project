#ifndef LLVM_LIB_TARGET_DAARCH34_DAARCH34REGISTERINFO_H
#define LLVM_LIB_TARGET_DAARCH34_DAARCH34REGISTERINFO_H

#define GET_REGINFO_HEADER // for DAArch34GenRegisterInfo
#include "DAArch34GenRegisterInfo.inc"

namespace llvm {

struct DAArch34RegisterInfo : public DAArch34GenRegisterInfo {
  DAArch34RegisterInfo();
};

} // end namespace llvm

#endif // LLVM_LIB_TARGET_DAARCH34_DAARCH34REGISTERINFO_H
