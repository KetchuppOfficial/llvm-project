add_llvm_component_library(LLVMDAArch34Desc
    DAArch34MCTargetDesc.cpp

LINK_COMPONENTS
    MC
    Support
    DAArch34Info

ADD_TO_COMPONENT
    DAArch34
)
