::Windows Registry Editor Version 5.00

::[HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\UsbFlags]
::"IgnoreHWSerNum05C69025"=hex:01
::"GlobalDisableSerNumGen"=hex:01
::"GenericUSBDeviceString"="USB Device"
::"GenericCompositeUSBDeviceString"="Composite USB Device"


@echo off
echo "add IgnoreHWSerNum list"
reg add "HKLM\SYSTEM\CurrentControlSet\Control\usbflags" /v IgnoreHWSerNum05C69025 /t REG_BINARY /d 01 /f
reg add "HKLM\SYSTEM\CurrentControlSet\Control\usbflags" /v GlobalDisableSerNumGen /t REG_BINARY /d 01 /f
pause

::@echo off
::echo "delete IgnoreHWSerNum registry"
::reg delete "HKLM\SYSTEM\CurrentControlSet\Control\usbflags" /v IgnoreHWSerNum05C69025 /f
::pause