#include "TargetInfo/DAArch34TargetInfo.h"

#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/Compiler.h"
#include "llvm/TargetParser/Triple.h"

namespace llvm {

Target &getTheDAArch34Target() {
  static Target TheDAArch34Target;
  return TheDAArch34Target;
}

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeDAArch34TargetInfo() {
  RegisterTarget<Triple::daarch34> X(
      getTheDAArch34Target(), "daarch34",
      "Dolgov Alexander Architecture (from region 34)", "DAArch34");
}

} // end namespace llvm
