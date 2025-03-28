include "llvm/Target/Target.td"
// include "clang/AST/PropertiesBase.td"

include "DAArch34RegisterInfo.td"
include "DAArch34InstrInfo.td"
include "DAArch34CallingConv.td"

// Processor Itinearies - the scheduling information for the target processor
class Proc<string Name, list<SubtargetFeature> Features>
    : Processor<Name, NoItineraries, Features>;

def : Proc<"generic", []>;

def DAArch34InstrInfo : InstrInfo;

def DAArch34 : Target {
    let InstructionSet = DAArch34InstrInfo;
}
