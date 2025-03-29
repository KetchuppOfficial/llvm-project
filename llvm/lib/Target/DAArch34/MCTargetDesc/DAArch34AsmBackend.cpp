#include <cstdint>
#include <memory>

#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/bit.h"
#include "llvm/MC/MCAsmBackend.h"
#include "llvm/MC/MCObjectWriter.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/Support/EndianStream.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/TargetParser/Triple.h"

#include "DAArch34AsmBackend.h"

namespace llvm {

class Target;
class MCAssembler;
class MCFixup;
class MCValue;
class MCRegisterInfo;
class MCTargetOptions;

namespace {

class DAArch34AsmBackend : public MCAsmBackend {
public:
  explicit DAArch34AsmBackend(const Target &T)
      : MCAsmBackend(endianness::little), TheTarget(T) {}

  unsigned getNumFixupKinds() const override { return 0; }

  bool writeNopData(raw_ostream &O, uint64_t Count,
                    const MCSubtargetInfo *STI) const override {
    if (Count % 4) {
      return false;
    }

    constexpr unsigned kNOP = 0x01000000;
    const uint64_t NumNops = Count / 4;
    for (uint64_t I = 0; I != NumNops; ++I) {
      support::endian::write<uint32_t>(O, kNOP, Endian);
    }

    return true;
  }

private:
  const Target &TheTarget;
};

class ELFDAArch34AsmBackend : public DAArch34AsmBackend {
public:
  ELFDAArch34AsmBackend(const Target &T, Triple::OSType OSType)
      : DAArch34AsmBackend(T), OSType(OSType) {}

  void applyFixup(const MCAssembler &Asm, const MCFixup &Fixup,
                  const MCValue &Target, MutableArrayRef<char> Data,
                  uint64_t Value, bool IsResolved,
                  const MCSubtargetInfo *STI) const override {}

  std::unique_ptr<MCObjectTargetWriter>
  createObjectTargetWriter() const override {
    return std::unique_ptr<MCObjectTargetWriter>{};
  }

private:
  Triple::OSType OSType;
};

} // end unnamed namespace

MCAsmBackend *createDAArch34AsmBackend(const Target &T,
                                       const MCSubtargetInfo &STI,
                                       const MCRegisterInfo &MRI,
                                       const MCTargetOptions &Options) {
  return new ELFDAArch34AsmBackend(T, STI.getTargetTriple().getOS());
}

} // end namespace llvm
