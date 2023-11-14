#!/bin/sh

apt-get update && apt-get upgrade -y
apt-get -y install python3-pip ninja-build python3.11-venv cmake build-essential
