#/** @file NetmfPkg.dsc
# 
#  Copyright (c) 2014, Intel Corporation. All rights reserved.<BR>
#  This program and the accompanying materials
#  are licensed and made available under the terms and conditions of the BSD License
#  which accompanies this distribution.  The full text of the license may be found at
#  http://opensource.org/licenses/bsd-license.php
#
#  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
#  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
#
# **/

[Defines]
  PLATFORM_NAME                  = NetmfPkg
  PLATFORM_GUID                  = 42944AAF-C8B2-46AA-A741-0A7D3843C403
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x00010005
  OUTPUT_DIRECTORY               = Build/NetmfPkg
  SUPPORTED_ARCHITECTURES        = IA32
  BUILD_TARGETS                  = DEBUG|RELEASE
  SKUID_IDENTIFIER               = DEFAULT
  
  DEFINE NT32 = TRUE
  
  DEFINE DEBUG_MESSAGE_ENABLE = TRUE

[LibraryClasses]
  BaseLib|MdePkg/Library/BaseLib/BaseLib.inf
  BaseMemoryLib|MdePkg/Library/BaseMemoryLib/BaseMemoryLib.inf
  MemoryAllocationLib|MdePkg/Library/UefiMemoryAllocationLib/UefiMemoryAllocationLib.inf
  UefiLib|MdePkg/Library/UefiLib/UefiLib.inf
  UefiBootServicesTableLib|MdePkg/Library/UefiBootServicesTableLib/UefiBootServicesTableLib.inf
  UefiRuntimeServicesTableLib|MdePkg/Library/UefiRuntimeServicesTableLib/UefiRuntimeServicesTableLib.inf
  DxeServicesTableLib|MdePkg/Library/DxeServicesTableLib/DxeServicesTableLib.inf
  DevicePathLib|MdePkg/Library/UefiDevicePathLib/UefiDevicePathLib.inf
  PrintLib|MdePkg/Library/BasePrintLib/BasePrintLib.inf
  PciLib|MdePkg/Library/BasePciLibCf8/BasePciLibCf8.inf
  PciCf8Lib|MdePkg/Library/BasePciCf8Lib/BasePciCf8Lib.inf
  IoLib|MdePkg/Library/BaseIoLibIntrinsic/BaseIoLibIntrinsic.inf
  SynchronizationLib|MdePkg/Library/BaseSynchronizationLib/BaseSynchronizationLib.inf
  PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf
  PeCoffLib|MdePkg/Library/BasePeCoffLib/BasePeCoffLib.inf
  PeCoffGetEntryPointLib|MdePkg/Library/BasePeCoffGetEntryPointLib/BasePeCoffGetEntryPointLib.inf
  PeCoffExtraActionLib|MdePkg/Library/BasePeCoffExtraActionLibNull/BasePeCoffExtraActionLibNull.inf
  CacheMaintenanceLib|MdePkg/Library/BaseCacheMaintenanceLib/BaseCacheMaintenanceLib.inf
  DxeServicesLib|MdePkg/Library/DxeServicesLib/DxeServicesLib.inf
  HobLib|MdePkg/Library/DxeHobLib/DxeHobLib.inf
  SerialPortLib|MdePkg/Library/BaseSerialPortLibNull/BaseSerialPortLibNull.inf
  UefiDriverEntryPoint|MdePkg/Library/UefiDriverEntryPoint/UefiDriverEntryPoint.inf
!if $(DEBUG_MESSAGE_ENABLE) == TRUE
  DebugLib|IntelFrameworkModulePkg/Library/PeiDxeDebugLibReportStatusCode/PeiDxeDebugLibReportStatusCode.inf
  ReportStatusCodeLib|MdeModulePkg/Library/DxeReportStatusCodeLib/DxeReportStatusCodeLib.inf
!else
  DebugLib|MdePkg/Library/BaseDebugLibNull/BaseDebugLibNull.inf
  ReportStatusCodeLib|MdePkg/Library/BaseReportStatusCodeLibNull/BaseReportStatusCodeLibNull.inf
