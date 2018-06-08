#!/bin/bash

txt_file=`mktemp --suffix=.txt`
trap "rm -f $txt_file" EXIT

#txt
cat <<'TXTEND' > $txt_file
No      Name                                    Price           Type
1       Mac_OS_X_Cleanup_Suite                  $9.99           Software
2       Basics_Notebook                         $14.99          Lifestyle
3       Tactical_Pen                            $25.99          Lifestyle
4       Scapple                                 $19.00          Unknown
5       Nano_Tool_Pack                          $11.99          Unknown
6       Ditto_Bluetooth_Altering_Device         $33.00          Tech
7       Nano_Prowler_Mini_Drone                 $36.99          Tech
TXTEND

#In Awk, compound expressions are built using the && referred to as (and) and the || referred to as (or) compound operators.

awk '
    ($3 ~ /^\$[2-9][0-9]*\.[0-9][0-9]$/) &&
    ($4=="Lifestyle") {
        printf "%s\t%s\n",$0,"*";
    }
' $txt_file
