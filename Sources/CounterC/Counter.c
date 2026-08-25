// Copyright © 2026 Saleem Abdulrasool <compnerd@compnerd.org>. All rights reserved.
// SPDX-License-Identifier: BSD-3-Clause

#include "CounterC.h"
#include "CounterSupport.h"

#include <stddef.h>
#include <stdlib.h>

typedef struct CCounter {
  IResettableCounter Interface;
  LONG References;
  int32_t Value;
} CCounter;

_Static_assert(offsetof(CCounter, Interface) == 0,
               "the COM interface is the object address point");

static CCounter *CCounter_FromInterface(IResettableCounter *value) {
  return (CCounter *)value;
}

static HRESULT STDMETHODCALLTYPE
CCounter_QueryInterface(IResettableCounter *self, REFIID iid, void **object) {
  if (!object)
    return E_POINTER;
  *object = NULL;

  if (IsEqualIID(iid, &IID_IUnknown) || IsEqualIID(iid, &IID_ICounter))
    *object = (ICounter *)self;
  else if (IsEqualIID(iid, &IID_IResettableCounter))
    *object = self;
  else
    return E_NOINTERFACE;

  self->lpVtbl->AddRef(self);
  return S_OK;
}

static ULONG STDMETHODCALLTYPE CCounter_AddRef(IResettableCounter *self) {
  return (ULONG)InterlockedIncrement(&CCounter_FromInterface(self)->References);
}

static ULONG STDMETHODCALLTYPE CCounter_Release(IResettableCounter *self) {
  CCounter *counter = CCounter_FromInterface(self);
  const LONG references = InterlockedDecrement(&counter->References);
  if (!references) {
    CounterServer_Unlock();
    free(counter);
  }
  return (ULONG)references;
}

static HRESULT STDMETHODCALLTYPE CCounter_Next(IResettableCounter *self,
                                               int32_t *value) {
  if (!value)
    return E_POINTER;
  *value = CCounter_FromInterface(self)->Value++;
  return S_OK;
}

static HRESULT CCounter_CreateWithValue(int32_t value, ICounter **result);

static HRESULT STDMETHODCALLTYPE CCounter_Clone(IResettableCounter *self,
                                                ICounter **counter) {
  return CCounter_CreateWithValue(CCounter_FromInterface(self)->Value, counter);
}

static HRESULT STDMETHODCALLTYPE CCounter_Reset(IResettableCounter *self) {
  CCounter_FromInterface(self)->Value = 0;
  return S_OK;
}

static const IResettableCounterVtbl CCounter_VTable = {
  CCounter_QueryInterface,
  CCounter_AddRef,
  CCounter_Release,
  CCounter_Next,
  CCounter_Clone,
  CCounter_Reset,
};

static HRESULT CCounter_CreateWithValue(int32_t value, ICounter **result) {
  if (!result)
    return E_POINTER;
  *result = NULL;

  CCounter *counter = (CCounter *)calloc(1, sizeof(*counter));
  if (!counter)
    return E_OUTOFMEMORY;
  counter->Interface.lpVtbl = &CCounter_VTable;
  counter->References = 1;
  counter->Value = value;
  CounterServer_Lock();
  *result = (ICounter *)&counter->Interface;
  return S_OK;
}

HRESULT CCounter_Create(ICounter **counter) {
  return CCounter_CreateWithValue(0, counter);
}
