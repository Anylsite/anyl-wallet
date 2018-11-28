#!/bin/bash
set -e

update-alternatives --install /usr/bin/python python /usr/bin/python2.7 1
update-alternatives --install /usr/bin/python python /usr/bin/python3 2
pip install protobuf

Zephyr_RTOS=zephyr_rtos
Zephyr_SDK=zephyr_sdk

#Reset to the home directoryy
cd ~

#Set the enviornment for execution
source ${Zephyr_RTOS}/zephyr/zephyr-env.sh
export ZEPHYR_TOOLCHAIN_VARIANT=zephyr
export ZEPHYR_SDK_INSTALL_DIR=/opt/sdk/zephyr-sdk

cd ${Zephyr_RTOS}/zephyr/samples/

# clone the repo
git clone /home/circleci/project anyledger-wallet
cd anyledger-wallet

cd examples/wallet/ && mkdir build && cd build/
cmake -DCMAKE_CXX_FLAGS="-Werror $CMAKE_CXX_FLAGS" -DCMAKE_C_FLAGS="-Werror $CMAKE_C_FLAGS" -GNinja -DBOARD=nrf52840_pca10056 -DBUILD_XCOMPILE=1 ../
ninja
