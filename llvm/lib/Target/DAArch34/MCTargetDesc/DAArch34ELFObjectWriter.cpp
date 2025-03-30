#include <cstdint>
#include <memory>

#include "llvm/BinaryFormat/ELF.h"
#include "llvm/MC/MCELFObjectWriter.h"
#include "llvm/MC/MCFixup.h"

#include "DAArch34ELFObjectWriter.h"

namespace llvm {

class MCContext;
class MCSymbol;
class MCValue;

namespace {

class DAArch34ELFObjectWriter : public MCELFObjectTargetWriter {
public:
  DAArch34ELFObjectWriter(bool Is64Bit, uint8_t OSABI)
      : MCELFObjectTargetWriter(Is64Bit, OSABI, ELF::EM_DAARCH34,
                                /*HasRelocationAddend*/ true) {}

protected:
  unsigned getRelocType(MCContext &Ctx, const MCValue &Target,
                        const MCFixup &Fixup, bool IsPCRel) const override {
    const MCFixupKind Kind = Fixup.getKind();
    if (Kind >= FirstLiteralRelocationKind)
      return Kind - FirstLiteralRelocationKind;

    llvm_unreachable("Unimplemented fixup -> relocation");
  }

  bool needsRelocateWithSymbol(const MCValue &Val, const MCSymbol &Sym,
                               unsigned Type) const override {
    return false;
  }
};

} // end unnamed namespace

std::unique_ptr<MCObjectTargetWriter>
createDAArch34ELFObjectWriter(bool Is64Bit, uint8_t OSABI) {
  return std::make_unique<DAArch34ELFObjectWriter>(Is64Bit, OSABI);
}

} // end namespace llvm
