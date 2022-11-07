# Build guideline

Follow this document to build the application on your platform (Windows)

# Tools & Dependencies
**Qt libraries and Qt Creator**
 Download the Qt for your platform [Qt offline installer](https://www.qt.io/offline-installers) and install it.
 Visual studio 2017 or 2019 (for Windows)
```
 Notes: 
    Qt version 5.12 or higher is recommended
    Windows: Select compiler MSCV 2017 (or 2019) 64 - bit (or 32 bit depends on your OS)
```
**vcpkg **
A number of [open source libraries](https://github.com/bitcoin/bitcoin/blob/master/doc/dependencies.md) are required in order to be able to build Bitcoin Core in **libnunchuk**.
An options for installing the dependencies in a Visual Studio compatible manner is vcpkg.
Use Microsoft's [vcpkg](https://docs.microsoft.com/en-us/cpp/vcpkg) to download the source packages and build locally. This is the recommended approach.

Following quick steps:
> Clone vcpkg to your disk (such as C:\) and install it
```
	open window Powershell
	git clone https://github.com/Microsoft/vcpkg.git
	cd vcpkg\
	.\vcpkg\bootstrap-vcpkg.bat
	Add to system variable PATH: /path/to/your/vcpkg (such as : C:\vcpkg)
```
> Using vcpkg install the dependencies
```
	vcpkg install boost:x64-windows zeromq:x64-windows libevent:x64-windows berkeleydb:x64-windows sqlite3:x64-windows
	vcpkg install sqlcipher:x64-windows qt5:x64-windows qt5-graphicaleffects:x64-windows qt5-quickcontrols:x64-windows qtKeychain:x64-windows
	vcpkg integrate install
```
> After install completed. Add to system variable PATH:
```
	/path/to/your/vcpkg/installed/x64-windows/include
	/path/to/your/vcpkg/installed/x64-windows
	
such as:
	C:\vcpkg\installed\x64-windows\include
	C:\vcpkg\installed\x64-windows	
```
**Build Oml lib**
>**Clone project olm from github, To build olm as a shared library run:**
```
	open window Powershell
	git clone https://gitlab.matrix.org/matrix-org/olm.git
	cd olm
	cd build
	cmake ../
	cmake --build . --config Release -j8
```

**Build nunchuck-client-qt**
>**Clone project and fetch submodules**
``` 
	open window Powershell
	git clone https://gitlab.com/nunchuck/nunchuck-qt
	cd nunchuck-qt
	git submodule update --init --recursive
```
**Build [contrib/libnunchuk](https://github.com/nunchuk-io/libnunchuk)**

>**Build Bitcoin Core ([Windows build](https://github.com/bitcoin/bitcoin/tree/master/doc#building))**
```
	cd contrib/libnunchuk/contrib/bitcoin/build_msvc
	py -3 msvc-autogen.py
	msbuild /m bitcoin.sln /p:Platform=x64 /p:Configuration=Release /t:build
	or
	msbuild -property:Configuration=Release -maxCpuCount -verbosity:minimal bitcoin.sln
```
>**Build nunchuck-client-qt**
```
	cmake -E make_directory build
	cd .\build\
	cmake [path tp your location]\nunchuck-qt -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=[path to your vcpkg]\vcpkg\scripts\buildsystems\vcpkg.cmake -DVCPKG_TARGET_TRIPLET=x64-windows -DUR__DISABLE_TESTS=ON
	cmake --build . --config Release -j8
	...
        Run the application
```
