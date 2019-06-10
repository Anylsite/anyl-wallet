#!/bin/bash
set -e
set -x

update-alternatives --install /usr/bin/python python /usr/bin/python2.7 1
update-alternatives --install /usr/bin/python python /usr/bin/python3 2
pip install protobuf

Zephyr_RTOS=zephyr_rtos
Zephyr_SDK=zephyr_sdk
PROJECT_DIR=/home/circleci/project

#Reset to the home directoryy
cd ~

#Set the enviornment for execution
source ${Zephyr_RTOS}/zephyr/zephyr-env.sh
export ZEPHYR_TOOLCHAIN_VARIANT=zephyr
export ZEPHYR_SDK_INSTALL_DIR=/opt/sdk/zephyr-sdk

# cd to the samples directory
cd ${Zephyr_RTOS}/zephyr/samples/

# clone the anyledger repo
if [ -d $PROJECT_DIR ];then
    # for CI use directory which contains already cloned source
    git clone /home/circleci/project anyledger-wallet
else
    git clone -b st_fixes https://github.com/pcppcp/anyledger-wallet.git anyledger-wallet
fi
WALLET_DIR=`pwd`/anyledger-wallet/examples/wallet
BUILD_DIR=$WALLET_DIR/build
mkdir -p $BUILD_DIR

# build the wallet for x86
cd $BUILD_DIR && mkdir x86 && cd x86
cmake -DCMAKE_CXX_FLAGS="-Werror $CMAKE_CXX_FLAGS" -DCMAKE_C_FLAGS="-Werror $CMAKE_C_FLAGS" -GNinja -DBOARD=qemu_x86 -DBUILD_XCOMPILE=1 $WALLET_DIR
ninja
# build the wallet for nrf52
cd $BUILD_DIR && mkdir nrf52 && cd nrf52
cmake -DCMAKE_CXX_FLAGS="-Werror $CMAKE_CXX_FLAGS" -DCMAKE_C_FLAGS="-Werror $CMAKE_C_FLAGS" -GNinja -DBOARD=nrf52840_pca10056 -DBUILD_XCOMPILE=1 $WALLET_DIR
ninja
