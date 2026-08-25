# Copyright © 2026 Saleem Abdulrasool <compnerd@compnerd.org>. All rights reserved.
# SPDX-License-Identifier: BSD-3-Clause

add_library(CounterC STATIC
  Counter.c)

target_include_directories(CounterC PUBLIC
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>")

target_link_libraries(CounterC PUBLIC
  CounterABI)

target_link_libraries(CounterC PRIVATE
  CounterSupport
  uuid)
