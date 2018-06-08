@echo off

rem How to create lnk in cmd.exe

set srcfile=%1
set lnkFile=%2
call :createShort "%srcfile%" "%lnkfile%"
goto :eof

::Arguments              目标程序参数
::Description            快捷方式备注
::FullName               返回快捷方式完整路径
::Hotkey                 快捷方式快捷键
::IconLocation           快捷方式图标，不设则使用默认图标
::TargetPath             目标
::WindowStyle            窗口启动状态
::WorkingDirectory       起始位置

:createShort
mshta VBScript:Execute("Set process=CreateObject(""WScript.Shell""):Set b=process.CreateShortcut(""%~2.lnk""):b.TargetPath=""%~1"":b.WorkingDirectory=""%~dp1"":b.Save:close")
