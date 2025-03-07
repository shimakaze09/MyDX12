ADD_TARGET(
        MODE STATIC
        SOURCE
        "${PROJECT_SOURCE_DIR}/include/MyDX12"
        INC
        "include"
        LIB_INTERFACE
        d3dcompiler.lib
        D3D12.lib
        dxgi.lib
        DEFINE
        UNICODE
        _UNICODE
        L_OPTION_INTERFACE
        /SUBSYSTEM:WINDOWS
)