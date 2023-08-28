# Very-Simple-SMTPS

[![Build Status](http://ec2-52-63-33-178.ap-southeast-2.compute.amazonaws.com:31580/api/badges/matthewT53/Very-Simple-SMTPS/status.svg?ref=refs/heads/main)](http://ec2-52-63-33-178.ap-southeast-2.compute.amazonaws.com:31580/matthewT53/Very-Simple-SMTPS)

![smtps-image](docs/readme_image.jpg)

## Contents:
- [About](#about)
- [Features](#features)
- [How to use](#how-to-use)
    - [Conancenter](#conancenter)
    - [Download release](#download-a-release)
    - [Example usage](#example-usage)
- [How to build](#how-to-build)
    - [Scripts](#scripts)
    - [Building](#building)
    - [Testing](#testing)
    - [Example](#example)
    - [Clean up](#clean-up)
- [SMTPS server providers](#smtps-server-providers)
    - [Amazon](#amazon-ses)
    - [Google](#google)

## About:

A small library written in modern C++ that can be used to send emails with binary attachments.

## Features:
### SMTP authentication:

This library currently only supports the `LOGIN/PLAIN` SMTP authentication method which means you will need to supply your `username` and `password` for your email account when you're sending emails with this library.

## How to use:

This library can be integrated into your project by using the `conan package manager` or by downloading a release and copying this library's code into your project.

### Conancenter:
This is still a work in progress. Submitted a PR for this [here](https://github.com/conan-io/conan-center-index/pull/19398)

### Download a release:
Grab the latest release from [here](https://github.com/matthewT53/Very-Simple-SMTPS/releases/tag/v1.0.0).

Build it using the instructions below.

Copy over the include folder into your own project and copy either the `libsmtp_lib.so` or `libsmtp_lib.a` library file under `.conan/src` into your own project and link with it.

### Example usage:
```c++
#include "attachment/attachment.hpp"
#include "email/email.hpp"

using namespace smtp;

int main(void) {
  EmailParams params{
      "gmail_account@gmail.com",         // smtp username
      "google_app_password",             // smtp password
      "smtps://smtp.gmail.com:465",      // smtp server
      "target@gmail.com",                // to
      "gmail_account@gmail.com",         // from
      "",                                // cc
      "Testing sending attachments",     // subject
      "Hey listen friend here are some attachments for you to play with!", // body
  };
  Email e{params};

  Attachment a{"mountain-beach.jpg"};
  Attachment a2{"mountain-beach2.jpg"};
  e.addAttachment(a);
  e.addAttachment(a2);

  e.send();

  return 0;
}
```

## How to build:
### Scripts:

The scripts below need to be run from the project root i.e `./Very-Simple-SMTPS`

```bash
$ ./scripts/venv.sh     # Creates or starts the python virtual environment required to build and run tests
$ ./scripts/setup.sh    # Installs tools required to build and run this project
$ ./scripts/build.sh    # Builds the library
$ ./scripts/test.sh     # Runs the tests
$ ./scripts/cleanup.sh  # Removes build artifacts
```

### Building:
- [Optional] Consider running the `./scripts/setup.sh` script to install tools required for building.
- To build, this project relies on the [conan package manager](https://docs.conan.io/1/introduction.html) and [meson](https://mesonbuild.com/).
- The virtual environment needs to be created and the user needs to have activated this virtual environment before this library can be built.
```bash
# create or activate python virtual env
$ source ./scripts/venv.sh

# build the library and tests in DEBUG mode by default
$ ./scripts/build.sh

# build the library and tests in RELEASE mode
$ ./scripts/build.sh release
```

### Testing:
- The unit testing framework used is [doctest](https://github.com/doctest/doctest)
- To run the tests:
```bash
$ ./scripts/test.sh
```

### Example:
- There is an example of how to send an email with attachments under `./examples/send_attachments.cpp`
- Fill in the credentials and you should be good to go!
- To run the example:
```bash
$ ./.conan/examples/examples
```

### Clean up:
- Remove all build artifacts:
```bash
$ ./scripts/cleanup.sh
```

## SMTPS server providers:
### Amazon SES:
- This library has been tested against Amazon SES's SMTPS servers.
- Sender and receiver email addresses do need to be registered and verified in the AWS SES product.
- One of the many SMTPS servers: `smtps://email-smtp.ap-southeast-2.amazonaws.com:465`

### Google:
- Google doesn't seem to allow access to `less secure apps` anymore.
- To use Google's SMTPS server, two step verification needs to be enabled on the Gmail account.
- Once this has been enabled, we can create an `App password`.
- This password will be used as the SMTP password.
- The SMTP username is the Gmail address.
- SMTPS server: `smtps://smtp.gmail.com:465`
