//
// Created by Admin on 7/03/2025.
//

#pragma once

#include "../Util.h"

namespace My::MyDX12 {
// ref:
// https://docs.microsoft.com/en-us/windows/win32/direct3d12/creating-descriptor-heaps
struct DescriptorHeapWrapper : Util::ComPtrHolder<ID3D12DescriptorHeap> {
  DescriptorHeapWrapper() { memset(this, 0, sizeof(DescriptorHeapWrapper)); }

  HRESULT Create(ID3D12Device* pDevice, const D3D12_DESCRIPTOR_HEAP_DESC* desc);

  HRESULT Create(ID3D12Device* pDevice, D3D12_DESCRIPTOR_HEAP_TYPE Type,
                 UINT NumDescriptors, bool bShaderVisible = false);

  void Clear() {
    raw.Reset();
    memset(this, 0, sizeof(DescriptorHeapWrapper));
  }

  UINT Size() {
    if (IsNull())
      return 0;

    return raw->GetDesc().NumDescriptors;
  }

  bool Empty() { return Size() == 0; }

  operator ID3D12DescriptorHeap*() { return raw.Get(); }

  D3D12_CPU_DESCRIPTOR_HANDLE hCPU(UINT index) {
    return {hCPUHeapStart.ptr + index * HandleIncrementSize};
  }

  D3D12_GPU_DESCRIPTOR_HANDLE hGPU(UINT index) {
    assert(Desc.Flags & D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE);
    return {hGPUHeapStart.ptr + index * HandleIncrementSize};
  }

  D3D12_DESCRIPTOR_HEAP_DESC Desc;
  D3D12_CPU_DESCRIPTOR_HANDLE hCPUHeapStart;
  D3D12_GPU_DESCRIPTOR_HANDLE hGPUHeapStart;
  UINT HandleIncrementSize;
};
}  // namespace My::MyDX12
