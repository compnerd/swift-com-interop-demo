// Copyright © 2026 Saleem Abdulrasool <compnerd@compnerd.org>. All rights reserved.
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#include "CounterClasses.h"

#include <stdint.h>
#include <unknwn.h>

EXTERN_C const IID IID_ICounter;
EXTERN_C const IID IID_IResettableCounter;

#if defined(__cplusplus) && !defined(CINTERFACE)

class DECLSPEC_UUID("5B459E10-3A0E-4E86-A676-6A1F57A40102") SwiftCounter;

MIDL_INTERFACE("5B459E10-3A0E-4E86-A676-6A1F57A40001")
ICounter : public IUnknown {
  virtual HRESULT STDMETHODCALLTYPE Next(int32_t *value) = 0;
  virtual HRESULT STDMETHODCALLTYPE Clone(ICounter **counter) = 0;
};

MIDL_INTERFACE("5B459E10-3A0E-4E86-A676-6A1F57A40002")
IResettableCounter : public ICounter {
  virtual HRESULT STDMETHODCALLTYPE Reset() = 0;
};

#else

typedef struct ICounter ICounter;
typedef struct IResettableCounter IResettableCounter;

typedef struct ICounterVtbl {
  HRESULT (STDMETHODCALLTYPE *QueryInterface)(ICounter *self, REFIID iid,
                                              void **object);
  ULONG (STDMETHODCALLTYPE *AddRef)(ICounter *self);
  ULONG (STDMETHODCALLTYPE *Release)(ICounter *self);
  HRESULT (STDMETHODCALLTYPE *Next)(ICounter *self, int32_t *value);
  HRESULT (STDMETHODCALLTYPE *Clone)(ICounter *self, ICounter **counter);
} ICounterVtbl;

struct ICounter {
  const ICounterVtbl *lpVtbl;
};

typedef struct IResettableCounterVtbl {
  HRESULT (STDMETHODCALLTYPE *QueryInterface)(IResettableCounter *self,
                                              REFIID iid, void **object);
  ULONG (STDMETHODCALLTYPE *AddRef)(IResettableCounter *self);
  ULONG (STDMETHODCALLTYPE *Release)(IResettableCounter *self);
  HRESULT (STDMETHODCALLTYPE *Next)(IResettableCounter *self, int32_t *value);
  HRESULT (STDMETHODCALLTYPE *Clone)(IResettableCounter *self,
                                     ICounter **counter);
  HRESULT (STDMETHODCALLTYPE *Reset)(IResettableCounter *self);
} IResettableCounterVtbl;

struct IResettableCounter {
  const IResettableCounterVtbl *lpVtbl;
};

#define ICounter_QueryInterface(self, iid, object)                              \
  ((self)->lpVtbl->QueryInterface((self), (iid), (object)))
#define ICounter_AddRef(self) ((self)->lpVtbl->AddRef((self)))
#define ICounter_Release(self) ((self)->lpVtbl->Release((self)))
#define ICounter_Next(self, value) ((self)->lpVtbl->Next((self), (value)))
#define ICounter_Clone(self, counter) ((self)->lpVtbl->Clone((self), (counter)))

#endif
