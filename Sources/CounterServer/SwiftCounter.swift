// Copyright © 2026 Saleem Abdulrasool <compnerd@compnerd.org>. All rights reserved.
// SPDX-License-Identifier: BSD-3-Clause

package import CounterInterfaces
internal import CounterSupport
package import WinSDK

@com(implementation: "5B459E10-3A0E-4E86-A676-6A1F57A40102")
package final class SwiftCounter: IResettableCounter {
  private var value: Int32

  package init(value: Int32 = 0) {
    self.value = value
    CounterServer_Lock()
  }

  deinit {
    CounterServer_Unlock()
  }

  package func Next(_ output: UnsafeMutablePointer<Int32>?) -> HRESULT {
    guard let output else { return E_POINTER }
    output.pointee = value
    value += 1
    return COM.S_OK
  }

  package func Clone(_ output: UnsafeMutablePointer<(any ICounter)?>?)
      -> HRESULT {
    guard let output else { return E_POINTER }
    output.pointee = SwiftCounter(value: value)
    return COM.S_OK
  }

  package func Reset() -> HRESULT {
    value = 0
    return COM.S_OK
  }
}

@c(SwiftCounter_Create)
package func create(_ output: UnsafeMutablePointer<(any ICounter)?>?)
    -> HRESULT {
  guard let output else { return E_POINTER }
  output.pointee = SwiftCounter()
  return COM.S_OK
}
