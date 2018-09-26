#!/bin/sh
set -e
apt-get update
apt-get install -y libgtest-dev g++ make cmake wget build-essential clang clang-tidy-3.8 ninja-build
pip install -r requirements.txt
# ln -s /usr/bin/python3 /usr/local/bin/python
cd /usr/src/gtest && cmake . && make && cp *a /usr/local/lib/
