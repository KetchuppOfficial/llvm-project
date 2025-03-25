#include "DAArch34ISelLowering.h"

#include "llvm/CodeGenTypes/MachineValueType.h" // for MVT (Machine Value Type)

#include "DAArch34FrameLowering.h"

#define GET_REGINFO_ENUM
#define GET_REGINFO_HEADER
#define GET_REGINFO_TARGET_DESC // for DAArch34::GPRRegClass
#include "DAArch34GenRegisterInfo.inc"

#define DEBUG_TYPE "sim-lower"

namespace llvm {

DAArch34TargetLowering::DAArch34TargetLowering(const TargetMachine &TM,
                                               const DAArch34Subtarget &STI)
    : TargetLowering(TM), STI(STI) {
  addRegisterClass(MVT::i32, &DAArch34::GPRRegClass);
}

const char *DAArch34TargetLowering::getTargetNodeName(unsigned Opcode) const {
  switch (Opcode) {
  case DAArch34ISD::CALL:
    return "DAArch34ISD::CALL";
  case DAArch34ISD::RET:
    return "DAArch34ISD::RET";
  default:
    return nullptr;
  }
}

} // end namespace llvm
