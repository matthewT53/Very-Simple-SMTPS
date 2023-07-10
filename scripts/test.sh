#!/bin/bash

if [ ! -d test_data ]; then
    mkdir test_data
fi
cd .conan ; meson test -v
