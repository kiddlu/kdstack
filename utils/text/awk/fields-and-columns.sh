#!/bin/bash

cat `readlink -f $0`

uname -a | awk '//{ print $0 }'
uname -a | awk '//{ print $1  $2  $4}'
uname -a | awk '//{ print $1, $2, $4 "\n"}'
#uname -a | awk '//{ printf $1 ""hello world"" $2, $4, "\n"}'

txt_file=`mktemp --suffix=.txt`
trap "rm -f $txt_file" EXIT

#txt
cat <<'TXTEND' > $txt_file
No	Item_Name		Unit_Price	Quantity	Price
1	Mouse			#20,000		   1		#20,000
2 	Monitor			#500,000	   1		#500,000
3	RAM_Chips		#150,000	   2		#300,000
4	Ethernet_Cables	        #30,000		   4		#120,000
TXTEND

awk '//{ print $2, $3 }' $txt_file
echo ""
awk '//{printf "%-10s %s\n",$2, $3 }' $txt_file
################# END #################
