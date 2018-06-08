#!/bin/sh

UNC_PATH=$1
cmd /k "pushd $UNC_PATH && cmder.bat"
