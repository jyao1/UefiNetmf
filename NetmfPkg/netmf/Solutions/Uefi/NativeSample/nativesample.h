////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//-- Stream --//

#define  STREAM__OUT        STREAM__DEFAULT_DEBUG_CHANNEL

//--  RamTest --//

#define  BUS_WIDTH         0x00008000
#define  RAMTestBase       0x08400000
#define  RAMTestSize       0x00010000
#define  ENDIANESS         LE_ENDIAN

//--  Serial Port  --//

#define  COMTestPort       ConvertCOM_ComPort(COM1)

//--  GPIO --//
    
//#define  GPIOTestPin       GPIO_PIN_NONE
#define  LEGACY_GPIO_FOR_LED_BLINK              4
#define  GPIOTestPin       LEGACY_GPIO_FOR_LED_BLINK

//-- SPI --//

#define  SPIChipSelect     MC9328MXL_GPIO::c_Port_B_18  // P6_Pin_15 - 20 pin connector
#define  SPIModule         MC9328MXL_SPI::c_SPI1

//-- Timer --//

#define  DisplayInterval    5
#define  TimerDuration      30

//--//

