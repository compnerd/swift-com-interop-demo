# Copyright © 2026 Saleem Abdulrasool <compnerd@compnerd.org>. All rights reserved.
# SPDX-License-Identifier: BSD-3-Clause

add_library(CounterCpp OBJECT
  Counter.cpp)

target_link_libraries(CounterCpp PRIVATE
  CounterC
  CounterSupport)
