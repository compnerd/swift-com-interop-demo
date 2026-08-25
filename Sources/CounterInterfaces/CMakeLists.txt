# Copyright © 2026 Saleem Abdulrasool <compnerd@compnerd.org>. All rights reserved.
# SPDX-License-Identifier: BSD-3-Clause

add_library(CounterInterfaces STATIC
  CounterInterfaces.swift)
set_target_properties(CounterInterfaces PROPERTIES
  PREFIX "lib")

target_include_directories(CounterInterfaces INTERFACE
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>")
