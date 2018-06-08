@echo off

if '%*'=='' call :help & exit /b
if '%1'=='usb' (
reg query HKLM\SYSTEM\ControlSet001\Enum\USB /s
)
exit /b

:help
echo This is reg query cmd to enum hardware bus
echo.
echo %0 usb : enum USB
