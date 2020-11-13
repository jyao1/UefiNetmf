////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <tinyhal.h>

//////////////////////////////////////////////////////////////////////////////////

BOOL DebuggerPort_Initialize( COM_HANDLE ComPortNum )
{
    NATIVE_PROFILE_PAL_COM();
    switch(ExtractTransport(ComPortNum))
    {
        case USART_TRANSPORT:
            return USART_Initialize( ConvertCOM_ComPort(ComPortNum), HalSystemConfig.USART_DefaultBaudRate, USART_PARITY_NONE, 8, USART_STOP_BITS_ONE, USART_FLOW_NONE );
    }

    return FALSE;
}

BOOL DebuggerPort_Uninitialize( COM_HANDLE ComPortNum )
{
    NATIVE_PROFILE_PAL_COM();
    switch(ExtractTransport(ComPortNum))
    {
        case USART_TRANSPORT:
            return USART_Uninitialize( ConvertCOM_ComPort(ComPortNum) );
    }

    return FALSE;
}

int DebuggerPort_Write( COM_HANDLE ComPortNum, const char* Data, size_t size )
{
    NATIVE_PROFILE_PAL_COM();
    
    UINT32       transport = ExtractTransport(ComPortNum);
    const char*  dataTmp   = Data;
    INT32        totWrite  = 0;
    int          retries   = 100;

    while(size > 0 && retries--)
    {
        int ret = 0;
        
        switch(transport)
        {
            case USART_TRANSPORT:
                ret = USART_Write( ConvertCOM_ComPort( ComPortNum ), dataTmp, size );
                break;
        }
        if(ret < 0)
        {
            break;
        }
        else if(ret == 0)
        {
            // if interrupts are off and our buffer is full then there is nothing we can do
            if(!INTERRUPTS_ENABLED_STATE()) break;

            Events_WaitForEvents(0, 1);
        }
        else
        {
            retries   = 50;   // reset retries
            size     -= ret;
            dataTmp  += ret;
            totWrite += ret;
        }
    }

    return totWrite;
}


int DebuggerPort_Read( COM_HANDLE ComPortNum, char* Data, size_t size )
{
    NATIVE_PROFILE_PAL_COM();
    int ret = 0;

    switch(ExtractTransport(ComPortNum))
    {
        case USART_TRANSPORT:
            ret = USART_Read( ConvertCOM_ComPort( ComPortNum ), Data, size );
            break;
    }

    return ret;
}


BOOL DebuggerPort_Flush( COM_HANDLE ComPortNum )
{
    NATIVE_PROFILE_PAL_COM();
    switch(ExtractTransport(ComPortNum))
    {
        case USART_TRANSPORT:
            return USART_Flush( ConvertCOM_ComPort( ComPortNum ) );
    }

    return FALSE;
}

//////////////////////////////////////////////////////////////////////////////////

void CPU_InitializeCommunication()
{
    NATIVE_PROFILE_PAL_COM();
    // STDIO can be different from the Debug Text port
    // do these first so we can print out messages

    if(COM_IsSerial(HalSystemConfig.DebugTextPort))
    {
        USART_Initialize( ConvertCOM_ComPort(HalSystemConfig.DebugTextPort), HalSystemConfig.USART_DefaultBaudRate, USART_PARITY_NONE, 8, USART_STOP_BITS_ONE, USART_FLOW_NONE );
    }

    if(COM_IsSerial(HalSystemConfig.stdio))
    {
        USART_Initialize( ConvertCOM_ComPort(HalSystemConfig.stdio), HalSystemConfig.USART_DefaultBaudRate, USART_PARITY_NONE, 8, USART_STOP_BITS_ONE, USART_FLOW_NONE );
    }
}

void CPU_UninitializeCommunication()
{
    NATIVE_PROFILE_PAL_COM();
    // STDIO can be different from the Debug Text port
    // do these first so we can print out messages

    if(COM_IsSerial(HalSystemConfig.DebugTextPort))
    {
        USART_Uninitialize( ConvertCOM_ComPort(HalSystemConfig.DebugTextPort) );
    }
    if(COM_IsSerial(HalSystemConfig.stdio))
    {
        USART_Uninitialize( ConvertCOM_ComPort(HalSystemConfig.stdio) );
    }
}


void CPU_ProtectCommunicationGPIOs( BOOL On )
{
    NATIVE_PROFILE_PAL_COM();

    switch(ExtractTransport(HalSystemConfig.DebugTextPort))
    {
        case USART_TRANSPORT:
            CPU_USART_ProtectPins( ConvertCOM_ComPort(HalSystemConfig.DebugTextPort), On );
            return ;
        default:
            return;
    }
}
