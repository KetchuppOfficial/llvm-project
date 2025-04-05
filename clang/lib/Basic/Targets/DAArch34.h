#ifndef CLANG_LIB_BASIC_TARGETS_DAARCH34_H
#define CLANG_LIB_BASIC_TARGETS_DAARCH34_H

#include <string_view>

#include "clang/Basic/TargetInfo.h"
#include "llvm/ADT/ArrayRef.h"
#include "llvm/Support/Compiler.h"

namespace clang {

class TargetOptions;

namespace targets {

class LLVM_LIBRARY_VISIBILITY DAArch34TargetInfo : public TargetInfo {
public:
  DAArch34TargetInfo(const llvm::Triple &Triple, const TargetOptions &Opts)
      : TargetInfo(Triple) {
    NoAsmVariants = true;
    LongLongAlign = 32;
    SuitableAlign = 32;
    SizeType = UnsignedInt;
    PtrDiffType = SignedInt;
    IntPtrType = SignedInt;
    WCharType = UnsignedChar;
    WIntType = UnsignedInt;
    TargetInfo::resetDataLayout("e-m:e-p:32:32-i8:8:32-i16:16:32-i64:64-n32");
  }

  void getTargetDefines(const LangOptions &Opts,
                        MacroBuilder &Builder) const override;

  /// Return information about target-specific builtins for
  /// the current primary target, and info about which builtins are non-portable
  /// across the current set of primary and secondary targets.
  llvm::ArrayRef<Builtin::Info> getTargetBuiltins() const override;

  /// Returns the kind of __builtin_va_list type that should be used
  /// with this target.
  BuiltinVaListKind getBuiltinVaListKind() const override {
    /// typedef void* __builtin_va_list;
    return TargetInfo::VoidPtrBuiltinVaList;
  }

  /// Returns a string of target-specific clobbers, in LLVM format.
  std::string_view getClobbers() const override { return ""; }

  llvm::ArrayRef<const char *> getGCCRegNames() const override {
    static const char *GCCRegNames[] = {
        "D0",  "D1",  "D2",  "D3",  "D4",  "D5",  "D6",  "D7",
        "D8",  "D9",  "D10", "D11", "D12", "D13", "D14", "D15",
        "D16", "D17", "D18", "D19", "D20", "D21", "D22", "D23",
        "D24", "D25", "D26", "D27", "D28", "D29", "D30", "D31"};
    return llvm::ArrayRef(GCCRegNames);
  }

  llvm::ArrayRef<TargetInfo::GCCRegAlias> getGCCRegAliases() const override {
    return std::nullopt;
  }

  bool validateAsmConstraint(const char *&Name,
                             TargetInfo::ConstraintInfo &Info) const override {
    return false;
  }
};

} // end namespace targets

} // end namespace clang

#endif // CLANG_LIB_BASIC_TARGETS_DAARCH34_H
