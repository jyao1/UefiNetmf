////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _QUARK_USART_H_
#define _QUARK_USART_H_

typedef BOOLEAN BOOL;
typedef UINTN   size_t;
typedef UINT32  GPIO_PIN;
#include <USART_decl.h>

#define TIMEOUT_STALL_INTERVAL                  10 // in microseconds

#define PCI_BUS_NUMBER_UART              0x0
#define PCI_DEVICE_NUMBER_UART           0x14
#define PCI_FUNCTION_NUMBER_UART_1       0x5
#define PCI_FUNCTION_NUMBER_UART_2       0x1

#define R_IOH_UART_MEMBAR                 0x10
#define   B_IOH_UART_MEMBAR_ADDRESS_MASK    0xFFFFF000  // [31:12].

#define IOH_UART_CLK_FREQ 44236800

//
// ---------------------------------------------
// UART Register Offset and Bitmask of Value
// ---------------------------------------------
//
#define R_UART_BAUD_THR       0
#define R_UART_BAUD_LOW       0
#define R_UART_BAUD_HIGH      0x04
#define R_UART_IER            0x04
#define R_UART_FCR            0x08
#define   B_UARY_FCR_TRFIFIE  BIT0
#define   B_UARY_FCR_RESETRF  BIT1
#define   B_UARY_FCR_RESETTF  BIT2
#define R_UART_LCR            0x0C
#define   B_UARY_LCR_DLAB     BIT7
#define R_UART_MCR            0x10
#define R_UART_LSR            0x14
#define   B_UART_LSR_RXRDY    BIT0
#define   B_UART_LSR_TXRDY    BIT5
#define   B_UART_LSR_TEMT     BIT6
#define R_UART_MSR            0x18
#define R_UART_SCR            0x1C



typedef void (*PFNUsartEvent) (void* context, unsigned int event);

struct USART_Driver
{
    static BOOL Initialize  ( int ComPortNum, int BaudRate, int Parity, int DataBits, int StopBits, int FlowValue );
    static BOOL Uninitialize( int ComPortNum                              );

    static int  Write( int ComPortNum, const char* Data, size_t size );
    static int  Read ( int ComPortNum, char*       Data, size_t size );
    static BOOL Flush( int ComPortNum                                );

    static BOOL AddCharToRxBuffer     ( int ComPortNum, char  c     );
    static BOOL RemoveCharFromTxBuffer( int ComPortNum, char& c     );
    static INT8 PowerSave             ( int ComPortNum, INT8 Enable );

    static void PrepareForClockStop();
    static void ClockStopFinished();

    static void SendXOFF( int ComPortNum, const UINT32 Flag );
    static void SendXON ( int ComPortNum, const UINT32 Flag );

    static void CloseAllPorts();

    static int  BytesInBuffer( int ComPortNum, BOOL fRx );
    static void DiscardBuffer( int ComPortNum, BOOL fRx );

    static BOOL ConnectEventSink( int ComPortNum, int EventType, void *pContext, PFNUsartEvent pfnUsartEvtHandler, void **ppArg );
    static void SetEvent( int ComPortNum, unsigned int event );

};

#endif
