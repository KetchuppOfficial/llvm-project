// include "llvm/Target/Target.td" before this file

class DAArch34Instr<bits<6> Op, dag Outs, dag Ins, string Asm, list<dag> pattern> : Instruction {
    let Namespace = "DAArch34";
    let DecoderNamespace = "DAArch34";
    let OutOperandList = Outs;
    let InOperandList = Ins;
    let AsmString = Asm;
    let Pattern = pattern;

    let Size = 4;

    field bits<32> Instr;

    bits<5> rd = 0;
    bits<5> rs1 = 0;
    bits<16> rs2_or_imm = 0;

    let Instr{5-0} = Op;
    let Instr{6-10} = rd;
    let Instr{11-15} = rs1;
    let Instr{16-31} = rs2_or_imm;
}

class DAArch34PseudoInstr<dag Outs, dag Ins, string Asm, list<dag> Pattern>
    : DAArch34Instr<0, Outs, Ins, Asm, Pattern> {
    let isPseudo = true;
    let isCodeGenOnly = true;
}
