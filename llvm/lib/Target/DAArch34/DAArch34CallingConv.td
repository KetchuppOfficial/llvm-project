/// CCIfType - If the current argument is one of the specified types, apply
/// Action A.

def RetCC_DAArch34 : CallingConv<[
    /// CCAssignToReg - This action matches if there is a register in the specified
    /// list that is still available.  If so, it assigns the value to the first
    /// available register and succeeds.
    CCIfType<[i32], CCAssignToReg<[D10, D11]>>, // return values

    /// CCAssignToStack - This action always matches: it assigns the value to a
    /// stack slot of the specified size and alignment on the stack.  If size is
    /// zero then the ABI size is used; if align is zero then the ABI alignment
    /// is used - these may depend on the target or subtarget.
    CCIfType<[i32], CCAssignToStack<4, 4>>
]>;

def CC_DAArch34 : CallingConv<[
    /// CCPromoteToType - If applied, this promotes the specified current value to
    /// the specified type.
    CCIfType<[i8, i16], CCPromoteToType<i32>>,

    CCIfType<[i32], CCAssignToReg<[D10, D11, D12, D13, D14, D15, D16, D17]>>, // function arguments

    CCIfType<[i32], CCAssignToStack<4, 4>>
]>;

def CalleeSaved_DAArch34 : CalleeSavedRegs<(add D2, D8, D9, (sequence "D%u", 18, 27))>;
