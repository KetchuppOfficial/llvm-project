#ifndef LLVM_LIB_TARGET_DAARCH34_DAARCH34MACHINEFUNCTIONINFO_H
#define LLVM_LIB_TARGET_DAARCH34_DAARCH34MACHINEFUNCTIONINFO_H

#include <optional>

#include "llvm/CodeGen/MachineFunction.h"

namespace llvm {

class DAArch34FunctionInfo : public MachineFunctionInfo {
public:
  DAArch34FunctionInfo() = default;
  explicit DAArch34FunctionInfo(MachineFunction &MF) {}

  void setVarArgsFrameIndex(int Off) { VarArgsFrameIndex = Off; }
  int getVarArgsFrameIndex() const { return VarArgsFrameIndex; }

  void setVarArgsSaveSize(int Size) { VarArgsSaveSize = Size; }
  int getVarArgsSaveSize() const { return VarArgsSaveSize; }

  unsigned getCalleeSavedStackSize() const { return CalleeSavedStackSize; }
  void setCalleeSavedStackSize(unsigned Size) { CalleeSavedStackSize = Size; }

  void setReturnStackOffset(unsigned Off) {
    assert(!ReturnStackOffset && "Return stack offset set twice");
    ReturnStackOffset.emplace(Off);
  }

  unsigned getReturnStackOffset() const {
    assert(ReturnStackOffset && "Return stack offset not set");
    return *ReturnStackOffset;
  }

private:
    virtual void anchor();

    unsigned CalleeSavedStackSize = 0;

    unsigned VarArgsFrameIndex = 0;
    unsigned VarArgsSaveSize = 0;

    std::optional<unsigned> ReturnStackOffset;
};

} // end namespace llvm

#endif // LLVM_LIB_TARGET_DAARCH34_DAARCH34MACHINEFUNCTIONINFO_H