!endif
  DebugPrintErrorLevelLib|MdePkg/Library/BaseDebugPrintErrorLevelLib/BaseDebugPrintErrorLevelLib.inf

  CLRStartup|NetmfPkg/netmf/Solutions/Uefi/StartupLib/CLRStartup_uefi.inf
  Core|NetmfPkg/netmf/CLR/Core/Core.inf
  Hardware|NetmfPkg/netmf/CLR/Core/Hardware/Hardware_stub.inf
  InterruptHandler|NetmfPkg/netmf/CLR/Core/Hardware/InterruptHandler/InterruptHandler_stub.inf
  HeapPersistence|NetmfPkg/netmf/CLR/Core/HeapPersistence/HeapPersistence_stub.inf
  I2C|NetmfPkg/netmf/CLR/Core/I2C/I2C_stub.inf
  IOPort|NetmfPkg/netmf/CLR/Core/IOPort/IOPort_stub.inf
  RPC|NetmfPkg/netmf/CLR/Core/RPC/RPC_stub.inf
  Serialization|NetmfPkg/netmf/CLR/Core/Serialization/Serialization_stub.inf
  Stream|NetmfPkg/netmf/CLR/Core/Stream/Stream_stub.inf
  Xml|NetmfPkg/netmf/CLR/Core/Xml/Xml_stub.inf
  CorLib|NetmfPkg/netmf/CLR/Libraries/CorLib/CorLib.inf
  SPOT|NetmfPkg/netmf/CLR/Libraries/SPOT/SPOT.inf
  SPOT_Crypto|NetmfPkg/netmf/CLR/Libraries/SPOT/SPOT_Crypto/SPOT_Crypto_stub.inf
  SPOT_Messaging|NetmfPkg/netmf/CLR/Libraries/SPOT/SPOT_Messaging/SPOT_Messaging_stub.inf
  SPOT_Serialization|NetmfPkg/netmf/CLR/Libraries/SPOT/SPOT_Serialization/SPOT_Serialization_stub.inf
  SPOT_Graphics|NetmfPkg/netmf/CLR/Libraries/SPOT_Graphics/SPOT_Graphics_stub.inf
  SPOT_Hardware|NetmfPkg/netmf/CLR/Libraries/SPOT_Hardware/SPOT_Hardware_stub.inf
  SPOT_Serial|NetmfPkg/netmf/CLR/Libraries/SPOT_Hardware/SPOT_Serial/SPOT_Serial_stub.inf
  SPOT_Usb|NetmfPkg/netmf/CLR/Libraries/SPOT_Hardware/SPOT_Usb/SPOT_Usb_stub.inf
  SPOT_IO|NetmfPkg/netmf/CLR/Libraries/SPOT_IO/SPOT_IO_stub.inf
  SPOT_Net|NetmfPkg/netmf/CLR/Libraries/SPOT_Net/SPOT_Net_stub.inf
  SPOT_Net_Security|NetmfPkg/netmf/CLR/Libraries/SPOT_Net_Security/SPOT_Net_Security_stub.inf
  SPOT_TimeService|NetmfPkg/netmf/CLR/Libraries/SPOT_TimeService/SPOT_TimeService_stub.inf
  SPOT_Touch|NetmfPkg/netmf/CLR/Libraries/SPOT_Touch/SPOT_Touch_stub.inf
  System_Xml|NetmfPkg/netmf/CLR/Libraries/System_Xml/System_Xml_stub.inf
  Debugger|NetmfPkg/netmf/CLR/Debugger/Debugger_stub.inf
  Diagnostics|NetmfPkg/netmf/DeviceCode/Targets/Native/Uefi/DeviceCode/Diagnostics/Diagnostics_debug.inf
  Graphics|NetmfPkg/netmf/CLR/Graphics/Graphics_stub.inf
  Messaging|NetmfPkg/netmf/CLR/Messaging/Messaging_stub.inf
  CRC|NetmfPkg/netmf/Support/crc/CRC.inf
  #HAL_GlobalLock|NetmfPkg/netmf/DeviceCode/Drivers/Stubs/GlobalLock/HAL_GlobalLock_stubs.inf
  HAL_GlobalLock|NetmfPkg/netmf/DeviceCode/Targets/Native/Uefi/DeviceCode/HAL_GlobalLock/HAL_GlobalLock_uefi.inf
  HAL_Power|NetmfPkg/netmf/DeviceCode/Drivers/Stubs/Processor/stubs_power/HAL_Power_stubs.inf
  HAL_Display|NetmfPkg/netmf/DeviceCode/Drivers/Display/stubs/HAL_Display_stubs.inf
  HAL_Backlight|NetmfPkg/netmf/DeviceCode/Drivers/Backlight/stubs/HAL_Backlight_stubs.inf
  HAL_Watchdog|NetmfPkg/netmf/DeviceCode/Drivers/Stubs/Processor/stubs_WATCHDOG/HAL_Watchdog_stubs.inf
  #HAL_Time|NetmfPkg/netmf/DeviceCode/Drivers/Stubs/Processor/stubs_time/HAL_Time_stubs.inf
  HAL_Time|NetmfPkg/netmf/DeviceCode/Targets/Native/Uefi/DeviceCode/HAL_Time/HAL_Time_uefi.inf
