#!/bin/sh

mkdir -p build
cd build
rm -rf libsimmetrics
git clone git://github.com/globin/libsimmetrics -b db-lab
cd libsimmetrics
./install.sh
cd ..
cmake ..
make
