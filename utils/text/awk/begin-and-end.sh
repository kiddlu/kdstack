#!/bin/bash

txt_file=`mktemp --suffix=.txt`
trap "rm -f $txt_file" EXIT

#txt
cat <<'TXTEND' > $txt_file
news.tecmint.com
tecmint.com
linuxsay.com
windows.tecmint.com
tecmint.com
news.tecmint.com
tecmint.com
linuxsay.com
tecmint.com
news.tecmint.com
tecmint.com
linuxsay.com
windows.tecmint.com
tecmint.com
TXTEND

awk '
    BEGIN { print "The number of times tecmint.com appears in the file is:" ; }
    /^tecmint.com/ { counter += 1; }
    END { printf "%s\n", counter; }
' $txt_file
