add_llvm_component_library(LLVMDAArch34Info
    DAArch34TargetInfo.cpp

LINK_COMPONENTS
    MC
    Support

ADD_TO_COMPONENT
    DAArch34
)
