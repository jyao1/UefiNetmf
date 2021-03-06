////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>
#include <tests.h>
#include "nativesample.h"

//--//

HAL_DECLARE_NULL_HEAP();

void ApplicationEntryPoint()
{
    BOOL result;
   
    GPIO        gpioTest   ( GPIOTestPin );   

    do
    {   
        debug_printf ("gpioTest.Execute\r\n");
        result = gpioTest.Execute  ( STREAM__OUT );

    } while(FALSE); // run only once!

    debug_printf ( "test done\r\n" );
    //while(TRUE);
}

