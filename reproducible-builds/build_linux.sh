#!/bin/bash
set -euo pipefail

PROJECT_ROOT="$(CDPATH='' cd -- "$(dirname -- "$0")/.." && pwd -P)"
# Override the builder fallback with the timestamp of the source revision.
SOURCE_DATE_EPOCH="$(
    git -c safe.directory="$PROJECT_ROOT" \
        -C "$PROJECT_ROOT" log -1 --format=%ct
)"
case "$SOURCE_DATE_EPOCH" in
    ''|*[!0-9]*)
        echo "Invalid SOURCE_DATE_EPOCH: $SOURCE_DATE_EPOCH" >&2
        exit 1
        ;;
esac
export SOURCE_DATE_EPOCH

# Build project
cd "$PROJECT_ROOT"
mkdir -p build && cd build

export LDFLAGS="-L$OPENSSL_ROOT_DIR/lib -lssl -lcrypto -static-libgcc -static-libstdc++"
export CPPFLAGS="-I$OPENSSL_ROOT_DIR/include"
export CXXFLAGS="-O2 $CPPFLAGS"
export CFLAGS="-O2 $CPPFLAGS"

cmake .. \
    -DCMAKE_BUILD_TYPE=Release \
    -DUR__DISABLE_TESTS=ON \
    -DCMAKE_PREFIX_PATH="$OPENSSL_ROOT_DIR;$QT_INSTALLED_PREFIX" \
    -DQt5_DIR=$QT5_DIR

make -j$(nproc)

# After build, call package script
cd ..
"$PROJECT_ROOT/reproducible-builds/package_linux.sh"
