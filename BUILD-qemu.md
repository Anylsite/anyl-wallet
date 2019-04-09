# wallet instructions

This document describes how to start `anyledger-wallet` project using a QEMU virtual machine.

... assuming:
- you have the [anyledger repo](https://github.com/AnyLedger/anyledger-wallet) and [zephyr SDK](https://docs.zephyrproject.org/latest/getting_started/getting_started.html#set-up-a-development-system) set up (first try to build `blinky` example from the zephyr repo, then clone anyledger into `samples/anyledger/anyledger-wallet`)
- you have a running ethereum node [(e.g. parity)](https://github.com/paritytech/parity-ethereum/releases) and a [proxy](https://github.com/AnyLedger/anyledger-wallet/blob/master/misc/lighttpd.conf) that buffers chunked-encoding replies of the node's JSONRPC interface.


## part one: networking
1) set up basic networking stuff

[Zephyr QEMU Networking](https://docs.zephyrproject.org/1.13.0/subsystems/networking/qemu_setup.html)

2) start the parity ethereum node and make the JSON-RPC port listen at localhost:18545
```
parity --light --jsonrpc-port 18545
```

if you get `Provided Host header is not whitelisted.` error when you do step 3, that means you need
```
parity --light --jsonrpc-port 18545 --jsonrpc-hosts="all"
```

3) start the proxy with `lighttpd -f misc/lighttpd.conf`, which will forward incoming IPV6 port 8545 requests to IPV4 18545 and translate from HTTP chunked-encoding to Content-length.

```
> curl -X POST --header "Content-Type: application/json"  --data '{"jsonrpc":"2.0","method":"web3_clientVersion","params":[],"id":987}' [::1]:8545
 
{"jsonrpc":"2.0","result":"Parity-Ethereum//v2.4.5-stable-76d4064-20190408/x86_64-linux-gnu/rustc1.33.0","id":987}
```


## part two: build the QEMU target
1) create an empty file `pk.txt` in the root of the `anyledger-wallet` repository and paste your hex-encoded private key to it.

2) build the wallet

`$ mkdir build && cd build && cmake -GNinja  -DBOARD=qemu_x86 -DBUILD_XCOMPILE=1 ../ && ninja`

## part three: play with the wallet
1) run the emulator (the wallet prompt should appear immediately)

`$ ninja run`


2) check if your private key is correct

`uart:~$ wallet pk`


3) sync your account nonce (No output for this one)

`uart:~$ wallet sync`

4) send a transaction and check etherscan if it did go through (value of the transaction will be 0)

`uart:~$ wallet transfer <to_address>`
