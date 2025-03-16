//
// Created by Admin on 8/03/2025.
//

#include <MyDX12/UploadBuffer.h>

using namespace My;

MyDX12::UploadBuffer::UploadBuffer(ID3D12Device* device, UINT64 size,
                                   D3D12_RESOURCE_FLAGS flag)
    : size{size} {
  ThrowIfFailed(device->CreateCommittedResource(
      &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), D3D12_HEAP_FLAG_NONE,
      &CD3DX12_RESOURCE_DESC::Buffer(size, flag),
      D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&resource)));

  ThrowIfFailed(
      resource->Map(0, nullptr, reinterpret_cast<void**>(&mappedData)));
}

MyDX12::UploadBuffer::~UploadBuffer() {
  resource->Unmap(0, nullptr);
}

void MyDX12::UploadBuffer::Set(UINT64 offset, const void* data, UINT64 size) {
  assert(size > 0 && data);
  assert(offset + size <= Size());
  memcpy(mappedData + offset, data, size);
}

MyDX12::DynamicUploadBuffer::DynamicUploadBuffer(ID3D12Device* device,
                                                 D3D12_RESOURCE_FLAGS flag)
    : device{device}, flag{flag} {}

ID3D12Resource* MyDX12::DynamicUploadBuffer::GetResource() const noexcept {
  return buffer ? buffer->GetResource() : nullptr;
}

UINT64 MyDX12::DynamicUploadBuffer::Size() const noexcept {
  return buffer ? buffer->Size() : 0;
}

void MyDX12::DynamicUploadBuffer::Reserve(size_t size) {
  if (size <= Size())
    return;

  auto newBuffer = std::make_unique<MyDX12::UploadBuffer>(device, size, flag);
  if (buffer)
    newBuffer->Set(0, buffer->GetMappedData(), buffer->Size());
  buffer = std::move(newBuffer);
}

void MyDX12::DynamicUploadBuffer::FastReserve(size_t size) {
  if (size <= Size())
    return;

  buffer = std::make_unique<MyDX12::UploadBuffer>(device, size, flag);
}

void MyDX12::DynamicUploadBuffer::Set(UINT64 offset, const void* data,
                                      UINT64 size) {
  assert(buffer);
  buffer->Set(offset, data, size);
}
