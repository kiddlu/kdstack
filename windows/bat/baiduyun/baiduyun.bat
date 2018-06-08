@ECHO OFF&PUSHD %~DP0 &TITLE 绿化和卸载
mode con cols=40 lines=20
color A
Rd "%WinDir%\system32\test_permissions" >NUL 2>NUL
Md "%WinDir%\System32\test_permissions" 2>NUL||(Echo 请使用右键管理员身份运行！&&Pause >nul&&Exit)
Rd "%WinDir%\System32\test_permissions" 2>NUL
SetLocal EnableDelayedExpansion

:Menu
Cls
@ echo.
@ echo.　　　　　 菜 单 选 项
@ echo.
@ echo.     绿化[必须] → 请输入1
@ echo.
@ echo.     创建桌面图标 → 请输入2
@ echo.
@ echo.     注册浏览器加速下载 → 请输入3
@ echo.
@ echo.     注册右键上传备份 → 请输入4
@ echo.
@ echo.     卸载 → 请输入5
@ echo.
set /p xj=     输入数字按回车：
if /i "%xj%"=="1" Goto Install
if /i "%xj%"=="2" Goto Establish
if /i "%xj%"=="3" Goto Addjs
if /i "%xj%"=="4" Goto Backup
if /i "%xj%"=="5" Goto Uninstall
@ echo.
echo      选择无效，请重新输入
ping -n 2 127.1>nul 
goto menu

:Install
@ echo.
ECHO 　　　正在安装中..请稍等..
taskkill /f /im BaiduYunGuanjia.exe>NUL 2>NUL
del /f AppSettingApp.dat  >NUL 2>NUL
rd/s/q "%tmp%\bdyunguanjiaskinres" 2>NUL
rd/s/q "%AppData%\BaiduYunGuanjia" 2>NUL
rd/s/q "%AppData%\Baidu\BaiduYunGuanjia"2>NUL
regsvr32 /s npYunWebDetect.dll
reg delete "HKCU\Software\Baidu\BaiduYunGuanjia" /f >NUL 2>NUL
reg delete "HKCR\CLSID\{679F137C-3162-45da-BE3C-2F9C3D093F64}" /f>NUL 2>NUL
reg delete "HKCU\SOFTWARE\Classes\CLSID\{679F137C-3162-45da-BE3C-2F9C3D093F64}" /f>NUL 2>NUL
reg delete "HKLM\SOFTWARE\Classes\CLSID\{679F137C-3162-45da-BE3C-2F9C3D093F64}" /f>NUL 2>NUL
reg delete "HKCU\SOFTWARE\Microsoft\Windows\CurrentVersion\Explorer\MyComputer\NameSpace\{679F137C-3162-45da-BE3C-2F9C3D093F64}" /f>NUL 2>NUL
reg delete "HKLM\SOFTWARE\Microsoft\Windows\CurrentVersion\Explorer\MyComputer\NameSpace\{679F137C-3162-45da-BE3C-2F9C3D093F64}" /f>NUL 2>NUL
reg delete "HKLM\SOFTWARE\Wow6432Node\Classes\CLSID\{679F137C-3162-45da-BE3C-2F9C3D093F64}" /f>NUL 2>NUL
reg delete "HKLM\SOFTWARE\Wow6432Node\Microsoft\Windows\CurrentVersion\Explorer\MyComputer\NameSpace\{679F137C-3162-45da-BE3C-2F9C3D093F64}" /f>NUL 2>NUL
goto Install2

:Establish
mshta VBScript:Execute("Set a=CreateObject(""WScript.Shell""):Set b=a.CreateShortcut(a.SpecialFolders(""Desktop"") & ""\百度云管家.lnk""):b.TargetPath=""%~dp0BaiduYunGuanjia.exe"":b.WorkingDirectory=""%~dp0"":b.Save:close")
goto fanhui

:Addjs
regsvr32 /s npYunWebDetect.dll
goto fanhui

:Backup
@ echo.
echo 　　　正在注册中..请稍等..
if exist "%WinDir%\SysWOW64" regsvr32 /s YunShellExt64.dll
if not exist "%WinDir%\SysWOW64" regsvr32 /s YunShellExt.dll
goto fanhui

:Uninstall
@ echo.
echo 　　　正在卸载中..请稍等.
taskkill /f /im BaiduYunGuanjia*>NUL 2>NUL
regsvr32 /s /u YunShellExt.dll
regsvr32 /s /u npYunWebDetect.dll
del /f AppSettingApp.dat  >NUL 2>NUL
del /f CompleteTaskRecodFile >NUL 2>NUL
rd/s/q "%tmp%\bdyunguanjiaskinres" 2>NUL
rd/s/q "%AppData%\BaiduYunGuanjia" 2>NUL
rd/s/q "%AppData%\BaiduYunKernel" 2>NUL
rd/s/q "%AppData%\Baidu\BaiduYunGuanjia"2>NUL
if exist "%WinDir%\SysWOW64" regsvr32 /s /u YunShellExt64.dll
reg delete "HKCU\Software\Baidu\BaiduYunGuanjia" /f >NUL 2>NUL
reg delete "HKCR\CLSID\{679F137C-3162-45da-BE3C-2F9C3D093F64}" /f>NUL 2>NUL
reg delete "HKCU\SOFTWARE\Classes\CLSID\{679F137C-3162-45da-BE3C-2F9C3D093F64}" /f>NUL 2>NUL
reg delete "HKLM\SOFTWARE\Classes\CLSID\{679F137C-3162-45da-BE3C-2F9C3D093F64}" /f>NUL 2>NUL
reg delete "HKLM\SOFTWARE\Wow6432Node\Classes\CLSID\{679F137C-3162-45da-BE3C-2F9C3D093F64}" /f>NUL 2>NUL
reg delete "HKCU\SOFTWARE\Microsoft\Windows\CurrentVersion\Explorer\MyComputer\NameSpace\{679F137C-3162-45da-BE3C-2F9C3D093F64}" /f>NUL 2>NUL
reg delete "HKLM\SOFTWARE\Microsoft\Windows\CurrentVersion\Explorer\MyComputer\NameSpace\{679F137C-3162-45da-BE3C-2F9C3D093F64}" /f>NUL 2>NUL
reg delete "HKLM\SOFTWARE\Wow6432Node\Microsoft\Windows\CurrentVersion\Explorer\MyComputer\NameSpace\{679F137C-3162-45da-BE3C-2F9C3D093F64}" /f>NUL 2>NUL
goto Uninstall2

:fanhui
@ ECHO.
ECHO 　　　设置完成..
ping -n 2 127.8>nul
goto menu

:Install2
@ ECHO.
ECHO 　　　安装完成..
ping -n 2 127.8>nul
goto menu

:Uninstall2
@ ECHO.
ECHO 　　　卸载完成..
ping -n 2 127.8>nul
goto menu