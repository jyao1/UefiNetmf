////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
extern "C" {
#endif

#include <Uefi.h>
#include <Library/IoLib.h>
#include <Library/PciLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <IndustryStandard/Pci22.h>

#ifdef __cplusplus
}
#endif

#include "Quark_usart.h"
#include "platform_selector.h"

//--//

// XOFF bit flags
static const INT8 XOFF_FLAG_FULL  = 0x01;
static const INT8 XOFF_CLOCK_HALT = 0x02;

//--//


BOOL USART_Initialize( int ComPortNum, int BaudRate, int Parity, int DataBits, int StopBits, int FlowValue )
{
    return USART_Driver::Initialize( ComPortNum, BaudRate, Parity, DataBits, StopBits, FlowValue );
}

BOOL USART_Uninitialize( int ComPortNum )
{
    return USART_Driver::Uninitialize( ComPortNum );
}

int USART_Write( int ComPortNum, const char* Data, size_t size )
{
    return USART_Driver::Write( ComPortNum, Data, size );
}

int USART_Read( int ComPortNum, char* Data, size_t size )
{
    return USART_Driver::Read( ComPortNum, Data, size );
}

BOOL USART_Flush( int ComPortNum )
{
    return USART_Driver::Flush( ComPortNum );
}

BOOL USART_AddCharToRxBuffer( int ComPortNum, char c )
{
    return USART_Driver::AddCharToRxBuffer( ComPortNum, c );
}

BOOL USART_RemoveCharFromTxBuffer( int ComPortNum, char& c )
{
    return USART_Driver::RemoveCharFromTxBuffer( ComPortNum, c );
}

INT8 USART_PowerSave( int ComPortNum, INT8 Enable )
{
    return USART_Driver::PowerSave( ComPortNum, Enable );
}

void USART_PrepareForClockStop()
{
    USART_Driver::PrepareForClockStop();
}

void USART_ClockStopFinished()
{
    USART_Driver::ClockStopFinished();
}

void USART_ForceXON(int ComPortNum) 
{
    USART_Driver::SendXON( ComPortNum, XOFF_FLAG_FULL );
}

void USART_CloseAllPorts()
{   
    USART_Driver::CloseAllPorts();
}

int  USART_BytesInBuffer( int ComPortNum, BOOL fRx )
{
    return USART_Driver::BytesInBuffer( ComPortNum, fRx );
}

void USART_DiscardBuffer( int ComPortNum, BOOL fRx )
{
    USART_Driver::DiscardBuffer( ComPortNum, fRx );
}

BOOL USART_ConnectEventSink( int ComPortNum, int EventType, void *pContext, PFNUsartEvent pfnUsartEvtHandler, void **ppArg )
{
    return USART_Driver::ConnectEventSink( ComPortNum, EventType, pContext, pfnUsartEvtHandler, ppArg );
}

void USART_SetEvent( int ComPortNum, unsigned int event )
{
    USART_Driver::SetEvent( ComPortNum, event );
}


//--//

#define TX_USART_BUFFER_SIZE_DEBUG   16
#define RX_USART_BUFFER_SIZE_DEBUG   16
#define TX_USART_BUFFER_SIZE_HI_VOL  4096
#define RX_USART_BUFFER_SIZE_HI_VOL  512

//--//

#define USART_BUFFER_HIGH_WATER_MARK(x) (((x) * 3) / 4)
#define USART_BUFFER_LOW_WATER_MARK(x)  (((x) * 1) / 4)

#ifdef  PLATFORM_DEPENDENT_TX_USART_BUFFER_SIZE
#define TX_USART_BUFFER_SIZE    PLATFORM_DEPENDENT_TX_USART_BUFFER_SIZE
#else
#define TX_USART_BUFFER_SIZE    512
#endif

#ifdef  PLATFORM_DEPENDENT_RX_USART_BUFFER_SIZE
#define RX_USART_BUFFER_SIZE    PLATFORM_DEPENDENT_RX_USART_BUFFER_SIZE
#else
#define RX_USART_BUFFER_SIZE    512
#endif

//--//

#if TOTAL_USART_PORT == 0
UINT8 TxBuffer_Com[1];     // Allows compile to complete although in this case it won't be linked
UINT8 RxBuffer_Com[1];
#else
UINT8 TxBuffer_Com[TX_USART_BUFFER_SIZE * TOTAL_USART_PORT];
UINT8 RxBuffer_Com[RX_USART_BUFFER_SIZE * TOTAL_USART_PORT];
#endif

//--//

