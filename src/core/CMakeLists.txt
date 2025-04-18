SET(c_options_private "")
IF (MSVC)
    LIST(APPEND c_options_private "/wd4996;/MP")
ELSEIF ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    #
ELSEIF ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    #
ENDIF ()

ADD_TARGET(
        MODE STATIC
        RET_TARGET_NAME CORE
        SOURCE
        "${PROJECT_SOURCE_DIR}/include/MyDX12"
        "_deps/DirectXTK12/Shaders/Compiled/SpriteEffect_SpriteVertexShader.inc"
        INC
        "${PROJECT_SOURCE_DIR}/include"
        INC_PRIVATE
        "${PROJECT_SOURCE_DIR}/include/MyDX12/_deps/DirectXTK12"
        LIB
        My::MyTemplate_core
        My::MyFG_core
        LIB_INTERFACE
        d3dcompiler.lib
        D3D12.lib
        dxgi.lib
        dxguid.lib
        C_OPTION_PRIVATE
        ${c_options_private}
        DEFINE
        UNICODE
        _UNICODE
        NOMINMAX
)

IF (${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.16")
    TARGET_PRECOMPILE_HEADERS(MyDX12_core PRIVATE _deps/DirectXTK12/pch.h)
ENDIF ()

SET(SHADER_SOURCES
        _deps/DirectXTK12/Shaders/AlphaTestEffect.fx
        _deps/DirectXTK12/Shaders/BasicEffect.fx
        _deps/DirectXTK12/Shaders/Common.fxh
        _deps/DirectXTK12/Shaders/DebugEffect.fx
        _deps/DirectXTK12/Shaders/DualTextureEffect.fx
        _deps/DirectXTK12/Shaders/EnvironmentMapEffect.fx
        _deps/DirectXTK12/Shaders/GenerateMips.hlsl
        _deps/DirectXTK12/Shaders/Lighting.fxh
        _deps/DirectXTK12/Shaders/NormalMapEffect.fx
        _deps/DirectXTK12/Shaders/PBRCommon.fxh
        _deps/DirectXTK12/Shaders/PBREffect.fx
        _deps/DirectXTK12/Shaders/PixelPacking_Velocity.hlsli
        _deps/DirectXTK12/Shaders/PostProcess.fx
        _deps/DirectXTK12/Shaders/RootSig.fxh
        _deps/DirectXTK12/Shaders/SkinnedEffect.fx
        _deps/DirectXTK12/Shaders/SpriteEffect.fx
        _deps/DirectXTK12/Shaders/Structures.fxh
        _deps/DirectXTK12/Shaders/ToneMap.fx
        _deps/DirectXTK12/Shaders/Utilities.fxh
)

ADD_CUSTOM_COMMAND(
        OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/_deps/DirectXTK12/Shaders/Compiled/SpriteEffect_SpriteVertexShader.inc"
        MAIN_DEPENDENCY "${CMAKE_CURRENT_SOURCE_DIR}/_deps/DirectXTK12/Shaders/CompileShaders.cmd"
        DEPENDS ${SHADER_SOURCES}
        COMMENT "Generating HLSL shaders..."
        COMMAND "CompileShaders.cmd"
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/_deps/DirectXTK12/Shaders"
        USES_TERMINAL
)