<!-- Copyright © 2026 Saleem Abdulrasool <compnerd@compnerd.org>. All rights reserved. -->
<!-- SPDX-License-Identifier: BSD-3-Clause -->

# Swift COM interop counter

This small registration-free COM server demonstrates the two directions of
Swift COM interop:

```text
Swift client -> C COM object
C++ client   -> Swift COM object
```

Both clients also call both implementations. The C declarations provide the
canonical ABI where direct Swift/C++ interop is unavailable. A tiny Swift
helper module contains package-scoped protocols with the same IIDs and vtable
layout. The protocols are not part of the package's public API.

The C class identifier is imported from `CounterClasses.h`. Swift clients use
the compiler-synthesized `SwiftCounter.CLSID`; C++ uses `__uuidof(SwiftCounter)`
without adding another stored GUID.

The clients activate each object as `ICounter`, invoke `Next`, and then obtain
`IResettableCounter`. C++ spells that operation `QueryInterface`; Swift spells
it `as? any IResettableCounter`. `Clone(ICounter **)` exercises a typed COM
interface out parameter in both languages.

## What to inspect

The in-process paths keep the generated code easy to compare:

- calls on the C object lower to an ordinary vtable load and indirect call;
- calls from C++ enter the native Swift COM vtable;
- a Swift COM existential occupies one machine word;
- native Swift entries perform only the object-address adjustment required by
  the Swift object layout before entering the implementation.

Build with debug information to step from the clients through
`QueryInterface`, the vtable entry, and the C or Swift method body. The COM
module remains visible in the Swift cast path, so the language boundary does
not remove source-level debugging.

## Build and test

Use a Windows Swift toolchain containing the experimental COM module:

```powershell
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
```

CTest runs the C++ and Swift clients beside `CounterServer.dll`. Their embedded
manifests activate the C and Swift classes without registry changes or
administrator privileges.

The useful targets are:

| Target | Purpose |
|--------|---------|
| `CounterInterfaces` | Package-scoped Swift view of the shared COM ABI |
| `CounterServer` | Mixed C, C++, and Swift in-process server |
| `counter-cxx-client` | C++ consumer of the C and Swift objects |
| `counter-swift-client` | Swift consumer of the C and Swift objects |

`Scripts/Inspect.ps1` lists the server exports and finds the Swift COM method
witnesses in the disassembly:

```powershell
.\Scripts\Inspect.ps1 -Server .\build\bin\Debug\CounterServer.dll
```
