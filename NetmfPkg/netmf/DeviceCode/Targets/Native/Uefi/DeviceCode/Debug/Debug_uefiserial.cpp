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
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PrintLib.h>
#include <Library/SerialPortLib.h>

#define MAX_DEBUG_MESSAGE_LENGTH  0x100

void debug_printf(char const* Format, ... )
{
  CHAR8    Buffer[MAX_DEBUG_MESSAGE_LENGTH];
  VA_LIST  Marker;

  //
  // Convert the DEBUG() message to an ASCII String
  //
  VA_START (Marker, Format);
  AsciiVSPrint (Buffer, sizeof (Buffer), Format, Marker);
  VA_END (Marker);

  //
  // Send the print string to a Serial Port 
  //
  SerialPortWrite ((UINT8 *)Buffer, AsciiStrLen (Buffer));
}

void lcd_printf(char const * Format,...)
{
}

#ifdef __cplusplus
}
#endif