!if $(NT32) == TRUE
  HAL_Gpio|NetmfPkg/netmf/DeviceCode/Drivers/Stubs/Processor/stubs_gpio/HAL_Gpio_stubs.inf
!else
  HAL_Gpio|NetmfPkg/netmf/DeviceCode/Targets/Native/Quark/DeviceCode/Gpio/HAL_Gpio_Quark.inf
!endif
  HAL_Usart|NetmfPkg/netmf/DeviceCode/Drivers/Stubs/Processor/stubs_USART/HAL_Usart_stubs.inf
  HAL_IntC|NetmfPkg/netmf/DeviceCode/Drivers/Stubs/Processor/stubs_INTC/HAL_IntC_stubs.inf
  HAL_tinyhal|NetmfPkg/netmf/Solutions/Uefi/TinyCLR/HAL_tinyhal.inf
  HAL_Assert|NetmfPkg/netmf/DeviceCode/Targets/Native/Uefi/DeviceCode/HAL_Assert/HAL_Assert_uefi.inf
  #PAL_AsyncProcCall|NetmfPkg/netmf/DeviceCode/pal/AsyncProcCall/stubs/PAL_AsyncProcCall_stubs.inf
  PAL_AsyncProcCall|NetmfPkg/netmf/DeviceCode/pal/AsyncProcCall/PAL_AsyncProcCall.inf
  PAL_BlockStorage|NetmfPkg/netmf/DeviceCode/pal/BlockStorage/stubs/PAL_BlockStorage_stubs.inf
  #PAL_Time|NetmfPkg/netmf/DeviceCode/pal/time/stubs/PAL_Time_stubs.inf
  PAL_Time|NetmfPkg/netmf/DeviceCode/pal/time/PAL_Time.inf
  #PAL_Event|NetmfPkg/netmf/DeviceCode/pal/events/stubs/PAL_Event_stubs.inf
  PAL_Event|NetmfPkg/netmf/DeviceCode/pal/events/PAL_Event.inf
  #PAL_ComDirector|NetmfPkg/netmf/DeviceCode/pal/COM/stubs/PAL_ComDirector_stubs.inf
  PAL_ComDirector|NetmfPkg/netmf/DeviceCode/Targets/Native/Uefi/DeviceCode/PAL_COM/PAL_ComDirector_serial.inf
  #PAL_Usart|NetmfPkg/netmf/DeviceCode/pal/COM/usart/stubs/PAL_Usart_stubs.inf
  #PAL_Usart|NetmfPkg/netmf/DeviceCode/pal/COM/usart/PAL_Usart.inf
  #PAL_Usart|NetmfPkg/netmf/DeviceCode/Targets/Native/Quark/DeviceCode/Usart/PAL_Usart_Quark.inf
  PAL_Usart|NetmfPkg/netmf/DeviceCode/Targets/Native/Uefi/DeviceCode/PAL_Usart/PAL_Usart_uefi.inf
  PAL_Watchdog|NetmfPkg/netmf/DeviceCode/pal/watchdog/stubs/PAL_Watchdog_stubs.inf
  PAL_Piezo|NetmfPkg/netmf/DeviceCode/pal/piezo/stubs/PAL_Piezo_stubs.inf
  #PAL_PalEvent|NetmfPkg/netmf/DeviceCode/pal/palevent/Stubs/PAL_PalEvent_stubs.inf
  PAL_PalEvent|NetmfPkg/netmf/DeviceCode/pal/palevent/PAL_PalEvent.inf
  PAL_TimeService|NetmfPkg/netmf/DeviceCode/pal/timeservice/stubs/PAL_TimeService_stubs.inf
  PAL_Heap|NetmfPkg/netmf/DeviceCode/Targets/Native/Uefi/DeviceCode/PAL_Heap/PAL_Heap_uefi.inf
  PAL_tinycrt|NetmfPkg/netmf/DeviceCode/pal/tinycrt/PAL_tinycrt.inf
  Configuration|NetmfPkg/netmf/DeviceCode/Targets/Native/Uefi/DeviceCode/Configuration/Configuration.inf
  Debug|NetmfPkg/netmf/DeviceCode/Targets/Native/Uefi/DeviceCode/Debug/Debug_uefistatuscode.inf
  TinyCLR|NetmfPkg/netmf/Solutions/Uefi/TinyCLR/TinyCLR.inf
  CLR_RT_InteropAssembliesTable|NetmfPkg/netmf/Solutions/Uefi/TinyCLR/CLR_RT_InteropAssembliesTable.inf

  NativeTestLib|NetmfPkg/netmf/Test/Native/Src/NativeTestLib.inf

  UefiSupportLib|NetmfPkg/netmf/DeviceCode/Targets/Native/Uefi/DeviceCode/UefiSupportLib/UefiSupportLib.inf
  IntrinsicLib|NetmfPkg/Library/IntrinsicLib/IntrinsicLib.inf

