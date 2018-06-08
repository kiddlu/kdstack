#!/bin/bash

cat `readlink -f $0`

#Exuberant Ctags
ctags -R -f .ctags > /dev/null 2>&1
rm .ctags

#cscope -Rbq -f .cscope
