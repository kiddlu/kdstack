#!/bin/bash

txt_file=`mktemp --suffix=.txt`
trap "rm -f $txt_file" EXIT

#txt
cat <<'TXTEND' > $txt_file
No	Item_Name		Unit_Price	Quantity	Price
1	Mouse			#20,000		   1		#20,000
2 	Monitor			#500,000	   1		#500,000
3	RAM_Chips		#150,000	   2		#300,000
4	Ethernet_Cables	        #30,000		   4		#120,000
4	Ethernet_Cables	        #30,000		   4		#120,000
4	Ethernet_Cables	        #30,000		   4		#120,000
4	Ethernet_Cables	        #30,000		   4		#120,000
TXTEND

#FILENAME : current input file name( do not change variable name)
#FR : number of the current input line (that is input line 1, 2, 3… so on, do not change variable name)
#NF : number of fields in current input line (do not change variable name)
#OFS : output field separator
#FS : input field separator
#ORS : output record separator
#RS : input record separator

awk '
    NR==4 {
        print $0
    }
    END {
        print FILENAME
        print "Number of records in file is: ", NR
        print "Record:",NR,"has",NF,"fields"
    }
' $txt_file

echo ""

cat <<'TXTEND' > $txt_file
SYSTEM:*:18:544:,S-1-5-18::
LocalService:*:19:544:U-NT AUTHORITY\LocalService,S-1-5-19::
NetworkService:*:20:544:U-NT AUTHORITY\NetworkService,S-1-5-20::
Administrators:*:544:544:,S-1-5-32-544::
Administrator:unused:500:513:U-ACER-4750G\Administrator,S-1-5-21-980505181-3150591650-1979602488-500:/home/mobaxterm:/bin/bash
root:unused:1021:513:U-ACER-4750G\root,S-1-5-21-980505181-3150591650-1979602488-1021:/home/mobaxterm:/bin/bash
huangxiaolu:unused:1000:513:U-ACER-4750G\huangxiaolu,S-1-5-21-980505181-3150591650-1979602488-1000:/home/mobaxterm:/bin/bash
TXTEND

awk '
    BEGIN {
        FS=":" ;
    }
    {
        print $1, $4;
    }
' $txt_file
echo ""

awk -F':' '
    BEGIN {
        OFS="==>";
    }
    //{
        print $1, $4 ;
    }
' $txt_file
