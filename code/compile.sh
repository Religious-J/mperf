#/bin/bash

gcc -shared -fpic -o mperf.so ./mperf.cc -ldl -Og -g
