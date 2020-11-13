////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>

volatile INT32 SystemStates[SYSTEM_STATE_TOTAL_STATES];

void SystemState_Set  ( SYSTEM_STATE State )
{
    GLOBAL_LOCK(irq);

    SystemState_SetNoLock( State );
}

void SystemState_Clear( SYSTEM_STATE State )
{
    GLOBAL_LOCK(irq);

    SystemState_ClearNoLock( State );
}

BOOL SystemState_Query( SYSTEM_STATE State )
{
    GLOBAL_LOCK(irq);

    return SystemState_QueryNoLock( State );
}

void SystemState_SetNoLock  ( SYSTEM_STATE State )
{
//    ASSERT_IRQ_MUST_BE_OFF();

    SystemStates[State]++;
}
void SystemState_ClearNoLock( SYSTEM_STATE State )
{
//    ASSERT_IRQ_MUST_BE_OFF();

    SystemStates[State]--;
}
BOOL SystemState_QueryNoLock( SYSTEM_STATE State )
{
//    ASSERT_IRQ_MUST_BE_OFF();

    return (SystemStates[State] > 0) ? TRUE : FALSE;
}
