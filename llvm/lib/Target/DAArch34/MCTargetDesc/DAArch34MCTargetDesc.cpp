#include "llvm/MC/MCDwarf.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/TargetRegistry.h"

#include "DAArch34InstPrinter.h"
#include "DAArch34MCAsmInfo.h"
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

MCAsmInfo *createDAArch34MCAsmInfo(const MCRegisterInfo &MRI, const Triple &TT,
                                   const MCTargetOptions &Options) {
  MCAsmInfo *MAI = new DAArch34ELFMCAsmInfo(TT);
  unsigned SP = MRI.getDwarfRegNum(DAArch34::D2, true /* isEH */);
  MAI->addInitialFrameState(MCCFIInstruction::cfiDefCfa(nullptr, SP, 0));
  return MAI;
}

MCInstPrinter *createDAArch34MCInstPrinter(const Triple &TT,
                                           unsigned SyntaxVariant,
                                           const MCAsmInfo &MAI,
                                           const MCInstrInfo &MII,
                                           const MCRegisterInfo &MRI) {
  return new DAArch34InstrPrinter(MAI, MII, MRI);
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
  TargetRegistry::RegisterMCAsmInfo(TheDAArch34Target, createDAArch34MCAsmInfo);
  TargetRegistry::RegisterMCInstPrinter(TheDAArch34Target, createDAArch34MCInstPrinter);
}

} // end namespace llvm