/**

  Get Uart memory mapped base address.

  Apply relavant mask before returning.

  @return Uart memory mapped base address.

**/
STATIC
UINT32
GetUartMmioBaseAddress (
  IN UINTN Index
  )
{
  UINT32   RegData32;

  if (Index == 0) {
    RegData32 = PciRead32 (PCI_LIB_ADDRESS(PCI_BUS_NUMBER_UART, PCI_DEVICE_NUMBER_UART, PCI_FUNCTION_NUMBER_UART_1, R_IOH_UART_MEMBAR));
  } else {
    RegData32 = PciRead32 (PCI_LIB_ADDRESS(PCI_BUS_NUMBER_UART, PCI_DEVICE_NUMBER_UART, PCI_FUNCTION_NUMBER_UART_2, R_IOH_UART_MEMBAR));
  }

  return RegData32 & B_IOH_UART_MEMBAR_ADDRESS_MASK;
}

BOOL USART_Driver::Initialize( int ComPortNum, int BaudRate, int Parity, int DataBits, int StopBits, int FlowValue )
{
  UINT32  Divisor;
  UINT8   OutputData;
  UINT32  UartMmioBase;

  if((ComPortNum < 0) || (ComPortNum >= TOTAL_USART_PORT))
  {
    return FALSE;
  }

  UartMmioBase = GetUartMmioBaseAddress (ComPortNum);
  PciOr16 (PCI_LIB_ADDRESS(PCI_BUS_NUMBER_UART, PCI_DEVICE_NUMBER_UART, PCI_FUNCTION_NUMBER_UART_1, PCI_COMMAND_OFFSET), 0x7);

  //
  // Calculate divisor for baud generator
  //
  Divisor = (UINT32)(((UINTN) IOH_UART_CLK_FREQ) / (UINTN)(BaudRate) / 16);

  //
  // Set communications format
  //
  switch (DataBits) {
  case 5:
    OutputData = 0;
    break;

  case 6:
    OutputData = 1;
    break;

  case 7:
    OutputData = 2;
    break;

  case 8:
    OutputData = 3;
    break;

  default:
    OutputData = 0;
    break;
  }

  switch (StopBits) {
  case USART_STOP_BITS_ONEPOINTFIVE:
  case USART_STOP_BITS_TWO:
    OutputData |= 0x04;
    break;

  case USART_STOP_BITS_ONE:
  case USART_STOP_BITS_NONE:
  default:
    OutputData &= 0xFB;
    break;
  }

  switch (Parity) {
  case USART_PARITY_EVEN:
    OutputData |= 0x18;
    break;

  case USART_PARITY_ODD:
    OutputData |= 0x08;
    OutputData &= 0xCF;
    break;

  case USART_PARITY_MARK:
    OutputData |= 0x28;
    break;

  case USART_PARITY_SPACE:
    OutputData |= 0x38;
    break;

  case USART_PARITY_NONE:
  default:
    OutputData &= 0xF7;
  }  

  MmioWrite8 (
    UartMmioBase + R_UART_LCR,
    (UINT8)(OutputData | B_UARY_LCR_DLAB)
    );

  //
  // Configure baud rate
  //
  MmioWrite8 (
    UartMmioBase + R_UART_BAUD_HIGH,
    (UINT8) (Divisor >> 8)
    );

  MmioWrite8 (
    UartMmioBase + R_UART_BAUD_LOW,
    (UINT8) (Divisor & 0xff)
    );

  //
  // Switch back to bank 0
  //
  MmioWrite8 (
    UartMmioBase + R_UART_LCR,
    OutputData
    );

  //
  // Enable FIFO, reset receive FIFO and transmit FIFO
  //
  MmioWrite8 (
    UartMmioBase + R_UART_FCR,
    (UINT8) (B_UARY_FCR_TRFIFIE | B_UARY_FCR_RESETRF | B_UARY_FCR_RESETTF)
    );
  
  return TRUE;
}


BOOL USART_Driver::Uninitialize( int ComPortNum )
{
  if((ComPortNum < 0) || (ComPortNum >= TOTAL_USART_PORT))
  {
    return FALSE;
  }

  return TRUE;
}


int USART_Driver::Write( int ComPortNum, const char* Data, size_t size )
{
  UINTN   Result;
  UINT32  UartMmioBase;

  if((ComPortNum < 0) || (ComPortNum >= TOTAL_USART_PORT))
  {
    return 0;
  }
  UartMmioBase = GetUartMmioBaseAddress (ComPortNum);

  Result = size;
   
  while (size--) {         
    //
    // Wait for the serail port to be ready, to make sure both the transmit FIFO
    // and shift register empty.
    //
    while ((MmioRead8 (UartMmioBase + R_UART_LSR) & B_UART_LSR_TEMT) == 0);
    
    MmioWrite8 (UartMmioBase + R_UART_BAUD_THR, *Data++);      
  }

  return Result;
}


