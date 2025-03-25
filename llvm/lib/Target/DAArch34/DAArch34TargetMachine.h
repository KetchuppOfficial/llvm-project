#ifndef LLVM_LIB_TARGET_DAARCH34_DAARCH34TARGETMACHINE_H
#define LLVM_LIB_TARGET_DAARCH34_DAARCH34TARGETMACHINE_H

#include <memory>
#include <optional>

#include "llvm/ADT/StringRef.h"
#include "llvm/CodeGen/TargetPassConfig.h"
#include "llvm/Support/CodeGen.h" // for Reloc, CodeModel, CodeGenOptLevel
#include "llvm/Target/TargetMachine.h"

#include "DAArch34Subtarget.h"

namespace llvm {

class Target;
class TargetOptions;
class Triple;
class TargetLoweringObjectFile;

namespace legacy {

class PassManagerBase;

} // end namespace legacy

extern Target TheDAArch34Target;

class DAArch34TargetMachine : public LLVMTargetMachine {
public:
  DAArch34TargetMachine(const Target &T, const Triple &TT, StringRef CPU,
                        StringRef FS, const TargetOptions &Options,
                        std::optional<Reloc::Model> RM,
                        std::optional<CodeModel::Model> CM, CodeGenOptLevel OL,
                        bool JIT, bool LittleEndian = true);

  /// Create a pass configuration object to be used by addPassToEmitX methods
  /// for generating a pipeline of CodeGen passes.
  TargetPassConfig *createPassConfig(legacy::PassManagerBase &PM) override;

  TargetLoweringObjectFile *getObjFileLowering() const override {
    return TLOF.get();
  }

  const DAArch34Subtarget *getSubtargetImpl(const Function &) const override {
    return &Subtarget;
  }

private:
  std::unique_ptr<TargetLoweringObjectFile> TLOF;
  DAArch34Subtarget Subtarget;
};

} // end namespace llvm

#endif // LLVM_LIB_TARGET_DAARCH34_DAARCH34TARGETMACHINE_H
