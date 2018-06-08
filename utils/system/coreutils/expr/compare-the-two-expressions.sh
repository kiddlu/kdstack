#!/bin/bash

var1='10'
var2='20'

# matching numbers with '='.
expr $var1 = $var2
#0

# displays 1 when arg1 is less than arg2
expr $var1 \< $var2
#1

# display 1 when arg1 is not equal to arg2
expr $var1 \!= $var2
#1
