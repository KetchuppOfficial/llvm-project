#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/TargetRegistry.h"

#include "TargetInfo/DAArch34TargetInfo.h"

#define GET_REGINFO_ENUM // for enum of registers
#define GET_REGINFO_MC_DESC // for InitDAArch34MCRegisterInfo
#include "DAArch34GenRegisterInfo.inc"

namespace llvm {

static MCRegisterInfo *createDAArch34MCRegisterInfo(const Triple &TT) {
  auto *X = new MCRegisterInfo{};
  InitDAArch34MCRegisterInfo(X, DAArch34::D0);
  return X;
}

extern "C" void LLVMInitializeDAArch34TargetMC() {
  Target &TheSimTarget = getTheDAArch34Target();
  TargetRegistry::RegisterMCRegInfo(TheSimTarget, createDAArch34MCRegisterInfo);
}

} // end namespace llvm
