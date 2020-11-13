/** @file
  Root include file to support building.

Copyright (c) 2014, Intel Corporation. All rights reserved.<BR>
This program and the accompanying materials
are licensed and made available under the terms and conditions of the BSD License
which accompanies this distribution.  The full text of the license may be found at
http://opensource.org/licenses/bsd-license.php

THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef __UEFI_SUPPORT_H
#define __UEFI_SUPPORT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <Base.h>

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>

VOID
EFIAPI
DebugAssert (
  IN CONST CHAR8  *FileName,
  IN UINTN        LineNumber,
  IN CONST CHAR8  *Description
  );

#undef NULL
#define NULL 0

#define offsetof(type, member) ( (int) & ((type*)0) -> member )

typedef UINTN          size_t;

typedef unsigned char BOOL;

typedef char               CHAR;
typedef char*              LPSTR;
typedef const char*        LPCSTR;

typedef wchar_t            WCHAR;
typedef WCHAR*             LPWSTR;
typedef const WCHAR*       LPCWSTR;

typedef unsigned char      BYTE;
typedef unsigned char*     PBYTE;

typedef unsigned short     WORD;
typedef unsigned long      DWORD;

typedef CHAR8 *va_list;
#define va_start(Marker, Parameter) (Marker = (va_list) ((UINTN) & (Parameter) + _INT_SIZE_OF (Parameter)))
#define va_arg(Marker, TYPE)   (*(TYPE *) ((Marker += _INT_SIZE_OF (TYPE)) - _INT_SIZE_OF (TYPE)))
#define va_end(Marker)      (Marker = (va_list) 0)

void * memset (void *dest, unsigned char ch, unsigned int count);
void * memcpy (void *dest, const void *src, unsigned int count);
int memcmp(const void* s1, const void* s2, unsigned int n);
void * memmove (void *dest, const void *src, unsigned int count);
char * strcpy (char* s1, const char* s2);
char * strncpy (char* s1, const char* s2, unsigned int n);
int strcmp(const char* s1, const char* s2);
int strncmp (const char* s1, const char* s2, unsigned int n);
char *strchr(char *string, unsigned char c); 

void           *malloc     (size_t);
void           *realloc    (void *, size_t);
void           free        (void *);

void *bsearch(const void *key, const void *base, size_t num, size_t width, int (*compare)(const void *, const void *)) ;

#define __func__  __FUNCTION__

#ifdef __cplusplus
}
#endif

#endif
