////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>

//--//

HAL_SYSTEM_CONFIG HalSystemConfig =
{
    { TRUE },                                       // HAL_DRIVER_CONFIG_HEADER Header;
            
    //--//

    {                                               // UINT32      DebuggerPorts[MAX_DEBUGGERS];
        DEBUGGER_PORT,
    },

    {
        MESSAGING_PORT,                             // UINT32      MessagingPorts[MAX_MESSAGING];
    },

    //--//

    DEBUG_TEXT_PORT,                                // UINT32  DebugTextPort;
    115200,                                         // UINT32  USART_DefaultBaudRate;
    STDIO,                                          // FILE*   stdio;

    { SRAM1_MEMORY_Base, SRAM1_MEMORY_Size },       // HAL_SYSTEM_MEMORY_CONFIG RAM1;
    { FLASH_MEMORY_Base, FLASH_MEMORY_Size },       // HAL_SYSTEM_MEMORY_CONFIG FLASH;
};

OEM_MODEL_SKU OEM_Model_SKU = {
        0xFF,       // UINT8   OEM;
        0,          // UINT8   Model;
        0xFFFF,     // UINT16  SKU;
};
