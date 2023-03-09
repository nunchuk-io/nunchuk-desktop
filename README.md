# Build guideline
Follow this document to build the application on your platform
# Tools
**Qt libraries and Qt Creator**

Download the Qt for your platform [Qt offline installer](https://www.qt.io/offline-installers) and install it.
Visual studio 2017 or 2019 (for Windows)
```
Notes:
Qt version 5.12 or higher is recommended
Windows: Select compiler MSCV 2017 (or 2019) 64 - bit (or 32 bit depends on your OS)
Linux: Compiler GCC 64 or 32
MAC: Compiler GCC 64 or 32 (macOS)
```
**Setup**
>**Clone project and fetch submodules**
>
```
git clone https://gitlab.com/nunchuck/nunchuck-qt
cd nunchuck-qt
git submodule update --init --recursive
```
**Build [contrib/libnunchuk](https://github.com/nunchuk-io/libnunchuk)**
>**Build Bitcoin Core ([details](https://github.com/bitcoin/bitcoin/tree/master/doc#building))**
>
```
cd contrib/libnunchuk/contrib/bitcoin
./autogen.sh
./configure --without-gui --disable-zmq --with-miniupnpc=no --with-incompatible-bdb --disable-bench --disable-tests -enable-module-ecdh # important
make -j8
cd ..
```
>**Build Sqlcipher ([details](https://github.com/sqlcipher/sqlcipher))**
>
```
cd libnunchuk/contrib/sqlcipher
./configure --enable-tempstore=yes CFLAGS="-DSQLITE_HAS_CODEC" LDFLAGS="-lcrypto"
make -j8
popd
cd ..
```
>**Build nunchuck-client-qt**
>
```
Open Qt Creator
Open nunchuck-client-qt project via CMakeLists.txt
Select your Kit (include compiler) : Desktop Qt %{Qt:Version} GCC 64bit, (such as Desktop Qt 5.12.8 GCC 64bit)
Run qmake
Build
...
Run the application
```
# Linux - Notes
**udev rules**

This directory contains all of the udev rules for the supported devices as retrieved from vendor websites and repositories.

These are necessary for the devices to be reachable on linux environments.
- `References` (All rules): https://github.com/bitcoin-core/HWI/tree/master/hwilib/udev
>**Usage**
Apply these rules by copying them to `/etc/udev/rules.d/` and notifying `udevadm`.
>
>Your user will need to be added to the `plugdev` group, which needs to be created if it does not already exist.
Commands:
>```
>    curl -OL https://raw.githubusercontent.com/bitcoin-core/HWI/master/hwilib/udev/20-hw1.rules
>    curl -OL https://raw.githubusercontent.com/bitcoin-core/HWI/master/hwilib/udev/51-coinkite.rules
>    curl -OL https://raw.githubusercontent.com/bitcoin-core/HWI/master/hwilib/udev/51-hid-digitalbitbox.rules
>    curl -OL https://raw.githubusercontent.com/bitcoin-core/HWI/master/hwilib/udev/51-trezor.rules
>    curl -OL https://raw.githubusercontent.com/bitcoin-core/HWI/master/hwilib/udev/51-usb-keepkey.rules
>    curl -OL https://raw.githubusercontent.com/bitcoin-core/HWI/master/hwilib/udev/52-hid-digitalbitbox.rules
>    curl -OL https://raw.githubusercontent.com/bitcoin-core/HWI/master/hwilib/udev/53-hid-bitbox02.rules
>    curl -OL https://raw.githubusercontent.com/bitcoin-core/HWI/master/hwilib/udev/54-hid-bitbox02.rules
>    curl -OL https://raw.githubusercontent.com/bitcoin-core/HWI/master/hwilib/udev/55-usb-jade.rules
>   
>    sudo cp *.rules /etc/udev/rules.d/ && \
>    sudo udevadm trigger && \
>    sudo udevadm control --reload-rules && \
>    sudo groupadd plugdev && \
>    sudo usermod -aG plugdev `whoami`
>```
**Run environment**

In some other linux versions, Nunchuk may not be able to run, you can refer to the following solutions:
1. There is no libfuse2 package by default. The AppImage distribution (and more generally, all existing AppImage's) are built expecting libfuse2 support. This means that AppImage's will not run on Ubuntu 22.04 by default.
You can try this command by manually before run nunchuk (to install libfuse2):  
`sudo apt install libfuse2*`
2. Some linux version need to updated SSL for compatibility package `libssl1.1`
Try running the appimage from the terminal with `LD_DEBUG=libs` and see what libraries it complains about before the segfault.
*`(Suggested by nunchuk's user)`*

3. Ubuntu update, should keep all the old repositories (end of the update process, when prompted to delete old repositories, files or keep them, just clicked no, keep old files)
*`(Suggested by nunchuk's user)`*

# macOS - Notes
**Prerequisites**

Python 3, libusb are required.

Commands:
```
brew install libusb
brew install python
```
