////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <TinyCLR_Runtime.h>
#include <TinyCLR_Checks.h>
#include <TinyCLR_Diagnostics.h>
#include <TinyCLR_Graphics.h>
#include <TinyCLR_Hardware.h>

#include <TinyCLR_Application.h>

#include <corlib_native.h>
#include <spot_native.h>
#include <spot_graphics_native.h>
#include <spot_hardware_native.h>
#include <spot_net_native.h>
#include <spot_net_security_native.h>


#include <tinyhal.h>

////////////////////////////////////////////////////////////////////////////////
void ApplicationEntryPoint()
{
    CLR_SETTINGS clrSettings;
    
    memset(&clrSettings, 0, sizeof(CLR_SETTINGS));

    clrSettings.MaxContextSwitches         = 50;
    clrSettings.WaitForDebugger            = false;
    clrSettings.EnterDebuggerLoopAfterExit = true;

    ClrStartup( clrSettings );
    
#if !defined(BUILD_RTM)
    debug_printf( "Exiting.\r\n" );
#else
    ::CPU_Reset();
#endif
}

unsigned int Solution_GetReleaseInfo(MfReleaseInfo& releaseInfo)
{
    MfReleaseInfo::Init(releaseInfo,
                        VERSION_MAJOR, VERSION_MINOR, VERSION_BUILD, VERSION_REVISION,
                        OEMSYSTEMINFOSTRING, hal_strlen_s(OEMSYSTEMINFOSTRING)
                        );
    return TRUE; // alternatively, return false if you didn't initialize the releaseInfo structure.
}