[LibraryClasses.common.UEFI_APPLICATION]
  UefiApplicationEntryPoint|MdePkg/Library/UefiApplicationEntryPoint/UefiApplicationEntryPoint.inf
  ShellLib|ShellPkg/Library/UefiShellLib/UefiShellLib.inf
  FileHandleLib|MdePkg/Library/UefiFileHandleLib/UefiFileHandleLib.inf
  ShellCEntryLib|ShellPkg/Library/UefiShellCEntryLib/UefiShellCEntryLib.inf
  UefiHiiServicesLib|MdeModulePkg/Library/UefiHiiServicesLib/UefiHiiServicesLib.inf
  HiiLib|MdeModulePkg/Library/UefiHiiLib/UefiHiiLib.inf
  DebugPrintErrorLevelLib|MdePkg/Library/BaseDebugPrintErrorLevelLib/BaseDebugPrintErrorLevelLib.inf
  SortLib|MdeModulePkg/Library/UefiSortLib/UefiSortLib.inf

[PcdsFixedAtBuild.common]
!if $(DEBUG_MESSAGE_ENABLE) == TRUE
  gEfiMdePkgTokenSpaceGuid.PcdDebugPropertyMask|0x1f
  gEfiMdePkgTokenSpaceGuid.PcdDebugPrintErrorLevel|0x80080046
  gEfiMdePkgTokenSpaceGuid.PcdReportStatusCodePropertyMask|0x07
!else
  gEfiMdePkgTokenSpaceGuid.PcdDebugPropertyMask|0x00
  gEfiMdePkgTokenSpaceGuid.PcdDebugPrintErrorLevel|0x00000000
  gEfiMdePkgTokenSpaceGuid.PcdReportStatusCodePropertyMask|0x00
!endif

