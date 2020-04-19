# Very-Simple-SMTPS

[![Build status](https://ci.appveyor.com/api/projects/status/914ruifjnopxs3gy?svg=true)](https://ci.appveyor.com/project/matthewT53/very-simple-smtps)

A small library written in C++ that uses libcurl to be able to send emails with binary attachments.

## Using in your project:
* Simply copy everything headers and src files into your own project. 
* Also remove the meson.build files that are in each folder.
* **Note:** This library relies on the curl library, so you would need to install libcurl:
```sh
    sudo apt-get install libcurl4 libcurl4-openssl-dev
```
* If you are on Windows, then you need to download the library file (*.a) and link to it.

## Examples:
* To run the examples, a few more dependencies will be required.
* To install these dependencies, please run the setup.sh script:
```sh
    # Installs python3 tools, ninja, cpputest as well as libcurl.
    $ sudo ./scripts/setup.sh
```
* **Note:** The recommendation is to execute this script while inside a python virtual environment.

## Building:
* To **build** the tests and the examples:
```sh
    $ ./scripts/build.sh
```
* To run the unit tests:
```sh
    $ ./scripts/run_tests.sh
```

## Contributing:
#### Coding standards:
* Just keep it consistent with the existing code please.
