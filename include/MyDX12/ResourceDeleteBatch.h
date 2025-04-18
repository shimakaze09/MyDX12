#pragma once

#include <future>

#include "Util.h"

namespace Smkz::MyDX12 {
class ResourceDeleteBatch {
 public:
  void Add(ID3D12Resource* resource) { resources.push_back(resource); }

  // add delete resources event to command queue
  // [async]
  // - delete resources
  std::future<void> Commit(ID3D12Device* device, ID3D12CommandQueue*);

 private:
  std::vector<ID3D12Resource*> resources;
};
}  // namespace Smkz::MyDX12
