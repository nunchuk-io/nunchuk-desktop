#!/usr/bin/env bash
set -euo pipefail

prepare_build() {
    export PROJECT_DIR="${PROJECT_DIR:-/project}"
    export TAG="${TAG:-0.0.0}"
    if [[ "${PROJECT_DIR}" != /* || ! -f "${PROJECT_DIR}/CMakeLists.txt" ]]; then
        echo "PROJECT_DIR must be an absolute Nunchuk source directory: ${PROJECT_DIR}" >&2
        exit 1
    fi
    if [[ ! "${TAG}" =~ ^[0-9]+\.[0-9]+\.[0-9]+([.-][0-9A-Za-z]+)*$ ]]; then
        echo "Invalid TAG: ${TAG}" >&2
        exit 1
    fi
    cd "${PROJECT_DIR}"

    # Trust the mounted checkout and its submodules inside the disposable builder.
    git config --global --add safe.directory '*'
    SOURCE_DATE_EPOCH="$(git log -1 --format=%ct)"
    export SOURCE_DATE_EPOCH TZ=UTC LANG=C.UTF-8 LC_ALL=C.UTF-8 ZERO_AR_DATE=1

    export CPPFLAGS="-ffile-prefix-map=${PROJECT_DIR}=. -fdebug-prefix-map=${PROJECT_DIR}=. -fmacro-prefix-map=${PROJECT_DIR}=."
    export CFLAGS="${CPPFLAGS}" CXXFLAGS="${CPPFLAGS}"
    export LDFLAGS="-static-libgcc -static-libstdc++"
}

build_application() {
    local openssl_libdir
    openssl_libdir="/usr/lib/$(dpkg-architecture -qDEB_HOST_MULTIARCH)"

    # Start clean and use the builder's toolchain and frozen Ubuntu OpenSSL.
    cmake -E remove_directory build
    cmake -S . -B build -G Ninja \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_AUTORCC_OPTIONS="--format-version;1" \
        -DCMAKE_C_COMPILER="${CC}" \
        -DCMAKE_CXX_COMPILER="${CXX}" \
        -DCMAKE_AR="${AR}" \
        -DCMAKE_NM="${NM}" \
        -DCMAKE_RANLIB="${RANLIB}" \
        -DCMAKE_PREFIX_PATH="${QT_INSTALLED_PREFIX};/usr" \
        -DQt6_DIR="${QT6_DIR}" \
        -DOPENSSL_INCLUDE_DIR=/usr/include \
        -DOPENSSL_CRYPTO_LIBRARY="${openssl_libdir}/libcrypto.a" \
        -DOPENSSL_SSL_LIBRARY="${openssl_libdir}/libssl.a" \
        -DUR__DISABLE_TESTS=ON
    cmake --build build --parallel "$(nproc)"
}

main() {
    prepare_build
    build_application
    "${PROJECT_DIR}/reproducible-builds/package_linux.sh"
}

main
