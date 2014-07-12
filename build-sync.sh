#! /bin/bash

rm -rf Build
mkdir Build

g++ -c  "./sync.cpp" -std=gnu++0x  -g -O0 -Wall  -o ./Build/sync.o -I. -I. -I/usr/local/include -I/usr/include/mysql -I/usr/include
g++ -o ./Build/sync ./Build/sync.o -L. -L/usr/local/lib -L/usr/lib/mysql -L/usr/lib -lmysqlclient -lconfig