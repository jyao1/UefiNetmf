/** @file

  Copyright (c) 2014, Intel Corporation. All rights reserved.<BR>
  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

void ApplicationEntryPoint();
void HAL_Initialize();
void HAL_Uninitialize();
BOOLEAN HAL_Time_Initialize ();
BOOLEAN HAL_Time_Uninitialize ();

int HeapBegin;
int HeapEnd;

#ifdef __cplusplus
extern "C" {
#endif
  
#include <Base.h>
#include <Library/MemoryAllocationLib.h>

#include <Uefi.h>

#define MAX_HEAP_SIZE  SIZE_16MB

EFI_STATUS
EFIAPI
UefiMain (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  int         Alloc;

  //
  //
  //
  Alloc = (int)AllocatePool (MAX_HEAP_SIZE);
  if (Alloc == 0) {
    return EFI_SUCCESS;
  }
  HeapBegin = Alloc;
  HeapEnd = HeapBegin + MAX_HEAP_SIZE;
  
  HAL_Time_Initialize();
  HAL_Initialize();

  ApplicationEntryPoint ();

  HAL_Uninitialize();
  HAL_Time_Uninitialize();

  return EFI_SUCCESS;
}

#ifdef __cplusplus
}
#endif
