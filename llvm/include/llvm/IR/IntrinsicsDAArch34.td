let TargetPrefix = "daarch34" in {

def int_daarch34_put_pixel : Intrinsic<[/* returns nothing */],
                                       [llvm_i32_ty, llvm_i32_ty],
                                       [IntrNoMem, IntrHasSideEffects]>,
                             ClangBuiltin<"putPixel">;

def int_daarch34_set_color : Intrinsic<[/* returns nothing */],
                                       [llvm_i32_ty, llvm_i32_ty, llvm_i32_ty],
                                       [IntrNoMem, IntrHasSideEffects]>,
                             ClangBuiltin<"setColor">;

def int_daarch34_update_screen : Intrinsic<[/* returns nothing */],
                                           [/* takes nothing */],
                                           [IntrNoMem, IntrHasSideEffects]>,
                                 ClangBuiltin<"updateScreen">;

def int_daarch34_rand : Intrinsic<[llvm_i32_ty],
                                  [/* takes nothing*/],
                                  [IntrNoMem]>,
                        ClangBuiltin<"Rand">;
}
