// Copyright © 2026 Saleem Abdulrasool <compnerd@compnerd.org>. All rights reserved.
// SPDX-License-Identifier: BSD-3-Clause

#include "CounterABI.h"

#include <cstdint>
#include <iostream>
#include <stdexcept>

namespace {

void CHECK(HRESULT result, const char *operation) {
  if (FAILED(result))
    throw std::runtime_error(operation);
}

void Exercise(const char *name, REFCLSID clsid) {
  ICounter *base = nullptr;
  CHECK(CoCreateInstance(clsid, nullptr, CLSCTX_INPROC_SERVER, IID_ICounter,
                         reinterpret_cast<void **>(&base)),
        "CoCreateInstance");

  std::int32_t value = 0;
  CHECK(base->Next(&value), "Next");

  IResettableCounter *counter = nullptr;
  CHECK(base->QueryInterface(__uuidof(IResettableCounter),
                             reinterpret_cast<void **>(&counter)),
        "QueryInterface");
  base->Release();

  ICounter *clone = nullptr;
  CHECK(counter->Clone(&clone), "Clone");
  CHECK(counter->Reset(), "Reset");
  CHECK(counter->Next(&value), "Next after Reset");
  counter->Release();

  std::int32_t clonedValue = 0;
  CHECK(clone->Next(&clonedValue), "Clone.Next");
  clone->Release();
  std::cout << name << ": reset = " << value << ", clone = " << clonedValue
            << '\n';
}

} // namespace

int main() try {
  CHECK(CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED), "CoInitializeEx");
  try {
    Exercise("C", CLSID_CCounter);
    Exercise("Swift", __uuidof(SwiftCounter));
  } catch (...) {
    CoUninitialize();
    throw;
  }
  CoUninitialize();
  return 0;
} catch (const std::exception &error) {
  std::cerr << error.what() << '\n';
  return 1;
}
