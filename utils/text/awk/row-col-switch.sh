#!/bin/bash

txt_file=`mktemp --suffix=.txt`
trap "rm -f $txt_file" EXIT

#txt
cat <<'TXTEND' > $txt_file
name age
alice 21
ryan 30
TXTEND

row=`sed -n '$=' $txt_file`
col=`awk 'NR==1{print NF}' $txt_file`

awk '
{
    for (i = 1; i <= NF; i++) {
		    if (NR == 1) {
			      result[i] = $i;
		    } else {
			      result[i] = result[i] " " $i
		    }
  	}
}

END {
    for (i = 1; result[i] != ""; i++) {
		  print result[i];
	  }
}' $txt_file
