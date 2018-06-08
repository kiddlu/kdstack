#!/bin/bash


uname -a | awk '
    {
        hostname=$2;
        print hostname;
    }
'
