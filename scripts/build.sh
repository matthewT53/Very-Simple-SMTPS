#!/bin/bash

if [[ $1 == "release" ]]; then
  conan install -of .conan . --build missing -pr ./profiles/release.profile
  conan build -of .conan . -pr ./profiles/release.profile
else
  conan install -of .conan . --build missing -pr ./profiles/debug.profile
  conan build -of .conan . -pr ./profiles/debug.profile
fi
