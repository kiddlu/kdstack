#!/bin/bash

cat `readlink -f $0`

txt_file=`mktemp --suffix=.txt`
trap "rm -f $txt_file" EXIT

#txt
cat <<'TXTEND' > $txt_file
Sarah L			35    	F
Aaron Kili		40    	M
John  Doo		20    	M
Kili  Seth		49    	M
TXTEND

awk '{
    if ( $3 <= 25 ) {
        print "User",$1,$2,"is less than 25 years old" ;
    } else {
        print "User",$1,$2,"is more than 25 years old" ; 
    }
}' $txt_file

################# END #################
