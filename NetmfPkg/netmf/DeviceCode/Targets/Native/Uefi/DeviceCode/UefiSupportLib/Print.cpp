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

#ifdef __cplusplus
}
#endif

int hal_vsnprintf( char* buffer, unsigned int len, const char* format, VA_LIST arg )
{
  UINTN Index;
  UINTN Len;
  char  str[512];

  //
  // convert %s -> %a
  //
  AsciiStrnCpy (str, format, sizeof(str));
  Len = AsciiStrLen (str);
  for (Index = 0; Index < Len; Index++) {
    if (str[Index] == '%') {
      char *Ptr;
      Ptr = &str[Index+1];
      if (*Ptr >= '0' && *Ptr <= '9') {
        Ptr++;
      }
      if (*Ptr >= '0' && *Ptr <= '9') {
        Ptr++;
      }
      if (*Ptr == 's') {
        *Ptr = 'a';
      }
    }
  }
  return (int)AsciiVSPrint (buffer, len, str, arg);
}

int hal_snprintf_float( char* buffer, unsigned int len, const char* format, INT32 f )
{
  return -1;
}

int hal_snprintf_double( char* buffer, unsigned int len, const char* format, INT64& d )
{
  return -1;
}

