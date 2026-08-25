# Copyright © 2026 Saleem Abdulrasool <compnerd@compnerd.org>. All rights reserved.
# SPDX-License-Identifier: BSD-3-Clause

# CMake's MSVC platform settings add raw link.exe options here. Swift targets
# link through swiftc, which interprets those options as input files.
# Required linker options are passed through LINKER: instead.
set(CMAKE_EXE_LINKER_FLAGS "")
foreach(configuration DEBUG MINSIZEREL RELEASE RELWITHDEBINFO)
  set(CMAKE_EXE_LINKER_FLAGS_${configuration} "")
endforeach()

add_executable(counter-swift-client
  main.swift)

set_target_properties(counter-swift-client PROPERTIES
  Swift_MODULE_NAME CounterSwiftClient)

target_link_libraries(counter-swift-client PRIVATE
  CounterABI
  CounterInterfaces
  CounterServer
  ole32)

target_link_options(counter-swift-client PRIVATE
  "LINKER:/MANIFEST:EMBED"
  "LINKER:/MANIFESTINPUT:${COMCounter_SOURCE_DIR}/Manifests/counter-swift-client.exe.manifest")

if(BUILD_TESTING)
  add_test(NAME COMCounter.SwiftClient
    COMMAND $<TARGET_FILE:counter-swift-client>)
  set_tests_properties(COMCounter.SwiftClient PROPERTIES
    WORKING_DIRECTORY "$<TARGET_FILE_DIR:counter-swift-client>")
endif()
