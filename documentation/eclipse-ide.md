# Importing Any Ledger Wallet to Eclipse 

This tutorial will show you the way to compile and debug Any Ledger Wallet in Eclipse IDE, the current version is 2018-09, if something change in future versions, please, keep this tutorial up to date.

### Requeriments

Get [AnyLedger-wallet](https://github.com/AnyLedger/anyledger-wallet) source code. ```git clone https://github.com/AnyLedger/anyledger-wallet.git```

Check the [README.md](https://github.com/AnyLedger/anyledger-wallet/blob/master/README.md) of the project and ensure you have all you need to compile it.

Download [GNU MCU Eclipse IDE for C/C++ Developers](https://github.com/gnu-mcu-eclipse/org.eclipse.epp.packages/releases/), open Eclipse and follow this instructions:

* Go to the Eclipse menu → Help → Eclipse Marketplace…, find GNU MCU Eclipse and install
* If anything go bad in the Eclipse instalation, this [link](https://gnu-mcu-eclipse.github.io/plugins/install/) will give you more details.
* To get a more comfortable IDE experience: Eclipse Menu → Window →  Preferences, General →  Appearance, Theme: Dark.
* Configure the path to the pyOCD GDB server by opening the menu Window->Preferences, navigating to MCU, and setting the Global pyOCD Path, in linux should be /usr/bin, to check it run: ```$ whereis pyocd-gdbserver```

### Importing Any Ledger Wallet to Eclipse IDE
* Set CONFIG_OPENOCD_SUPPORT=y in the configuration file (Usually: prj.conf)
* Ensure your build directory is not a subdirectory of AnyLedger-Wallet, this is not supported well by Eclipse.
* Build the project with this command lines:
```
mkdir build && cd build
cmake -G"Eclipse CDT4 - Ninja" -DBOARD=frdm_k64f %ZEPHYR_BASE%\samples\anyledger-wallet
ninja
```
* In Eclipse, import your generated project by opening the menu File->Import... and selecting the option Existing Projects into Workspace. Browse to your application build directory in the choice, Select root directory:. Check the box for your project in the list of projects found and click the Finish button.

### Create a Debugger Configuration

* Open the menu ```Run->Debug Configurations...```
* Select **GDB PyOCD Debugging** making a double click on it, and configure the following options:
  - C/C++ Application ```YOUR-BUILD-FOLDER/zephyr/zephyr.elf```
* In the Debugger tab
  - pyOCD Setup:
    - Executable path: ```$pyocd_path\$pyocd_executable```
    - Uncheck “Allocate console for semihosting”
  - Board Setup
    - Bus speed: 64000000 Hz
    - Uncheck “Enable semihosting”
  - GDB Client Setup
    - Find and put the path to the executable arm-none-eabi-gdb, for example:
     ``` 
        #On Linux: /usr/bin/arm-none-eabi-gdb
        #On Windows: C:\gcc-arm-none-eabi-6_2017-q2-update\bin\arm-none-eabi-gdb.exe
    ```
   - In the SVD Path tab:
     - File path: <zephyr base>\ext\

### Related links
* https://docs.zephyrproject.org/1.13.0/application/application.html#eclipse-debugging 
* https://os.mbed.com/users/c1728p9/notebook/debugging-with-eclipse-and-pyocd/

