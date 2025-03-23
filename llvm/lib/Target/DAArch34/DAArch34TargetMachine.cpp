#include "DAArch34TargetMachine.h"
#include "TargetInfo/DAArch34TargetInfo.h"

#include "llvm/ADT/StringRef.h"
#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/CodeGen.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"
#include "llvm/TargetParser/Triple.h"

#include <optional>
#include <string>

using namespace llvm;

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeDAArch34Target() {
  // Register the target.
  RegisterTargetMachine<DAArch34TargetMachine> A(getTheDAArch34Target());
}

namespace {

std::string computeDataLayout(const Triple &TT, StringRef CPU,
                              const TargetOptions &Options, bool LittleEndian) {
  return std::string{"e-m:e-p:32:32-i8:8:32-i16:16:32-i64:64-n32"};
}

Reloc::Model getEffectiveRelocModel(bool JIT, std::optional<Reloc::Model> RM) {
  if (!RM || JIT)
    return Reloc::Model::Static;
  return *RM;
}

} // end unnamed namespace

DAArch34TargetMachine::DAArch34TargetMachine(const Target &T, const Triple &TT,
                                             StringRef CPU, StringRef FS,
                                             const TargetOptions &Options,
                                             std::optional<Reloc::Model> RM,
                                             std::optional<CodeModel::Model> CM,
                                             CodeGenOptLevel OL, bool JIT,
                                             bool LittleEndian)
    : LLVMTargetMachine(T, computeDataLayout(TT, CPU, Options, LittleEndian),
                        TT, CPU, FS, Options, getEffectiveRelocModel(JIT, RM),
                        getEffectiveCodeModel(CM, CodeModel::Small), OL),
      TLOF(std::make_unique<TargetLoweringObjectFileELF>()) {
  initAsmInfo();
}
