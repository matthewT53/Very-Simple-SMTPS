#!/bin/bash

# Set the GCC compiler to version 8+ for C++17 features to work.
export CC=gcc-9
export CXX=g++-9

meson build && cd build
ninja
