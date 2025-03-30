#ifndef LLVM_LIB_TARGET_DAARCH34_MCTARGETDESC_DAARCH34INSTPRINTER_H
#define LLVM_LIB_TARGET_DAARCH34_MCTARGETDESC_DAARCH34INSTPRINTER_H

namespace llvm {

class Triple;
class MCAsmInfo;
class MCInstrInfo;
class MCInstPrinter;
class MCRegisterInfo;

MCInstPrinter *createDAArch34MCInstPrinter(const Triple &TT,
                                           unsigned SyntaxVariant,
                                           const MCAsmInfo &MAI,
                                           const MCInstrInfo &MII,
                                           const MCRegisterInfo &MRI);

} // end namespace llvm

#endif // LLVM_LIB_TARGET_DAARCH34_MCTARGETDESC_DAARCH34INSTPRINTER_H
