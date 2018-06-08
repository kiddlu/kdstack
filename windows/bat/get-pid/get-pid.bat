@ echo off

rem Note: Session Name for privileged Administrative consoles is sometimes blank.
if not defined SESSIONNAME set SESSIONNAME=Console

setlocal

rem Instance Set
set instance=%DATE% %TIME% %RANDOM%
title %instance%
rem echo Instance: "%instance%"

rem PID Find
for /f "usebackq tokens=2" %%a in (`tasklist /FO list /FI "SESSIONNAME eq %SESSIONNAME%" /FI "USERNAME eq %USERNAME%" /FI "WINDOWTITLE eq %instance%" ^| %SYSTEMROOT%\system32\find /i "PID:"`) do set PID=%%a
if not defined PID for /f "usebackq tokens=2" %%a in (`tasklist /FO list /FI "SESSIONNAME eq %SESSIONNAME%" /FI "USERNAME eq %USERNAME%" /FI "WINDOWTITLE eq Administrator:  %instance%" ^| %SYSTEMROOT%\system32\find /i "PID:"`) do set PID=%%a
if not defined PID echo !Error: Could not determine the Process ID of the current script.  Exiting.& exit /b 1

rem Current Task Show
rem echo PID: "%PID%"
tasklist /v /FO list /FI "PID eq %PID%"

rem Title Reset to Image Name (Image Name can contain spaces and will not be tokenized through usage of * token and %%b variable to access the remaining line without tokenization.)
for /f "usebackq tokens=2*" %%a in (`tasklist /V /FO list /FI "PID eq %PID%" ^| %SYSTEMROOT%\system32\find /i "Image Name:"`) do title %%b

pause
