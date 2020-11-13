////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright (c) Microsoft Corporation.  All rights reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
extern "C" {
#endif

#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/SerialIo.h>

#ifdef __cplusplus
}
#endif

#include "usart_uefi.h"
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

#if TOTAL_USART_PORT == 0
EFI_SERIAL_IO_PROTOCOL  *mSerialIoProtocol[1];
#else
EFI_SERIAL_IO_PROTOCOL  *mSerialIoProtocol[TOTAL_USART_PORT];
#endif

//--//

BOOL USART_Driver::Initialize( int ComPortNum, int BaudRate, int Parity, int DataBits, int StopBits, int FlowValue )
{
  EFI_STATUS         Status;
  EFI_HANDLE         *SerialIoHandles;
  UINTN              NumberSerialIoHandles;
  EFI_PARITY_TYPE    EfiParity;
  EFI_STOP_BITS_TYPE EfiStopBits;

  if((ComPortNum < 0) || (ComPortNum >= TOTAL_USART_PORT))
  {
    return FALSE;
  }

  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiSerialIoProtocolGuid,
                  NULL,
                  &NumberSerialIoHandles,
                  &SerialIoHandles
                  );
  if (EFI_ERROR (Status)) {
    return FALSE;
  }

  if ((UINTN)ComPortNum >= NumberSerialIoHandles) {
    return FALSE;
  }

  Status = gBS->HandleProtocol (SerialIoHandles[ComPortNum], &gEfiSerialIoProtocolGuid, (VOID **)&mSerialIoProtocol[ComPortNum]);
  if (EFI_ERROR (Status)) {
    return FALSE;
  }
  
  switch (Parity) {
  case USART_PARITY_EVEN:
    EfiParity = EvenParity;
    break;
  case USART_PARITY_ODD:
    EfiParity = OddParity;
    break;
  case USART_PARITY_MARK:
    EfiParity = MarkParity;
    break;
  case USART_PARITY_SPACE:
    EfiParity = SpaceParity;
    break;
  case USART_PARITY_NONE:
    EfiParity = NoParity;
    break;
  default:
    EfiParity = DefaultParity;
    break;
  }

  switch (StopBits) {
  case USART_STOP_BITS_ONEPOINTFIVE:
    EfiStopBits = OneFiveStopBits;
    break;
  case USART_STOP_BITS_TWO:
    EfiStopBits = TwoStopBits;
    break;
  case USART_STOP_BITS_ONE:
    EfiStopBits = OneStopBit;
    break;
  case USART_STOP_BITS_NONE:
    EfiStopBits = DefaultStopBits;
    break;
  default:
    EfiStopBits = DefaultStopBits;
    break;
  }

  Status = (mSerialIoProtocol[ComPortNum])->SetAttributes (
                                              mSerialIoProtocol[ComPortNum],
                                              BaudRate,
                                              0, // ReceiveFifoDepth
                                              0, // Timeout
                                              EfiParity,
                                              DataBits,
                                              EfiStopBits
                                              );
  if (EFI_ERROR (Status)) {
    return FALSE;
  }

  return TRUE;
}


BOOL USART_Driver::Uninitialize( int ComPortNum )
{
  if((ComPortNum < 0) || (ComPortNum >= TOTAL_USART_PORT))
  {
    return FALSE;
  }

  mSerialIoProtocol[ComPortNum] = 0;

  return TRUE;
}


int USART_Driver::Write( int ComPortNum, const char* Data, size_t size )
{
  EFI_STATUS  Status;
  UINTN       BufferSize;

  if((ComPortNum < 0) || (ComPortNum >= TOTAL_USART_PORT))
  {
    return 0;
  }

  BufferSize = size;
  Status = (mSerialIoProtocol[ComPortNum])->Write (
                                              mSerialIoProtocol[ComPortNum],
                                              &BufferSize,
                                              (VOID *)Data
                                              );
  if (EFI_ERROR(Status)) {
    return 0;
  }

  return BufferSize;
}


int USART_Driver::Read( int ComPortNum, char* Data, size_t size )
{
  EFI_STATUS  Status;
  UINTN       BufferSize;

  if((ComPortNum < 0) || (ComPortNum >= TOTAL_USART_PORT))
  {
    return 0;
  }

  BufferSize = size;
  Status = (mSerialIoProtocol[ComPortNum])->Read (
                                              mSerialIoProtocol[ComPortNum],
                                              &BufferSize,
                                              (VOID *)Data
                                              );
  if (EFI_ERROR(Status)) {
    return 0;
  }
  return BufferSize;
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

