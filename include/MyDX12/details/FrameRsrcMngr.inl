//
// Created by Admin on 12/03/2025.
//

#pragma once

#include <MyTemplate/Func.h>

namespace My::MyDX12 {
template <typename T>
T& FrameRsrcMngr::GetResource(std::string_view name) {
  static_assert(!std::is_reference_v<T>);
  assert(HaveResource(name));
  return std::any_cast<T&>(resourceMap.find(name)->second);
}

template <typename T>
const T& FrameRsrcMngr::GetResource(std::string_view name) const {
  return const_cast<FrameRsrcMngr*>(this)->GetResource<T>();
}

template <typename T>
FrameRsrcMngr& FrameRsrcMngr::RegisterResource(std::string name, T&& resource) {
  assert(!HaveResource(name));
  resourceMap.emplace(std::move(name), std::move(resource));
  return *this;
}

template <typename Func>
FrameRsrcMngr& FrameRsrcMngr::DelayUpdateResource(std::string name,
                                                  Func&& updator) {
  using ArgList = FuncTraits_ArgList<std::decay_t<Func>>;
  static_assert(Length_v<ArgList> == 1);
  using Arg = Front_t<ArgList>;
  delayUpdatorMap.emplace_back(std::tuple{
      std::move(name), std::function{[updator = std::forward<Func>(updator)](
                                         std::any& resource) mutable {
        updator(std::any_cast<Arg>(resource));
      }}});
  return *this;
}
}  // namespace My::MyDX12
