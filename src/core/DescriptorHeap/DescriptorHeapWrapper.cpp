//
// Created by Admin on 7/03/2025.
//

#include <MyDX12/DescriptorHeap/DescriptorHeapWrapper.h>

using namespace My;

HRESULT MyDX12::DescriptorHeapWrapper::Create(
    ID3D12Device* pDevice, const D3D12_DESCRIPTOR_HEAP_DESC* desc) {
  HRESULT hr =
      pDevice->CreateDescriptorHeap(desc, IID_PPV_ARGS(raw.GetAddressOf()));
  if (FAILED(hr))
    return hr;

  hCPUHeapStart = raw->GetCPUDescriptorHandleForHeapStart();
  hGPUHeapStart = raw->GetGPUDescriptorHandleForHeapStart();

  HandleIncrementSize = pDevice->GetDescriptorHandleIncrementSize(Desc.Type);
  return hr;
}

HRESULT MyDX12::DescriptorHeapWrapper::Create(ID3D12Device* pDevice,
                                              D3D12_DESCRIPTOR_HEAP_TYPE Type,
                                              UINT NumDescriptors,
                                              bool bShaderVisible) {
  D3D12_DESCRIPTOR_HEAP_DESC Desc;
  Desc.Type = Type;
  Desc.NumDescriptors = NumDescriptors;
  Desc.Flags = (bShaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE
                               : D3D12_DESCRIPTOR_HEAP_FLAG_NONE);
  Desc.NodeMask = 0;

  return Create(pDevice, &Desc);
}
