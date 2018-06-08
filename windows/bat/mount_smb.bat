@echo off

::for /f "delims=" %%i in (' dir  ^| busybox grep "Volume in drive" ^| busybox awk {"print $4"} ') do (set DRIVE_PATH=%%i:)
::echo %DRIVE_PATH%
::
::for /f "delims=" %%j in (' net use ^| busybox grep "%DRIVE_PATH%" ^| busybox awk {"print $3"} ^| busybox sed {"s/\(.*\)\\\(.*\)/\1/g"} ^| busybox sed {"s/\\//g"} ') do (set NET_IP=%%j)
::
::if "%NET_IP%"=="" (
::	echo Not a Net Drive
::	exit /b 33
::) else (
::	echo %NET_IP%
::)
::
::for /f "delims=" %%k in (' cmdkey.exe /list ^| busybox grep "%NETIP%" -A 3 ^| busybox grep "User" ^| busybox sed {"s/\(.*\)\\\(.*\)/\2/g"} ') do (set NET_USER=%%k)
::echo %NET_USER%

for /f "delims=" %%i in (' net use ^| busybox grep "Unavailable" ^| busybox wc -l ') do (set NUMBER=%%i)
::echo %NUMBER%

:LOOP
if not "%NUMBER%"=="0" (
	for /f "delims=" %%i in (' net use ^| busybox grep "Unavailable" ^| busybox sed -n %NUMBER%p ^| busybox awk {"print $2"} ') do (set DRIVE=%%i)
	for /f "delims=" %%i in (' net use ^| busybox grep "Unavailable" ^| busybox sed -n %NUMBER%p ^| busybox awk {"print $3"} ') do (set NETADDR=%%i)
	net use %DRIVE% %NETADDR%
	set /a NUMBER-=1
	goto LOOP
) else (
	echo.
	echo Mount Successfully
)
