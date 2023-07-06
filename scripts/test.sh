#!/bin/bash

if [ ! -d test_data ]; then
    mkdir test_data
fi
./.conan/tests/smtp_tests -c
