//
// Created by Admin on 12/03/2025.
//

#pragma once

#include "Rsrc.h"

#include <MyFG/Compiler.h>

#include <functional>

namespace My::MyDX12::FG {
class RsrcMngr;

class Executor {
 public:
  Executor& RegisterPassFunc(size_t passNodeIdx,
                             std::function<void(const PassRsrcs&)> func) {
    passFuncs[passNodeIdx] = std::move(func);
    return *this;
  }

  void NewFrame() { passFuncs.clear(); }

  void Execute(const MyFG::Compiler::Result& crst, RsrcMngr& rsrcMngr);

 private:
  std::unordered_map<size_t, std::function<void(const PassRsrcs&)>> passFuncs;
};
}  // namespace My::MyDX12::FG
