// Copyright © 2026 Saleem Abdulrasool <compnerd@compnerd.org>. All rights reserved.
// SPDX-License-Identifier: BSD-3-Clause

internal import CounterClasses
private import CounterInterfaces
internal import CounterServer

private func exercise(_ name: String, _ base: any ICounter) throws {
  _ = try base.next()

  guard let counter = base as? any IResettableCounter else {
    fatalError("\(name) does not expose IResettableCounter")
  }
  let clone = try counter.clone()
  try counter.reset()
  print("\(name): reset = \(try counter.next()), clone = \(try clone.next())")
}

try withCOMContext(.apartment, activation: .inproc) {
  print("any ICounter size = \(MemoryLayout<any ICounter>.size)")
  let c = try CoCreateInstance(CLSID_CCounter, as: ICounter.self)
  try exercise("C", c)
  let swift = try CoCreateInstance(SwiftCounter.CLSID, as: ICounter.self)
  try exercise("Swift", swift)
}
