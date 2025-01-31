#!/bin/bash -eux

$CXX $CXXFLAGS $LIB_FUZZING_ENGINE\
    -I/usr/local/lib/clang/18/include\
    fuzzer.cpp huffman.cpp \
    -o $OUT/huffman_fuzzer
