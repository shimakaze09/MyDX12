#include <MyDX12/Desc.h>
#include <MyDX12/Device.h>

using namespace Smkz;

void MyDX12::Device::CreateCommittedResource(D3D12_HEAP_TYPE heap_type,
                                             SIZE_T size,
                                             ID3D12Resource** resources) {
  const auto defaultHeapProperties =
      CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
  const auto bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(size);
  ThrowIfFailed(raw->CreateCommittedResource(
      &defaultHeapProperties, D3D12_HEAP_FLAG_NONE, &bufferDesc,
      D3D12_RESOURCE_STATE_COMMON, nullptr, IID_PPV_ARGS(resources)));
}

void MyDX12::Device::CreateDescriptorHeap(UINT size,
                                          D3D12_DESCRIPTOR_HEAP_TYPE type,
                                          ID3D12DescriptorHeap** pHeap) {
  D3D12_DESCRIPTOR_HEAP_DESC cbvHeapDesc;
  cbvHeapDesc.NumDescriptors = size;
  cbvHeapDesc.Type = type;
  cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
  cbvHeapDesc.NodeMask = 0;
  ThrowIfFailed(raw->CreateDescriptorHeap(&cbvHeapDesc, IID_PPV_ARGS(pHeap)));
}

void MyDX12::Device::CreateSRV_Tex2D(
    ID3D12Resource* pResource, D3D12_CPU_DESCRIPTOR_HANDLE DestDescriptor) {
  D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc =
      Desc::SRV::Tex2D(pResource->GetDesc().Format);
  raw->CreateShaderResourceView(pResource, &srvDesc, DestDescriptor);
}
