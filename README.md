# Very-Simple-SMTPS

[![Build Status](http://ec2-52-63-33-178.ap-southeast-2.compute.amazonaws.com:31580/api/badges/matthewT53/Very-Simple-SMTPS/status.svg?ref=refs/heads/main)](http://ec2-52-63-33-178.ap-southeast-2.compute.amazonaws.com:31580/matthewT53/Very-Simple-SMTPS)

A small library written in C++ that uses libcurl to be able to send emails with binary attachments.

## Using in your project:
* Simply copy everything headers and src files into your own project.
* Also remove the meson.build files that are in each folder.
* **Note:** This library relies on the curl library, so you would need to install libcurl:
```sh
    sudo apt-get install libcurl4 libcurl4-openssl-dev # or libcurl4-gnutls-dev
```
* If you are on Windows, then you need to download the library file (*.a) and link to it.
* **NOTE:** When sending attachments, the fullpath to the file must be provided!

### Working with email providers:
* If you are using this library with gmail smtp then **you will need to enable access to less secure apps in your google account settings.**
* These links should help:
1. https://support.google.com/accounts/answer/6010255?hl=en
2. https://docs.bitnami.com/aws/faq/troubleshooting/troubleshoot-smtp-gmail/

## Examples:
* To run the examples, a few more dependencies will be required.
* To install these dependencies, please run the setup.sh script:
```sh
    # Installs python3 tools, ninja, cpputest as well as libcurl.
    $ sudo ./scripts/setup.sh
```
* **Note:** The recommendation is to execute this script while inside a python virtual environment.

## Building:
* To **build** the tests and the examples, **you will need gcc-9 or newer** to compile the latest C++17 features.
* To change the compiler, set the CC and CXX environment variables in scripts/build.sh:
```bash
#!/bin/bash

# Set the GCC compiler to version 8+ for C++17 features to work.
export CC=gcc-9     # Change it here
export CXX=g++-9    # And here

meson build && cd build
ninja
```
* **Note:** The build script must be run from the project's root directory.
```sh
    $ ./scripts/build.sh
```
* To run the unit tests:
```sh
    $ ./scripts/run_tests.sh
```

## Contributing:
#### Coding standards:
* See: https://github.com/lefticus/cppbestpractices
