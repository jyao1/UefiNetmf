/** @file

  Copyright (c) 2014, Intel Corporation. All rights reserved.<BR>
  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include <PiDxe.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Protocol/SimpleFileSystem.h>
#include <Guid/FileInfo.h>

/**
  Function gets the file information from an open file descriptor, and stores it
  in a buffer allocated from pool.

  @param[in] FHand                A file handle

  @retval UINTN                  File Size
**/
UINTN
GetFileLength (
  IN EFI_FILE_HANDLE              FHand
  )
{
  EFI_STATUS                      Status;
  EFI_FILE_INFO                   *Buffer;
  UINTN                           BufferSize;

  ///
  /// Initialize for GrowBuffer loop
  ///
  BufferSize  = SIZE_OF_EFI_FILE_INFO + 200;

  ///
  /// Call the real function
  ///
  do {
    Buffer = AllocateZeroPool (BufferSize);
    Status = FHand->GetInfo (
                      FHand,
                      &gEfiFileInfoGuid,
                      &BufferSize,
                      Buffer
                      );
    DEBUG ((DEBUG_INFO, "GetInfo %r", Status));
    DEBUG ((DEBUG_INFO, " FileSize 0x%x Buffer 0x%x\n", BufferSize, Buffer));
    if (!EFI_ERROR (Status)) {
      break;
    }
    if (Status != EFI_BUFFER_TOO_SMALL) {
      FreePool (Buffer);
      break;
    }
    FreePool (Buffer);
  } while (TRUE);

  if (Buffer) {
    BufferSize = (UINTN)Buffer->FileSize;
    FreePool (Buffer);
  } else {
  	BufferSize = 0;
  }
  return BufferSize;
}

EFI_STATUS
ReadFile (
  IN CHAR16 *FileName,
  OUT VOID  **FileBuffer,
  OUT UINTN *FileBufferSize
  )
{
  EFI_STATUS                          Status ;
  EFI_HANDLE                          *SimpleFileSystemHandles;
  UINTN                               NumberSimpleFileSystemHandles;
  UINTN                               Index;
  EFI_SIMPLE_FILE_SYSTEM_PROTOCOL     *Fs;
  EFI_FILE_HANDLE                     Root;
  EFI_FILE_HANDLE                     FileHandle;
  UINTN                               BufferSize;
  VOID                                *Buffer;

  DEBUG ((DEBUG_INFO, "ReadFile entry\n"));
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiSimpleFileSystemProtocolGuid,
                  NULL,
                  &NumberSimpleFileSystemHandles,
                  &SimpleFileSystemHandles
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  for (Index = 0;Index < NumberSimpleFileSystemHandles;Index++) {
    Status = gBS->HandleProtocol (SimpleFileSystemHandles[Index], &gEfiSimpleFileSystemProtocolGuid, (VOID **)&Fs);
    if (EFI_ERROR (Status)) {
      continue;
    }
    
    Status = Fs->OpenVolume (Fs, &Root);
    if (EFI_ERROR (Status)) {
      continue;
    }

    FileHandle = NULL;
    Status = Root->Open (Root, &FileHandle, FileName, EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE, 0);
    DEBUG ((DEBUG_INFO, "Find %S %r\n", FileName, Status));
    if (EFI_ERROR (Status)) {
      Root->Close (Root);
      continue;
    }
    BufferSize = GetFileLength (FileHandle);
    if (BufferSize == 0) {
      FileHandle->Close (FileHandle);
      DEBUG ((DEBUG_INFO, "FileSize is 0\n"));
      Root->Close (Root);
      continue;
    }
    Buffer = AllocateZeroPool (BufferSize);
    DEBUG ((DEBUG_INFO, "BufferSize:0x%x Buffer:0x%x\n", BufferSize, Buffer));
    if (Buffer == NULL) {
      DEBUG ((DEBUG_INFO, "AllocateZeroPool:Fail\n"));
      FileHandle->Close (FileHandle);
      Root->Close (Root);
      continue;
    }
    Status = FileHandle->Read (FileHandle, &BufferSize, Buffer);
    DEBUG ((DEBUG_INFO, "Read %S %r\n", FileName, Status));
    DEBUG ((DEBUG_INFO, "BufferSize:0x%x Buffer:0x%x\n", BufferSize, Buffer));

    Root->Close (Root);

    //
    // Image read correctly
    //
    if (!EFI_ERROR (Status)) {
      *FileBuffer = Buffer;
      *FileBufferSize = BufferSize;
      DEBUG ((DEBUG_INFO, "ReadFile exit\n"));
      return EFI_SUCCESS;
    }

    //
    // Something wrong
    //
    FreePool (Buffer);
  }

  DEBUG ((DEBUG_INFO, "ReadFile exit\n"));
  return EFI_NOT_FOUND;
}
