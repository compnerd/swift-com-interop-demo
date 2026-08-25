# Copyright © 2026 Saleem Abdulrasool <compnerd@compnerd.org>. All rights reserved.
# SPDX-License-Identifier: BSD-3-Clause

# CMake's MSVC platform settings add raw link.exe options here. Swift targets
# link through swiftc, which interprets those options as input files.
# Required linker options are passed through LINKER: instead.
set(CMAKE_SHARED_LINKER_FLAGS "")
foreach(configuration DEBUG MINSIZEREL RELEASE RELWITHDEBINFO)
  set(CMAKE_SHARED_LINKER_FLAGS_${configuration} "")
endforeach()

add_library(CounterServer SHARED
  SwiftCounter.swift)

target_include_directories(CounterServer INTERFACE
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>")

target_link_libraries(CounterServer PRIVATE
  CounterCpp
  CounterInterfaces)

target_link_libraries(CounterServer PUBLIC
  CounterSupport)

# The Windows SDK declares these entry points with STDAPI, so adding
# __declspec(dllexport) to their definitions gives them conflicting linkage.
# Export them at link time; x86 requires stdcall-decorated aliases.

if(CMAKE_SIZEOF_VOID_P EQUAL 4)
  target_link_options(CounterServer PRIVATE
    "LINKER:/EXPORT:DllCanUnloadNow=_DllCanUnloadNow@0"
    "LINKER:/EXPORT:DllGetClassObject=_DllGetClassObject@12")
else()
  target_link_options(CounterServer PRIVATE
    "LINKER:/EXPORT:DllCanUnloadNow"
    "LINKER:/EXPORT:DllGetClassObject")
endif()
