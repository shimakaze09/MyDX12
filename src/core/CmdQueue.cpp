//
// Created by Admin on 7/03/2025.
//

#include <MyDX12/CmdQueue.h>

using namespace My;

void DX12::CmdQueue::Execute(ID3D12GraphicsCommandList* list) {
  raw->ExecuteCommandLists(1, reinterpret_cast<ID3D12CommandList* const*>(&list));
}
