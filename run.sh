#!/bin/bash
rm -f match.log
rm -f trace.txt
/usr/bin/time -o test.time gomuko -m 100 -f first.in -s second.in -l match.log -t trace.txt > test.out
