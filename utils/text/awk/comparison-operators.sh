#!/bin/bash

txt_file=`mktemp --suffix=.txt`
trap "rm -f $txt_file" EXIT

#txt
cat <<'TXTEND' > $txt_file
No      Item_Name               Quantity        Price
1       Mangoes                    45           $3.45
2       Apples                     25           $2.45
3       Pineapples                 5            $4.45
4       Tomatoes                   25           $3.45
5       Onions                     15           $1.45
6       Bananas                    30           $3.45
TXTEND



#> - greater than
#< - less than
#>= - greater than or equal to
#<= - less than or equal to
#== - equal to
#!= - not equal to
#some_value ~ / pattern/ - true if some_value matches pattern
#some_value !~ / pattern/ - true if some_value does not match pattern


awk '
    $3 <= 25 { printf "%s\t%s\n", $0,"**" ; };
    $3 > 25 { print $0 ;}
' $txt_file
