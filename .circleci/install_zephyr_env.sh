#!/bin/bash
set -e

Zephyr_RTOS=zephyr_rtos
Zephyr_SDK=zephyr_sdk

# Clone the Zephyr and install required dependencies
mkdir ${Zephyr_RTOS} && cd ${Zephyr_RTOS}
git clone https://github.com/zephyrproject-rtos/zephyr.git
pip3 install -r zephyr/scripts/requirements.txt
cd ../

# Get the Zephyr SDK
mkdir ${Zephyr_SDK} && cd ${Zephyr_SDK}
wget https://github.com/zephyrproject-rtos/meta-zephyr-sdk/releases/download/0.9.5/zephyr-sdk-0.9.5-setup.run
sh zephyr-sdk-0.9.5-setup.run --quiet -- -d /opt/sdk/zephyr-sdk
rm -rf zephyr-sdk-0.9.5-setup.run
cd ../
