#!/bin/bash
#./execute.sh
#shell script to compile all files in Linux Bash
g++ shmp1.cpp -o shmp1 -lpthread -lrt
g++ shmc1.cpp -o shmc1 -lpthread -lrt

#-lpthread is the thread library snecessary for POSIX compilation in Linux
#-lrt is the realtime extensions library also needed for POSIX compilation in Linux
