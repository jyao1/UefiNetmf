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
#include <Library/UefiLib.h>

#include <Uefi.h>

#define MAX_HEAP_SIZE  SIZE_16MB

typedef struct {
  char     *Name;
  wchar_t  *FileName;
  char     *Start;
  char     *End;
} ASSEMBLY_FILE_STRUCT;

ASSEMBLY_FILE_STRUCT mAssemblyFileStruct[5] = {
  {"main file",             L"Microsoft.SPOT.HelloWorld.pe", (char *)NULL, (char *)NULL},
  {"mscorlib",              L"mscorlib.pe",                  (char *)NULL, (char *)NULL},
  {"Microsoft.SPOT.Native", L"Microsoft.SPOT.Native.pe",     (char *)NULL, (char *)NULL},
  {"support file",          (wchar_t *)NULL,                 (char *)NULL, (char *)NULL},
  {(char *)NULL,            (wchar_t *)NULL,                 (char *)NULL, (char *)NULL},
};

EFI_STATUS
ReadFile (
  IN CHAR16 *FileName,
  OUT VOID  **FileBuffer,
  OUT UINTN *FileBufferSize
  );

EFI_STATUS
EFIAPI
UefiMain (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  UINTN       Index;
  int         Alloc;
  VOID        *FileBuffer;
  UINTN       FileBufferSize;
  EFI_STATUS  Status;

  Alloc = (int)AllocatePool (MAX_HEAP_SIZE);
  if (Alloc == 0) {
    return EFI_SUCCESS;
  }
  HeapBegin = Alloc;
  HeapEnd = HeapBegin + MAX_HEAP_SIZE;

  for (Index = 0; ; Index++) {
    if (mAssemblyFileStruct[Index].Name == NULL) {
      break;
    }
    Status = ReadFile ((CHAR16 *)mAssemblyFileStruct[Index].FileName, &FileBuffer, &FileBufferSize);
    if (!EFI_ERROR (Status)) {
      mAssemblyFileStruct[Index].Start = (char *)FileBuffer;
      mAssemblyFileStruct[Index].End = mAssemblyFileStruct[Index].Start + FileBufferSize;
    }
  }
  
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
