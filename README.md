# Build instructions 

Follow these steps to build the Nunchuk desktop application for your platform.

# Tools
**Qt libraries and Qt Creator**

 Download Qt for your platform [Qt offline installer](https://www.qt.io/offline-installers).
 You will also need Visual studio 2017 or 2019 (for Windows).
```
 Notes: 
        Qt version 5.12 or higher is recommended
        Windows: Select compiler MSCV 2017 (or 2019) 64 - bit (or 32 bit depends on your OS)
 	Linux: Compiler GCC 64 or 32
 	MAC:   Compiler GCC 64 or 32 (macOS)
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
	./configure --without-gui --disable-zmq --with-miniupnpc=no # important
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
	Select your Kit (include compiler) : Desktop Qt %{Qt:Version} GCC 64bit,  (such as Desktop Qt 5.12.8 GCC 64bit)
	Run qmake
	Build
	...
        Run the application
```
