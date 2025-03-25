#ifndef LLVM_LIB_TARGET_DAARCH34_DAARCH34ISELLOWERING_H
#define LLVM_LIB_TARGET_DAARCH34_DAARCH34ISELLOWERING_H

#include "llvm/CodeGen/ISDOpcodes.h"
#include "llvm/CodeGen/TargetLowering.h"

namespace llvm {

namespace DAArch34ISD {

enum NodeType : unsigned {
  FIRST_NUMBER = ISD::BUILTIN_OP_END,
  RET,
  CALL,
  BR_CC
};

} // end namespace DAArch34ISD

class TargetMachine;
class DAArch34Subtarget;

class DAArch34TargetLowering : public TargetLowering {
public:
  explicit DAArch34TargetLowering(const TargetMachine &TM, const DAArch34Subtarget &STI);

  /// This method returns the name of a target specific DAG node.
  const char *getTargetNodeName(unsigned Opcode) const override;

  const DAArch34Subtarget &getSubtarget() const { return STI; }

private:
  const DAArch34Subtarget &STI;
};

} // end namespace llvm

#endif // LLVM_LIB_TARGET_DAARCH34_DAARCH34ISELLOWERING_H