int USART_Driver::Read( int ComPortNum, char* Data, size_t size )
{
  UINTN   Result;
  UINT32  UartMmioBase;
  UINTN   Elapsed;

  if((ComPortNum < 0) || (ComPortNum >= TOTAL_USART_PORT))
  {
    return 0;
  }
  UartMmioBase = GetUartMmioBaseAddress (ComPortNum);
  Result = 0;
  Elapsed = 0;

  while (size--) {          
    //
    // Wait for the serail port to be ready.
    //
    while ((MmioRead8 (UartMmioBase + R_UART_LSR) & B_UART_LSR_RXRDY) == 0) {
      gBS->Stall (TIMEOUT_STALL_INTERVAL);
      Elapsed += TIMEOUT_STALL_INTERVAL;
      if (Elapsed >= 1000) {
        return Result;
      }
    }

    *Data++ = MmioRead8 (UartMmioBase + R_UART_BAUD_THR); 
    Result ++;
  }
  
  return Result;
}


BOOL USART_Driver::Flush( int ComPortNum )
{
  return TRUE;
}

BOOL USART_Driver::AddCharToRxBuffer( int ComPortNum, char c )
{
  return FALSE;
}

BOOL USART_Driver::RemoveCharFromTxBuffer( int ComPortNum, char& c )
{
  return FALSE;
}


INT8 USART_Driver::PowerSave( int ComPortNum, INT8 Enable )
{
  return 0;
}


void USART_Driver::PrepareForClockStop()
{
}

void USART_Driver::ClockStopFinished()
{
}

//--//

void USART_Driver::SendXOFF( INT32 ComPortNum, const UINT32 Flag )
{
}

void USART_Driver::SendXON( INT32 ComPortNum, const UINT32 Flag )
{
}

void USART_Driver::CloseAllPorts()
{
}

int USART_Driver::BytesInBuffer( int ComPortNum, BOOL fRx )
{
  return 0;
}

void USART_Driver::DiscardBuffer( int ComPortNum, BOOL fRx )
{
}

BOOL USART_Driver::ConnectEventSink( int ComPortNum, int EventType, void* pContext, PFNUsartEvent pfnUsartEvtHandler, void** ppArg )
{
  return FALSE;
}

void USART_Driver::SetEvent( int ComPortNum, unsigned int event )
{
}

//--//
#if 0
STREAM_DRIVER_DETAILS* COM1_driver_details( UINT32 handle )
{
    static STREAM_DRIVER_DETAILS details = { 
        DRIVER_BUFFERED_IO, 
        &RxBuffer_Com[ RX_USART_BUFFER_SIZE * ConvertCOM_ComPort( COM1 ) ], 
        &TxBuffer_Com[ TX_USART_BUFFER_SIZE * ConvertCOM_ComPort( COM1 ) ], 
        RX_USART_BUFFER_SIZE, 
        TX_USART_BUFFER_SIZE, 
        TRUE, 
        TRUE, 
        FALSE 
    };
        
    return &details;
}

int COM1_read( char* buffer, size_t size )
{
    return USART_Read( ConvertCOM_ComPort( COM1 ), buffer, size );
}

int COM1_write( char* buffer, size_t size )
{
    return USART_Write( ConvertCOM_ComPort( COM1 ), buffer, size );
}

//--//

STREAM_DRIVER_DETAILS* COM2_driver_details( UINT32 handle )
{
    static STREAM_DRIVER_DETAILS details = { 
        DRIVER_BUFFERED_IO, 
        &RxBuffer_Com[ RX_USART_BUFFER_SIZE * ConvertCOM_ComPort( COM2 ) ], 
        &TxBuffer_Com[ TX_USART_BUFFER_SIZE * ConvertCOM_ComPort( COM2 ) ], 
        RX_USART_BUFFER_SIZE, 
        TX_USART_BUFFER_SIZE, 
        TRUE, 
        TRUE, 
        FALSE 
    };
    
    return &details;
}

int COM2_read( char* buffer, size_t size )
{
    return USART_Read(  ConvertCOM_ComPort( COM2 ), buffer, size );
}

int COM2_write( char* buffer, size_t size )
{
    return USART_Write(  ConvertCOM_ComPort( COM2 ), buffer, size );
}
#endif

