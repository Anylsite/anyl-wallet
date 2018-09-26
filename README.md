# anyledger-wallet

## Overview

🏦 AnyLedger Embedded Hardware wallet for Internet of Things

LWM2M C client containing crypto libraries to safely manage, generate and store private keys, to generate blockchain addresses and to sign transactions.

Additionally, it will contain a RTOS ([Zephyr](https://github.com/AnyLedger/zephyr)) and support for Trusted Execution Environment.

The current implementation uses Ethereum, but the same architecture will be used for HyperLedger and other chains in the future. The current implementation is using Nordic nRF52840.

Any IoT device flashing the embedded wallet can safely connect to a given blockchain. The connection itself is given by [AnyLedge Hub](https://github.com/AnyLedger/anyledger-hub) which, among the many functionalities, runs the blockchain nodes, the LWM2M server and gives access to storage solutions.


## Additional Info

An older Ethereum implementation in C++ for Particle Photon can be found [here](https://github.com/AnyLedger/embedded-ethereum-wallet).