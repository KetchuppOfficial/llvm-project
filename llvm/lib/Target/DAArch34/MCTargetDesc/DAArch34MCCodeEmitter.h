#ifndef LLVM_LIB_TARGET_DAARCH34_MCTARGETDESC_DAARCH34MCCODEEMITTER_H
#define LLVM_LIB_TARGET_DAARCH34_MCTARGETDESC_DAARCH34MCCODEEMITTER_H

namespace llvm {

class MCCodeEmitter;
class MCInstrInfo;
class MCContext;

MCCodeEmitter *createDAArch34MCCodeEmitter(const MCInstrInfo &MCII,
                                           MCContext &Ctx);

} // end namespace llvm

#endif // LLVM_LIB_TARGET_DAARCH34_MCTARGETDESC_DAARCH34MCCODEEMITTER_H
