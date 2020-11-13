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
   
    RAM         RamTest    ( (UINT32*)RAMTestBase, (UINT32)RAMTestSize, (ENDIAN_TYPE)ENDIANESS, BUS_WIDTH );
    TimedEvents eventsTest;
    UART        usartTest  ( COMTestPort, 115200, USART_PARITY_NONE, 8, USART_STOP_BITS_ONE, USART_FLOW_NONE );
    GPIO        gpioTest   ( GPIOTestPin );   
//    SPI         spiTest    ( SPIChipSelect, SPIModule, g_EEPROM_STM95x );
    Timers      timersTest ( DisplayInterval, TimerDuration );

    do
    {   
        debug_printf ("RamTest.Execute\r\n");
        result = RamTest.Execute   ( STREAM__OUT );       

        debug_printf ("eventsTest.Execute\r\n");
        result = eventsTest.Execute( STREAM__OUT );

        debug_printf ("usartTest.Execute\r\n");
        result = usartTest.Execute ( STREAM__OUT );

        debug_printf ("gpioTest.Execute\r\n");
        result = gpioTest.Execute  ( STREAM__OUT );

//        debug_printf ("spiTest.Execute\r\n");
//        result = spiTest.Execute   ( STREAM__OUT );

        debug_printf ("timersTest.Execute\r\n");
        result = timersTest.Execute( STREAM__OUT );

    } while(FALSE); // run only once!

    debug_printf ( "test done\r\n" );
    while(TRUE);
}

