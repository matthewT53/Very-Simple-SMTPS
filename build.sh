#!/bin/bash

meson build && cd build 
ninja

if [[ $? -eq 0 ]]; then 
    echo "[+] Running unit tests"
    ./tests/smtp_tests -c

    if [[ $? -eq 0 ]]; then 
        echo "[+] All tests passed!"
        echo "[+] Running examples: "
        ./examples/examples
    else 
        echo "[!] Unit tests failed, not running examples!"
    fi
fi