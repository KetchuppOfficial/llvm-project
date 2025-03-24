#include "DAArch34MCAsmInfo.h"

#include "llvm/MC/MCDirectives.h"

namespace llvm {

DAArch34ELFMCAsmInfo::DAArch34ELFMCAsmInfo(const Triple &TheTriple) {
  /// These directives are used to output some unit of integer data to the
  /// current section.  If a data directive is set to null, smaller data
  /// directives will be used to emit the large sizes.  Defaults to "\t.byte\t",
  /// "\t.short\t", "\t.long\t", "\t.quad\t"
  Data32bitsDirective = "\t.word\t";
  Data64bitsDirective = nullptr;

  ZeroDirective = "\t.space\t";
  CommentString = ";";

  /// This is true if the assembler allows @ characters in symbol names.
  /// Defaults to false.
  AllowAtInName = true;

  /// This attribute, if not MCSA_Invalid, is used to declare a symbol as having
  /// hidden visibility.
  HiddenVisibilityAttr = MCSA_Invalid;

  /// This attribute, if not MCSA_Invalid, is used to declare an undefined
  /// symbol as having hidden visibility.
  HiddenDeclarationVisibilityAttr = MCSA_Invalid;

  /// This attribute, if not MCSA_Invalid, is used to declare a symbol as having
  /// protected visibility.
  ProtectedVisibilityAttr = MCSA_Invalid;
}

} // end namespace llvm
