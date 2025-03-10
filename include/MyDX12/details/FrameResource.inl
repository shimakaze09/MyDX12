//
// Created by Admin on 8/03/2025.
//

#pragma once

namespace My::DX12 {
template <typename T>
T* FrameResource::GetResource(const std::string& name) const {
  assert(HaveResource(name));
  return reinterpret_cast<T*>(resourceMap.find(name)->second);
}

template <typename T>
FrameResource& FrameResource::RegisterResource(std::string name, T* pResource) {
  RegisterResource(std::move(name), pResource,
                   [](void* ptr) { delete reinterpret_cast<T*>(ptr); });
  return *this;
}
}  // namespace My::DX12
