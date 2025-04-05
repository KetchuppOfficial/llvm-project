#include "clang/Basic/MacroBuilder.h"

#include "DAArch34.h"

namespace clang::targets {

void DAArch34TargetInfo::getTargetDefines(const LangOptions &Opts,
                                          MacroBuilder &Builder) const {
    Builder.defineMacro("__daarch34__");
}

llvm::ArrayRef<Builtin::Info> DAArch34TargetInfo::getTargetBuiltins() const {
  return std::nullopt;
}

} // end namespace clang::targets
