# Very-Simple-SMTPS

[![Build status](https://ci.appveyor.com/api/projects/status/914ruifjnopxs3gy?svg=true)](https://ci.appveyor.com/project/matthewT53/very-simple-smtps)

A small library written in C++ that uses libcurl to be able to send emails with binary attachments.

## Using in your project:
* Simply copy everything headers and src files into your own project. 
* Also remove the meson.build files that are in each folder.
* **Note:** This library relies on the curl library, so yo uwould need to install libcurl:
```sh
    sudo apt-get install libcurl4 libcurl4-openssl-dev
```

## Examples:
* To run the examples, a few more dependencies will be required.
* To install these dependencies, please run the setup.sh script:
```sh
    # Installs python3 tools, ninja, cpputest as well as libcurl.
    $ sudo ./setup.sh
```
* To **build** the tests and the examples:
```sh
    $ ./build.sh
```

## Contributing:
#### Coding standards:
* Just keep it consistent with the excisting code please.
