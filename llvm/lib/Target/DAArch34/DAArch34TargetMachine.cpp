#include "DAArch34TargetMachine.h"
#include "TargetInfo/DAArch34TargetInfo.h"

#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"
#include "llvm/MC/TargetRegistry.h" // for RegisterTargetMachine

#include <optional>

namespace llvm {

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeDAArch34Target() {
  // Register the target.
  RegisterTargetMachine<DAArch34TargetMachine> A(getTheDAArch34Target());
}

static Reloc::Model getEffectiveRelocModel(bool JIT,
                                           std::optional<Reloc::Model> RM) {
  if (!RM || JIT)
    return Reloc::Model::Static;
  return *RM;
}

DAArch34TargetMachine::DAArch34TargetMachine(const Target &T, const Triple &TT,
                                             StringRef CPU, StringRef FS,
                                             const TargetOptions &Options,
                                             std::optional<Reloc::Model> RM,
                                             std::optional<CodeModel::Model> CM,
                                             CodeGenOptLevel OL, bool JIT,
                                             bool LittleEndian)
    : LLVMTargetMachine(T, "e-m:e-p:32:32-i8:8:32-i16:16:32-i64:64-n32", TT,
                        CPU, FS, Options, getEffectiveRelocModel(JIT, RM),
                        getEffectiveCodeModel(CM, CodeModel::Small), OL),
      TLOF(std::make_unique<TargetLoweringObjectFileELF>()) {
  initAsmInfo();
}

} // end namespace llvm
