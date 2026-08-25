# Copyright © 2026 Saleem Abdulrasool <compnerd@compnerd.org>. All rights reserved.
# SPDX-License-Identifier: BSD-3-Clause

add_executable(counter-cxx-client
  main.cpp
  "${COMCounter_SOURCE_DIR}/Manifests/counter-cxx-client.exe.manifest")

target_link_libraries(counter-cxx-client PRIVATE
  CounterABI
  ole32)

if(BUILD_TESTING)
  add_test(NAME COMCounter.CxxClient
    COMMAND $<TARGET_FILE:counter-cxx-client>)
  set_tests_properties(COMCounter.CxxClient PROPERTIES
    WORKING_DIRECTORY "$<TARGET_FILE_DIR:counter-cxx-client>")
endif()
