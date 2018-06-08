#!/bin/bash

#grep -o Print only the matched (non-empty) parts of a matching line, with each such part on a separate output line.

echo $1 | grep -o . | sort | tr -d '\n'; echo
