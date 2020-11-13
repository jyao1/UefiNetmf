/** @file

  Copyright (c) 2014, Intel Corporation. All rights reserved.<BR>
  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/


#ifdef __cplusplus
extern "C" {
#endif

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>

#ifdef __cplusplus
}
#endif

class TestClass {
private:
  UINTN  Member1;
  UINTN  *Addr;
public:
  TestClass()
  {
    DEBUG ((EFI_D_INFO, "TestClass - 0x%x\n", Member1));
    Member1 = 1;
    Addr = new UINTN;
  }
  ~TestClass()
  {
    Member1 = 0;
    delete Addr;
    DEBUG ((EFI_D_INFO, "TestClass(D) - 0x%x\n", Member1));
  }
  VOID
  SetNumber (
    IN UINTN Number
    )
  {
    DEBUG ((EFI_D_INFO, "SetNumber - 0x%x\n", Number));
    Member1 = Number;
  }
  UINTN
  GetNumber (
    VOID
    )
  {
    DEBUG ((EFI_D_INFO, "GetNumber - 0x%x\n", Member1));
    return Member1;
  }
};

typedef class TestClass TestClass;

TestClass testClass;
TestClass testClass2;

UINTN  DummySymbol;

EFI_STATUS
EFIAPI
MainEntryPoint (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  )
{
#if 1
  UINTN *Ptr;
  UINTN *Array;

  Ptr = new UINTN;
  Array = new UINTN[100];

  *Ptr = 5;

  delete Ptr;
  delete[] Array;

  Print ((CHAR16 *)L"Number - 0x%x\n", testClass.GetNumber ());
  testClass.SetNumber (2);
  Print ((CHAR16 *)L"Number - 0x%x\n", testClass.GetNumber ());

  Print ((CHAR16 *)L"Number - 0x%x\n", testClass2.GetNumber ());
  testClass2.SetNumber (3);
  Print ((CHAR16 *)L"Number - 0x%x\n", testClass2.GetNumber ());
#endif
  return EFI_SUCCESS;
}
