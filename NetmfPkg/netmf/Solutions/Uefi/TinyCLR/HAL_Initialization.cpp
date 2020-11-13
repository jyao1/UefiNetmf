////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>

void HAL_Initialize(void)
{
    HAL_CONTINUATION::InitializeList();
    HAL_COMPLETION  ::InitializeList();

    Time_Initialize();
    Events_Initialize();

    CPU_GPIO_Initialize();

    ENABLE_INTERRUPTS();

    PalEvent_Initialize ();

    TimeService_Initialize();

#if defined(ENABLE_NATIVE_PROFILER)
    Native_Profiler_Init();
#endif
}

void HAL_Uninitialize(void)
{
#if defined(ENABLE_NATIVE_PROFILER)
    Native_Profiler_Stop();
#endif

    TimeService_UnInitialize();

    PalEvent_Uninitialize();

    DISABLE_INTERRUPTS();

    CPU_GPIO_Uninitialize();

    Events_Uninitialize();
    Time_Uninitialize();

    HAL_CONTINUATION::Uninitialize();
    HAL_COMPLETION  ::Uninitialize();
}

void HAL_EnterBooterMode()
{
}
