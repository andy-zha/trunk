#!/bin/bash

rm -rf build
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=GTEST -DCMAKE_BUILD_RPATH=/opt/ide ..

# 生成可执行文件，不生成gcov文件
#make
#生成可执行文件，且生成gcov文件
make gen_coverage_html
