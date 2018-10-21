#!/bin/bash

Zephyr_RTOS=zephyr_rtos
Zephyr_SDK=zephyr_sdk

#Set the enviornment for execution
source ${Zephyr_RTOS}/zephyr/zephyr-env.sh
export ZEPHYR_TOOLCHAIN_VARIANT=zephyr
export ZEPHYR_SDK_INSTALL_DIR=/opt/sdk/zephyr-sdk

cd ${Zephyr_RTOS}/zephyr/samples/

# Clone the repo
git clone https://github.com/AnyLedger/anyledger-wallet.git

cd anyledger-wallet/
mkdir build && cd build/


cmake -GNinja -DBOARD=nrf52840_pca10056 -DBUILD_XCOMPILE=1 ../
ninja
