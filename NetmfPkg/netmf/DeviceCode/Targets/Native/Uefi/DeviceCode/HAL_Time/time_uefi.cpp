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

VOID
EFIAPI
CLRUefiTimerHandler (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  );

#ifdef __cplusplus
}
#endif

#include "UefiSupport.h"
#include <tinyhal.h>

// in femtoseconds (10^-15)
#define UEFI_TIMER_PERIOD_UNIT       1000000000000000ull
#define MILLI_SECONDS_PER_1_SECCOND  1000
#define MICRO_SECONDS_PER_1_SECCOND  1000000

#define CRL_TIMER_INTERVAL_IN_MICRO_SECONDS  55

STATIC EFI_CPU_ARCH_PROTOCOL *mCpuAp;
UINT64                mTimerPeriod;
EFI_EVENT             mUefiTimerEvent;
UINT64                mNextCompareValue = (UINT64)-1;

EFI_TPL   mAppTpl;

BOOL HAL_Time_Initialize()
{
    EFI_STATUS  Status;
    UINT64      TimerValue;
    
    mAppTpl = gBS->RaiseTPL (TPL_HIGH_LEVEL);
    gBS->RestoreTPL (mAppTpl);

    Status = gBS->LocateProtocol (
                    &gEfiCpuArchProtocolGuid,
                    NULL,
                    (VOID **)&mCpuAp
                    );

    Status = mCpuAp->GetTimerValue (
                       mCpuAp,
                       0,
                       &TimerValue,
                       &mTimerPeriod
                       );
    debug_printf ("HAL_Time_Initialize - TimerValue - %ld, TimerPeriod - %ld\r\n", TimerValue, mTimerPeriod);

    Status = gBS->CreateEvent (
                    EVT_TIMER | EVT_NOTIFY_SIGNAL,
                    TPL_CALLBACK,
                    CLRUefiTimerHandler,
                    NULL,
                    &mUefiTimerEvent
                    );

    Status = gBS->SetTimer (mUefiTimerEvent, TimerPeriodic, CRL_TIMER_INTERVAL_IN_MICRO_SECONDS * 10);

    return FALSE;
}

BOOL HAL_Time_Uninitialize()
{
    EFI_STATUS  Status;
    EFI_TPL     CurrTpl;
    
    CurrTpl = gBS->RaiseTPL (TPL_HIGH_LEVEL);
    gBS->RestoreTPL (CurrTpl);
    if (CurrTpl > mAppTpl) {
      gBS->RestoreTPL (mAppTpl);
    }

    Status = gBS->SetTimer (mUefiTimerEvent, TimerPeriodic, 0);
    Status = gBS->CloseEvent (mUefiTimerEvent);

    return FALSE;
}

#ifdef __cplusplus
extern "C" {
#endif

UINT64 HAL_Time_CurrentTicks()
{
    EFI_STATUS  Status;
    UINT64      TimerValue;

    Status = mCpuAp->GetTimerValue (
                       mCpuAp,
                       0,
                       &TimerValue,
                       &mTimerPeriod
                       );
    return TimerValue;
}

#ifdef __cplusplus
}
#endif

INT64 HAL_Time_TicksToTime( UINT64 Ticks )
{
    // unit is 10^-7 sec
    // 1 tick = mTimerPeriod / 10^15 * 10^7;
    return DivU64x64Remainder (MultU64x64 (Ticks, mTimerPeriod), UEFI_TIMER_PERIOD_UNIT/TIME_CONVERSION__TO_SECONDS, NULL);
}

INT64 HAL_Time_CurrentTime()
{
    return HAL_Time_TicksToTime(HAL_Time_CurrentTicks());
}

void HAL_Time_Sleep_MicroSeconds( UINT32 uSec )
{
    gBS->Stall (uSec);
}

void HAL_Time_Sleep_MicroSeconds_InterruptEnabled( UINT32 uSec )
{
    HAL_Time_Sleep_MicroSeconds (uSec);
}

void HAL_Time_GetDriftParameters  ( INT32* a, INT32* b, INT64* c )
{
    *a = 1;
    *b = 1;
    *c = 0;
}

UINT32 CPU_SystemClock()
{
    return 0;
}


UINT32 CPU_TicksPerSecond()
{
    // 1 tick = mTimerPeriod / 10^15 second;
    // 1 second = 10^15 / mTimerPeriod tick;
    return (UINT32)DivU64x64Remainder (UEFI_TIMER_PERIOD_UNIT, mTimerPeriod, NULL);
}

UINT64 CPU_MillisecondsToTicks( UINT64 mSec )
{
    debug_printf ("CPU_MillisecondsToTicks - 0x%016lx mSec -> 0x%016lx Ticks\r\n", mSec, DivU64x64Remainder (MultU64x64 (UEFI_TIMER_PERIOD_UNIT/MILLI_SECONDS_PER_1_SECCOND, mSec), mTimerPeriod, NULL));
    return DivU64x64Remainder (MultU64x64 (UEFI_TIMER_PERIOD_UNIT/MILLI_SECONDS_PER_1_SECCOND, mSec), mTimerPeriod, NULL);
}

UINT64 CPU_MillisecondsToTicks( UINT32 mSec )
{
    return DivU64x64Remainder (MultU64x64 (UEFI_TIMER_PERIOD_UNIT/MILLI_SECONDS_PER_1_SECCOND, mSec), mTimerPeriod, NULL);
}

UINT64 CPU_MicrosecondsToTicks( UINT64 uSec )
{
    return DivU64x64Remainder (MultU64x64 (UEFI_TIMER_PERIOD_UNIT/MICRO_SECONDS_PER_1_SECCOND, uSec), mTimerPeriod, NULL);
}

UINT32 CPU_MicrosecondsToTicks( UINT32 uSec )
{
    return (UINT32)DivU64x64Remainder (MultU64x64 (UEFI_TIMER_PERIOD_UNIT/MICRO_SECONDS_PER_1_SECCOND, uSec), mTimerPeriod, NULL);
}

UINT32 CPU_MicrosecondsToSystemClocks( UINT32 uSec )
{
    return 0;
}

UINT64 CPU_TicksToTime( UINT64 Ticks )
{
    return 0;
}

UINT64 CPU_TicksToTime( UINT32 Ticks32 )
{
    return 0;
}


void HAL_Time_SetCompare( UINT64 CompareValue )
{
  //debug_printf ("HAL_Time_SetCompare - 0x%016lx\r\n", CompareValue);
  mNextCompareValue = CompareValue;
}

#ifdef __cplusplus
extern "C" {
#endif

VOID
EFIAPI
CLRUefiTimerHandler (
  IN EFI_EVENT  Event,
  IN VOID       *Context
  )
{
  UINT64  CurrentTick;
  EFI_TPL     Tpl;

  CurrentTick = HAL_Time_CurrentTicks ();
  //debug_printf ("CLRUefiTimerHandler - 0x%016lx\r\n", CurrentTick);
  if (CurrentTick >= mNextCompareValue) {
    // this also schedules the next one, if there is one
    Tpl = gBS->RaiseTPL (TPL_HIGH_LEVEL);
    HAL_COMPLETION::DequeueAndExec();
    gBS->RestoreTPL (Tpl);
  } else {
    //
    // Because we are limited in the resolution of timer,
    // resetting the compare will properly configure the next interrupt.
    //
    //HAL_Time_SetCompare ( mNextCompareValue );
  }
}

#ifdef __cplusplus
}
#endif
