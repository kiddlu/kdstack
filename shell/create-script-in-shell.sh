#!/use/bin/env bash
source ~/.winixrc

# This script shows how to create and exe
# cmd/py/awk/other dynamic language script in shell

echo ""
echo "This is shell script $@ on"
bash --version | grep bash

cmd_file=`mktemp --suffix=.cmd`
py_file=`mktemp --suffix=.py`
pl_file=`mktemp --suffix=.pl`
trap "rm -f $cmd_file $py_file $pl_file" EXIT

#cmd
cat <<'CMDEND' > $cmd_file
@echo off
echo;
echo This is windows cmd %* on
echo %OS%
CMDEND

cmd /c `cygpath -w $cmd_file` $@

#python
cat <<'PYEND' > $py_file
#/usr/bin/env python

import sys
print 
print ("This is python script %s on" % sys.argv[1:])
print sys.platform
PYEND

python $py_file $@

#perl
cat <<'PLEND' > $pl_file
#/usr/bin/env perl

print "\n";
print "This is perl script $ARGV on\n";
print "version $] $^O\n";
PLEND

perl $pl_file $@
