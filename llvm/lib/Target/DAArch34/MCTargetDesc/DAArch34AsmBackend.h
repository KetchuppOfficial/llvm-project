#ifndef LLVM_LIB_TARGET_DAARCH34_MCTARGETDESC_DAARCH34ASMBACKEND_H
#define LLVM_LIB_TARGET_DAARCH34_MCTARGETDESC_DAARCH34ASMBACKEND_H

namespace llvm {

class Target;
class MCAsmBackend;
class MCRegisterInfo;
class MCSubtargetInfo;
class MCTargetOptions;

MCAsmBackend *createDAArch34AsmBackend(const Target &T,
                                       const MCSubtargetInfo &STI,
                                       const MCRegisterInfo &MRI,
                                       const MCTargetOptions &Options);

} // end namespace llvm

#endif // LLVM_LIB_TARGET_DAARCH34_MCTARGETDESC_DAARCH34ASMBACKEND_H
