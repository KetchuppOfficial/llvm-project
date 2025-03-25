#include "DAArch34TargetMachine.h"
#include "TargetInfo/DAArch34TargetInfo.h"

#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"
#include "llvm/CodeGen/TargetPassConfig.h"
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
      TLOF(std::make_unique<TargetLoweringObjectFileELF>()),
      Subtarget(*this, TT, CPU.str(), FS.str()) {
  initAsmInfo();
}

FunctionPass *createDAArch34ISelDag(DAArch34TargetMachine &TM,
                                    CodeGenOptLevel OptLevel);

namespace {

class DAArch34PassConfig : public TargetPassConfig {
public:
  DAArch34PassConfig(DAArch34TargetMachine &TM, PassManagerBase &PM)
      : TargetPassConfig(TM, PM) {}

  DAArch34TargetMachine &getDAArch34TargetMachine() const {
    return getTM<DAArch34TargetMachine>();
  }

  /// addInstSelector - This method should install an instruction selector pass,
  /// which converts from LLVM code to machine instructions.
  bool addInstSelector() override {
    addPass(createDAArch34ISelDag(getDAArch34TargetMachine(), getOptLevel()));
    return false;
  }
};

} // end unnamed namespace

TargetPassConfig *DAArch34TargetMachine::createPassConfig(PassManagerBase &PM) {
  return new DAArch34PassConfig(*this, PM);
}

} // end namespace llvm
