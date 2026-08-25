# Copyright © 2026 Saleem Abdulrasool <compnerd@compnerd.org>. All rights reserved.
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.29)

project(COMCounter LANGUAGES C CXX Swift)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED YES)
set(CMAKE_C_EXTENSIONS NO)

set(CMAKE_MSVC_RUNTIME_LIBRARY MultiThreadedDLL)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>" CACHE PATH
    "Directory containing the runnable COM demo")

add_compile_definitions("$<$<COMPILE_LANGUAGE:C,CXX>:_WIN32_WINNT=0x0A00>"
                        "$<$<COMPILE_LANGUAGE:C,CXX>:UNICODE>"
                        "$<$<COMPILE_LANGUAGE:C,CXX>:_UNICODE>")

add_compile_options("$<$<COMPILE_LANGUAGE:Swift>:SHELL:-Xfrontend -enable-experimental-com-interop>"
                    "$<$<COMPILE_LANGUAGE:Swift>:SHELL:-Xfrontend -com-interop-model=microsoft>"
                    "$<$<COMPILE_LANGUAGE:Swift>:SHELL:-package-name COMCounter>"
                    "$<$<AND:$<COMPILE_LANGUAGE:Swift>,$<CONFIG:Debug,RelWithDebInfo>>:-debug-info-format=codeview>")

add_link_options("$<$<AND:$<LINK_LANGUAGE:Swift>,$<CONFIG:Debug,RelWithDebInfo>>:LINKER:/DEBUG>")

include(CTest)

add_subdirectory(Sources)
