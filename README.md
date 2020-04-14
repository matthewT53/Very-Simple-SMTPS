# Very-Simple-SMTPS
[![Build status](https://ci.appveyor.com/api/projects/status/914ruifjnopxs3gy?svg=true)](https://ci.appveyor.com/project/matthewT53/very-simple-smtps)
A small library written in c++ that uses libcurl to be able to send emails with binary attachments.

## Building:

### Dependencies:
#### Installing Meson:
* Official installation guide: https://mesonbuild.com/Quick-guide.html
```sh
    pip install meson 
```

#### Installing ninja:
```sh
    $ sudo apt-get install python3 python3-pip python3-setuptools python3-wheel ninja-build
```

#### CppUTest and libcurl:
* To send emails to a smtp server, we rely on libcurl.
* Our unit tests require the CppUTest unit testing framework found here: https://cpputest.github.io/
```sh
    # libcurl:
    sudo apt-get install libcurl4-openssl-dev

    # cpputest for unit testing:
    sudo apt-get install cpputest
```

## Examples:
* To try out the examples, you will need meson and ninja set up on your system.
* To build and run simply, run the commands:
```sh
    # Meson will create the build directory and running ninja will build this library as well as both 
    # the unit tests and the examples.
    $ meson build ; cd build 
    $ ninja 

    # To run the examples:
    $ cd examples ; ./examples 

    # To run the unit tests:
    # Make sure you are in the 'build' directory
    $ cd tests ; ./smtp_tests
```

## Contributing:
#### Coding standards:


## Using in your project:
* Copy the headers and src directories into your project.
