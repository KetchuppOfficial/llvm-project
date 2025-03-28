#include "llvm/CodeGen/CallingConvLower.h"
#include "llvm/CodeGen/ISDOpcodes.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/SelectionDAGNodes.h"
#include "llvm/CodeGen/ValueTypes.h"
#include "llvm/CodeGenTypes/MachineValueType.h" // for MVT (Machine Value Type)
#include "llvm/Support/Alignment.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Target/TargetMachine.h"

#include "DAArch34FrameLowering.h"
#include "DAArch34ISelLowering.h"
#include "DAArch34MachineFuntionInfo.h"
#include "DAArch34Subtarget.h"

#define GET_REGINFO_ENUM
#include "DAArch34GenRegisterInfo.inc"

#define DEBUG_TYPE "daarch34-lower"

namespace llvm {

DAArch34TargetLowering::DAArch34TargetLowering(const TargetMachine &TM,
                                               const DAArch34Subtarget &STI)
    : TargetLowering(TM), STI(STI) {
  addRegisterClass(MVT::i32, &DAArch34::GPRRegClass);

  computeRegisterProperties(STI.getRegisterInfo());

  setStackPointerRegisterToSaveRestore(DAArch34::D2);

  for (unsigned Opc = 0; Opc != ISD::NodeType::BUILTIN_OP_END; ++Opc) {
    // Try to expand this to other ops, otherwise use a libcall.
    setOperationAction(Opc, MVT::i32, LegalizeAction::Expand);
  }

  for (auto Op : {ISD::ADD, ISD::MUL, ISD::LOAD, ISD::STORE, ISD::Constant,
                  ISD::UNDEF, ISD::FRAMEADDR}) {
    // The target natively supports this operation.
    setOperationAction(Op, MVT::i32, LegalizeAction::Legal);
  }

  // Use the LowerOperation hook to implement custom lowering.
  setOperationAction(ISD::BR_CC, MVT::i32, LegalizeAction::Custom);
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

void DAArch34TargetLowering::ReplaceNodeResults(
    SDNode *N, SmallVectorImpl<SDValue> &Results, SelectionDAG &DAG) const {
  llvm_unreachable(
      "DAArch34TargetLowering::ReplaceNodeResults: not implemented");
}

#include "DAArch34GenCallingConv.inc"

namespace {

static constexpr const MCPhysReg ArgGPRs[] = {
    DAArch34::D10, DAArch34::D11, DAArch34::D12, DAArch34::D13,
    DAArch34::D14, DAArch34::D15, DAArch34::D16, DAArch34::D17};

// EVT - extended value type
Align getPrefTypeAlign(SelectionDAG &DAG, EVT VT) {
  return DAG.getDataLayout().getPrefTypeAlign(
      VT.getTypeForEVT(*DAG.getContext()));
}

} // end unnamed namespace

/// CCState - This class holds information needed while lowering arguments and
/// return values.  It captures which registers are already assigned and which
/// stack slots are used.  It provides accessors to allocate these values.

/// CCValAssign - Represent assignment of one arg/retval to a location.

/// MVT - Machine Value Type. Every type that is supported natively by some
/// processor targeted by LLVM occurs here. This means that any legal value
/// type can be represented by an MVT.

SDValue
DAArch34TargetLowering::LowerCall(TargetLowering::CallLoweringInfo &CLI,
                                  SmallVectorImpl<SDValue> &InVals) const {
  assert(!CLI.IsTailCall);
  auto &DAG = CLI.DAG;
  auto &MF = DAG.getMachineFunction();
  auto &Outs = CLI.Outs;
  auto &OutVals = CLI.OutVals;
  auto &DL = CLI.DL;
  EVT PtrVT = getPointerTy(DAG.getDataLayout());

  // Analyze the operands of the call, assigning locations to each operand.
  SmallVector<CCValAssign, 16> ArgLocs;
  CCState CCInfo(CLI.CallConv, CLI.IsVarArg, MF, ArgLocs, *DAG.getContext());
  CCInfo.AnalyzeCallOperands(Outs,
                             CC_DAArch34); // CC_DAArch34 is tablegen'erated

  // Create local copies for byval args
  SDValue Chain = CLI.Chain;
  SmallVector<SDValue, 8> ByValArgs;
  for (std::size_t I = 0, E = Outs.size(); I != E; ++I) {
    ISD::ArgFlagsTy Flags = Outs[I].Flags;
    if (!Flags.isByVal())
      continue;

    const auto Size = Flags.getByValSize();
    const Align Alignment = Flags.getNonZeroByValAlign();

    int FI = MF.getFrameInfo().CreateStackObject(Size, Alignment,
                                                 /*isSpillSlot=*/false);
    SDValue FIPtr = DAG.getFrameIndex(FI, getPointerTy(DAG.getDataLayout()));
    SDValue SizeNode = DAG.getConstant(Size, DL, MVT::i32);

    Chain = DAG.getMemcpy(Chain, DL, FIPtr, OutVals[I], SizeNode, Alignment,
                          /*IsVolatile=*/false,
                          /*AlwaysInline=*/false,
                          /*CI=*/nullptr, std::nullopt, MachinePointerInfo(),
                          MachinePointerInfo());
    ByValArgs.push_back(FIPtr);
  }

  const auto NumBytes = CCInfo.getStackSize();
  Chain = DAG.getCALLSEQ_START(Chain, NumBytes, 0 /*OutSize*/, DL);

  // Copy argument values to their designated locations.
  SmallVector<std::pair<Register, SDValue>, 8> RegsToPass;
  SmallVector<SDValue, 8> MemOpChains;
  SDValue StackPtr;
  for (std::size_t I = 0, J = 0, E = ArgLocs.size(); I != E; ++I) {
    const CCValAssign &VA = ArgLocs[I];
    SDValue ArgValue = OutVals[I];
    ISD::ArgFlagsTy Flags = Outs[I].Flags;
    // Promote the value if needed.
    // For now, only handle fully promoted and indirect arguments.
    if (VA.getLocInfo() != CCValAssign::Indirect) {
      assert(VA.getLocInfo() == CCValAssign::Full);
    } else {
      // Store the argument in a stack slot and pass its address.
      Align StackAlign =
          std::max(getPrefTypeAlign(DAG, Outs[I].ArgVT),
                   getPrefTypeAlign(DAG, ArgValue.getValueType()));
      TypeSize StoredSize = ArgValue.getValueType().getStoreSize();
      const auto ArgIndex = Outs[I].OrigArgIndex;
      const auto ArgPartOffset = Outs[I].PartOffset;
      assert(ArgPartOffset == 0);
      // Calculate the total size to store. We don't have access to what we're
      // actually storing other than performing the loop and collecting the
      // info.
      SmallVector<std::pair<SDValue, SDValue>> Parts;
      for (; I + 1 != E && Outs[I + 1].OrigArgIndex == ArgIndex; ++I) {
        SDValue PartValue = OutVals[I + 1];
        EVT PartVT = PartValue.getValueType();
        StoredSize += PartVT.getStoreSize();
        StackAlign = std::max(StackAlign, getPrefTypeAlign(DAG, PartVT));
        const unsigned PartOffset = Outs[I + 1].PartOffset - ArgPartOffset;
        Parts.emplace_back(PartValue, DAG.getIntPtrConstant(PartOffset, DL));
      }

      SDValue SpillSlot = DAG.CreateStackTemporary(StoredSize, StackAlign);
      int FI = cast<FrameIndexSDNode>(SpillSlot)->getIndex();
      MemOpChains.push_back(
          DAG.getStore(Chain, DL, ArgValue, SpillSlot,
                       MachinePointerInfo::getFixedStack(MF, FI)));
      for (const auto &[PartValue, PartOffset] : Parts) {
        SDValue Address =
            DAG.getNode(ISD::ADD, DL, PtrVT, SpillSlot, PartOffset);
        MemOpChains.push_back(
            DAG.getStore(Chain, DL, PartValue, Address,
                         MachinePointerInfo::getFixedStack(MF, FI)));
      }
      ArgValue = SpillSlot;
    }

    // Use local copy if it is a byval arg.
    if (Flags.isByVal())
      ArgValue = ByValArgs[J++];

    if (VA.isRegLoc()) {
      // Queue up the argument copies and emit them at the end.
      RegsToPass.emplace_back(VA.getLocReg(), ArgValue);
    } else {
      assert(VA.isMemLoc() && "Argument not register or memory");

      // Work out the address of the stack slot.
      if (!StackPtr.getNode())
        StackPtr = DAG.getCopyFromReg(Chain, DL, DAArch34::D2, PtrVT);
      SDValue Address =
          DAG.getNode(ISD::ADD, DL, PtrVT, StackPtr,
                      DAG.getIntPtrConstant(VA.getLocMemOffset(), DL));

      // Emit the store.
      MemOpChains.push_back(
          DAG.getStore(Chain, DL, ArgValue, Address, MachinePointerInfo()));
    }
  }

  // Join the stores, which are independent of one another.
  if (!MemOpChains.empty())
    Chain = DAG.getNode(ISD::TokenFactor, DL, MVT::Other, MemOpChains);

  SDValue Glue;

  // Build a sequence of copy-to-reg nodes, chained and glued together.
  for (auto &[Reg, SDVal] : RegsToPass) {
    Chain = DAG.getCopyToReg(Chain, DL, Reg, SDVal, Glue);
    Glue = Chain.getValue(1);
  }

  auto Callee = CLI.Callee;

  // No external symbols support
  if (GlobalAddressSDNode *S = dyn_cast<GlobalAddressSDNode>(Callee)) {
    // llvm_unreachable("How do i suppose to lower this?");
    const GlobalValue *GV = S->getGlobal();
    assert(getTargetMachine().shouldAssumeDSOLocal(GV));
    Callee = DAG.getTargetGlobalAddress(GV, DL, PtrVT, 0, 0);
  }

  // The first call operand is the chain and the second is the target address.
  SmallVector<SDValue, 8> Ops{Chain, Callee};

  // Add argument registers to the end of the list so that they are
  // known live into the call.
  for (auto &[Reg, SDVal] : RegsToPass)
    Ops.push_back(DAG.getRegister(Reg, SDVal.getValueType()));

  // Add a register mask operand representing the call-preserved registers.
  const TargetRegisterInfo *TRI = STI.getRegisterInfo();
  const uint32_t *Mask = TRI->getCallPreservedMask(MF, CLI.CallConv);
  Ops.push_back(DAG.getRegisterMask(Mask));

  // Glue the call to the argument copies, if any.
  if (Glue.getNode())
    Ops.push_back(Glue);

  // Emit the call.
  SDVTList NodeTys = DAG.getVTList(MVT::Other, MVT::Glue);

  Chain = DAG.getNode(DAArch34ISD::CALL, DL, NodeTys, Ops);
  DAG.addNoMergeSiteInfo(Chain.getNode(), CLI.NoMerge);
  Glue = Chain.getValue(1);

  // Mark the end of the call, which is glued to the call itself.
  Chain = DAG.getCALLSEQ_END(Chain, DAG.getConstant(NumBytes, DL, PtrVT, true),
                             DAG.getConstant(0, DL, PtrVT, true), Glue, DL);
  Glue = Chain.getValue(1);

  // Assign locations to each value returned by this call.
  SmallVector<CCValAssign, 16> RVLocs;
  CCState RetCCInfo(CLI.CallConv, CLI.IsVarArg, MF, RVLocs, *DAG.getContext());
  RetCCInfo.AnalyzeCallResult(CLI.Ins, RetCC_DAArch34);

  // Copy all of the result registers out of their specified physreg.
  for (auto &VA : RVLocs) {
    // Copy the value out
    SDValue RetValue =
        DAG.getCopyFromReg(Chain, DL, VA.getLocReg(), VA.getLocVT(), Glue);
    // Glue the RetValue to the end of the call sequence
    Chain = RetValue.getValue(1);
    Glue = RetValue.getValue(2);

    assert(VA.getLocInfo() == CCValAssign::Full);
    InVals.push_back(RetValue);
  }

  return Chain;
}

namespace {

// Convert Val to a ValVT. Should not be called for CCValAssign::Indirect
// values.
SDValue convertLocVTToValVT(SelectionDAG &DAG, SDValue Val,
                            const CCValAssign &VA, const SDLoc &DL) {
  if (VA.getValVT() != MVT::SimpleValueType::i32)
    llvm_unreachable("types other than i32 are not supported");

  switch (VA.getLocInfo()) {
  case CCValAssign::LocInfo::Full:
    return Val;
  case CCValAssign::LocInfo::BCvt:
    return DAG.getNode(ISD::BITCAST, DL, VA.getValVT(), Val);
  default:
    llvm_unreachable("Unexpected CCValAssign::LocInfo");
  }
}

// The implementation should be changed if more than one type is supported
SDValue convertValVTToLocVT(SelectionDAG &DAG, SDValue Val,
                            const CCValAssign &VA, const SDLoc &DL) {
  return convertLocVTToValVT(DAG, Val, VA, DL);
}

SDValue unpackFromRegLoc(SelectionDAG &DAG, SDValue Chain,
                         const CCValAssign &VA, const SDLoc &DL,
                         const DAArch34TargetLowering &TLI) {
  EVT LocVT = VA.getLocVT();

  MachineRegisterInfo &RegInfo = DAG.getMachineFunction().getRegInfo();
  const TargetRegisterClass *RC = TLI.getRegClassFor(LocVT.getSimpleVT());
  Register VReg = RegInfo.createVirtualRegister(RC);
  RegInfo.addLiveIn(VA.getLocReg(), VReg);

  SDValue Val = DAG.getCopyFromReg(Chain, DL, VReg, LocVT);
  if (VA.getLocInfo() == CCValAssign::Indirect)
    return Val;
  return convertLocVTToValVT(DAG, Val, VA, DL);
}

// The caller is responsible for loading the full value if the argument is
// passed with CCValAssign::Indirect.
SDValue unpackFromMemLoc(SelectionDAG &DAG, SDValue Chain,
                         const CCValAssign &VA, const SDLoc &DL) {
  // LoadExtType - load with extension
  const auto ExtType = [&VA] {
    switch (VA.getLocInfo()) {
    case CCValAssign::LocInfo::Full:     // The value fills the full location
    case CCValAssign::LocInfo::Indirect: // The location contains pointer to the
                                         // value.
    case CCValAssign::LocInfo::BCvt:     // The value is bit-converted in the
                                         // location.
      return ISD::LoadExtType::NON_EXTLOAD; // no extension
    default:
      llvm_unreachable("Unexpected CCValAssign::LocInfo");
    }
  }();

  const EVT LocVT = VA.getLocVT();
  const EVT ValVT = VA.getValVT();
  MachineFrameInfo &MFI = DAG.getMachineFunction().getFrameInfo();
  const int FI =
      MFI.CreateFixedObject(ValVT.getStoreSize(), VA.getLocMemOffset(),
                            /*IsImmutable=*/true);
  const EVT PtrVT =
      MVT::getIntegerVT(DAG.getDataLayout().getPointerSizeInBits(0));

  return DAG.getExtLoad(
      ExtType, DL, LocVT, Chain, DAG.getFrameIndex(FI, PtrVT),
      MachinePointerInfo::getFixedStack(DAG.getMachineFunction(), FI), ValVT);
}

} // end anonymous namespace

SDValue DAArch34TargetLowering::LowerFormalArguments(
    SDValue Chain, CallingConv::ID CallConv, bool IsVarArg,
    const SmallVectorImpl<ISD::InputArg> &Ins, const SDLoc &DL,
    SelectionDAG &DAG, SmallVectorImpl<SDValue> &InVals) const {
  switch (CallConv) {
  case CallingConv::C:
  case CallingConv::Fast:
    break;
  default:
    report_fatal_error("Unsupported calling convention");
  }

  MachineFunction &MF = DAG.getMachineFunction();
  EVT PtrVT = getPointerTy(DAG.getDataLayout());
  unsigned StackSlotSize = MVT(MVT::i32).getSizeInBits() / 8;
  // Used with vargs to acumulate store chains.

  // Assign locations to all of the incoming arguments.
  SmallVector<CCValAssign, 16> ArgLocs;
  CCState CCInfo(CallConv, IsVarArg, MF, ArgLocs, *DAG.getContext());
  CCInfo.AnalyzeFormalArguments(Ins, CC_DAArch34);

  for (std::size_t I = 0, E = ArgLocs.size(); I != E; ++I) {
    CCValAssign &VA = ArgLocs[I];

    SDValue ArgValue;
    if (VA.isRegLoc())
      ArgValue = unpackFromRegLoc(DAG, Chain, VA, DL, *this);
    else
      ArgValue = unpackFromMemLoc(DAG, Chain, VA, DL);

    if (VA.getLocInfo() == CCValAssign::Indirect) {
      InVals.push_back(DAG.getLoad(VA.getValVT(), DL, Chain, ArgValue,
                                   MachinePointerInfo()));
      const auto ArgIndex = Ins[I].OrigArgIndex;
      const auto ArgPartOffset = Ins[I].PartOffset;
      assert(ArgPartOffset == 0);
      for (; I + 1 != E && Ins[I + 1].OrigArgIndex == ArgIndex; ++I) {
        const auto PartOffset = Ins[I + 1].PartOffset - ArgPartOffset;
        SDValue Offset = DAG.getIntPtrConstant(PartOffset, DL);
        SDValue Address = DAG.getNode(ISD::ADD, DL, PtrVT, ArgValue, Offset);
        InVals.push_back(DAG.getLoad(ArgLocs[I + 1].getValVT(), DL, Chain,
                                     Address, MachinePointerInfo()));
      }
      continue;
    }

    InVals.push_back(ArgValue);
  }

  std::vector<SDValue> OutChains;

  if (IsVarArg) {
    ArrayRef<MCPhysReg> ArgRegs(ArgGPRs);
    const auto Idx = CCInfo.getFirstUnallocated(ArgRegs);

    int VaArgOffset, VarArgsSaveSize;

    // If all registers are allocated, then all varargs must be passed on the
    // stack and we don't need to save any argregs.
    if (ArgRegs.size() == Idx) {
      VaArgOffset = CCInfo.getStackSize();
      VarArgsSaveSize = 0;
    } else {
      VarArgsSaveSize = StackSlotSize * (ArgRegs.size() - Idx);
      VaArgOffset = -VarArgsSaveSize;
    }

    // Record the frame index of the first variable argument
    // which is a value necessary to VASTART.
    MachineFrameInfo &MFI = MF.getFrameInfo();
    auto *UFI = MF.getInfo<DAArch34FunctionInfo>();
    int FI = MFI.CreateFixedObject(StackSlotSize, VaArgOffset, true);
    UFI->setVarArgsFrameIndex(FI);

    // If saving an odd number of registers then create an extra stack slot to
    // ensure that the frame pointer is 2*XLEN-aligned, which in turn ensures
    // offsets to even-numbered registered remain 2*XLEN-aligned.
    if (Idx % 2) {
      MFI.CreateFixedObject(StackSlotSize, VaArgOffset - (int)StackSlotSize,
                            true);
      VarArgsSaveSize += StackSlotSize;
    }

    MachineRegisterInfo &RegInfo = MF.getRegInfo();

    // Copy the integer registers that may have been used for passing varargs
    // to the vararg save area.
    for (std::size_t I = Idx; I < ArgRegs.size();
         ++I, VaArgOffset += StackSlotSize) {
      const Register Reg =
          RegInfo.createVirtualRegister(&DAArch34::GPRRegClass);
      RegInfo.addLiveIn(ArgRegs[I], Reg);
      const SDValue ArgValue = DAG.getCopyFromReg(Chain, DL, Reg, MVT::i32);
      FI = MFI.CreateFixedObject(StackSlotSize, VaArgOffset, true);
      const SDValue PtrOff =
          DAG.getFrameIndex(FI, getPointerTy(DAG.getDataLayout()));
      const SDValue Store =
          DAG.getStore(Chain, DL, ArgValue, PtrOff,
                       MachinePointerInfo::getFixedStack(MF, FI));
      cast<StoreSDNode>(Store.getNode())
          ->getMemOperand()
          ->setValue(static_cast<Value *>(nullptr));
      OutChains.push_back(Store);
    }
    UFI->setVarArgsSaveSize(VarArgsSaveSize);
  }

  // All stores are grouped in one node to allow the matching between
  // the size of Ins and InVals. This only happens for vararg functions.
  if (!OutChains.empty()) {
    OutChains.push_back(Chain);
    return DAG.getNode(ISD::TokenFactor, DL, MVT::Other, OutChains);
  }
  return Chain;
}

SDValue
DAArch34TargetLowering::LowerReturn(SDValue Chain, CallingConv::ID CallConv,
                                    bool IsVarArg,
                                    const SmallVectorImpl<ISD::OutputArg> &Outs,
                                    const SmallVectorImpl<SDValue> &OutVals,
                                    const SDLoc &DL, SelectionDAG &DAG) const {
  SmallVector<CCValAssign, 16> RVLocs;
  CCState CCInfo(CallConv, IsVarArg, DAG.getMachineFunction(), RVLocs,
                 *DAG.getContext());

  /// AnalyzeReturn - Analyze the returned values of a return,
  /// incorporating info about the result values into this state.
  CCInfo.AnalyzeReturn(Outs, RetCC_DAArch34);

  SDValue Glue;
  SmallVector<SDValue, 4> RetOps(1, Chain);

  // Copy the result values into the output registers.
  for (std::size_t I = 0, E = RVLocs.size(); I != E; ++I) {
    const CCValAssign &VA = RVLocs[I];
    assert(VA.isRegLoc() && "Can only return in registers!");

    const Register LocReg = VA.getLocReg();
    Chain = DAG.getCopyToReg(
        Chain, DL, LocReg, convertValVTToLocVT(DAG, OutVals[I], VA, DL), Glue);

    // Guarantee that all emitted copies are stuck together.
    Glue = Chain.getValue(1);
    RetOps.push_back(DAG.getRegister(LocReg, VA.getLocVT()));
  }

  RetOps[0] = Chain; // Update chain.

  // Add the glue node if we have it.
  if (Glue.getNode()) {
    RetOps.push_back(Glue);
  }

  return DAG.getNode(DAArch34ISD::RET, DL, MVT::Other, RetOps);
}

bool DAArch34TargetLowering::CanLowerReturn(
    CallingConv::ID CallConv, MachineFunction &MF, bool IsVarArg,
    const SmallVectorImpl<ISD::OutputArg> &Outs, LLVMContext &Context) const {
  SmallVector<CCValAssign, 16> RVLocs;
  CCState CCInfo(CallConv, IsVarArg, MF, RVLocs, Context);

  /* CheckReturn - Analyze the return values of a function, returning
   * true if the return can be performed without sret-demotion, and
   * false otherwise.
   *
   * Note: sret-demotion (or structure return demotion) is a compiler
   * optimization technique used to transform "return by reference" (or "return
   * by hidden pointer") into a "return by value", making function calls more
   * efficient when possible (when structures are small enough).
   */

  if (!CCInfo.CheckReturn(Outs, RetCC_DAArch34))
    return false;
#if 0
  if (IsVarArg && CCInfo.getStackSize())
    llvm_unreachable("functions with variadic arguments are not supported");
#endif
  return true;
}

//===----------------------------------------------------------------------===//
// Target Optimization Hooks
//===----------------------------------------------------------------------===//

SDValue DAArch34TargetLowering::PerformDAGCombine(SDNode *N,
                                                  DAGCombinerInfo &DCI) const {
  return SDValue{};
}

// AddrMode represents an addressing mode of:
//    BaseGV + BaseOffs + BaseReg + Scale*ScaleReg + ScalableOffset*vscale
// Notes:
// - BaseGV represents addressing relative to a global object
// - vscale is used for scalable vector types
bool DAArch34TargetLowering::isLegalAddressingMode(const DataLayout &DL,
                                                   const AddrMode &AM, Type *Ty,
                                                   unsigned AS,
                                                   Instruction *I) const {
  // No global is ever allowed as a base.
  if (AM.BaseGV)
    return false;

  // There are 16 bits for an immediate in the instruction encoding
  if (!isInt<16>(AM.BaseOffs))
    return false;

  switch (AM.Scale) {
  case 0:                  // If Scale is zero, there is no ScaleReg.
    return true;           // "r+i" or just "i", depending on HasBaseReg.
  case 1:                  // Scale of 1 indicates a reg with no scale.
    return !AM.HasBaseReg; // allow "r+i", disallow "r+r" or "r+r+i".
  default:
    return false;
  }
}

} // end namespace llvm
