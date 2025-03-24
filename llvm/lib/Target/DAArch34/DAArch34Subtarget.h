#ifndef LLVM_LIB_TARGET_DAARCH34_DAARCH34SUBTARGET_H
#define LLVM_LIB_TARGET_DAARCH34_DAARCH34SUBTARGET_H

#include "llvm/ADT/StringRef.h"

#define GET_SUBTARGETINFO_HEADER // for DAArch34GenSubtargetInfo
#include "DAArch34GenSubtargetInfo.inc"

namespace llvm {

class TargetMachine;

class DAArch34Subtarget : public DAArch34GenSubtargetInfo {
public:
  DAArch34Subtarget(const TargetMachine &TM, StringRef CPU, StringRef TuneCPU,
                    StringRef FS);

  void ParseSubtargetFeatures(StringRef CPU, StringRef TuneCPU, StringRef FS);
};

} // end namespace llvm

#endif // LLVM_LIB_TARGET_DAARCH34_DAARCH34SUBTARGET_H
