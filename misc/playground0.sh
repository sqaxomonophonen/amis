#!/usr/bin/env bash
set -e
cd $(dirname $0)
cc -g -O0 playground0.c -o playground0
./playground0
sox -r 28800 -e signed -b 8 -c 1 playground0.raw -r 48000 -b 16 playground0.wav
mplayer playground0.wav

