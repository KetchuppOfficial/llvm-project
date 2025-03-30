// include "llvm/Target/Target.td" before this file

include "DAArch34InstrFormats.td"

class SImmAsmOperand<int Width> : AsmOperandClass {
    let Name = "SImm"#Width;
    let RenderMethod = "addImmOperands";
    let DiagnosticType = !strconcat("Invalid", Name);
}

def simm16 : Operand<i32>, ImmLeaf<i32, [{ return isInt<16>(Imm); }]> {
    let ParserMatchClass = SImmAsmOperand<16>;
    let EncoderMethod = "getSImm16OpValue";
    let DecoderMethod = "decodeSImmOperand<16>";
    let MCOperandPredicate = [{
        if (int64_t Imm; MCOp.evaluateAsConstantImm(Imm))
            return isInt<16>(Imm);
        return MCOp.isBareSymbolRef();
    }];
    let OperandType = "kSIMM16";
    let OperandNamespace = "DAArch34Op";
}

// "brind" stands for "branch indirect". It's a def of SDNode (see TargetSelectionDAG.td)
let hasSideEffects = false,
    mayLoad = false,
    mayStore = false,
    isCall = true,
    isBranch = true,
    isIndirectBranch = true,
    isBarrier = true,
    isTerminator = true,
    rd = 0,
    rs2_or_imm = 0 in
def BR : DAArch34Instr<0x2, (outs), (ins GPR:$rs1), "br $rs1", [(brind GPR:$rs1)]>;

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
def PseudoRET : DAArch34PseudoInstr<(outs), (ins), "# pseudo ret", [(daarch34_ret)]>,
                PseudoInstExpansion<(BR D0)>;

let isAsCheapAsAMove = true,
    isCodeGenOnly = true,
    isReMaterializable = true,
    rs1 = 0 in
def LI : DAArch34Instr<0x1, (outs GPR:$rd), (ins simm16:$rs2_or_imm), "li $rd, $rs2_or_imm",
                      [(set GPR:$rd, simm16:$rs2_or_imm)]>;
