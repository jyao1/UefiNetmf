////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "spot_hardware_serial.h"

static const CLR_RT_MethodHandler method_lookup[] =
{
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    Library_spot_hardware_serial_native_System_IO_Ports_SerialPort::InternalOpen___VOID,
    Library_spot_hardware_serial_native_System_IO_Ports_SerialPort::InternalClose___VOID,
    Library_spot_hardware_serial_native_System_IO_Ports_SerialPort::Read___I4__SZARRAY_U1__I4__I4,
    Library_spot_hardware_serial_native_System_IO_Ports_SerialPort::Write___I4__SZARRAY_U1__I4__I4,
    Library_spot_hardware_serial_native_System_IO_Ports_SerialPort::InternalDispose___VOID,
    Library_spot_hardware_serial_native_System_IO_Ports_SerialPort::Flush___VOID,
    Library_spot_hardware_serial_native_System_IO_Ports_SerialPort::BytesInBuffer___I4__BOOLEAN,
    Library_spot_hardware_serial_native_System_IO_Ports_SerialPort::DiscardBuffer___VOID__BOOLEAN,
    NULL,
};

const CLR_RT_NativeAssemblyData g_CLR_AssemblyNative_Microsoft_SPOT_Hardware_SerialPort =
{
    "Microsoft.SPOT.Hardware.SerialPort",
    0x36DE26C1,
    method_lookup
};
