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

def btarget16 : Operand<OtherVT> {
    let ParserMatchClass = SImmAsmOperand<16>;
    let EncoderMethod = "getBranchTarget16OpValue";
    let DecoderMethod = "decodeSImmOperand<16>";
    let MCOperandPredicate = [{
        if (int64_t Imm; MCOp.evaluateAsConstantImm(Imm))
            return isInt<16>(Imm);
        return MCOp.isBareSymbolRef();
    }];
    let OperandType = "OPERAND_PCREL";
    let PrintMethod = "printBranchOperand";
}

let hasSideEffects = false,
    mayLoad = false,
    mayStore = false,
    isBarrier = true,
    isBranch = true,
    isTerminator = true,
    rd = 0,
    rs1 = 0 in
def B : DAArch34Instr<0x1, (outs), (ins btarget16:$rs2_or_imm), "b $rs2_or_imm",
                      [(br bb:$rs2_or_imm)]>;

let hasSideEffects = false,
    mayLoad = false,
    mayStore = false,
    isBarrier = true,
    isBranch = true,
    isTerminator = true in
class DAArch34BrCond<int Opcode, CondCode cc, string Name>
    : DAArch34Instr<Opcode,
                    (outs),
                    (ins GPRExceptZero:$rd, GPRExceptZero:$rs1, btarget16:$rs2_or_imm),
                    Name # " $rd, $rs1, $rs2_or_imm",
                    [(brcc cc, GPRExceptZero:$rd, GPRExceptZero:$rs1, bb:$rs2_or_imm)]>;

def BEQ : DAArch34BrCond<0x2, SETEQ, "beq">;
def BNE : DAArch34BrCond<0x3, SETNE, "bne">;
def BLT : DAArch34BrCond<0x4, SETLT, "blt">;
def BGE : DAArch34BrCond<0x5, SETGE, "bge">;


let hasSideEffects = false,
    mayLoad = false,
    mayStore = true in
def ST : DAArch34Instr<0x6, (outs), (ins GPR:$rs1, GPRExceptZero:$rd, simm16:$rs2_or_imm),
                       "st $rs1, [$rd, $rs2_or_imm]", []>;

def AddrRegImm : ComplexPattern<iPTR, // type of return value
                                2, // the number of operands
                                "SelectAddrRegImm">; // function used for pattern matching

def : Pat<(store (i32 GPR:$rs1), (AddrRegImm (i32 GPRExceptZero:$rd), simm16:$rs2_or_imm)),
          (ST GPR:$rs1, GPRExceptZero:$rd, simm16:$rs2_or_imm)>;


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
def BR : DAArch34Instr<0x7, (outs), (ins GPRExceptZero:$rs1), "br $rs1",
                       [(brind GPRExceptZero:$rs1)]>;

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
                PseudoInstExpansion<(BR D10)>;

let isAsCheapAsAMove = true,
    isCodeGenOnly = true,
    isReMaterializable = true,
    rs1 = 0 in
def LI : DAArch34Instr<0x8, (outs GPRExceptZero:$rd), (ins simm16:$rs2_or_imm),
                       "li $rd, $rs2_or_imm", [(set GPRExceptZero:$rd, simm16:$rs2_or_imm)]>;

let isAsCheapAsAMove = true,
    isReMaterializable = true,
    hasSideEffects = false,
    mayLoad = false,
    mayStore = false,
    rs2_or_imm = 0 in
def MOV : DAArch34Instr<0x9, (outs GPRExceptZero:$rd), (ins GPR:$rs1), "mov $rd, $rs1",
                        [(set GPRExceptZero:$rd, GPR:$rs1)]>;

let rd = 0 in
def putPixel : DAArch34Instr<0x3D, (outs), (ins GPR:$rs1, GPR:$rs2_or_imm),
                             "putPixel $rs1, $rs2_or_imm",
                             [(int_daarch34_put_pixel GPR:$rs1, GPR:$rs2_or_imm)]>;

def setColor : DAArch34Instr<0x3E, (outs), (ins GPR:$rd, GPR:$rs1, GPR:$rs2_or_imm),
                             "setColor $rd, $rs1, $rs2_or_imm",
                             [(int_daarch34_set_color GPR:$rd, GPR:$rs1, GPR:$rs2_or_imm)]>;

let rd = 0,
    rs1 = 0,
    rs2_or_imm = 0 in
def updateScreen : DAArch34Instr<0x3F, (outs), (ins), "updateScreen",
                                 [(int_daarch34_update_screen)]>;

def Rand : DAArch34PseudoInstr<(outs GPRExceptZero:$rd), (ins), "# pseudo Rand",
                               [(set GPRExceptZero:$rd, (int_daarch34_rand))]>,
           PseudoInstExpansion<(MOV GPRExceptZero:$rd, D0)>;
