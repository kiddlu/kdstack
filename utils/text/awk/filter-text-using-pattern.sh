#!/bin/bash

txt_file=`mktemp --suffix=.txt`
trap "rm -f $txt_file" EXIT

#txt
cat <<'TXTEND' > $txt_file
No	Item_Name		Quantity	Price
1	Mangoes			   10		$2.45
2	Apples			   20		$1.50
3	Bananas			   5		$0.90
4	Pineapples		   10		$3.46
5	Oranges			   10		$0.78
6	Tomatoes		   5		$0.55
7	Onions			   5            $0.45
TXTEND

awk '/ *\$[2-9]\.[0-9][0-9] */ { print $0 "*" ; } / *\$[0-1]\.[0-9][0-9] */ { print ; }' $txt_file

echo ""
awk '/ *\$[2-9]\.[0-9][0-9] */ { printf "%-10s %-10s %-10s %-10s\n", $1, $2, $3, $4 "*" ; } / *\$[0-1]\.[0-9][0-9] */ { printf "%-10s %-10s %-10s %-10s\n", $1, $2, $3, $4; }' $txt_file
