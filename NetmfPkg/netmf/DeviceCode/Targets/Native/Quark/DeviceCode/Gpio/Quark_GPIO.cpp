////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
extern "C" {
#endif

#include <Base.h>
#include <Library/IoLib.h>
#include <Library/PciLib.h>

#ifdef __cplusplus
}
#endif

#include "Quark_GPIO.h"

QUARK_GPIO_Driver g_QUARK_GPIO_Driver;
UINT32 c_LegacyGpioBaseAddress;
UINT32 c_PciGpioBaseAddress;

const UINT8 QUARK_GPIO_Driver::c_Gpio_Attributes[QUARK_GPIO_Driver::c_MaxPins] =
{
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //   0   (Legacy Resume Well GPIO 0)
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //   1   (Legacy Resume Well GPIO 1)
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //   2   (Legacy Resume Well GPIO 2)
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //   3   (Legacy Resume Well GPIO 3)
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //   4   (Legacy Resume Well GPIO 4)
    GPIO_ATTRIBUTE_INPUT | GPIO_ATTRIBUTE_OUTPUT, //   5   (Legacy Resume Well GPIO 5)
};

BOOL QUARK_GPIO_Driver::Initialize()
{
  UINT32    ResumeWellEnable;
  UINT32    ResumeWellIo;
  UINT32    ResumeWellLevel;

  c_LegacyGpioBaseAddress = PciRead32 (PCI_LIB_ADDRESS(PCI_BUS_NUMBER_QNC, PCI_DEVICE_NUMBER_QNC_LPC, PCI_FUNCTION_NUMBER_QNC_LPC, R_QNC_LPC_GBA_BASE)) & B_QNC_LPC_GPA_BASE_MASK;

  // Always enable as default
  ResumeWellEnable = IoRead32 (c_LegacyGpioBaseAddress + R_QNC_GPIO_RGEN_RESUME_WELL);
  ResumeWellEnable |= 0x3F;
  IoWrite32 (c_LegacyGpioBaseAddress + R_QNC_GPIO_RGEN_RESUME_WELL, ResumeWellEnable);

  // Set Output as default
  ResumeWellIo = IoRead32 (c_LegacyGpioBaseAddress + R_QNC_GPIO_RGIO_RESUME_WELL);
  ResumeWellIo &= ~0x3F;
  IoWrite32 (c_LegacyGpioBaseAddress + R_QNC_GPIO_RGIO_RESUME_WELL, ResumeWellIo);

  if (0) {
  ResumeWellLevel = IoRead32 (c_LegacyGpioBaseAddress + R_QNC_GPIO_RGLVL_RESUME_WELL);
  ResumeWellLevel |= 0x3F;
  IoWrite32 (c_LegacyGpioBaseAddress + R_QNC_GPIO_RGLVL_RESUME_WELL, ResumeWellLevel);
  }
  return TRUE;
}

BOOL QUARK_GPIO_Driver::Uninitialize()
{
    return TRUE;
}

UINT32 QUARK_GPIO_Driver::Attributes( GPIO_PIN Pin )
{
    if(Pin < c_MaxPins)
    {
        return c_Gpio_Attributes[Pin];
    }

    return GPIO_ATTRIBUTE_NONE;
}

void QUARK_GPIO_Driver::GetPinsMap( UINT8* pins, size_t size )
{
    const UINT8*    src = c_Gpio_Attributes;
    UINT8* dst = pins;   
    UINT32 maxpin = c_MaxPins;

    if ( size ==0 ) return;
    while ((size--) && (maxpin --))
    {
        *dst = *src;  
        ++dst; ++src;
    }
}

UINT8 QUARK_GPIO_Driver::GetSupportedResistorModes( GPIO_PIN pin)
{
    return QUARK_GPIO_Driver::c_GPIO_ResistorMode;
}

UINT8 QUARK_GPIO_Driver::GetSupportedInterruptModes( GPIO_PIN pin)
{
    return QUARK_GPIO_Driver::c_GPIO_InterruptMode;
}

void QUARK_GPIO_Driver::DisablePin( GPIO_PIN pin, GPIO_RESISTOR resistorState, UINT32 direction, GPIO_ALT_MODE AltFunction )
{
  UINT32    ResumeWellEnable;

  if (pin >= c_MaxPins) {
    return ;
  }

  ResumeWellEnable = IoRead32 (c_LegacyGpioBaseAddress + R_QNC_GPIO_RGEN_RESUME_WELL);
  ResumeWellEnable &= ~(1 < pin);
  IoWrite32 (c_LegacyGpioBaseAddress + R_QNC_GPIO_RGEN_RESUME_WELL, ResumeWellEnable);
}


