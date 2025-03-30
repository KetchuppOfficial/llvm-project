#ifndef LLVM_LIB_TARGET_DAARCH34_MCTARGET_DESC_DAARCH34ELFOBJECTWRITER_H
#define LLVM_LIB_TARGET_DAARCH34_MCTARGET_DESC_DAARCH34ELFOBJECTWRITER_H

#include <cstdint>
#include <memory>

#include "llvm/MC/MCObjectWriter.h"

namespace llvm {

std::unique_ptr<MCObjectTargetWriter>
createDAArch34ELFObjectWriter(bool Is64Bit, uint8_t OSABI);

} // namespace llvm

#endif // LLVM_LIB_TARGET_DAARCH34_MCTARGET_DESC_DAARCH34ELFOBJECTWRITER_H
