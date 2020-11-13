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

#include <Base.h>
  
#include <Guid/StatusCodeDataTypeId.h>
#include <Guid/StatusCodeDataTypeDebug.h>

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/ReportStatusCodeLib.h>

#define MAX_DEBUG_MESSAGE_LENGTH  0x100

void debug_printf(char const* Format, ... )
{
  UINT64          Buffer[(EFI_STATUS_CODE_DATA_MAX_SIZE / sizeof (UINT64)) + 1];
  EFI_DEBUG_INFO  *DebugInfo;
  UINTN           TotalSize;
  VA_LIST         VaListMarker;
  BASE_LIST       BaseListMarker;
  CHAR8           *FormatString;
  BOOLEAN         Long;

  //
  // Compute the total size of the record.
  // Note that the passing-in format string and variable parameters will be constructed to 
  // the following layout:
  //
  //         Buffer->|------------------------|
  //                 |         Padding        | 4 bytes
  //      DebugInfo->|------------------------|
  //                 |      EFI_DEBUG_INFO    | sizeof(EFI_DEBUG_INFO)
  // BaseListMarker->|------------------------|
  //                 |           ...          |
  //                 |   variable arguments   | 12 * sizeof (UINT64)
  //                 |           ...          |
  //                 |------------------------|
  //                 |       Format String    |
  //                 |------------------------|<- (UINT8 *)Buffer + sizeof(Buffer)
  //
  TotalSize = 4 + sizeof (EFI_DEBUG_INFO) + 12 * sizeof (UINT64) + AsciiStrSize (Format);

  //
  // If the TotalSize is larger than the maximum record size, then return
  //
  if (TotalSize > sizeof (Buffer)) {
    return;
  }

  //
  // Fill in EFI_DEBUG_INFO
  //
  // Here we skip the first 4 bytes of Buffer, because we must ensure BaseListMarker is
  // 64-bit aligned, otherwise retrieving 64-bit parameter from BaseListMarker will cause
  // exception on IPF. Buffer starts at 64-bit aligned address, so skipping 4 types (sizeof(EFI_DEBUG_INFO))
  // just makes address of BaseListMarker, which follows DebugInfo, 64-bit aligned.
  //
  DebugInfo             = (EFI_DEBUG_INFO *)(Buffer) + 1;
  DebugInfo->ErrorLevel = (UINT32)EFI_D_INFO;
  BaseListMarker        = (BASE_LIST)(DebugInfo + 1);
  FormatString          = (CHAR8 *)((UINT64 *)(DebugInfo + 1) + 12);

  //
  // Copy the Format string into the record
  //
  AsciiStrCpy (FormatString, Format);

  //
  // The first 12 * sizeof (UINT64) bytes following EFI_DEBUG_INFO are for variable arguments
  // of format in DEBUG string, which is followed by the DEBUG format string.
  // Here we will process the variable arguments and pack them in this area.
  //
  VA_START (VaListMarker, Format);
  for (; *Format != '\0'; Format++) {
    //
    // Only format with prefix % is processed.
    //
    if (*Format != '%') {
      continue;
    }
    Long = FALSE;
    //
    // Parse Flags and Width
    //
    for (Format++; TRUE; Format++) {
      if (*Format == '.' || *Format == '-' || *Format == '+' || *Format == ' ') {
        //
        // These characters in format field are omitted.
        //
        continue;
      }
      if (*Format >= '0' && *Format <= '9') {
        //
        // These characters in format field are omitted.
        //
        continue;
      }
      if (*Format == 'L' || *Format == 'l') {
        //
        // 'L" or "l" in format field means the number being printed is a UINT64
        //
        Long = TRUE;
        continue;
      }
      if (*Format == '*') {
        //
        // '*' in format field means the precision of the field is specified by
        // a UINTN argument in the argument list.
        //
        BASE_ARG (BaseListMarker, UINTN) = VA_ARG (VaListMarker, UINTN);
        continue;
      }
      if (*Format == '\0') {
        //
        // Make no output if Format string terminates unexpectedly when
        // looking up for flag, width, precision and type. 
        //
        Format--;
      }
      //
      // When valid argument type detected or format string terminates unexpectedly,
      // the inner loop is done.
      //
      break;
    }
    
    //
    // Pack variable arguments into the storage area following EFI_DEBUG_INFO.
    //
    if ((*Format == 'p') && (sizeof (VOID *) > 4)) {
      Long = TRUE;
    }
    if (*Format == 'p' || *Format == 'X' || *Format == 'x' || *Format == 'd') {
      if (Long) {
        BASE_ARG (BaseListMarker, INT64) = VA_ARG (VaListMarker, INT64);
      } else {
        BASE_ARG (BaseListMarker, int) = VA_ARG (VaListMarker, int);
      }
    } else if (*Format == 's' || *Format == 'S' || *Format == 'a' || *Format == 'g' || *Format == 't') {
      BASE_ARG (BaseListMarker, VOID *) = VA_ARG (VaListMarker, VOID *);
    } else if (*Format == 'c') {
      BASE_ARG (BaseListMarker, UINTN) = VA_ARG (VaListMarker, UINTN);
    } else if (*Format == 'r') {
      BASE_ARG (BaseListMarker, RETURN_STATUS) = VA_ARG (VaListMarker, RETURN_STATUS);
    }

    //
    // If the converted BASE_LIST is larger than the 12 * sizeof (UINT64) allocated bytes, then ASSERT()
    // This indicates that the DEBUG() macro is passing in more argument than can be handled by 
    // the EFI_DEBUG_INFO record
    //
    ASSERT ((CHAR8 *)BaseListMarker <= FormatString);

    //
    // If the converted BASE_LIST is larger than the 12 * sizeof (UINT64) allocated bytes, then return
    //
    if ((CHAR8 *)BaseListMarker > FormatString) {
      VA_END (VaListMarker);
      return;
    }
  }
  VA_END (VaListMarker);

  //
  // Send the DebugInfo record
  //
  REPORT_STATUS_CODE_EX (
    EFI_DEBUG_CODE,
    (EFI_SOFTWARE_DXE_BS_DRIVER | EFI_DC_UNSPECIFIED),
    0,
    0,
    &gEfiStatusCodeDataTypeDebugGuid,
    DebugInfo,
    TotalSize
    );
}

void lcd_printf(char const * format,...)
{
}

#ifdef __cplusplus
}
#endif
