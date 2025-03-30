//
// Created by Admin on 7/03/2025.
//

#include <DirectXColors.h>

#include "../common/d3dApp.h"

using namespace DirectX;

class InitDirect3DApp : public D3DApp {
 public:
  InitDirect3DApp(HINSTANCE hInstance);
  ~InitDirect3DApp();

  virtual bool Initialize() override;

 private:
  virtual void OnResize() override;
  virtual void Update(const GameTimer& gt) override;
  virtual void Draw(const GameTimer& gt) override;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine,
                   int showCmd) {
  // Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

  try {
    InitDirect3DApp theApp(hInstance);
    if (!theApp.Initialize())
      return 0;

    return theApp.Run();
  } catch (My::MyDX12::Util::Exception& e) {
    MessageBox(nullptr, e.ToString().c_str(), L"HR Failed", MB_OK);
    return 0;
  }
}

InitDirect3DApp::InitDirect3DApp(HINSTANCE hInstance) : D3DApp(hInstance) {}

InitDirect3DApp::~InitDirect3DApp() {}

bool InitDirect3DApp::Initialize() {
  if (!D3DApp::Initialize())
    return false;

  return true;
}

void InitDirect3DApp::OnResize() {
  D3DApp::OnResize();
}

void InitDirect3DApp::Update(const GameTimer& gt) {}

void InitDirect3DApp::Draw(const GameTimer& gt) {
  // Reuse the memory associated with command recording.
  // We can only reset when the associated command lists have finished execution
  // on the GPU.
  ThrowIfFailed(mDirectCmdListAlloc->Reset());

  // A command list can be reset after it has been added to the command queue
  // via ExecuteCommandList.
  // Reusing the command list reuses memory.
  ThrowIfFailed(mCommandList->Reset(mDirectCmdListAlloc.Get(), nullptr));

  // Indicate a state transition on the resource usage.
  DirectX::TransitionResource(mCommandList.Get(), CurrentBackBuffer(),
                              D3D12_RESOURCE_STATE_PRESENT,
                              D3D12_RESOURCE_STATE_RENDER_TARGET);

  // Set the viewport and scissor rect.  This needs to be reset whenever the
  // command list is reset.
  mCommandList->RSSetViewports(1, &mScreenViewport);
  mCommandList->RSSetScissorRects(1, &mScissorRect);

  // Clear the back buffer and depth buffer.
  mCommandList->ClearRenderTargetView(CurrentBackBufferView(),
                                      Colors::LightSteelBlue, 0, nullptr);
  mCommandList->ClearDepthStencilView(
      DepthStencilView(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL,
      1.0f, 0, 0, nullptr);

  // Specify the buffers we are going to render to.
  // 'OM' -> Output Merge
  const auto curBackView = CurrentBackBufferView();
  const auto depthView = DepthStencilView();
  mCommandList->OMSetRenderTargets(1, &curBackView, true, &depthView);

  // Indicate a state transition on the resource usage.
  DirectX::TransitionResource(mCommandList.Get(), CurrentBackBuffer(),
                              D3D12_RESOURCE_STATE_RENDER_TARGET,
                              D3D12_RESOURCE_STATE_PRESENT);

  // Done recording commands.
  ThrowIfFailed(mCommandList->Close());

  // Add the command list to the queue for execution.
  ID3D12CommandList* cmdsLists[] = {mCommandList.Get()};
  mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

  // swap the back and front buffers
  ThrowIfFailed(mSwapChain->Present(0, 0));
  mCurrBackBuffer = (mCurrBackBuffer + 1) % SwapChainBufferCount;

  // Wait until frame commands are complete.  This waiting is inefficient and is
  // done for simplicity.  Later we will show how to organize our rendering code
  // so we do not have to wait per frame.
  FlushCommandQueue();
}
