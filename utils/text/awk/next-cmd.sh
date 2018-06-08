#!/bin/bash

txt_file=`mktemp --suffix=.txt`
trap "rm -f $txt_file" EXIT

#txt
cat <<'TXTEND' > $txt_file
No      Item_Name               Price           Quantity
1       Mangoes                 $3.45              5
2       Apples                  $2.45              25
3       Pineapples              $4.45              55
4       Tomatoes                $3.45              25
5       Onions                  $1.45              15
6       Bananas                 $3.45              30
TXTEND

#In Awk, compound expressions are built using the && referred to as (and) and the || referred to as (or) compound operators.

awk '
    $4 <= 20 {
        printf "%s\t%s\n", $0,"*" ;
        next;
    }

    $4 > 20 {
        print $0 ;
    }
' $txt_file
