//
// Created by Admin on 7/03/2025.
//

#include <MyDX12/Device.h>

#include <MyDX12/Desc.h>

using namespace My;

void DX12::Device::CreateCommittedResource(D3D12_HEAP_TYPE heap_type,
                                           SIZE_T size,
                                           ID3D12Resource** resources) {
  ThrowIfFailed(raw->CreateCommittedResource(
      &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT), D3D12_HEAP_FLAG_NONE,
      &CD3DX12_RESOURCE_DESC::Buffer(size), D3D12_RESOURCE_STATE_COMMON,
      nullptr, IID_PPV_ARGS(resources)));
}

void DX12::Device::CreateDescriptorHeap(UINT size,
                                        D3D12_DESCRIPTOR_HEAP_TYPE type,
                                        ID3D12DescriptorHeap** pHeap) {
  D3D12_DESCRIPTOR_HEAP_DESC cbvHeapDesc;
  cbvHeapDesc.NumDescriptors = size;
  cbvHeapDesc.Type = type;
  cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
  cbvHeapDesc.NodeMask = 0;
  ThrowIfFailed(raw->CreateDescriptorHeap(&cbvHeapDesc, IID_PPV_ARGS(pHeap)));
}

void DX12::Device::CreateSRV_Tex2D(ID3D12Resource* pResource,
                                   D3D12_CPU_DESCRIPTOR_HANDLE DestDescriptor) {
  D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = Desc::SRC::Tex2D(pResource);
  raw->CreateShaderResourceView(pResource, &srvDesc, DestDescriptor);
}
