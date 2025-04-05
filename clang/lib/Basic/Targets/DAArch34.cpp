#include "clang/Basic/Builtins.h"
#include "clang/Basic/MacroBuilder.h"
#include "clang/Basic/TargetBuiltins.h"

#include "DAArch34.h"

namespace clang {

static constexpr Builtin::Info BuiltinInfo[] = {
#define BUILTIN(ID, TYPE, ATTRS)                                               \
  {#ID, TYPE, ATTRS, nullptr, HeaderDesc::NO_HEADER, LanguageID::ALL_LANGUAGES},
#include "clang/Basic/BuiltinsDAArch34.def"
#undef BUILTIN
};

namespace targets {

void DAArch34TargetInfo::getTargetDefines(const LangOptions &Opts,
                                          MacroBuilder &Builder) const {
  Builder.defineMacro("__daarch34__");
}

llvm::ArrayRef<Builtin::Info> DAArch34TargetInfo::getTargetBuiltins() const {
  return llvm::ArrayRef(BuiltinInfo, DAArch34::LastTSBuiltin - Builtin::FirstTSBuiltin);
}

} // end namespace targets

} // end namespace clang
