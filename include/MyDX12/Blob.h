//
// Created by Admin on 7/03/2025.
//

#pragma once

#include "Util.h"

namespace My::MyDX12 {
// ID3DBlob is a CPU buffer wrapper
// raw : Microsoft::WRL::ComPtr<ID3DBlob>
// .   : simple API
// ->  : raw API
struct Blob : Util::ComPtrHolder<ID3DBlob> {
  using Util::ComPtrHolder<ID3DBlob>::ComPtrHolder;

  void Create(SIZE_T size);
  void Copy(const void* data, SIZE_T size);
  void Create(const void* data, SIZE_T size);
};
}  // namespace My::MyDX12
