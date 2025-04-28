#include "llvm/ADT/StringRef.h"
#include "llvm/CodeGen/SelectionDAGISel.h"
#include "llvm/CodeGen/SelectionDAGNodes.h"
#include "llvm/Support/CodeGen.h"

#include "DAArch34TargetMachine.h"

#define GET_INSTRINFO_ENUM // for enum with instructions
#include "DAArch34GenInstrInfo.inc"

#define GET_REGINFO_ENUM // for enum with register
#include "DAArch34GenRegisterInfo.inc"

#include "DAArch34ISelLowering.h"

#define DEBUG_TYPE "daarch34-isel"

namespace llvm {

namespace {

class DAArch34DagToDagISel : public SelectionDAGISel {
  const DAArch34Subtarget *Subtarget = nullptr;

public:
  static char ID;

  explicit DAArch34DagToDagISel(DAArch34TargetMachine &TM,
                                CodeGenOptLevel OptLevel)
      : SelectionDAGISel(TM, OptLevel) {}

  bool runOnMachineFunction(MachineFunction &MF) override {
    Subtarget = &MF.getSubtarget<DAArch34Subtarget>();
    return SelectionDAGISel::runOnMachineFunction(MF);
  }

  bool SelectAddrFrameIndex(SDValue Addr, SDValue &Base, SDValue &Offset) {
    if (auto *FIN = dyn_cast<FrameIndexSDNode>(Addr)) {
      Base = CurDAG->getTargetFrameIndex(FIN->getIndex(), MVT::i32);
      Offset = CurDAG->getTargetConstant(0, SDLoc(Addr), MVT::i32);
      return true;
    }

    return false;
  }

  bool SelectAddrRegImm(SDValue Addr, SDValue &Base, SDValue &Offset) {
    if (SelectAddrFrameIndex(Addr, Base, Offset))
      return true;

    MVT VT = Addr.getSimpleValueType();

    // checks that Addr is an add-like operation with operand 1 being a constant
    if (CurDAG->isBaseWithConstantOffset(Addr)) {
      const int64_t CVal =
          cast<ConstantSDNode>(Addr.getOperand(1))->getSExtValue();
      if (isInt<16>(CVal)) {
        Base = Addr.getOperand(0);
        if (auto *FIN = dyn_cast<FrameIndexSDNode>(Base))
          Base = CurDAG->getTargetFrameIndex(FIN->getIndex(), VT);
        Offset = CurDAG->getTargetConstant(CVal, SDLoc{Addr}, VT);
        return true;
      }
    }

    Base = Addr;
    Offset = CurDAG->getTargetConstant(0, SDLoc{Addr}, VT);
    return true;
  }

  void Select(SDNode *Node) override;
#include "DAArch34GenDAGISel.inc"
};

void DAArch34DagToDagISel::Select(SDNode *Node) {
  if (Node->isMachineOpcode()) {
    Node->setNodeId(-1);
    return;
  }
  SDLoc DL(Node);
  SelectCode(Node);
}

class DAArch34DAGToDAGISelLegacy : public SelectionDAGISelLegacy {
public:
  static char ID;

  explicit DAArch34DAGToDAGISelLegacy(DAArch34TargetMachine &TM,
                                      CodeGenOptLevel OptLevel)
      : SelectionDAGISelLegacy(
            ID, std::make_unique<DAArch34DagToDagISel>(TM, OptLevel)) {}

  StringRef getPassName() const override {
    return "DAArch34 DAG->DAG pattern instruction selection";
  }
};

char DAArch34DAGToDAGISelLegacy::ID = 0;

} // end unnamed namespace

FunctionPass *createDAArch34ISelDag(DAArch34TargetMachine &TM,
                                    CodeGenOptLevel OptLevel) {
  return new DAArch34DAGToDAGISelLegacy(TM, OptLevel);
}

} // end namespace llvm
