#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/TargetRegistry.h"

#include "TargetInfo/DAArch34TargetInfo.h"

#define GET_REGINFO_ENUM    // for enum of registers
#define GET_REGINFO_MC_DESC // for InitDAArch34MCRegisterInfo
#include "DAArch34GenRegisterInfo.inc"

#include "MCTargetDesc/DAArch34Info.h" // to introduce namespace DAArch34Op
#define GET_INSTRINFO_MC_DESC          // for InitDAArch34MCInstrInfo
#include "DAArch34GenInstrInfo.inc"

#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCSubtargetInfo.h"
#define GET_SUBTARGETINFO_MC_DESC // for createDAArch34MCSubtargetInfoImpl
#include "DAArch34GenSubtargetInfo.inc"

namespace llvm {

namespace {

MCRegisterInfo *createDAArch34MCRegisterInfo(const Triple &TT) {
  auto *X = new MCRegisterInfo{};
  InitDAArch34MCRegisterInfo(X, DAArch34::D0);
  return X;
}

MCInstrInfo *createDAArch34MCInstrInfo() {
  auto *X = new MCInstrInfo{};
  InitDAArch34MCInstrInfo(X);
  return X;
}

MCSubtargetInfo *createDAArch34MCSubtargetInfo(const Triple &TT, StringRef CPU,
                                               StringRef FS) {
  return createDAArch34MCSubtargetInfoImpl(TT, CPU, CPU /* TuneCPU */, FS);
}

} // end unnamed namespace

extern "C" void LLVMInitializeDAArch34TargetMC() {
  Target &TheDAArch34Target = getTheDAArch34Target();
  TargetRegistry::RegisterMCRegInfo(TheDAArch34Target,
                                    createDAArch34MCRegisterInfo);
  TargetRegistry::RegisterMCInstrInfo(TheDAArch34Target,
                                      createDAArch34MCInstrInfo);
  TargetRegistry::RegisterMCSubtargetInfo(TheDAArch34Target,
                                          createDAArch34MCSubtargetInfo);
}

} // end namespace llvm
