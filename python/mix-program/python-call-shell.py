#!/usr/bin/env python

# 1
import subprocess

output = subprocess.Popen(['ls','-l'],stdout=subprocess.PIPE,shell=True).communicate()
print output[0]

# 2
import commands

return_code, output = commands.getstatusoutput('ls -l')
print output

# 3
import os

process = os.popen('ls -l') # return file
output = process.read()
print output
process.close()
