// Copyright © 2026 Saleem Abdulrasool <compnerd@compnerd.org>. All rights reserved.
// SPDX-License-Identifier: BSD-3-Clause

#include "CounterABI.h"
#include "CounterC.h"
#include "CounterSupport.h"

#include <atomic>
#include <new>

extern "C" HRESULT SwiftCounter_Create(ICounter **counter);

namespace {

std::atomic<unsigned long> ModuleLocks{0};

class CounterClassFactory final : public IClassFactory {
public:
  explicit CounterClassFactory(REFCLSID clsid) : ClassID(clsid) {
    CounterServer_Lock();
  }

  HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void **object) override {
    if (!object)
      return E_POINTER;
    *object = nullptr;
    if (iid != __uuidof(IUnknown) && iid != __uuidof(IClassFactory))
      return E_NOINTERFACE;
    *object = static_cast<IClassFactory *>(this);
    AddRef();
    return S_OK;
  }

  ULONG STDMETHODCALLTYPE AddRef() override { return ++References; }

  ULONG STDMETHODCALLTYPE Release() override {
    const ULONG references = --References;
    if (!references)
      delete this;
    return references;
  }

  HRESULT STDMETHODCALLTYPE CreateInstance(IUnknown *outer, REFIID iid,
                                           void **object) override {
    if (!object)
      return E_POINTER;
    *object = nullptr;
    if (outer)
      return CLASS_E_NOAGGREGATION;

    ICounter *counter = nullptr;
    const HRESULT result = ClassID == CLSID_CCounter
                               ? CCounter_Create(&counter)
                               : SwiftCounter_Create(&counter);
    if (FAILED(result))
      return result;
    const HRESULT queried = counter->QueryInterface(iid, object);
    counter->Release();
    return queried;
  }

  HRESULT STDMETHODCALLTYPE LockServer(BOOL lock) override {
    lock ? CounterServer_Lock() : CounterServer_Unlock();
    return S_OK;
  }

private:
  ~CounterClassFactory() { CounterServer_Unlock(); }

  std::atomic<ULONG> References{1};
  CLSID ClassID;
};

HRESULT CreateClassFactory(REFCLSID clsid, IClassFactory **factory) {
  if (!factory)
    return E_POINTER;
  *factory = nullptr;
  if (clsid != CLSID_CCounter && clsid != __uuidof(SwiftCounter))
    return CLASS_E_CLASSNOTAVAILABLE;
  *factory = new (std::nothrow) CounterClassFactory(clsid);
  return *factory ? S_OK : E_OUTOFMEMORY;
}

} // namespace

extern "C" void CounterServer_Lock(void) { ++ModuleLocks; }
extern "C" void CounterServer_Unlock(void) { --ModuleLocks; }

STDAPI DllGetClassObject(REFCLSID clsid, REFIID iid, void **object) {
  IClassFactory *factory = nullptr;
  HRESULT result = CreateClassFactory(clsid, &factory);
  if (FAILED(result))
    return result;
  result = factory->QueryInterface(iid, object);
  factory->Release();
  return result;
}

STDAPI DllCanUnloadNow(void) {
  return ModuleLocks == 0 ? S_OK : S_FALSE;
}