[Components]
  #
  # CLR sample
  #
  NetmfPkg/netmf/Solutions/Uefi/CLRSample/NetmfCLR.inf {
  <LibraryClasses>
    NULL|NetmfPkg/Library/UefiCppEntryPoint/CppCrt.inf
  }
  NetmfPkg/netmf/Solutions/Uefi/CLRSample/NetmfCLRHelloworld.inf {
  <LibraryClasses>
    NULL|NetmfPkg/Library/UefiCppEntryPoint/CppCrt.inf
  }
  NetmfPkg/netmf/Solutions/Uefi/CLRSample/NetmfCLRTimersTest.inf {
  <LibraryClasses>
    NULL|NetmfPkg/Library/UefiCppEntryPoint/CppCrt.inf
  }
  NetmfPkg/netmf/Solutions/Uefi/CLRSample/NetmfCLRThreadsTest.inf {
  <LibraryClasses>
    NULL|NetmfPkg/Library/UefiCppEntryPoint/CppCrt.inf
  }

  #
  # Native sample
  #
  NetmfPkg/netmf/Solutions/Uefi/NativeSample/main.inf {
  <LibraryClasses>
    NULL|NetmfPkg/Library/UefiCppEntryPoint/CppCrt.inf
    NativeSampleLib|NetmfPkg/netmf/Solutions/Uefi/NativeSample/NativeSample.inf
  }
  NetmfPkg/netmf/Solutions/Uefi/NativeSample/mainRamTest.inf {
  <LibraryClasses>
    NULL|NetmfPkg/Library/UefiCppEntryPoint/CppCrt.inf
    NativeSampleLib|NetmfPkg/netmf/Solutions/Uefi/NativeSample/NativeSampleRamTest.inf
  }
  NetmfPkg/netmf/Solutions/Uefi/NativeSample/mainTimersTest.inf {
  <LibraryClasses>
    NULL|NetmfPkg/Library/UefiCppEntryPoint/CppCrt.inf
    NativeSampleLib|NetmfPkg/netmf/Solutions/Uefi/NativeSample/NativeSampleTimersTest.inf
  }
  NetmfPkg/netmf/Solutions/Uefi/NativeSample/mainEventsTest.inf {
  <LibraryClasses>
    NULL|NetmfPkg/Library/UefiCppEntryPoint/CppCrt.inf
    NativeSampleLib|NetmfPkg/netmf/Solutions/Uefi/NativeSample/NativeSampleEventsTest.inf
  }
  NetmfPkg/netmf/Solutions/Uefi/NativeSample/mainGpioTest.inf {
  <LibraryClasses>
    NULL|NetmfPkg/Library/UefiCppEntryPoint/CppCrt.inf
    NativeSampleLib|NetmfPkg/netmf/Solutions/Uefi/NativeSample/NativeSampleGpioTest.inf
  }
  NetmfPkg/netmf/Solutions/Uefi/NativeSample/mainUartTest.inf {
  <LibraryClasses>
    NULL|NetmfPkg/Library/UefiCppEntryPoint/CppCrt.inf
    NativeSampleLib|NetmfPkg/netmf/Solutions/Uefi/NativeSample/NativeSampleUartTest.inf
  }

  #
  # Unit test
  #
  NetmfPkg/Library/UefiCppEntryPoint/TestCpp/TestCpp.inf {
  <LibraryClasses>
    NULL|NetmfPkg/Library/UefiCppEntryPoint/CppCrt.inf
  }

[BuildOptions]
!if $(NT32) == TRUE
  MSFT:DEBUG_*_*_DLINK_FLAGS = /EXPORT:InitializeDriver=$(IMAGE_ENTRY_POINT) /ALIGN:4096 /FILEALIGN:4096 /SUBSYSTEM:CONSOLE /BASE:0x10000
  MSFT:RELEASE_*_*_DLINK_FLAGS = /ALIGN:4096 /FILEALIGN:4096
