#ifndef LLVM_LIB_TARGET_DAARCH34_DAARCH34INSTLOWER_H
#define LLVM_LIB_TARGET_DAARCH34_DAARCH34INSTLOWER_H

namespace llvm {

class AsmPrinter;
class MachineInstr;
class MachineOperand;
class MCInst;
class MCOperand;

bool lowerDAArch34MachineInstrToMCInst(const MachineInstr *MI, MCInst &OutMI,
                                       AsmPrinter &AP);

bool lowerDAArch34MachineOperandToMCOperand(const MachineOperand &MO,
                                            MCOperand &MCOp,
                                            const AsmPrinter &AP);

} // end namespace llvm

#endif // LLVM_LIB_TARGET_DAARCH34_DAARCH34INSTLOWER_H
