#!/bin/bash
set -euo pipefail

# Build project
mkdir -p build && cd build

export LDFLAGS="-L$OPENSSL_ROOT_DIR/lib -lssl -lcrypto -static-libgcc -static-libstdc++"
export CPPFLAGS="-I$OPENSSL_ROOT_DIR/include"
export CXXFLAGS="-O0 $CPPFLAGS"
export CFLAGS="-O0 $CPPFLAGS"

cmake .. \
    -DCMAKE_BUILD_TYPE=Release \
    -DUR__DISABLE_TESTS=ON \
    -DCMAKE_PREFIX_PATH="$OPENSSL_ROOT_DIR;$QT_INSTALLED_PREFIX" \
    -DQt5_DIR=$QT5_DIR

make -j$(nproc)

# After build, call package script
cd ..
/project/reproducible-builds/package_linux.sh
