#ifndef LLVM_LIB_TARGET_DAARCH34_MCTARGETDESC_H
#define LLVM_LIB_TARGET_DAARCH34_MCTARGETDESC_H

#include "llvm/MC/MCInstrDesc.h"

namespace llvm {

namespace DAArch34Op {

enum OperandType : unsigned {
  kSIMM16 = MCOI::OperandType::OPERAND_FIRST_TARGET
};

} // end namespace DAArch34Op

} // end namespace llvm

#endif // LLVM_LIB_TARGET_DAARCH34_MCTARGETDESC_H
