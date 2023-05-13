#!/bin/bash

if [ ! -d test_data ]; then
    mkdir test_data
fi
./build/tests/smtp_tests -c
