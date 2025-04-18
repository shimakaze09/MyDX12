#pragma once

#include "Util.h"

namespace My::MyDX12 {
struct CmdQueue : Util::ComPtrHolder<ID3D12CommandQueue> {
  using Util::ComPtrHolder<ID3D12CommandQueue>::ComPtrHolder;
  void Execute(ID3D12GraphicsCommandList* list);
};
}  // namespace My::MyDX12
