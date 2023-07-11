#!/bin/bash

if [ ! -d .conan/test_data ]; then
    mkdir .conan/test_data
fi
cd .conan ; meson test -v
