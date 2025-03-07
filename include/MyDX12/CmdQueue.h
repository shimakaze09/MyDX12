//
// Created by Admin on 7/03/2025.
//

#pragma once

#include "Util.h"

namespace My::DX12 {
struct CmdQueue : ComPtrHolder<ID3D12CommandQueue> {
  void Execute(ID3D12GraphicsCommandList* list);
};
}
