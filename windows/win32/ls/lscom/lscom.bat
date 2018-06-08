@echo off

for /f "delims=" %%i in ('.\enumcom.exe ^| busybox grep QCUSB ^| busybox awk {"print $2"}') do (set COM_NUM=%%i)
busybox echo %COM_NUM%
