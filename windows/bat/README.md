#  Windows Batch Scripting



## Introduce

batch file is Windows default script format, some like Unix shell script。As I know, Mircosoft support batch file from DOS，including all kinds of Windows. Some batch files use ".cmd" as suffix，difference betweent ".bat" and ".cmd", pls [Google](https://www.google.com/#newwindow=1&q=difference+between+.cmd+and+.bat)

Benefits from learning batch file programming：

    1. Understand other's batch file，modify it by yourself
    2. Write batch file for who work on Windows(testers, hardware guys, normal people)
    3. Write batch file for product or special env(Windows server, industrial PC，debug PC)
    4. Write batch file for youself(So poor that I can't have a rMBP, I use Windows on my Acer 4750G)
    
    ...In a word, make life with Windows a little easier.(what? a word?)



## Compare With Shell:

| ---                 | Batch File                               | Shell Script                             |
| :------------------ | ---------------------------------------- | ---------------------------------------- |
| comments            | :: or rem some comment                   | # some comment                           |
| set var             | set str="hello"                          | str="hello" or export str="hello"        |
| set num var         | set /a var=0                             | var=0                                    |
| store output in var | for /f %%x in ('command') do set result=%%x | result=\`command\` or result=$(echo $(command)) |
| for loop            | for %%i in (%var%) do (....)             | for i in "$var"; do...done               |
| while loop          | :loop<br>if !somevar! GEQ %someothervar% GOTO endofloop<br>GOTO loop<br>:endofloop | while [ ! $somevar -ge $someothervar ]; do <br><br>done |
| if statement        | if %somevar% geq %someothervar% (<br><br>) | if [ $somevar -ge $someothervar ]; then<br><br>fi |
| check return        | IF %errorlevel% neq 0 (<br>GOTO :EOF<br>)<br>:EOF | if [ $? -ne 0 ]; then<br>exit 1<br>fi    |
| Parameter           | %0-%9, %*                                | $0-$9,$*                                 |
| "black hole"        | echo XXX > nul                           | echo XXX > /dev/null                     |
| call or cmd /c      | source or . (dot operator)               | "include" another script                 |



## Built-in Environment Variables
from: http://ss64.com/nt/syntax-variables.html

| Variable                | Volatile(Read-Only) | Default value (assuming the system drive is C: ) |
| ----------------------- | ------------------- | ---------------------------------------- |
| ALLUSERSPROFILE         |                     | C:\ProgramData                           |
| APPDATA                 |                     | C:\Users\{username}\AppData\Roaming      |
| CD                      | Y                   | The current directory (string).          |
| ClientName              | Y                   | Terminal servers only - the ComputerName of a remote host. |
| CMDEXTVERSION           | Y                   | The current Command Processor Extensions version number. (NT = "1", Win2000+ = "2".) |
| CMDCMDLINE              | Y                   | The original command line that invoked the Command Processor. |
| CommonProgramFiles      |                     | C:\Program Files\Common Files            |
| COMMONPROGRAMFILES(x86) |                     | C:\Program Files (x86)\Common Files      |
| COMPUTERNAME            |                     | {computername}                           |
| COMSPEC                 |                     | C:\Windows\System32\cmd.exe or if running a 32 bit WOW - C:\Windows\SysWOW64\cmd.exe |
| DATE                    | Y                   | The current date using same region specific format as DATE. |
| ERRORLEVEL              | Y                   | The current ERRORLEVEL value, automatically set when a program exits. |
| HighestNumaNodeNumber   | Y (hidden)          | The highest NUMA node number on this computer. |
| HOMEDRIVE               | Y                   | C:                                       |
| HOMEPATH                | Y                   | \Users\{username}                        |
| LOCALAPPDATA            |                     | C:\Users\{username}\AppData\Local        |
| LOGONSERVER             |                     | \\{domain_logon_server}                  |
| NUMBER_OF_PROCESSORS    | Y                   | The Number of processors running on the machine. |
| OS                      | Y                   | Operating system on the user's workstation. |
| PATH                    | User and System     | C:\Windows\System32\;C:\Windows\;C:\Windows\System32\Wbem;{plus program paths} |
| PATHEXT                 |                     | .COM; .EXE; .BAT; .CMD; .VBS; .VBE; .JS ; .WSF; .WSH; .MSC<br>The syntax is like the PATH variable - semicolon separators. |
| PROCESSOR_ARCHITECTURE  | Y                   | AMD64/IA64/x86 This doesn't tell you the architecture of the processor but only of the current process, so it returns "x86" for a 32 bit WOW process running on 64 bit Windows. See detecting OS 32/64 bit |
| PROCESSOR_ARCHITEW6432  |                     | =%ProgramFiles% (only available on 64 bit systems) |
| PROCESSOR_IDENTIFIER    | Y                   | Processor ID of the user's workstation.  |
| PROCESSOR_LEVEL         | Y                   | Processor level of the user's workstation. |
| PROCESSOR_REVISION      | Y                   | Processor version of the user's workstation. |
| ProgramW6432            |                     | =%PROCESSOR_ARCHITECTURE% (only available on 64 bit systems) |
| ProgramData             |                     | C:\ProgramData                           |
| ProgramFiles            |                     | C:\Program Files or C:\Program Files (x86) |
| ProgramFiles(x86)       |                     | C:\Program Files (x86)                   |
| PROMPT                  |                     | Code for current command prompt format,usually $P$G<br>C:> |
| PSModulePath            |                     | %SystemRoot%\system32\WindowsPowerShell\v1.0\Modules\ |
| Public                  |                     | C:\Users\Public                          |
| RANDOM                  | Y                   | A random integer number, anything from 0 to 32,767 (inclusive). |
| SessionName             |                     | Terminal servers only - for a terminal server session, SessionName is a combination of the connection name, followed by #SessionNumber. For a console session, SessionName returns "Console". |
| SYSTEMDRIVE             |                     | C:                                       |
| SYSTEMROOT              |                     | By default, Windows is installed to C:\Windows but there's no guarantee of that, Windows can be installed to a different folder, or a different drive letter. |
| TEMP and TMP            | User Variable       | C:\Users\{Username}\AppData\Local\Temp   |
| TIME                    | Y                   | The current time using same format as TIME. |
| UserDnsDomain           | Y User Variable     | Set if a user is a logged on to a domain and returns the fully qualified DNS domain that the currently logged on user's account belongs to. |
| USERDOMAIN              |                     | {userdomain}                             |
| USERNAME                |                     | {username}                               |
| USERPROFILE             |                     | %SystemDrive%\Users\{username}<br>This is equivalent to the $HOME environment variable in Unix/Linux |
| WINDIR                  |                     | Set by default as windir=%SystemRoot%<br>%windir% is a regular variable and can be changed, which makes it less robust than %systemroot% |

变量赋值
数值运算
字符串处理
重定向
程序参数



## Built-in Tools:
`copy，dir，clip, reg, pushd, popd, assoc, ftype,doskey`

## Third Party Tools:

1. [busybox-w32](http://frippery.org/busybox/)

2. [nircmd](http://www.nirsoft.net/utils/nircmd.html)

3. [sfk](http://stahlworks.com/dev/swiss-file-knife.html)

4. [gow](https://github.com/bmatzelle/gow)

5. [cygwin](https://www.cygwin.com/)

6. [MSYS2](http://msys2.github.io/)

7. [f2ko CMD Tools](http://www.f2ko.de/en/cmd.php)

8. [GnuWin](http://gnuwin32.sourceforge.net/)

9. [GNU utilities for Win32](http://unxutils.sourceforge.net/)

10. [Uwe Sieber](http://www.uwe-sieber.de/english.html)

11. [UltimateDeveloperAndPowerUsersToolList](http://www.hanselman.com/blog/ScottHanselmans2014UltimateDeveloperAndPowerUsersToolListForWindows.aspx)

12. [Win-Builds](http://win-builds.org/doku.php/start)

13. [ezwinports](https://sourceforge.net/projects/ezwinports/)

    ​

## Warning:
batch file cmd & keyword ignore key case
if you are not Administrator, 
Windows路径以及UNC路径
if的数值比较和字符串比较
The == comparison operator always results in a string comparison.




## Links:

[Command line reference](http://ss64.com/nt/)

[dostips](http://www.dostips.com/)

[Guide to Windows Batch Scripting](http://steve-jansen.github.io/guides/windows-batch-scripting/index.html)

[Converting DOS Batch Files to Shell Scripts](http://www.tldp.org/LDP/abs/html/dosbatch.html)

[Differences between windows batch and linux bash shell script syntax](http://www.xoogu.com/2012/differences-between-windows-batch-and-linux-bash-shell-script-syntax/#bash-batch-for)

[Command Line Interpreters: POSIX Shell, Cmd.exe, PowerShell](http://hyperpolyglot.org/shell)



## Sum:

If you are using Windows，I strong recommend you to install [Winix](https://github.com/kiddlu/Winix)，my excellent Windows environment,then you can write shell script on Windows, enjoy！！
