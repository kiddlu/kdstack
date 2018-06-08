#!/bin/bash

#s	  Substitute command
#/../../	  Delimiter
#day	  Regular Expression Pattern Search Pattern
#night	  Replacement string
echo day | sed 's/day/night/'
echo Sunday | sed 's/day/night/'

#delimiter
echo '/usr/local/bin' | sed 's/\/usr\/local\/bin/\/common\/bin/'
echo '/usr/local/bin' | sed 's_/usr/local/bin_/common/bin_'
echo '/usr/local/bin' | sed 's:/usr/local/bin:/common/bin:'
echo '/usr/local/bin' | sed 's|/usr/local/bin|/common/bin|'

#Using & as the matched string
echo '123 abc' | sed 's/[0-9][0-9]*/& &/'
echo '123 abc' | sed 's/[a-z][a-z]*/(&)/'
echo '123 abc' | sed -r 's/[0-9]+/& &/'
