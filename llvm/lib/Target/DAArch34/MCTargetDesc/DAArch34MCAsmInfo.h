#ifndef LLVM_LIB_TARGET_DAARCH34_MCTARGETDESC_DAARCH34MCASMINFO_H
#define LLVM_LIB_TARGET_DAARCH34_MCTARGETDESC_DAARCH34MCASMINFO_H

#include "llvm/MC/MCAsmInfoELF.h"

namespace llvm {

class Triple;

class DAArch34ELFMCAsmInfo : public MCAsmInfoELF {
public:
    explicit DAArch34ELFMCAsmInfo(const Triple &TheTriple);
};

} // end namespace llvm

#endif // LLVM_LIB_TARGET_DAARCH34_MCTARGETDESC_DAARCH34MCASMINFO_H
