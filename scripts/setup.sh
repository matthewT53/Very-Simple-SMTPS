#!/bin/sh

# Note: If libcurl4-gnutls-dev cannot be found then install libcurl4-openssl-dev instead and vice versa.
apt-get update && apt-get upgrade -y
apt-get -y install python3-pip ninja-build cpputest libcurl4 libcurl4-gnutls-dev python3.11-venv
