#!/bin/bash

txt_file=`mktemp --suffix=.txt`
trap "rm -f $txt_file" EXIT

#txt
cat <<'TXTEND' > $txt_file
BEGIN old
Mouse 20,000 1 20,000
Monitor 500,000 1 500,000 old
RAM_Chips 150,000 2 300,000     

#hahaha
Ethernet_Cables 30,000 4 120,000 old
END old
TXTEND


#Grouping with { and } like awk
sed -n '
	/BEGIN/,/END/ {
	     s/#.*//
	     s/[ ^I]*$//
	     /^$/ d
	     p
	}
' $txt_file

#sed -n '
#	1,100 {
#		/begin/,/end/ {
#		     s/#.*//
#		     s/[ ^I]*$//
#		     /^$/ d
#		     p
#		}
#	}
#'

echo ""
sed '
    /BEGIN/,/END/ {
      /BEGIN/n # skip over the line that has "begin" on it
      /END/ !{
        s/old/new/
      }
    }
' $txt_file
