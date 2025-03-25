add_llvm_component_group(DAArch34)

set(LLVM_TARGET_DEFINITIONS DAArch34.td)
tablegen(LLVM DAArch34GenRegisterInfo.inc -gen-register-info)
tablegen(LLVM DAArch34GenInstrInfo.inc -gen-instr-info)
tablegen(LLVM DAArch34GenSubtargetInfo.inc -gen-subtarget)
tablegen(LLVM DAArch34GenMCPseudoLowering.inc -gen-pseudo-lowering)
tablegen(LLVM DAArch34GenDAGISel.inc -gen-dag-isel)
add_public_tablegen_target(DAArch34CommonTableGen)

add_llvm_target(DAArch34CodeGen
    DAArch34TargetMachine.cpp
    DAArch34RegisterInfo.cpp
    DAArch34InstrInfo.cpp
    DAArch34Subtarget.cpp
    DAArch34AsmPrinter.cpp
    DAArch34ISelDagToDag.cpp
    DAArch34ISelLowering.cpp

LINK_COMPONENTS
    CodeGen
    Core
    Support
    Target
    DAArch34Info
    AsmPrinter
    SelectionDAG

ADD_TO_COMPONENT
    DAArch34
)

add_subdirectory(MCTargetDesc)
add_subdirectory(TargetInfo)
