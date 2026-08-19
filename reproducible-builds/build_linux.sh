#!/usr/bin/env bash
set -euo pipefail

PROJECT_ROOT=${PROJECT_ROOT:-/project}
BUILD_DIR="$PROJECT_ROOT/build"
BUILD_JOBS=${BUILD_JOBS:-4}

cd "$PROJECT_ROOT"
test -f CMakeLists.txt || {
    echo "ERROR: $PROJECT_ROOT is not a Nunchuk source checkout" >&2
    exit 1
}
test -d .git || {
    echo "ERROR: the source checkout must include .git metadata" >&2
    exit 1
}
if git submodule status --recursive | grep -Eq '^[-+U]'; then
    echo "ERROR: one or more submodules are not at the commit recorded by the source revision" >&2
    exit 1
fi
SOURCE_COMMIT=$(git rev-parse HEAD)
COMMIT_DATE_EPOCH=$(git log -1 --pretty=%ct)
if [[ -n "${SOURCE_DATE_EPOCH:-}" && "$SOURCE_DATE_EPOCH" != "$COMMIT_DATE_EPOCH" ]]; then
    echo "ERROR: SOURCE_DATE_EPOCH does not match the source commit time" >&2
    exit 1
fi
SOURCE_DATE_EPOCH=$COMMIT_DATE_EPOCH
TAG=${TAG:-$(git describe --tags --exact-match 2>/dev/null || true)}
if [[ -z "$TAG" ]]; then
    TAG="0.0.0-${SOURCE_COMMIT:0:12}"
fi
case "$TAG" in
    *[!A-Za-z0-9._+-]*)
        echo "ERROR: invalid build tag: $TAG" >&2
        exit 1
        ;;
esac
[[ "$SOURCE_DATE_EPOCH" =~ ^[0-9]+$ ]] || {
    echo "ERROR: SOURCE_DATE_EPOCH must be an integer" >&2
    exit 1
}

export SOURCE_COMMIT SOURCE_DATE_EPOCH TAG
export TZ=UTC
export LANG=C.UTF-8
export LC_ALL=C.UTF-8
export ZERO_AR_DATE=1
export ARFLAGS=crD

# OAuth values are intentionally excluded from the reproducible artifact.
# The unchanged application source falls back to runtime environment variables
# when these compile-time values are empty.
export OAUTH_CLIENT_ID=
export OAUTH_CLIENT_SECRET=
export OAUTH_REDIRECT_URI=

COMMON_FLAGS="-O0 -ffile-prefix-map=$PROJECT_ROOT=. -fdebug-prefix-map=$PROJECT_ROOT=."
export CPPFLAGS="-I$OPENSSL_ROOT_DIR/include"
export CFLAGS="$COMMON_FLAGS $CPPFLAGS"
export CXXFLAGS="$COMMON_FLAGS $CPPFLAGS"
export LDFLAGS="-L$OPENSSL_ROOT_DIR/lib -Wl,--build-id=sha1 -static-libgcc -static-libstdc++"

rm -rf -- "$BUILD_DIR" "$PROJECT_ROOT/nunchuk-linux-v$TAG" "$PROJECT_ROOT/dist"
if [[ -n "$(git status --porcelain=v1 --untracked-files=normal --ignore-submodules=none)" ]]; then
    echo "ERROR: the source checkout or one of its submodules has uncommitted changes" >&2
    exit 1
fi

cmake -S "$PROJECT_ROOT" -B "$BUILD_DIR" -G Ninja \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_C_COMPILER=gcc-14 \
    -DCMAKE_CXX_COMPILER=g++-14 \
    -DCMAKE_AR=gcc-ar-14 \
    -DCMAKE_RANLIB=gcc-ranlib-14 \
    -DCMAKE_NM=gcc-nm-14 \
    -DCMAKE_C_FLAGS_RELEASE="$CFLAGS" \
    -DCMAKE_CXX_FLAGS_RELEASE="$CXXFLAGS" \
    -DCMAKE_EXE_LINKER_FLAGS_RELEASE="$LDFLAGS" \
    -DCMAKE_PREFIX_PATH="$OPENSSL_ROOT_DIR;$QT_INSTALLED_PREFIX;/usr/local" \
    -DOPENSSL_ROOT_DIR="$OPENSSL_ROOT_DIR" \
    -DQt5_DIR="$QT5_DIR" \
    -DUR__DISABLE_TESTS=ON

cmake --build "$BUILD_DIR" --parallel "$BUILD_JOBS"
"$PROJECT_ROOT/reproducible-builds/package_linux.sh"
