include "llvm/Target/Target.td"
include "DAArch34RegisterInfo.td"
include "DAArch34InstrInfo.td"

def DAArch34InstrInfo : InstrInfo;

def DAArch34 : Target {
    let InstructionSet = DAArch34InstrInfo;
}
