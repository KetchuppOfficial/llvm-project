#ifndef LLVM_LIB_TARGET_DAARCH34_DAARCH34REGISTERINFO_H
#define LLVM_LIB_TARGET_DAARCH34_DAARCH34REGISTERINFO_H

#include <cstdint>

#include "llvm/ADT/BitVector.h"
#include "llvm/CodeGen/MachineBasicBlock.h"
#include "llvm/CodeGen/RegisterScavenging.h"
#include "llvm/IR/CallingConv.h"

#define GET_REGINFO_HEADER // for DAArch34GenRegisterInfo
#include "DAArch34GenRegisterInfo.inc"

namespace llvm {

class MachineFunction;

struct DAArch34RegisterInfo : public DAArch34GenRegisterInfo {
  DAArch34RegisterInfo();

  /// Return a null-terminated list of all of the callee-saved registers on
  /// this target. The registers should be in the order of desired callee-save
  /// stack frame offset. The first register is closest to the incoming stack
  /// pointer if stack grows down, and vice versa.
  /// Notice: This function does not take into account disabled CSRs.
  ///         In most cases you will want to use instead the function
  ///         getCalleeSavedRegs that is implemented in MachineRegisterInfo.
  const MCPhysReg *getCalleeSavedRegs(const MachineFunction *MF) const override;

  /// Return a mask of call-preserved registers for the given calling convention
  /// on the current function. The mask should include all call-preserved
  /// aliases. This is used by the register allocator to determine which
  /// registers can live across a call.
  ///
  /// The mask is an array containing (TRI::getNumRegs()+31)/32 entries.
  /// A set bit indicates that all bits of the corresponding register are
  /// preserved across the function call.  The bit mask is expected to be
  /// sub-register complete, i.e. if A is preserved, so are all its
  /// sub-registers.
  ///
  /// Bits are numbered from the LSB, so the bit for physical register Reg can
  /// be found as (Mask[Reg / 32] >> Reg % 32) & 1.
  ///
  /// A NULL pointer means that no register mask will be used, and call
  /// instructions should use implicit-def operands to indicate call clobbered
  /// registers.
  const uint32_t *getCallPreservedMask(const MachineFunction &MF,
                                       CallingConv::ID CC) const override;

  /// Returns a bitset indexed by physical register number indicating if a
  /// register is a special register that has particular uses and should be
  /// considered unavailable at all times, e.g. stack pointer, return address.
  /// A reserved register:
  /// - is not allocatable
  /// - is considered always live
  /// - is ignored by liveness tracking
  /// It is often necessary to reserve the super registers of a reserved
  /// register as well, to avoid them getting allocated indirectly. You may use
  /// markSuperRegs() and checkAllSuperRegsMarked() in this case.
  BitVector getReservedRegs(const MachineFunction &MF) const override;

  /// Returns true if the target requires (and can make use of) the register
  /// scavenger.
  bool requiresRegisterScavenging(const MachineFunction &MF) const override;

  /// This method must be overriden to eliminate abstract frame indices from
  /// instructions which may use them. The instruction referenced by the
  /// iterator contains an MO_FrameIndex operand which must be eliminated by
  /// this method. This method may modify or replace the specified instruction,
  /// as long as it keeps the iterator pointing at the finished product.
  /// SPAdj is the SP adjustment due to call frame setup instruction.
  /// FIOperandNum is the FI operand number.
  /// Returns true if the current instruction was removed and the iterator
  /// is not longer valid
  bool eliminateFrameIndex(MachineBasicBlock::iterator II, int SPAdj,
                           unsigned FIOperandNum,
                           RegScavenger *RS = nullptr) const override;

  /// This method should return the register used as a base for values allocated
  /// in the current stack frame.
  Register getFrameRegister(const MachineFunction &MF) const override;
};

} // end namespace llvm

#endif // LLVM_LIB_TARGET_DAARCH34_DAARCH34REGISTERINFO_H
