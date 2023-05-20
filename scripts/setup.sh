#!/bin/sh

# Note: If libcurl4-gnutls-dev cannot be found then install libcurl4-openssl-dev instead and vice versa.
apt-get update && apt-get upgrade -y
apt-get -y install python3-pip ninja-build python3.11-venv cmake build-essential
