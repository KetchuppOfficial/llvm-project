#include <cassert>

#include "DAArch34FrameLowering.h"
#include "DAArch34RegisterInfo.h"

#define GET_REGINFO_ENUM        // for enum with registers
#define GET_REGINFO_TARGET_DESC // for CalleeSaved_DAArch34_SaveList and
                                // getFrameLowering
#include "DAArch34GenRegisterInfo.inc"

namespace llvm {

DAArch34RegisterInfo::DAArch34RegisterInfo()
    : DAArch34GenRegisterInfo(DAArch34::D0) {}

const MCPhysReg *
DAArch34RegisterInfo::getCalleeSavedRegs(const MachineFunction *MF) const {
  return CalleeSaved_DAArch34_SaveList;
}

const uint32_t *
DAArch34RegisterInfo::getCallPreservedMask(const MachineFunction &MF,
                                           CallingConv::ID CC) const {
  return CalleeSaved_DAArch34_RegMask;
}

BitVector
DAArch34RegisterInfo::getReservedRegs(const MachineFunction &MF) const {
  const DAArch34FrameLowering *TFI = getFrameLowering(MF);

  BitVector Reserved(getNumRegs());
  Reserved.set(DAArch34::D2); // stack pointer

  if (TFI->hasFP(MF)) {
    Reserved.set(DAArch34::D8); // frame pointer
  }

  return Reserved;
}

bool DAArch34RegisterInfo::requiresRegisterScavenging(
    const MachineFunction &MF) const {
  return false;
}

bool DAArch34RegisterInfo::eliminateFrameIndex(MachineBasicBlock::iterator II,
                                               int SPAdj, unsigned FIOperandNum,
                                               RegScavenger *RS) const {
  assert(SPAdj == 0 && "Unexpected non-zero SPAdj value");

  MachineInstr &MI = *II;
  MachineFunction &MF = *MI.getMF();

  const auto FrameIndex = MI.getOperand(FIOperandNum).getIndex();
  Register FrameReg;
  auto Offset = getFrameLowering(MF)
                    ->getFrameIndexReference(MF, FrameIndex, FrameReg)
                    .getFixed();
  Offset += MI.getOperand(FIOperandNum + 1).getImm();

  if (!isInt<16>(Offset)) {
    llvm_unreachable(
        "invalid offset in DAArch34RegisterInfo::eliminateFrameIndex");
  }

  MI.getOperand(FIOperandNum).ChangeToRegister(FrameReg, false);
  MI.getOperand(FIOperandNum + 1).ChangeToImmediate(Offset);

  return false;
}

Register
DAArch34RegisterInfo::getFrameRegister(const MachineFunction &MF) const {
  const TargetFrameLowering *TFI = getFrameLowering(MF);
  return TFI->hasFP(MF) ? DAArch34::D8 : DAArch34::D2;
}

} // end namespace llvm
