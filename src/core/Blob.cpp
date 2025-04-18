#include <MyDX12/Blob.h>

using namespace Smkz;

void MyDX12::Blob::Create(SIZE_T size) {
  ThrowIfFailed(D3DCreateBlob(size, &raw));
}

void MyDX12::Blob::Copy(const void* data, SIZE_T size) {
  CopyMemory(raw->GetBufferPointer(), data, size);
}

void MyDX12::Blob::Create(const void* data, SIZE_T size) {
  Create(size);
  Copy(data, size);
}