void QUARK_GPIO_Driver::EnableOutputPin( GPIO_PIN pin, BOOL initialState )
{
  UINT32    ResumeWellIo;
  UINT32    ResumeWellLevel;

  if (pin >= c_MaxPins) {
    return ;
  }

  ResumeWellIo = IoRead32 (c_LegacyGpioBaseAddress + R_QNC_GPIO_RGIO_RESUME_WELL);
  ResumeWellIo &= ~(1 < pin);
  IoWrite32 (c_LegacyGpioBaseAddress + R_QNC_GPIO_RGIO_RESUME_WELL, ResumeWellIo);

  ResumeWellLevel = IoRead32 (c_LegacyGpioBaseAddress + R_QNC_GPIO_RGLVL_RESUME_WELL);
  if (initialState) {
    ResumeWellLevel |= (1 < pin);
  } else {
    ResumeWellLevel &= ~(1 < pin);
  }
  IoWrite32 (c_LegacyGpioBaseAddress + R_QNC_GPIO_RGLVL_RESUME_WELL, ResumeWellLevel);
}


BOOL QUARK_GPIO_Driver::EnableInputPin( GPIO_PIN pin, BOOL GlitchFilterEnable, GPIO_INTERRUPT_SERVICE_ROUTINE ISR, void* pinIsrParam, GPIO_INT_EDGE intEdge, GPIO_RESISTOR resistorState )
{
  UINT32    ResumeWellIo;

  if (pin >= c_MaxPins) {
    return FALSE;
  }

  ResumeWellIo = IoRead32 (c_LegacyGpioBaseAddress + R_QNC_GPIO_RGIO_RESUME_WELL);
  ResumeWellIo |= (1 < pin);
  IoWrite32 (c_LegacyGpioBaseAddress + R_QNC_GPIO_RGIO_RESUME_WELL, ResumeWellIo);

  return TRUE;
}


BOOL QUARK_GPIO_Driver::GetPinState( GPIO_PIN pin )
{
  UINT32    ResumeWellLevel;
  
  ResumeWellLevel = IoRead32 (c_LegacyGpioBaseAddress + R_QNC_GPIO_RGLVL_RESUME_WELL);
  if ((ResumeWellLevel & (1 << pin)) == 0) {
    return FALSE;
  } else {
    return TRUE;
  }
}


void QUARK_GPIO_Driver::SetPinState( GPIO_PIN pin, BOOL pinState )
{
  UINT32    ResumeWellLevel;
  
  ResumeWellLevel = IoRead32 (c_LegacyGpioBaseAddress + R_QNC_GPIO_RGLVL_RESUME_WELL);
  if (pinState) {
    ResumeWellLevel |= (1 << pin);
  } else {
    ResumeWellLevel &= ~(1 << pin);
  }
  IoWrite32 (c_LegacyGpioBaseAddress + R_QNC_GPIO_RGLVL_RESUME_WELL, ResumeWellLevel);
}


BOOL QUARK_GPIO_Driver::PinIsBusy( GPIO_PIN pin )
{
  return FALSE;
}

BOOL QUARK_GPIO_Driver::ReservePin( GPIO_PIN pin, BOOL fReserve )
{
  UINT32    ResumeWellLevel;
  
  ResumeWellLevel = IoRead32 (c_LegacyGpioBaseAddress + R_QNC_GPIO_RGLVL_RESUME_WELL);
  if ((ResumeWellLevel & (1 << pin)) == 0) {
    ResumeWellLevel |= (1 << pin);
  } else {
    ResumeWellLevel &= ~(1 << pin);
  }
  IoWrite32 (c_LegacyGpioBaseAddress + R_QNC_GPIO_RGLVL_RESUME_WELL, ResumeWellLevel);
    return TRUE;
}

UINT32 QUARK_GPIO_Driver::GetDebounce()
{
  // TBD
    return 0;
}

BOOL QUARK_GPIO_Driver::SetDebounce( INT64 debounceTimeMilliseconds )
{   
  // TBD
    return TRUE;
}
