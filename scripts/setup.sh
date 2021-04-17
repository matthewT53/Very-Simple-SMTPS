#!/bin/sh

# Note: If libcurl4-gnutls-dev cannot be found then install libcurl4-openssl-dev instead and vice versa.
sudo apt-get --assume-yes --fix-missing install python3 python3-pip python3-setuptools python3-wheel ninja-build cpputest libcurl4 libcurl4-openssl-dev
sudo pip3 install meson