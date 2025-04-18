#pragma once

#include <MyFG/Compiler.hpp>
#include <functional>

#include "Rsrc.h"

namespace Smkz::MyDX12::FG {
class RsrcMngr;

class Executor {
 public:
  using PassFunction =
      std::function<void(ID3D12GraphicsCommandList*, const PassRsrcs&)>;

  Executor& RegisterPassFunc(size_t passNodeIdx, PassFunction func) {
    passFuncs[passNodeIdx] = std::move(func);
    return *this;
  }

  void NewFrame() { passFuncs.clear(); }

  // TODO: parallel
  void Execute(ID3D12Device* device, ID3D12CommandQueue* cmdQueue,
               ID3D12CommandAllocator* alloc,
               const MyFG::Compiler::Result& crst, RsrcMngr& rsrcMngr);

 private:
  std::unordered_map<size_t, PassFunction> passFuncs;
};
}  // namespace Smkz::MyDX12::FG
