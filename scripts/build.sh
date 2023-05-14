#!/bin/bash

# conan install -of .conan . --build missing
# conan build -of .conan .

meson build && cd build
ninja
