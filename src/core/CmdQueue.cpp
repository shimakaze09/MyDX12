#include <MyDX12/CmdQueue.h>

#include <array>

using namespace My;

void MyDX12::CmdQueue::Execute(ID3D12GraphicsCommandList* list) {
  const std::array<ID3D12CommandList*, 1> listArr = {list};
  raw->ExecuteCommandLists(1, listArr.data());
}
