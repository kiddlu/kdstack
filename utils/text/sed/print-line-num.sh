#!/bin/bash

txt_file=`mktemp --suffix=.txt`
trap "rm -f $txt_file" EXIT

cat <<'TXTEND' > $txt_file
BEGIN old
Mouse 20,000 1 20,000
Monitor 500,000 1 500,000 old
RAM_Chips 150,000 2 300,000     

#hahaha
Ethernet_Cables 30,000 4 120,000 old
END old
TXTEND

# add line numbers first,
# then use grep, 
# then just print the number
cat -n $txt_file | grep 'haha' | awk '{print $1}'
sed -n '/haha/ =' $txt_file


#find the number of lines in a file
wc -l $txt_file | awk '{print $1}'

#at last line, print line num
#sed -n '${=}' $txt_file
sed -n '$=' $txt_file
