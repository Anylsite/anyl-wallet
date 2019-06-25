# anyledger-wallet

## Overview

🏦 AnyLedger Embedded Wallet for Internet of Things

C crypto libraries to safely manage, generate and store private keys, to generate blockchain addresses and to sign transactions. 

Additionally, it contains a RTOS app ([Zephyr](https://github.com/AnyLedger/zephyr)).
The current implementation uses Ethereum, but the same architecture will be used for all the other chains. The current implementation is using Nordic nRF52840.

Any IoT device flashing the embedded wallet can safely connect to a given blockchain, do transactions and interact with smart contracts. The connection itself is given by [AnyLedger Hub](https://github.com/AnyLedger/anyledger-hub) which, among the many functionalities, runs the blockchain nodes, the LWM2M server and gives access to storage solutions.

Future improvements in the pipeline: support for Trusted Execution Environment, FreeRTOS support, ESP32 support and more.

## Contributing

Have a look at our [contribution guidelines](https://github.com/AnyLedger/anyledger-wallet/blob/master/CONTRIBUTING.md).

## Compilation

### Dependencies

#### tests/x86 builds

##### Linux

`apt-get install libgtest-dev g++ make cmake wget build-essential clang clang-tidy-3.8 ninja-build`

##### macOS

GTest:

```
git clone https://github.com/google/googletest 
cd googletest  
mkdir build  
cd build  
cmake ..  
make  
make install
```

Additional dependencies (installed using [Homebrew](https://brew.sh)):

`brew install gcc make cmake wget ninja`

#### Python (this is required for ABI compiler)
`pip3 install --user -r requirements.txt`

#### solidity compiler
A working `solc` should be available in your `$PATH`. See the [Installation instructions](https://solidity.readthedocs.io/en/v0.4.21/installing-solidity.html).

### x86 build

This build is used to develop and test the wallet library on your desktop. It does not include any working application - its purpose is to provide a compileable framework
for unit testing of the individual components on POSIX system. You should always start with this build if you plan to do any changes to the core components.

1) clone wallet repo
`git clone git@github.com:AnyLedger/anyledger-wallet.git && cd anyledger-wallet`

2) create build directory and generate build targets
`mkdir build && cd build && cmake -GNinja ../examples/tests`

3) compile the library & run the tests
`ninja && ctest`

### zephyr build

Prerequisites: a working zephyr SDK (at least v0.9.5), see [here](https://docs.zephyrproject.org/latest/getting_started/getting_started.html) for instructions. 
Please make sure you can compile and flash at least blinky example before building the wallet.

Before using the actual hardware, you will probably want to test the functionality using the zephyr's QEMU image. (Use `-DBOARD=x86_qemu` when running `cmake`)

1) clone wallet repo to the zephyr's `samples` directory
`mkdir samples/anyledger/ && git clone git@github.com:AnyLedger/anyledger-wallet.git`

2) create build directory and generate build targets
`mkdir -p samples/anyledger/anyledger-wallet/build && cd samples/anyledger/anyledger-wallet/build && cmake -GNinja  -DBOARD=nrf52840_pca10056 -DBUILD_XCOMPILE=1  ../examples/wallet`

3) for hardware builds, compile & flash the firmware
`ninja && ninja flash`

## Using AnyLedger wallet

To start AnyLedger wallet using a QEMU virtual machine, [see here](https://github.com/AnyLedger/anyledger-wallet/blob/master/BUILD-qemu.md).

## Project structure

- `abi_compiler` - python tool for compiling Ethereum ABI definition into a C code
- `eth` - ethereum transaction utilities
- `utils` - string manipulation, JSON en+de-coding, run-length encoding, uint256 manipulators
- `examples` - examples of the actual apps
- `sawtooth` - Hyperledger sawtooth utilities
- `thirdparty` - CMakeFiles for 3rd party modules
- `tools` - random helper scripts
- `wallet` - MacOs compatibility hack
- `zephyr-wallet` - Zephyr-specific code for the AnyLedger wallet
