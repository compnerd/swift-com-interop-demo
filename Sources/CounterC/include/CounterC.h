// Copyright © 2026 Saleem Abdulrasool <compnerd@compnerd.org>. All rights reserved.
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#include "CounterABI.h"

#ifdef __cplusplus
extern "C" {
#endif

HRESULT CCounter_Create(ICounter **counter);

#ifdef __cplusplus
}
#endif
