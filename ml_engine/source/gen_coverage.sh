#!/bin/bash

cd build
./ml_gtest
lcov -d CMakeFiles/ml_gtest.dir/src -t ml_gtest -o 'ml_gtest.cov.info' -b . -c
genhtml -o cov_result ml_gtest.cov.info
