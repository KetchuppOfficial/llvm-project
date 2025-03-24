// include "llvm/Target/Target.td" before this file

include "DAArch34InstrFormats.td"

class SImmAsmOperand<int Width> : AsmOperandClass {
    let Name = "SImm"#Width;
    let RenderMethod = "addImmOperands";
    let DiagnosticType = !strconcat("Invalid", Name);
}

def simm16 : Operand<i32>, ImmLeaf<i32, [{ return isInt<16>(Imm); }]> {
    let ParserMatchClass = SImmAsmOperand<16>;
    let EncoderMethod = "getImmOpValue";
    let DecoderMethod = "decodeSImmOperand<16>";
    let MCOperandPredicate = [{
        int64_t Imm;
        if (MCOp.evaluateAsConstantImm(Imm))
            return isInt<16>(Imm);
        return MCOp.isBareSymbolRef();
    }];
    let OperandType = "kSIMM16";
    let OperandNamespace = "DAArch34Op";
}

// Setting opcodes to 0x0 is a temporary stub

// "brind" stands for "branch indirect". It's a def of SDNode (see TargetSelectionDAG.td)
let hasSideEffects = false,
    mayLoad = false,
    mayStore = false,
    isCall = true,
    isBranch = true,
    isIndirectBranch = true,
    isBarrier = true,
    isTerminator = true in
def BR : DAArch34Instr<0x0, (outs), (ins GPR:$rs1), "BR $rs1", [(brind GPR:$rs1)]>;

def daarch34_ret
    : SDNode<"DAArch34ISD::RET", // opcode
             SDTNone, // type requirements of a Selection DAG node
             // properties
             [SDNPHasChain, // R/W chain operand and result
              SDNPOptInGlue, // Read a flag operand
              SDNPVariadic]>; // Node has variable arguments

let isBarrier = true,
    isReturn = true,
    isTerminator = true in
def PseudoRET : DAArch34PseudoInstr<(outs), (ins), "# PseudoRET", [(daarch34_ret)]>,
                PseudoInstExpansion<(BR D0)>;

let isAsCheapAsAMove = true,
    isCodeGenOnly = true,
    isReMaterializable = true in
def MOVLI : DAArch34Instr<0x0, (outs GPR:$rd), (ins simm16:$imm), "MOVli $rd $imm",
                         [(set GPR:$rd, simm16:$imm)]>;
