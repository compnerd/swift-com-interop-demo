// Copyright © 2026 Saleem Abdulrasool <compnerd@compnerd.org>. All rights reserved.
// SPDX-License-Identifier: BSD-3-Clause

package import WinSDK

@com(interface: "5B459E10-3A0E-4E86-A676-6A1F57A40001")
package protocol ICounter: COM.IUnknown {
  func Next(_ value: UnsafeMutablePointer<Int32>?) -> HRESULT
  func Clone(_ counter: UnsafeMutablePointer<(any ICounter)?>?) -> HRESULT
}

@com(interface: "5B459E10-3A0E-4E86-A676-6A1F57A40002")
package protocol IResettableCounter: ICounter {
  func Reset() -> HRESULT
}

package struct CounterFailure: Error, Sendable, CustomStringConvertible {
  package let operation: StaticString
  package let hresult: HRESULT

  package var description: String {
    "\(operation) failed with \(hresult)"
  }
}

@inline(__always)
private func SUCCEEDED(_ result: HRESULT) -> Bool {
  result >= 0
}

extension ICounter {
  package func next() throws(CounterFailure) -> Int32 {
    var value: Int32 = 0
    let result = Next(&value)
    guard SUCCEEDED(result) else {
      throw CounterFailure(operation: "Next", hresult: result)
    }
    return value
  }

  package func clone() throws(CounterFailure) -> any ICounter {
    var counter: (any ICounter)?
    let result = Clone(&counter)
    guard SUCCEEDED(result) else {
      throw CounterFailure(operation: "Clone", hresult: result)
    }
    guard let counter else {
      throw CounterFailure(operation: "Clone", hresult: E_UNEXPECTED)
    }
    return counter
  }
}

extension IResettableCounter {
  package func reset() throws(CounterFailure) {
    let result = Reset()
    guard SUCCEEDED(result) else {
      throw CounterFailure(operation: "Reset", hresult: result)
    }
  }
}
