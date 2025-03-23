#ifndef LLVM_LIB_TARGET_DAARCH34_DAARCH34TARGETMACHINE_H
#define LLVM_LIB_TARGET_DAARCH34_DAARCH34TARGETMACHINE_H

#include "llvm/ADT/StringRef.h"
#include "llvm/CodeGen/TargetPassConfig.h"
#include "llvm/Support/CodeGen.h" // for Reloc, CodeModel, CodeGenOptLevel
#include "llvm/Target/TargetMachine.h"

#include <memory>
#include <optional>

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

  TargetPassConfig *createPassConfig(legacy::PassManagerBase &PM) override {
    return new TargetPassConfig(*this, PM);
  }

  TargetLoweringObjectFile *getObjFileLowering() const override {
    return TLOF.get();
  }

private:
  std::unique_ptr<TargetLoweringObjectFile> TLOF;
};

} // end namespace llvm

#endif // LLVM_LIB_TARGET_DAARCH34_DAARCH34TARGETMACHINE_H