!endif
  
  # warning C4804: '/' : unsafe use of type 'bool' in operation
  # warning C4164: '_ReturnAddress' : intrinsic function not declared
  # warning C4293: '<<' : shift count negative or too big, undefined behavior
  # warning C4800: 'UINT32' : forcing value to bool 'true' or 'false' (performance warning)
  # warning C4244: 'argument' : conversion from 'INT64' to 'CLR_INT32', possible loss of data
  # warning C4018: '<=' : signed/unsigned mismatch
  # warning C4702: unreachable code
  # warning C4245: '+=' : conversion from 'int' to 'CLR_UINT16', signed/unsigned mismatch
  # warning C4189: 'pLeft' : local variable is initialized but not referenced
  # warning C4389: '!=' : signed/unsigned mismatch
  MSFT:*_*_*_CC_FLAGS     = /wd4804 /wd4164 /wd4293 /wd4800 /wd4244 /wd4018 /wd4702 /wd4245 /wd4189 /wd4389 -U_WIN32 -DVERSION_MAJOR=4 -DVERSION_MINOR=1 -DVERSION_BUILD=2821 -DVERSION_REVISION=0 -DOEMSYSTEMINFOSTRING="\"UEFI Sample\"" -DUEFI -DUNICODE -D_UNICODE /GL-
  # -Wno-invalid-offsetof -fno-rtti : only for CPP
  GCC:*_*_*_CC_FLAGS      = -Wno-sign-compare -Wno-unused-variable -Wno-parentheses -Wno-switch -Wno-unused-function -Wno-write-strings -fno-exceptions -U_WIN32 -DVERSION_MAJOR=4 -DVERSION_MINOR=1 -DVERSION_BUILD=2821 -DVERSION_REVISION=0 -DOEMSYSTEMINFOSTRING="\"UEFI Sample\"" -DUEFI -DUNICODE -D_UNICODE
  #MSFT:DEBUG_*_*_CC_FLAGS = /Od
  #MSFT:DEBUG_*_*_CC_FLAGS = /D "_DEBUG"

  # Also need tool override - Elf32Convert/Elf64Convert
  GCC:*_GCC48_IA32_DLINK_FLAGS  == -nostdlib -n -q --gc-sections --script=$(EDK_TOOLS_PATH)/../NetmfPkg/Library/UefiCppEntryPoint/Scripts/gcc4.4-ld-script --entry $(IMAGE_ENTRY_POINT) -u $(IMAGE_ENTRY_POINT) -Map $(DEST_DIR_DEBUG)/$(BASE_NAME).map -melf_i386   --oformat=elf32-i386
  GCC:*_GCC48_X64_DLINK_FLAGS   == -nostdlib -n -q --gc-sections --script=$(EDK_TOOLS_PATH)/../NetmfPkg/Library/UefiCppEntryPoint/Scripts/gcc4.4-ld-script --entry $(IMAGE_ENTRY_POINT) -u $(IMAGE_ENTRY_POINT) -Map $(DEST_DIR_DEBUG)/$(BASE_NAME).map -melf_x86_64 --oformat=elf64-x86-64
  GCC:*_GCC49_IA32_DLINK_FLAGS  == -nostdlib -n -q --gc-sections --script=$(EDK_TOOLS_PATH)/../NetmfPkg/Library/UefiCppEntryPoint/Scripts/gcc4.9-ld-script --entry $(IMAGE_ENTRY_POINT) -u $(IMAGE_ENTRY_POINT) -Map $(DEST_DIR_DEBUG)/$(BASE_NAME).map -melf_i386   --oformat=elf32-i386
  GCC:*_GCC49_X64_DLINK_FLAGS   == -nostdlib -n -q --gc-sections --script=$(EDK_TOOLS_PATH)/../NetmfPkg/Library/UefiCppEntryPoint/Scripts/gcc4.9-ld-script --entry $(IMAGE_ENTRY_POINT) -u $(IMAGE_ENTRY_POINT) -Map $(DEST_DIR_DEBUG)/$(BASE_NAME).map -melf_x86_64 --oformat=elf64-x86-64
