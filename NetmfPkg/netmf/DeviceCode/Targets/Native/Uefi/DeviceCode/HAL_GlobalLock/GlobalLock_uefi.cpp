////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
extern "C" {
#endif

#include <Base.h>

#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/Cpu.h>

void debug_printf(char const* Format, ... );

#ifdef __cplusplus
}
#endif

//#define _DEBUG_SMART_IRQ_  1

#include "UefiSupport.h"
#include <tinyhal.h>

#define DISABLE_STATE   0xF
#define ENABLE_STATE    0x0

UINT32      gState;
EFI_TPL     gTpl;

UINT32
get_imask (
  VOID
  )
{
  EFI_TPL     Tpl;

  Tpl = gBS->RaiseTPL (TPL_HIGH_LEVEL);
  gBS->RestoreTPL (Tpl);

  if (Tpl != TPL_HIGH_LEVEL) {
    return ENABLE_STATE;
  } else {
    return DISABLE_STATE;
  }
}

VOID
set_imask (
  IN UINT32 imask
  )
{
  EFI_TPL     Tpl;

#ifdef _DEBUG_SMART_IRQ_
  debug_printf ("SmartPtr_IRQ::set_imask(0x%x)\r\n", imask);
#endif

  if (imask == DISABLE_STATE) {
    Tpl = gBS->RaiseTPL (TPL_HIGH_LEVEL);
    gTpl = Tpl;
  } else {
    Tpl = gTpl;
    gBS->RestoreTPL (Tpl);
  }
}

/**
  Record global interrupt state to local
  Disable global interrupt state
**/
SmartPtr_IRQ::SmartPtr_IRQ(void* context)
{ 
    Disable(); 
#ifdef _DEBUG_SMART_IRQ_
    debug_printf ("SmartPtr_IRQ::SmartPtr_IRQ (0x%x)\r\n", m_state);
#endif
}

/**
  Restore local interrupt state to global
**/
SmartPtr_IRQ::~SmartPtr_IRQ() 
{ 
    Restore();
#ifdef _DEBUG_SMART_IRQ_
    debug_printf ("SmartPtr_IRQ::~SmartPtr_IRQ (0x%x)\r\n", m_state);
#endif
}

/**
  Return if local interrupt state is disabled.
**/
BOOL SmartPtr_IRQ::WasDisabled()
{
    return (m_state == DISABLE_STATE);
}

/**
  If local interrupt state is disable, disable interrupt.

  NOTE: This logic is updated to always disable interrupt.
**/
void SmartPtr_IRQ::Acquire()
{
#if 0
    UINT32 Cp = m_state;
    if(Cp  == DISABLE_STATE)
    {
        Disable();
    }
#else
    set_imask(DISABLE_STATE);
#endif

#ifdef _DEBUG_SMART_IRQ_
    debug_printf ("SmartPtr_IRQ::Acquire (0x%x)\r\n", m_state);
#endif
}

/**
  Restore local interrupt to global, if it is enabled.
**/
void SmartPtr_IRQ::Release()
{
    UINT32 Cp = m_state;

    if(Cp != DISABLE_STATE) 
    {
        set_imask(Cp);
    }
#ifdef _DEBUG_SMART_IRQ_
    debug_printf ("SmartPtr_IRQ::Release (0x%x)\r\n", m_state);
#endif
}

/**
  Restore local interrupt state to global, if local interrupt is enabled.
  Then disable global interrupt.
**/
void SmartPtr_IRQ::Probe()
{
    UINT32 Cp = m_state;

    if(Cp != DISABLE_STATE)
    {
        set_imask(Cp);
        set_imask(DISABLE_STATE);
    }
#ifdef _DEBUG_SMART_IRQ_
    debug_printf ("SmartPtr_IRQ::Probe (0x%x)\r\n", m_state);
#endif
}

/**
  Get global interrupt interrupt state
**/
BOOL SmartPtr_IRQ::GetState(void* context)
{
    UINT32 Cp;
	
    Cp = get_imask();

    //debug_printf ("SmartPtr_IRQ::GetState - 0x%x\r\n", State);

    return (Cp != DISABLE_STATE);
}

/**
  Force disable global interrupt
**/
BOOL SmartPtr_IRQ::ForceDisabled(void* context)
{
    UINT32 Cp;

    Cp = get_imask();
    set_imask(DISABLE_STATE);

#ifdef _DEBUG_SMART_IRQ_
    debug_printf ("SmartPtr_IRQ::ForceDisabled\r\n");
#endif

    return (Cp != DISABLE_STATE);
}

/**
  Force enable global interrupt
**/
BOOL SmartPtr_IRQ::ForceEnabled(void* context)
{
    UINT32 Cp;

    Cp = get_imask();
    
    set_imask(ENABLE_STATE);

#ifdef _DEBUG_SMART_IRQ_
    debug_printf ("SmartPtr_IRQ::ForceEnabled\r\n");
#endif

    return (Cp != DISABLE_STATE);
}

/**
  Record global interrupt to local
  Disable global interrupt
**/
void SmartPtr_IRQ::Disable()
{
    UINT32 Cp;

    Cp = get_imask();

    set_imask(DISABLE_STATE);

    m_state = Cp;
#ifdef _DEBUG_SMART_IRQ_
    debug_printf ("SmartPtr_IRQ::Disable (0x%x)\r\n", m_state);
#endif
}

/**
  Restore local interrupt state to global, if local interrupt is enabled.
**/
void SmartPtr_IRQ::Restore()
{
    UINT32 Cp = m_state;

    if(Cp != DISABLE_STATE)
    {
        set_imask(Cp);
    }
#ifdef _DEBUG_SMART_IRQ_
    debug_printf ("SmartPtr_IRQ::Restore (0x%x)\r\n", m_state);
#endif
}

