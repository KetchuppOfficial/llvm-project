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
  explicit DAArch34TargetLowering(const TargetMachine &TM,
                                  const DAArch34Subtarget &STI);

  /// This method returns the name of a target specific DAG node.
  const char *getTargetNodeName(unsigned Opcode) const override;

  const DAArch34Subtarget &getSubtarget() const { return STI; }

  /// Return true if the addressing mode represented by AM is legal for this
  /// target, for a load/store of the specified type.
  ///
  /// The type may be VoidTy, in which case only return true if the addressing
  /// mode is legal for a load/store of any legal type.
  ///
  /// If the address space cannot be determined, it will be -1.
  ///
  bool isLegalAddressingMode(const DataLayout &DL, const AddrMode &AM, Type *Ty,
                             unsigned AS,
                             Instruction *I = nullptr) const override;

private:
  void ReplaceNodeResults(SDNode *N, SmallVectorImpl<SDValue> &Results,
                          SelectionDAG &DAG) const override;

  /// This method will be invoked for all target nodes and for any
  /// target-independent nodes that the target has registered with invoke it
  /// for.
  ///
  /// The semantics are as follows:
  /// Return Value:
  ///   SDValue.Val == 0   - No change was made
  ///   SDValue.Val == N   - N was replaced, is dead, and is already handled.
  ///   otherwise          - N should be replaced by the returned Operand.
  ///
  /// In addition, methods provided by DAGCombinerInfo may be used to perform
  /// more complex transformations.
  ///
  SDValue PerformDAGCombine(SDNode *N, DAGCombinerInfo &DCI) const override;

  /// This hook must be implemented to lower the incoming (formal) arguments,
  /// described by the Ins array, into the specified DAG. The implementation
  /// should fill in the InVals array with legal-type argument values, and
  /// return the resulting token chain value.
  SDValue LowerFormalArguments(SDValue Chain, CallingConv::ID CallConv,
                               bool IsVarArg,
                               const SmallVectorImpl<ISD::InputArg> &Ins,
                               const SDLoc &DL, SelectionDAG &DAG,
                               SmallVectorImpl<SDValue> &InVals) const override;

  /// This hook must be implemented to lower calls into the specified
  /// DAG. The outgoing arguments to the call are described by the Outs array,
  /// and the values to be returned by the call are described by the Ins
  /// array. The implementation should fill in the InVals array with legal-type
  /// return values from the call, and return the resulting token chain value.
  SDValue LowerCall(TargetLowering::CallLoweringInfo &CLI,
                    SmallVectorImpl<SDValue> &InVals) const override;

  /// This hook must be implemented to lower outgoing return values, described
  /// by the Outs array, into the specified DAG. The implementation should
  /// return the resulting token chain value.
  SDValue LowerReturn(SDValue Chain, CallingConv::ID CallConv, bool IsVarArg,
                      const SmallVectorImpl<ISD::OutputArg> &Outs,
                      const SmallVectorImpl<SDValue> &OutVals, const SDLoc &DL,
                      SelectionDAG &DAG) const override;

  /// This hook should be implemented to check whether the return values
  /// described by the Outs array can fit into the return registers.  If false
  /// is returned, an sret-demotion is performed.
  bool CanLowerReturn(CallingConv::ID CallConv, MachineFunction &MF,
                      bool IsVarArg,
                      const SmallVectorImpl<ISD::OutputArg> &Outs,
                      LLVMContext &Context) const override;

  const DAArch34Subtarget &STI;
};

} // end namespace llvm

#endif // LLVM_LIB_TARGET_DAARCH34_DAARCH34ISELLOWERING_H
