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

#define DEBUG_TYPE "sim-isel"

namespace llvm {

namespace {

class DAArch34DagToDagISel : public SelectionDAGISel {
public:
  static char ID;
  explicit DAArch34DagToDagISel(DAArch34TargetMachine &TM,
                                CodeGenOptLevel OptLevel)
      : SelectionDAGISel(TM, OptLevel) {}

  bool runOnMachineFunction(MachineFunction &MF) override {
    return SelectionDAGISel::runOnMachineFunction(MF);
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
