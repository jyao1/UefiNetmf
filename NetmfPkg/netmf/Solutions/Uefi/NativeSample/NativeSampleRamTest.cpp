////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>
#include <tests.h>
#include "nativesample.h"

//--//

HAL_DECLARE_NULL_HEAP();

//UINT8  TempRam[0x10000];

void ApplicationEntryPoint()
{
    BOOL result;
   
//    RAM         RamTest    ( (UINT32*)TempRam, (UINT32)sizeof(TempRam), (ENDIAN_TYPE)ENDIANESS, BUS_WIDTH );
    RAM         RamTest    ( (UINT32*)RAMTestBase, (UINT32)RAMTestSize, (ENDIAN_TYPE)ENDIANESS, BUS_WIDTH );

    do
    {   
        debug_printf ("RamTest.Execute\r\n");
        result = RamTest.Execute   ( STREAM__OUT );       
    } while(FALSE); // run only once!

    //while(TRUE);
}

