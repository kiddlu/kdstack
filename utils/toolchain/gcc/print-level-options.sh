#!/bin/sh

gcc -Q --help=optimizers -O
gcc -Q --help=optimizers -O1
gcc -Q --help=optimizers -O2
gcc -Q --help=optimizers -O3
gcc -Q --help=optimizers -Og
gcc -Q --help=optimizers -Os
gcc -Q --help=optimizers -Ofast
