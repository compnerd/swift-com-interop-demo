# Copyright © 2026 Saleem Abdulrasool <compnerd@compnerd.org>. All rights reserved.
# SPDX-License-Identifier: BSD-3-Clause

param(
  [Parameter(Mandatory = $true)]
  [string] $Server,
  [string] $PDB,
  [string] $LLVMObjDump = "llvm-objdump",
  [string] $LLVMPDBUtil = "llvm-pdbutil"
)

$ErrorActionPreference = "Stop"

Write-Host "-- COM server exports --"
& $LLVMObjDump -p $Server |
  Select-String "DllGetClassObject|DllCanUnloadNow|SwiftCounter_Create"

Write-Host "-- Swift COM method witnesses --"
& $LLVMObjDump -d --demangle $Server |
  Select-String "COM method witness|TWV|Next|Reset"

if ($PDB -and (Test-Path -LiteralPath $PDB) -and
    (Get-Command $LLVMPDBUtil -ErrorAction SilentlyContinue)) {
  Write-Host "-- COM runtime source references in debug information --"
  & $LLVMPDBUtil dump -l $PDB |
    Select-String "COM[/\\].*\.swift|IUnknown\.swift|COMActivation\.swift"
}
