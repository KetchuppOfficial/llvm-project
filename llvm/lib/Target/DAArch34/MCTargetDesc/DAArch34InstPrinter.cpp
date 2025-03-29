#include <cassert>

#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCInst.h"
#include "llvm/Support/raw_ostream.h"

#include "MCTargetDesc/DAArch34InstPrinter.h"

#define DEBUG_TYPE "asm-printer"

namespace llvm {

#include "DAArch34GenAsmWriter.inc"

void DAArch34InstPrinter::printRegName(raw_ostream &O, MCRegister Reg) const {
  O << getRegisterName(Reg);
}

void DAArch34InstPrinter::printInst(const MCInst *MI, uint64_t Address,
                                    StringRef Annot, const MCSubtargetInfo &STI,
                                    raw_ostream &O) {
  printInstruction(MI, Address, O);
  printAnnotation(O, Annot);
}

void DAArch34InstPrinter::printOperand(const MCInst *MI, unsigned OpNo,
                                       raw_ostream &O) {
  if (const MCOperand &MO = MI->getOperand(OpNo); MO.isReg()) {
    printRegName(O, MO.getReg());
  } else if (MO.isImm()) {
    O << MO.getImm();
  } else {
    assert(MO.isExpr() && "Unknown operand kind in printOperand");
    MO.getExpr()->print(O, &MAI);
  }
}

} // end namespace llvm
