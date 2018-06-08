:: setvar varname cmd
:: Set VARNAME to the output of CMD
:: Triple escape pipes, eg:
:: setvar x  dir c:\ ^^^| sort 
:: -----------------------------
@echo off
if "%*"=="" (
	call :help
	exit /b
)
SETLOCAL

:: Get command from argument 
for /F "tokens=1,*" %%a in ("%*") do set cmd=%%b

:: Get output and set var
for /F "usebackq delims=" %%a in (`%cmd%`) do (
     ENDLOCAL
     set %1=%%a
)

:: Show results 
SETLOCAL EnableDelayedExpansion
echo %1=!%1!
exit /b

:help
echo This is a tool for set variable (with pipes)
echo.
echo setvar varname cmd
echo Set VARNAME to the output of CMD
echo Triple escape pipes, eg:
echo setvar x  dir c:\ ^^^^^^^| sort 