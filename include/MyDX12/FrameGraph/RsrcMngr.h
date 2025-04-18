#pragma once

#include <unordered_set>

#include "../DescriptorHeapMngr.h"
#include "../Device.h"
#include "../GCmdList.h"
#include "Rsrc.h"

namespace Smkz::MyFG {
class FrameGraph;
}

namespace Smkz::MyDX12::FG {
// manage per pass resources
// CBV, SRV, UAV, RTV, DSV
class RsrcMngr {
 public:
  RsrcMngr();
  ~RsrcMngr();

  // clear resources
  // you can call it when resize window
  void Clear();

  // clear per frame datas
  // call it per frame before registerations
  void NewFrame();

  // descriptor heap reserve
  // call by Smkz::MyDX12::FG::Executor
  void DHReserve();

  // allocate handles in descriptor heap for each resource nodes
  // call by Smkz::MyDX12::FG::Executor
  void AllocateHandle();

  // import, create or reuse buffer for the resource node
  // call by Smkz::MyDX12::FG::Executor
  void Construct(ID3D12Device*, size_t rsrcNodeIdx);

  // recycle the buffer of the resource node
  // call by Smkz::MyDX12::FG::Executor
  void Destruct(ID3D12GraphicsCommandList*, size_t rsrcNodeIdx);

  // move the resource view of the source resource node to the destination
  // resource node call by Smkz::MyDX12::FG::Executor
  void Move(size_t dstRsrcNodeIdx, size_t srcRsrcNodeIdx);

  // - get the resource map (resource node index -> impl resource) of the pass
  // node
  // - we will
  //   1. change buffer state
  //   2. init handle
  // - call by Smkz::MyDX12::FG::Executor
  PassRsrcs RequestPassRsrcs(ID3D12Device*, ID3D12GraphicsCommandList*,
                             size_t passNodeIdx);

  // mark the resource node as imported
  RsrcMngr& RegisterImportedRsrc(size_t rsrcNodeIdx, SRsrcView view) {
    importeds[rsrcNodeIdx] = view;
    return *this;
  }

  // mark the resource node as temporal
  RsrcMngr& RegisterTemporalRsrc(size_t rsrcNodeIdx, RsrcType type) {
    temporals[rsrcNodeIdx] = type;
    return *this;
  }

  // provide details for the resource node of the pass node
  RsrcMngr& RegisterPassRsrcState(size_t passNodeIdx, size_t rsrcNodeIdx,
                                  RsrcState state);
  // a resource can have several descriptions
  RsrcMngr& RegisterPassRsrcImplDesc(size_t passNodeIdx, size_t rsrcNodeIdx,
                                     RsrcImplDesc desc);
  // a helper function to simplify register pass resource's state and impl
  // description
  RsrcMngr& RegisterPassRsrc(size_t passNodeIdx, size_t rsrcNodeIdx,
                             RsrcState state, RsrcImplDesc desc);

  // provide external handles for the resource node
  RsrcMngr& RegisterRsrcHandle(size_t rsrcNodeIdx, RsrcImplDesc desc,
                               D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle,
                               D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle = {0});

  // only support CBV, SRV, UAV
  RsrcMngr& RegisterRsrcTable(
      const std::vector<std::tuple<size_t, RsrcImplDesc>>& rsrcNodeIndices);

  // you should
  // 1. use RegisterImportedRsrc or RegisterTemporalRsrc to mark each resource
  // nodes
  // 2. use RegisterPassRsrcs to mark each resource nodes for every passes
  // if you do it correct, then return true, else return false
  // TODO : move
  bool CheckComplete(const MyFG::FrameGraph& fg);

 private:
  bool IsImported(size_t rsrcNodeIdx) const noexcept {
    return importeds.find(rsrcNodeIdx) != importeds.end();
  }

  // CSU : CBV, CSU, UAV

  void CSUDHReserve(UINT num);
  void DsvDHReserve(UINT num);
  void RtvDHReserve(UINT num);

  // type -> vector<view>
  std::vector<RsrcPtr> rsrcKeeper;
  std::unordered_map<RsrcType, std::vector<SRsrcView>> pool;

  // rsrcNodeIdx -> view
  std::unordered_map<size_t, SRsrcView> importeds;

  // rsrcNodeIdx -> type
  std::unordered_map<size_t, RsrcType> temporals;

  // passNodeIdx -> resource (state + descs) map
  std::unordered_map<
      size_t, std::unordered_map<
                  size_t, std::tuple<RsrcState, std::vector<RsrcImplDesc>>>>
      passNodeIdx2rsrcMap;

  // rsrcNodeIdx -> view
  std::unordered_map<size_t, SRsrcView> actives;

  // rsrcNodeIdx -> typeinfo
  std::unordered_map<size_t, RsrcTypeInfo> typeinfoMap;

  MyDX12::DynamicSuballocMngr* csuDynamicDH{nullptr};

  MyDX12::DescriptorHeapAllocation csuDH;
  std::vector<UINT> csuDHfree;
  std::unordered_set<UINT> csuDHused;

  MyDX12::DescriptorHeapAllocation rtvDH;
  std::vector<UINT> rtvDHfree;
  std::unordered_set<UINT> rtvDHused;

  MyDX12::DescriptorHeapAllocation dsvDH;
  std::vector<UINT> dsvDHfree;
  std::unordered_set<UINT> dsvDHused;
};
}  // namespace Smkz::MyDX12::FG
