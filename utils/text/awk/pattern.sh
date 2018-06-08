#!/bin/bash

txt_file=`mktemp --suffix=.txt`
trap "rm -f $txt_file" EXIT

#txt
cat <<'TXTEND' > $txt_file
SYSTEM:*:18:544:,S-1-5-18::
LocalService:*:19:544:U+NT AUTHORITY\LocalService,S-1-5-19::
NetworkService:*:20:544:U-NT AUTHORITY\NetworkService,S-1-5-20::
administrators:*:544:544:,S-1-5-32-544::
Administrator:unused:500:513:U-ACER-4750G\Administrator,S-1-5-21-980505181-3150591650-1979602488-500:/home/mobaxterm:/bin/bash
root:unused:1021:513:U-ACER-4750G\root,S-1-5-21-980505181-3150591650-1979602488-1021:/home/mobaxterm:/bin/bash
huangxiaolu:unused:1000:513:U-ACER-4750G\huangxiaolu,S-1-5-21-980505181-3150591650-1979602488-1000:/home/mobaxterm:/bin/bash
TXTEND

cat $txt_file | awk "/huangxiaolu/"'{ print $0 }' -

cat $txt_file | awk '/huangxiaolu/{ print $0 }' -

cat $txt_file | awk '/U.NT/{ print }' -

cat $txt_file | awk '/[aA]dmin[a-zA-Z 0-9]/{ print }' -

cat $txt_file | awk '/^L/{ print }' -
cat $txt_file | awk '/h$/{ print }' -

cat $txt_file | awk '/:\*:/{ print }' -
