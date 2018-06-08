#!/bin/bash

cd $1 && tar cvf - . | ( cd $2 && tar xvf - )
