@echo off

:: %* in a batch script refers to all the arguments (e.g. %1 %2 %3 %4 %5 ...%255)
:: only arguments %1 to %9 can be referenced by number.

echo.
echo parameter
echo %0

echo.
echo ~f Expand to a Fully qualified path name
echo %~f0

echo.
echo ~d Expand to a Drive letter only
echo %~d0

echo.
echo ~p Expand to a Path only
echo %~p0

echo.
echo ~n Expand to a file Name without file extension
echo %~n0

echo.
echo ~x Expand to a file eXtension only
echo %~x0

echo.
echo ~s Change the meaning of f, n, s and x to reference the Short 8.3 name
echo %~s0

echo.
echo ~ Expand removing any surrounding quotes
echo %~0

echo.
echo ~a Display the file attributes
echo %~a0

echo.
echo ~t Display the date/time
echo %~t0

echo.
echo ~z Display the file size
echo %~z0

echo.
echo ~\$PATH: Search the PATH environment variable and expand %1 to the fully qualified name of the first match found.
echo %~$PATH:0

echo.
echo The modifiers above can be combined:

echo ~dp Expand to a drive letter and path only
echo %~dp0

echo ~sp Expand to a path shortened to 8.3 characters
echo %~sp0

echo ~nx Expand to a file name and extension only
echo %~nx0
