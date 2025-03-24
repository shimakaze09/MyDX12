//
// Created by Admin on 12/03/2025.
//

#include <MyDX12/FrameGraph/Executor.h>

#include <MyDX12/FrameGraph/RsrcMngr.h>

using namespace My::MyDX12::FG;
using namespace My::MyDX12;
using namespace My;

void Executor::Execute(ID3D12Device* device, ID3D12CommandQueue* cmdQueue,
                       ID3D12CommandAllocator* alloc,
                       const MyFG::Compiler::Result& crst, RsrcMngr& rsrcMngr) {
  rsrcMngr.DHReserve();
  rsrcMngr.AllocateHandle();

  ComPtr<ID3D12GraphicsCommandList> cmdList;

  ThrowIfFailed(
      device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT,
                                alloc,    // Associated command allocator
                                nullptr,  // Initial PipelineStateObject
                                IID_PPV_ARGS(cmdList.GetAddressOf())));

  auto target = crst.idx2info.find(static_cast<size_t>(-1));
  if (target != crst.idx2info.end()) {
    const auto& passinfo = target->second;
    for (const auto& rsrc : passinfo.constructRsrcs)
      rsrcMngr.Construct(device, rsrc);
    for (const auto& rsrc : passinfo.destructRsrcs)
      rsrcMngr.Destruct(cmdList.Get(), rsrc);
    for (const auto& rsrc : passinfo.moveRsrcs) {
      auto dst = crst.moves_src2dst.find(rsrc)->second;
      rsrcMngr.Move(dst, rsrc);
    }
  }

  for (auto passNodeIdx : crst.sortedPasses) {
    const auto& passinfo = crst.idx2info.find(passNodeIdx)->second;

    for (const auto& rsrc : passinfo.constructRsrcs)
      rsrcMngr.Construct(device, rsrc);

    auto passRsrcs =
        rsrcMngr.RequestPassRsrcs(device, cmdList.Get(), passNodeIdx);
    passFuncs.find(passNodeIdx)->second(cmdList.Get(), passRsrcs);

    for (const auto& rsrc : passinfo.destructRsrcs)
      rsrcMngr.Destruct(cmdList.Get(), rsrc);

    for (const auto& rsrc : passinfo.moveRsrcs) {
      auto dst = crst.moves_src2dst.find(rsrc)->second;
      rsrcMngr.Move(dst, rsrc);
    }
  }

  cmdList->Close();
  ID3D12CommandList* cmdListArray[1] = {cmdList.Get()};
  cmdQueue->ExecuteCommandLists(1, cmdListArray);
}
