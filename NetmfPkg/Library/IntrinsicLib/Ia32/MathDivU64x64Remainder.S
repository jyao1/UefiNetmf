#------------------------------------------------------------------------------
#
#  Copyright (c) 2014, Intel Corporation. All rights reserved.<BR>
#  This program and the accompanying materials
#  are licensed and made available under the terms and conditions of the BSD License
#  which accompanies this distribution.  The full text of the license may be found at
#  http://opensource.org/licenses/bsd-license.php
#
#  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
#  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
#
# Module Name:
#
#   MathDivU64x64Remainder.S
#
# Abstract:
#
#   64-bit Math Worker Function.
#   Divides a 64-bit unsigned value with a 64-bit unsigned value and returns
#   a 64-bit unsigned result.
#
#------------------------------------------------------------------------------

    .686:
    .code:

ASM_GLOBAL ASM_PFX(__udivmoddi4), ASM_PFX(DivU64x64Remainder)

#------------------------------------------------------------------------------
#
# void __cdecl __udivmoddi4 (void)
#
#------------------------------------------------------------------------------
ASM_PFX(__udivmoddi4):
    # Original local stack when calling __udivmoddi4
    #               -----------------
    #               |               |
    #               |---------------|
    #               |               |
    #               |--  Divisor  --|
    #               |               |
    #               |---------------|
    #               |               |
    #               |--  Dividend --|
    #               |               |
    #               |---------------|
    #               |  ReturnAddr** |
    #       ESP---->|---------------|
    #

    #
    # Call native DivU64x64Remainder of BaseLib
    #
    jmp     ASM_PFX(DivU64x64Remainder)

    ret     $16
