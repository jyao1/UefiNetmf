////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _QUARK_GPIO_H_
#define _QUARK_GPIO_H_

typedef BOOLEAN BOOL;
typedef UINTN   size_t;
typedef UINT32  GPIO_PIN;
#include <CPU_GPIO_decl.h>

#define PCI_BUS_NUMBER_QNC              0x00
#define PCI_DEVICE_NUMBER_QNC_LPC       31
#define PCI_FUNCTION_NUMBER_QNC_LPC     0

#define R_QNC_LPC_GBA_BASE              0x44
#define   B_QNC_LPC_GPA_BASE_MASK         0x0000FFC0
//
// GPIO register offsets from GBA - "GPIO" (D31:F0:R44h)
//        Suggested Value for GBA = 0x1080
//
#define R_QNC_GPIO_CGEN_CORE_WELL       0x00
#define R_QNC_GPIO_CGIO_CORE_WELL       0x04
#define R_QNC_GPIO_CGLVL_CORE_WELL      0x08
#define R_QNC_GPIO_CGTPE_CORE_WELL      0x0C   // Core well GPIO Trigger Positive Edge Enable
#define R_QNC_GPIO_CGTNE_CORE_WELL      0x10   // Core well GPIO Trigger Negative Edge Enable
#define R_QNC_GPIO_CGGPE_CORE_WELL      0x14   // Core well GPIO GPE Enable
#define R_QNC_GPIO_CGSMI_CORE_WELL      0x18   // Core well GPIO SMI Enable
#define R_QNC_GPIO_CGTS_CORE_WELL       0x1C   // Core well GPIO Trigger Status
#define R_QNC_GPIO_RGEN_RESUME_WELL     0x20
#define R_QNC_GPIO_RGIO_RESUME_WELL     0x24
#define R_QNC_GPIO_RGLVL_RESUME_WELL    0x28
#define R_QNC_GPIO_RGTPE_RESUME_WELL    0x2C   // Resume well GPIO Trigger Positive Edge Enable
#define R_QNC_GPIO_RGTNE_RESUME_WELL    0x30   // Resume well GPIO Trigger Negative Edge Enable
#define R_QNC_GPIO_RGGPE_RESUME_WELL    0x34   // Resume well GPIO GPE Enable
#define R_QNC_GPIO_RGSMI_RESUME_WELL    0x38   // Resume well GPIO SMI Enable
#define R_QNC_GPIO_RGTS_RESUME_WELL     0x3C   // Resume well GPIO Trigger Status
#define R_QNC_GPIO_CNMIEN_CORE_WELL     0x40   // Core well GPIO NMI Enable
#define R_QNC_GPIO_RNMIEN_RESUME_WELL   0x44   // Resume well GPIO NMI Enable

extern UINT32 c_LegacyGpioBaseAddress;
extern UINT32 c_PciGpioBaseAddress;

typedef struct QUARK_GPIO_Driver
{
    static const UINT32 c_MaxPins =  6;
    static const UINT8 c_Gpio_Attributes[c_MaxPins];
    
    // this applies to all GPIO pins
    static const UINT8 c_NumberofResistorMode = 0;
    static const UINT8 c_GPIO_ResistorMode=0;

    static const UINT8 c_NumberofInterruptMode = 0;
    static const UINT8 c_GPIO_InterruptMode=0; 

    static const UINT32 c_Alt_Max_Mode       = 0;

    static BOOL Initialize  ();

    static BOOL Uninitialize();

    static UINT32 Attributes( GPIO_PIN Pin );

    static void DisablePin( GPIO_PIN Pin, GPIO_RESISTOR ResistorState, UINT32 direction, GPIO_ALT_MODE AltFunction );

    static void EnableOutputPin( GPIO_PIN Pin, BOOL InitialState );

    static BOOL EnableInputPin( GPIO_PIN Pin, BOOL GlitchFilterEnable, GPIO_INTERRUPT_SERVICE_ROUTINE ISR, void* ISR_Param, GPIO_INT_EDGE IntEdge, GPIO_RESISTOR ResistorState );

    static BOOL GetPinState( GPIO_PIN Pin );

    static void SetPinState( GPIO_PIN Pin, BOOL PinState );

    static BOOL PinIsBusy( GPIO_PIN Pin );

    static BOOL ReservePin( GPIO_PIN Pin, BOOL fReserve );

    static UINT32 GetDebounce();

    static BOOL SetDebounce( INT64 debounceTimeMilliseconds );

    static void GetPinsMap( UINT8* pins, size_t size );
    
    static UINT8 GetSupportedResistorModes( GPIO_PIN pin );

    static UINT8 GetSupportedInterruptModes( GPIO_PIN pin );
} QUARK_GPIO_Driver;

#endif
