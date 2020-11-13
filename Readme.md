# This is to show how to run .NetMF TinyCLR on UEFI.

It is a UEFI version TinyCLR driver. It can run .Net APP on UEFI (.Net Micro Framework assembly)

It is based upon MicroFrameworkPK_v4_1. (Appach license)

The old netmf code is at https://archive.codeplex.com/?p=netmf.
The new netmf code is at http://netmf.github.io/

# Feature
Support:
    CLR infrastructure
    Debug output
    PAL/HAL/CLR
    Timer
    Event
    AsyncProcCall
    Serial Port
    GPIO (Galileo)

Not Support:
    Watchdog, Interrupt
    Serialization, Stream, XML
    Messaging
    Graphics
    Debugger
    Crypto
    Hardware
    Network
    Display, TouchPanel
    BlockStorage, FAT
    Backlight, Battery, Button
    USB
    I2C/SPI

# Known limitation
This package is only the sample code to show the concept. It does not have a full validation such as robustness functional test and fuzzing test. It does not meet the production quality yet. Any codes including the API definition, the libary and the drivers are subject to change.

