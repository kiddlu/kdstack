#!/bin/bash

txt_file=`mktemp --suffix=.txt`
trap "rm -f $txt_file" EXIT

#txt
cat <<'TXTEND' > $txt_file
SYSTEM:*:18:544:,S-1-5-18::
LocalService:*:19:544:U-NT AUTHORITY\LocalService,S-1-5-19::
NetworkService:*:20:544:U-NT AUTHORITY\NetworkService,S-1-5-20::
Administrators:*:544:544:,S-1-5-32-544::
Administrator:unused:500:513:U-ACER-4750G\Administrator,S-1-5-21-980505181-3150591650-1979602488-500:/home/mobaxterm:/bin/bash
root:unused:1021:513:U-ACER-4750G\root,S-1-5-21-980505181-3150591650-1979602488-1021:/home/mobaxterm:/bin/bash
huangxiaolu:unused:1000:513:U-ACER-4750G\huangxiaolu,S-1-5-21-980505181-3150591650-1979602488-1000:/home/mobaxterm:/bin/bash
TXTEND

#-v – Awk option to declare a variable

#read user input
read -p "Please enter username:" username
#search for username in /etc/passwd file and print details on the screen
cat $txt_file | awk "/$username/"'{ print $0 }'
cat $txt_file | awk -v name="$username" ' $0 ~ name {print $0}'
