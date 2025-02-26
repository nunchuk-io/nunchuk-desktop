#!/bin/bash
NUNCHUK_SRC=$PWD
echo "export CC=gcc-14" >> ~/.bashrc
echo "export CXX=g++-14" >> ~/.bashrc
echo "export RANLIB=gcc-ranlib-14" >> ~/.bashrc
echo "export AR=gcc-ar-14" >> ~/.bashrc
echo "export NM=gcc-nm-14" >> ~/.bashrc
source ~/.bashrc
name=$1
if [ $name == "openssl" ]; then
    cd contrib/libnunchuk/contrib/openssl
    ./config --prefix="$PWD/lib"
    make -j8
    make install_dev
else
    echo "not build openssl"
fi
OPENSSL_ROOT_DIR=$NUNCHUK_SRC/contrib/libnunchuk/contrib/openssl/lib
export CPPFLAGS="-I$OPENSSL_ROOT_DIR/include"
echo "CPPFLAGS=$CPPFLAGS" >> ~/.bashrc
source ~/.bashrc
export LD_LIBRARY_PATH=$NUNCHUK_SRC/contrib/libnunchuk/contrib/openssl/lib:$LD_LIBRARY_PATH
# cmake -B build -DRANDLIB=$(which gcc-randlib-14) -DAR=$(which gcc-ar-14) -DNM=$(which gcc-nm-14) -DCMAKE_PREFIX_PATH=~/Qt/5.15.2/gcc_64/lib/cmake -DCMAKE_BUILD_TYPE=Debug # -DBOOST_ROOT=/usr/local/boost
