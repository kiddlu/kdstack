@echo off

set CUR_PATH=%~dp0
cd %CUR_PATH%
for /f "delims=" %%i in (' cd ') do (set CUR_PATH=%%i)
echo %CUR_PATH%
