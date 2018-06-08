#!/usr/bin/python

import sys

platform='unknown'

if sys.platform == 'linux2':
    platform='linux'
elif sys.platform == 'darwin':
    platform='macos'
elif sys.platform == 'win32':
    platform='windows'
elif sys.platform == 'cygwin':
    platform='cygwin'
else:
    platform=sys.platform

print('platform is ' + platform)
