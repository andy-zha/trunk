#!/bin/bash

rm -rf build
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=DEBUG -DCMAKE_BUILD_RPATH=/opt/ide ..
make
