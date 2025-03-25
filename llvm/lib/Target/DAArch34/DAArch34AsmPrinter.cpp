#include <memory>
#include <utility>

#include "llvm/ADT/StringRef.h"
#include "llvm/CodeGen/AsmPrinter.h"
#include "llvm/CodeGen/MachineInstr.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/TargetRegistry.h" // for RegisterAsmPrinter
#include "llvm/Target/TargetMachine.h"

#include "TargetInfo/DAArch34TargetInfo.h" // for getTheDAArch34Target

#define GET_INSTRINFO_ENUM // for enum with instructions
#include "DAArch34GenInstrInfo.inc"

#define GET_REGINFO_ENUM // for enum with register
#include "DAArch34GenRegisterInfo.inc"

#define DEBUG_TYPE "asm-printer"

namespace llvm {

class MCSubtargetInfo;

namespace {

class DAArch34AsmPrinter : public AsmPrinter {
public:
  explicit DAArch34AsmPrinter(TargetMachine &TM,
                              std::unique_ptr<MCStreamer> Streamer)
      : AsmPrinter(TM, std::move(Streamer)), STI{TM.getMCSubtargetInfo()} {}

  void emitInstruction(const MachineInstr *MI) override;

  /// getPassName - Return a nice clean name for a pass.  This usually
  /// implemented in terms of the name that is registered by one of the
  /// Registration templates, but can be overloaded directly.
  StringRef getPassName() const override { return "DAArch34 Assembly Printer"; }

  bool emitPseudoExpansionLowering(MCStreamer &OutStreamer,
                                   const MachineInstr *MI);

private:
  const MCSubtargetInfo *STI;
};

#include "DAArch34GenMCPseudoLowering.inc"

void DAArch34AsmPrinter::emitInstruction(const MachineInstr *MI) {
  emitPseudoExpansionLowering(*OutStreamer, MI);
}

} // end anonymous namespace

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeDAArch34AsmPrinter() {
  RegisterAsmPrinter<DAArch34AsmPrinter> X(getTheDAArch34Target());
}

} // end namespace llvm
