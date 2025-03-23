#ifndef DAARCH_REGISTER_INFO_TD
#define DAARCH_REGISTER_INFO_TD

include "llvm/Target/Target.td"

class DAArch34Reg<bits<5> Enc, string N> : Register<N> {
    let HWEncoding{4-0} = Enc;
    let Namespace = "DAArch34";
}

foreach I = 0-31 in {
    def D#I : DAArch34Reg<I, "D"#I>;
}

def GPR : RegisterClass<"DAArch34", [i32], 32, (add (sequence "D%u", 0, 31))>;

#endif // DAARCH_REGISTER_INFO_TD
