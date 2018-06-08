rem "if"
if 1==1 (
   dir
   echo 1==1!
) >> "somefile.txt"

rem "if/else"
if 1==1 (
   dir
   echo 1==1!
) >> "somefile.txt" else (
   echo what
) >> "somefile.txt"

rem "for"
for %%a in (1 2 3) do (
    echo %%a
) >> "somefile.txt"

rem standalone codeblock
(
    Echo All of this
    echo only gets
    echo redirected once!
) >> "somefile.txt"