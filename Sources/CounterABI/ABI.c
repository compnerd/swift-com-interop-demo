// Copyright © 2026 Saleem Abdulrasool <compnerd@compnerd.org>. All rights reserved.
// SPDX-License-Identifier: BSD-3-Clause

#include "CounterABI.h"

#include <stddef.h>

const IID IID_ICounter = {
  0x5B459E10, 0x3A0E, 0x4E86, {0xA6, 0x76, 0x6A, 0x1F, 0x57, 0xA4, 0x00, 0x01},
};

const IID IID_IResettableCounter = {
  0x5B459E10, 0x3A0E, 0x4E86, {0xA6, 0x76, 0x6A, 0x1F, 0x57, 0xA4, 0x00, 0x02},
};

const CLSID CLSID_CCounter = {
  0x5B459E10, 0x3A0E, 0x4E86, {0xA6, 0x76, 0x6A, 0x1F, 0x57, 0xA4, 0x01, 0x00},
};

_Static_assert(sizeof(ICounter *) == sizeof(void *),
               "a COM interface is one pointer");
_Static_assert(offsetof(ICounter, lpVtbl) == 0,
               "the vtable is the interface address point");
_Static_assert(offsetof(IResettableCounterVtbl, Reset) == sizeof(ICounterVtbl),
               "IResettableCounter extends the ICounter vtable");
