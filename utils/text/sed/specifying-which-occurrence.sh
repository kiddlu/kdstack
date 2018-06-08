#!/bin/bash


txt_file=`mktemp --suffix=.txt`
trap "rm -f $txt_file" EXIT

#txt
cat <<'TXTEND' > $txt_file
Mouse 20,000 1 20,000
Monitor 500,000 1 500,000
RAM_Chips 150,000 2 300,000
Ethernet_Cables 30,000 4 120,000
TXTEND

sed 's/[^ ]*/(&)/' $txt_file
echo ""
sed 's/[^ ][^ ]*/(&)/g' $txt_file

echo ""
sed 's/\([a-zA-Z]*\)\([a-zA-Z]*\) /\2/' $txt_file
echo ""
sed 's/[a-zA-Z]* //2' $txt_file

echo ""
sed 's/[a-zA-Z]*/DELETED/2' $txt_file
echo ""
sed 's/[^ ]*//2' $txt_file


cat <<'TXTEND' > $txt_file
SYSTEM:*:18:544:,S-1-5-18::
LocalService:*:19:544:U-NT AUTHORITY\LocalService,S-1-5-19::
NetworkService:*:20:544:U-NT AUTHORITY\NetworkService,S-1-5-20::
Administrators:*:544:544:,S-1-5-32-544::
Administrator:unused:500:513:U-ACER-4750G\Administrator,S-1-5-21-980505181-3150591650-1979602488-500:/home/mobaxterm:/bin/bash
root:unused:1021:513:U-ACER-4750G\root,S-1-5-21-980505181-3150591650-1979602488-1021:/home/mobaxterm:/bin/bash
huangxiaolu:unused:1000:513:U-ACER-4750G\huangxiaolu,S-1-5-21-980505181-3150591650-1979602488-1000:/home/mobaxterm:/bin/bash
TXTEND

echo ""
sed 's/[^:]*//2' $txt_file
echo ""
sed 's/[^:]*:/:/2' $txt_file

echo ""
sed 's/[^:]*//1' $txt_file

echo ""
sed 's/./&#######/10' $txt_file
#sed 's/^........../&#######/'
