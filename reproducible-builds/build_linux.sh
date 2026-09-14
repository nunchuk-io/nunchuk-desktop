#!/usr/bin/env bash
set -euo pipefail

PROJECT_DIR="${PROJECT_DIR:-/project}"
TAG="${TAG:-0.0.0}"

if [[ "${PROJECT_DIR}" != /* || ! -f "${PROJECT_DIR}/CMakeLists.txt" ]]; then
    echo "PROJECT_DIR must be an absolute Nunchuk source directory: ${PROJECT_DIR}" >&2
    exit 1
fi

if [[ ! "${TAG}" =~ ^[0-9]+\.[0-9]+\.[0-9]+([.-][0-9A-Za-z]+)*$ ]]; then
    echo "Invalid TAG: ${TAG}" >&2
    exit 1
fi

cd "${PROJECT_DIR}"

# The vendored zlib CMake project renames its tracked zconf.h during every
# out-of-source configure. Require the known-clean state, then restore that
# upstream side effect on every exit so the build never dirties the checkout.
ZLIB_SOURCE_DIR="${PROJECT_DIR}/contrib/libnunchuk/contrib/bbqr-cpp/contrib/zlib"
ZLIB_ZCONF="${ZLIB_SOURCE_DIR}/zconf.h"
ZLIB_ZCONF_INCLUDED="${ZLIB_SOURCE_DIR}/zconf.h.included"
if [[ ! -f "${ZLIB_ZCONF}" || -e "${ZLIB_ZCONF_INCLUDED}" ]]; then
    echo "Vendored zlib is not in its clean pre-configure state." >&2
    echo "Expected ${ZLIB_ZCONF} and no ${ZLIB_ZCONF_INCLUDED}." >&2
    exit 1
fi

restore_zlib_zconf() {
    local build_status=$?
    trap - EXIT

    if [[ -f "${ZLIB_ZCONF_INCLUDED}" && ! -e "${ZLIB_ZCONF}" ]]; then
        mv -- "${ZLIB_ZCONF_INCLUDED}" "${ZLIB_ZCONF}" || build_status=1
    elif [[ -e "${ZLIB_ZCONF_INCLUDED}" || ! -f "${ZLIB_ZCONF}" ]]; then
        echo "Could not restore the vendored zlib zconf.h source state." >&2
        build_status=1
    fi
    exit "${build_status}"
}
trap restore_zlib_zconf EXIT

# The source is mounted from the GitHub runner and therefore has a different
# numeric owner inside the disposable root container. Nested CMake projects run
# Git while generating build metadata, so trust repositories only in this
# isolated builder process.
git config --global --add safe.directory '*'

# Override the builder image's fallback (Dockerfile.linux's SOURCE_DATE_EPOCH,
# only meant for timestamps produced while building the image itself) with the
# timestamp of the actual source revision being packaged. This must be an
# unconditional assignment, not a "${SOURCE_DATE_EPOCH:-...}" default: the
# Docker image always has SOURCE_DATE_EPOCH set to a non-empty value, so a
# "${VAR:-default}" fallback would never take the git-derived value and every
# release would silently carry the image's fixed timestamp instead of its own
# commit's. Matches main's build_linux.sh, which uses this same unconditional
# form.
SOURCE_DATE_EPOCH="$(git -c safe.directory="${PROJECT_DIR}" -C "${PROJECT_DIR}" log -1 --format=%ct)"
if [[ ! "${SOURCE_DATE_EPOCH}" =~ ^[0-9]+$ ]]; then
    echo "Invalid SOURCE_DATE_EPOCH: ${SOURCE_DATE_EPOCH}" >&2
    exit 1
fi

export SOURCE_DATE_EPOCH
export TZ=UTC
export LANG=C.UTF-8
export LC_ALL=C.UTF-8
export ZERO_AR_DATE=1
umask 022

for required_tool in "${CC}" "${CXX}" "${AR}" "${NM}" "${RANLIB}"; do
    if [[ ! -x "${required_tool}" ]]; then
        echo "Pinned compiler/binutils tool is unavailable: ${required_tool}" >&2
        exit 1
    fi
done

readonly PREFIX_MAP_FLAGS="-ffile-prefix-map=${PROJECT_DIR}=. -fdebug-prefix-map=${PROJECT_DIR}=. -fmacro-prefix-map=${PROJECT_DIR}=."
export CPPFLAGS="-I${OPENSSL_ROOT_DIR}/include ${PREFIX_MAP_FLAGS}"
export CFLAGS="${CPPFLAGS}"
export CXXFLAGS="${CPPFLAGS}"
export LDFLAGS="-L${OPENSSL_ROOT_DIR}/lib -static-libgcc -static-libstdc++"

# A clean build directory is required for comparable outputs and prevents a
# stale Qt5 CMake cache from being reused after the Qt6 migration.
cmake -E remove_directory build

cmake -S . -B build -G Ninja \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_AUTORCC_OPTIONS="--format-version;1" \
    -DCMAKE_C_COMPILER="${CC}" \
    -DCMAKE_CXX_COMPILER="${CXX}" \
    -DCMAKE_AR="${AR}" \
    -DCMAKE_NM="${NM}" \
    -DCMAKE_RANLIB="${RANLIB}" \
    -DCMAKE_PREFIX_PATH="${OPENSSL_ROOT_DIR};${QT_INSTALLED_PREFIX};/usr" \
    -DOPENSSL_ROOT_DIR="${OPENSSL_ROOT_DIR}" \
    -DQt6_DIR="${QT6_DIR}" \
    -DUR__DISABLE_TESTS=ON

openssl_runtime_version="$(LD_LIBRARY_PATH="${OPENSSL_ROOT_DIR}/lib" "${OPENSSL_ROOT_DIR}/bin/openssl" version | awk '{ print $2 }')"
if [[ "${openssl_runtime_version}" != "${OPENSSL_VERSION}" ]]; then
    echo "Unexpected builder OpenSSL version: ${openssl_runtime_version}" >&2
    exit 1
fi

assert_cmake_cache_value() {
    local key="$1"
    local expected="$2"
    local actual

    actual="$(sed -n "s/^${key}:[^=]*=//p" build/CMakeCache.txt)"
    if [[ "${actual}" != "${expected}" ]]; then
        echo "Unexpected CMake cache value for ${key}: ${actual}; expected ${expected}" >&2
        exit 1
    fi
}

assert_cmake_cache_value CMAKE_C_COMPILER "${CC}"
assert_cmake_cache_value CMAKE_CXX_COMPILER "${CXX}"
assert_cmake_cache_value CMAKE_AR "${AR}"
assert_cmake_cache_value CMAKE_NM "${NM}"
assert_cmake_cache_value CMAKE_RANLIB "${RANLIB}"
assert_cmake_cache_value OPENSSL_CRYPTO_LIBRARY "${OPENSSL_ROOT_DIR}/lib/libcrypto.a"
assert_cmake_cache_value OPENSSL_SSL_LIBRARY "${OPENSSL_ROOT_DIR}/lib/libssl.a"

cmake --build build --parallel "$(nproc)"

"${PROJECT_DIR}/reproducible-builds/package_linux.sh"
