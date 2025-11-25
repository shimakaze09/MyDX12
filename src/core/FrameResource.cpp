#include <MyDX12/FrameResource.h>

using namespace My;

void MyDX12::FrameResource::Signal(ID3D12CommandQueue* cmdQueue,
                                   UINT64 cpuFence) {
  this->cpuFence = cpuFence;
  cmdQueue->Signal(gpuFence, cpuFence);
}

void MyDX12::FrameResource::BeginFrame() {
  if (gpuFence->GetCompletedValue() < cpuFence) {
    HANDLE eventHandle =
        CreateEventEx(nullptr, nullptr, false, EVENT_ALL_ACCESS);
    ThrowIfFailed(gpuFence->SetEventOnCompletion(cpuFence, eventHandle));
    WaitForSingleObject(eventHandle, INFINITE);
    CloseHandle(eventHandle);
  }

  for (const auto& name : delayUnregisterResources) UnregisterResource(name);
  delayUnregisterResources.clear();

  for (const auto& [name, updator] : delayUpdatorMap)
    updator(resourceMap.at(name));
  delayUpdatorMap.clear();
}

MyDX12::FrameResource& MyDX12::FrameResource::UnregisterResource(
    std::string_view name) {
  assert(HaveResource(name));

  resourceMap.erase(resourceMap.find(name));

  return *this;
}

MyDX12::FrameResource& MyDX12::FrameResource::DelayUnregisterResource(
    std::string name) {
  assert(HaveResource(name));
  delayUnregisterResources.emplace_back(std::move(name));
  return *this;
}
