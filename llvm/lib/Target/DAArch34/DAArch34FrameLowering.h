#ifndef LLVM_LIB_TARGET_DAARCH34_DAARCH34FRAMELOWERING_H
#define LLVM_LIB_TARGET_DAARCH34_DAARCH34FRAMELOWERING_H

#include "llvm/CodeGen/TargetFrameLowering.h"
#include "llvm/Support/Alignment.h"

namespace llvm {

class MachineBasicBlock;

class DAArch34FrameLowering : public TargetFrameLowering {
public:
  explicit DAArch34FrameLowering()
      : TargetFrameLowering(TargetFrameLowering::StackGrowsDown, Align{4}, 0) {}

  void emitPrologue(MachineFunction &MF,
                    MachineBasicBlock &MBB) const override {}
  void emitEpilogue(MachineFunction &MF,
                    MachineBasicBlock &MBB) const override {}

  bool hasFP(const MachineFunction &MF) const override { return false; }
};

} // end namespace llvm

#endif // LLVM_LIB_TARGET_DAARCH34_DAARCH34FRAMELOWERING_H
