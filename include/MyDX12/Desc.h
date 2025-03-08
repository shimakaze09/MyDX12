//
// Created by Admin on 7/03/2025.
//

#pragma once

#include "Util.h"

namespace My::DX12::Desc {
namespace SRV {
D3D12_SHADER_RESOURCE_VIEW_DESC Tex2D(ID3D12Resource* pResource);
D3D12_SHADER_RESOURCE_VIEW_DESC TexCube(ID3D12Resource* pResource);
}  // namespace SRV

namespace PSO {
D3D12_GRAPHICS_PIPELINE_STATE_DESC Basic(
    ID3D12RootSignature* rootSig, D3D12_INPUT_ELEMENT_DESC* pInputElementDescs,
    UINT NumElements, ID3DBlob* VS, ID3DBlob* PS,
    DXGI_FORMAT rtvFormat = DXGI_FORMAT_R8G8B8A8_UNORM,
    DXGI_FORMAT dsvFormat = DXGI_FORMAT_D24_UNORM_S8_UINT);
}  // namespace PSO
}  // namespace My::DX12::Desc
