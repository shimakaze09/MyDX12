#pragma once

#include "Util.h"

namespace Smkz::MyDX12 {
struct CmdQueue : Util::ComPtrHolder<ID3D12CommandQueue> {
  using Util::ComPtrHolder<ID3D12CommandQueue>::ComPtrHolder;
  void Execute(ID3D12GraphicsCommandList* list);
};
}  // namespace Smkz::MyDX12
