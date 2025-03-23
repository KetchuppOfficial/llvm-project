add_llvm_component_group(DAArch34)

add_llvm_target(DAArch34CodeGen
    DAArch34TargetMachine.cpp

LINK_COMPONENTS
    CodeGen
    Core
    Support
    Target
    DAArch34Info

ADD_TO_COMPONENT
    DAArch34
)

add_subdirectory(MCTargetDesc)
add_subdirectory(TargetInfo)
