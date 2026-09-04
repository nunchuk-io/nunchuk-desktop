#!/usr/bin/env bash
set -euo pipefail

PROJECT_DIR="${PROJECT_DIR:-$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)}"
LOCK_FILE="${PROJECT_DIR}/reproducible-builds/macos.lock.env"

if [[ ! -f "${LOCK_FILE}" ]]; then
    echo "Missing macOS dependency lock: ${LOCK_FILE}" >&2
    exit 1
fi

# shellcheck source=macos.lock.env
source "${LOCK_FILE}"

ARCH="${ARCH:-$(uname -m)}"
TAG="${TAG:-0.0.0}"
SOURCE_DATE_EPOCH="${SOURCE_DATE_EPOCH:-$(git -C "${PROJECT_DIR}" log -1 --pretty=%ct)}"
QT_ROOT="${QT_ROOT:-}"
BUILD_ROOT="${BUILD_ROOT:-/private/tmp/nunchuk-macos-reproducible/${ARCH}}"
OUTPUT_DIR="${OUTPUT_DIR:-${PROJECT_DIR}/out/macos/${ARCH}}"
DEPS_ROOT="${BUILD_ROOT}/deps"
DOWNLOAD_DIR="${BUILD_ROOT}/downloads"
SOURCE_ROOT="${BUILD_ROOT}/sources"

case "${ARCH}" in
    x86_64|arm64) ;;
    *)
        echo "Unsupported macOS architecture: ${ARCH}" >&2
        exit 1
        ;;
esac

if [[ "$(uname -s)" != "Darwin" || "$(uname -m)" != "${ARCH}" ]]; then
    echo "This is a native build: expected Darwin/${ARCH}, got $(uname -s)/$(uname -m)." >&2
    exit 1
fi
if [[ ! "${TAG}" =~ ^[0-9]+\.[0-9]+\.[0-9]+([.-][0-9A-Za-z]+)*$ ]]; then
    echo "Invalid TAG: ${TAG}" >&2
    exit 1
fi
if [[ ! "${SOURCE_DATE_EPOCH}" =~ ^[0-9]+$ ]]; then
    echo "Invalid SOURCE_DATE_EPOCH: ${SOURCE_DATE_EPOCH}" >&2
    exit 1
fi
if [[ -z "${QT_ROOT}" || ! -x "${QT_ROOT}/bin/macdeployqt" ]]; then
    echo "QT_ROOT must point to the pinned Qt ${QT_VERSION} macOS installation." >&2
    exit 1
fi
for controlled_directory in "${BUILD_ROOT}" "${OUTPUT_DIR}"; do
    if [[ "${controlled_directory}" != /* \
        || "${controlled_directory}" == / \
        || "${controlled_directory}" == "${PROJECT_DIR}" ]]; then
        echo "Refusing unsafe build/output directory: ${controlled_directory}" >&2
        exit 1
    fi
done

export SOURCE_DATE_EPOCH
export ZERO_AR_DATE=1
export TZ=UTC
export LANG=C
export LC_ALL=C
export MACOSX_DEPLOYMENT_TARGET
export CMAKE_GENERATOR=Ninja
export PATH="${QT_ROOT}/bin:${PATH}"
umask 022

readonly JOBS="$(sysctl -n hw.ncpu)"
readonly PREFIX_MAP_FLAGS="-ffile-prefix-map=${PROJECT_DIR}=/usr/src/nunchuk -fdebug-prefix-map=${PROJECT_DIR}=/usr/src/nunchuk -fmacro-prefix-map=${PROJECT_DIR}=/usr/src/nunchuk -ffile-prefix-map=${BUILD_ROOT}=/usr/src/nunchuk-build -fdebug-prefix-map=${BUILD_ROOT}=/usr/src/nunchuk-build -fmacro-prefix-map=${BUILD_ROOT}=/usr/src/nunchuk-build -ffile-prefix-map=${QT_ROOT}=/opt/Qt -fdebug-prefix-map=${QT_ROOT}=/opt/Qt -fmacro-prefix-map=${QT_ROOT}=/opt/Qt"
export CPPFLAGS="${PREFIX_MAP_FLAGS}"
export CFLAGS="${PREFIX_MAP_FLAGS}"
export CXXFLAGS="${PREFIX_MAP_FLAGS}"
export LDFLAGS="-Wl,-no_uuid"

download_checked() {
    local url="$1"
    local expected_sha256="$2"
    local destination="$3"

    curl --fail --location --retry 3 --show-error --output "${destination}" "${url}"
    printf '%s  %s\n' "${expected_sha256}" "${destination}" | shasum -a 256 --check
}

checkout_commit() {
    local url="$1"
    local commit="$2"
    local destination="$3"

    mkdir -p "${destination}"
    git -C "${destination}" init --quiet
    git -C "${destination}" remote add origin "${url}"
    git -C "${destination}" fetch --quiet --depth 1 origin "${commit}"
    git -C "${destination}" checkout --quiet --detach FETCH_HEAD
    if [[ "$(git -C "${destination}" rev-parse HEAD)" != "${commit}" ]]; then
        echo "Commit verification failed for ${destination}." >&2
        exit 1
    fi
}

# Some nested zlib CMake logic temporarily renames this tracked header even for
# an out-of-source build. Start only from a clean checkout and restore that
# mechanical rename on every normal success/failure path.
if [[ -n "$(git -C "${PROJECT_DIR}" status --porcelain --untracked-files=all)" ]]; then
    echo "The reproducible macOS builder requires a completely clean source tree." >&2
    git -C "${PROJECT_DIR}" status --short --untracked-files=all >&2
    exit 1
fi
git -C "${PROJECT_DIR}" submodule foreach --quiet --recursive '
    if test -n "$(git status --porcelain --untracked-files=all)"; then
        echo "Dirty source in submodule: $displaypath" >&2
        git status --short --untracked-files=all >&2
        exit 1
    fi
'

zlib_source_dir="${PROJECT_DIR}/contrib/libnunchuk/contrib/bbqr-cpp/contrib/zlib"
zconf_header="${zlib_source_dir}/zconf.h"
zconf_renamed="${zlib_source_dir}/zconf.h.included"
if [[ ! -f "${zconf_header}" || -e "${zconf_renamed}" ]]; then
    echo "Unexpected nested zlib header state before build." >&2
    exit 1
fi
restore_zlib_header() {
    if [[ ! -e "${zconf_header}" && -f "${zconf_renamed}" ]]; then
        mv "${zconf_renamed}" "${zconf_header}"
    fi
}
trap restore_zlib_header EXIT

cmake -E remove_directory "${BUILD_ROOT}"
cmake -E remove_directory "${OUTPUT_DIR}"
mkdir -p "${DEPS_ROOT}" "${DOWNLOAD_DIR}" "${SOURCE_ROOT}" "${OUTPUT_DIR}"

qtpaths_tool="${QT_ROOT}/bin/qtpaths"
if [[ ! -x "${qtpaths_tool}" ]]; then
    qtpaths_tool="${QT_ROOT}/bin/qtpaths6"
fi
actual_qt_version="$("${qtpaths_tool}" --qt-version 2>/dev/null || "${QT_ROOT}/bin/qmake" -query QT_VERSION)"
if [[ "${actual_qt_version}" != "${QT_VERSION}" ]]; then
    echo "Expected Qt ${QT_VERSION}, got ${actual_qt_version}." >&2
    exit 1
fi
for module in Qt6Core5Compat Qt6Multimedia Qt6NetworkAuth Qt6Positioning Qt6ShaderTools Qt6WebChannel Qt6WebEngineCore; do
    if [[ ! -d "${QT_ROOT}/lib/cmake/${module}" ]]; then
        echo "Pinned Qt installation is missing ${module}." >&2
        exit 1
    fi
done

if [[ "$(cmake --version | awk 'NR == 1 { print $3 }')" != "${CMAKE_VERSION}" ]]; then
    echo "Unexpected CMake version; install the version in macos.lock.env first." >&2
    exit 1
fi
if [[ "$(ninja --version)" != "${NINJA_VERSION%.1}" && "$(ninja --version)" != "${NINJA_VERSION}" ]]; then
    echo "Unexpected Ninja version: $(ninja --version)" >&2
    exit 1
fi

# Boost is installed from its signed release archive. Only the components used
# by the current source tree are built; the installed CMake package also
# provides Boost::headers to the embedded Bitcoin Core build.
boost_archive="${DOWNLOAD_DIR}/boost_${BOOST_VERSION//./_}.tar.gz"
download_checked \
    "https://archives.boost.io/release/${BOOST_VERSION}/source/boost_${BOOST_VERSION//./_}.tar.gz" \
    "${BOOST_SHA256}" \
    "${boost_archive}"
tar -xzf "${boost_archive}" -C "${SOURCE_ROOT}"
boost_source="${SOURCE_ROOT}/boost_${BOOST_VERSION//./_}"
(
    cd "${boost_source}"
    ./bootstrap.sh --prefix="${DEPS_ROOT}"
    ./b2 install \
        --prefix="${DEPS_ROOT}" \
        --with-filesystem \
        --with-program_options \
        --with-system \
        --with-thread \
        address-model=64 \
        link=static \
        runtime-link=static \
        threading=multi \
        variant=release \
        -j"${JOBS}"
)

libevent_archive="${DOWNLOAD_DIR}/libevent-${LIBEVENT_VERSION}.tar.gz"
download_checked \
    "https://github.com/libevent/libevent/releases/download/release-${LIBEVENT_VERSION}-stable/libevent-${LIBEVENT_VERSION}-stable.tar.gz" \
    "${LIBEVENT_SHA256}" \
    "${libevent_archive}"
tar -xzf "${libevent_archive}" -C "${SOURCE_ROOT}"
cmake -S "${SOURCE_ROOT}/libevent-${LIBEVENT_VERSION}-stable" \
    -B "${BUILD_ROOT}/libevent-build" \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX="${DEPS_ROOT}" \
    -DCMAKE_OSX_ARCHITECTURES="${ARCH}" \
    -DCMAKE_OSX_DEPLOYMENT_TARGET="${MACOSX_DEPLOYMENT_TARGET}" \
    -DEVENT__DISABLE_BENCHMARK=ON \
    -DEVENT__DISABLE_OPENSSL=ON \
    -DEVENT__DISABLE_REGRESS=ON \
    -DEVENT__DISABLE_SAMPLES=ON \
    -DEVENT__DISABLE_TESTS=ON \
    -DEVENT__LIBRARY_TYPE=STATIC
cmake --build "${BUILD_ROOT}/libevent-build" --parallel "${JOBS}"
cmake --install "${BUILD_ROOT}/libevent-build"

checkout_commit \
    "https://gitlab.matrix.org/matrix-org/olm.git" \
    "${OLM_COMMIT}" \
    "${SOURCE_ROOT}/olm"
cmake -S "${SOURCE_ROOT}/olm" \
    -B "${BUILD_ROOT}/olm-build" \
    -DBUILD_SHARED_LIBS=OFF \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX="${DEPS_ROOT}" \
    -DCMAKE_OSX_ARCHITECTURES="${ARCH}" \
    -DCMAKE_OSX_DEPLOYMENT_TARGET="${MACOSX_DEPLOYMENT_TARGET}" \
    -DOLM_TESTS=OFF
cmake --build "${BUILD_ROOT}/olm-build" --parallel "${JOBS}"
cmake --install "${BUILD_ROOT}/olm-build"

checkout_commit \
    "https://github.com/frankosterfeld/qtkeychain.git" \
    "${QTKEYCHAIN_COMMIT}" \
    "${SOURCE_ROOT}/qtkeychain"
cmake -S "${SOURCE_ROOT}/qtkeychain" \
    -B "${BUILD_ROOT}/qtkeychain-build" \
    -DBUILD_SHARED_LIBS=OFF \
    -DBUILD_TESTING=OFF \
    -DBUILD_TEST_APPLICATION=OFF \
    -DBUILD_TRANSLATIONS=OFF \
    -DBUILD_WITH_QT6=ON \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX="${DEPS_ROOT}" \
    -DCMAKE_OSX_ARCHITECTURES="${ARCH}" \
    -DCMAKE_OSX_DEPLOYMENT_TARGET="${MACOSX_DEPLOYMENT_TARGET}" \
    -DCMAKE_PREFIX_PATH="${QT_ROOT}"
cmake --build "${BUILD_ROOT}/qtkeychain-build" --parallel "${JOBS}"
cmake --install "${BUILD_ROOT}/qtkeychain-build"

openssl_archive="${DOWNLOAD_DIR}/openssl-${OPENSSL_VERSION}.tar.gz"
download_checked \
    "https://github.com/openssl/openssl/releases/download/openssl-${OPENSSL_VERSION}/openssl-${OPENSSL_VERSION}.tar.gz" \
    "${OPENSSL_SHA256}" \
    "${openssl_archive}"
tar -xzf "${openssl_archive}" -C "${SOURCE_ROOT}"
mkdir -p "${BUILD_ROOT}/openssl-build"
if [[ "${ARCH}" == "arm64" ]]; then
    openssl_target=darwin64-arm64-cc
else
    openssl_target=darwin64-x86_64-cc
fi
(
    cd "${BUILD_ROOT}/openssl-build"
    perl "${SOURCE_ROOT}/openssl-${OPENSSL_VERSION}/Configure" \
        "${openssl_target}" \
        --prefix="${DEPS_ROOT}" \
        --openssldir="${DEPS_ROOT}/ssl" \
        no-shared \
        no-tests
    make -j"${JOBS}"
    make install_sw
)
if [[ "$("${DEPS_ROOT}/bin/openssl" version | awk '{ print $2 }')" != "${OPENSSL_VERSION}" ]]; then
    echo "The pinned OpenSSL build could not be verified." >&2
    exit 1
fi

hwi_binary="${BUILD_ROOT}/hwi/hwi"
mkdir -p "$(dirname "${hwi_binary}")"
if [[ "${ARCH}" == "x86_64" ]]; then
    hwi_archive="${DOWNLOAD_DIR}/hwi-3.2.0-mac-x86_64.tar.gz"
    download_checked \
        "https://github.com/nogibi/HWI/releases/download/${HWI_VERSION}/hwi-3.2.0-mac-x86_64.tar.gz" \
        "${HWI_MACOS_X86_64_SHA256}" \
        "${hwi_archive}"
    mkdir -p "${BUILD_ROOT}/hwi-extract"
    tar -xzf "${hwi_archive}" -C "${BUILD_ROOT}/hwi-extract"
    extracted_hwi="$(find "${BUILD_ROOT}/hwi-extract" -type f -name hwi -perm -111 -print -quit)"
    if [[ -z "${extracted_hwi}" ]]; then
        echo "The pinned HWI archive did not contain an executable." >&2
        exit 1
    fi
    install -m 0755 "${extracted_hwi}" "${hwi_binary}"
else
    libusb_archive="${DOWNLOAD_DIR}/libusb-${LIBUSB_VERSION}.tar.bz2"
    download_checked \
        "https://github.com/libusb/libusb/releases/download/v${LIBUSB_VERSION}/libusb-${LIBUSB_VERSION}.tar.bz2" \
        "${LIBUSB_SHA256}" \
        "${libusb_archive}"
    tar -xjf "${libusb_archive}" -C "${SOURCE_ROOT}"
    (
        cd "${SOURCE_ROOT}/libusb-${LIBUSB_VERSION}"
        ./configure --prefix="${DEPS_ROOT}" --disable-static --enable-shared
        make -j"${JOBS}"
        make install
    )

    checkout_commit \
        "https://github.com/nogibi/HWI.git" \
        "${HWI_COMMIT}" \
        "${SOURCE_ROOT}/hwi"
    export POETRY_VIRTUALENVS_IN_PROJECT=true
    export POETRY_NO_INTERACTION=1
    export PYTHONHASHSEED=42
    export PKG_CONFIG_PATH="${DEPS_ROOT}/lib/pkgconfig"
    export DYLD_LIBRARY_PATH="${DEPS_ROOT}/lib"
    (
        cd "${SOURCE_ROOT}/hwi"
        poetry install --sync --no-root

        # HWI's upstream deterministic builder normalizes the stdlib sources
        # before PyInstaller compiles base_library.zip. Query the pinned Python
        # installation instead of hard-coding a pyenv path.
        python_stdlib="$(poetry run python -c 'import sysconfig; print(sysconfig.get_path("stdlib"))')"
        find "${python_stdlib}" -type f -name '*.py' -exec touch -t 201901010000.00 {} +

        shim_dir="${BUILD_ROOT}/hwi-tool-shims"
        mkdir -p "${shim_dir}"
        printf '%s\n' \
            '#!/bin/sh' \
            'if [ "$1" = "--prefix" ] && [ "$2" = "libusb" ]; then' \
            "  printf '%s\\n' '${DEPS_ROOT}'" \
            '  exit 0' \
            'fi' \
            'exit 64' > "${shim_dir}/brew"
        chmod 0755 "${shim_dir}/brew"
        PATH="${shim_dir}:${PATH}" poetry run pyinstaller --clean --noconfirm hwi.spec
    )
    if [[ -n "$(git -C "${SOURCE_ROOT}/hwi" status --porcelain --untracked-files=no)" ]]; then
        echo "The HWI source build modified a tracked locked input." >&2
        git -C "${SOURCE_ROOT}/hwi" status --short --untracked-files=no >&2
        exit 1
    fi
    built_hwi="$(find "${SOURCE_ROOT}/hwi/dist" -type f -name hwi -perm -111 -print -quit)"
    if [[ -z "${built_hwi}" ]]; then
        echo "The pinned HWI source build did not produce an executable." >&2
        exit 1
    fi
    install -m 0755 "${built_hwi}" "${hwi_binary}"
fi

if ! lipo "${hwi_binary}" -verify_arch "${ARCH}"; then
    echo "HWI does not contain the requested architecture ${ARCH}." >&2
    exit 1
fi
"${hwi_binary}" --version

export BOOST_ROOT="${DEPS_ROOT}"
export OPENSSL_ROOT_DIR="${DEPS_ROOT}"
export PKG_CONFIG_PATH="${DEPS_ROOT}/lib/pkgconfig"
export CPPFLAGS="-I${DEPS_ROOT}/include ${PREFIX_MAP_FLAGS}"
export CFLAGS="${CPPFLAGS}"
export CXXFLAGS="${CPPFLAGS}"
export LDFLAGS="-L${DEPS_ROOT}/lib -Wl,-no_uuid"

app_build_dir="${BUILD_ROOT}/app-build"
cmake -S "${PROJECT_DIR}" -B "${app_build_dir}" \
    -DCMAKE_AUTORCC_OPTIONS="--format-version;1" \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_C_FLAGS_RELEASE="-O2 -DNDEBUG ${PREFIX_MAP_FLAGS}" \
    -DCMAKE_CXX_FLAGS_RELEASE="-O2 -DNDEBUG ${PREFIX_MAP_FLAGS}" \
    -DCMAKE_EXE_LINKER_FLAGS="-Wl,-no_uuid" \
    -DCMAKE_FIND_USE_PACKAGE_REGISTRY=OFF \
    -DCMAKE_FIND_USE_SYSTEM_PACKAGE_REGISTRY=OFF \
    -DCMAKE_OSX_ARCHITECTURES="${ARCH}" \
    -DCMAKE_OSX_DEPLOYMENT_TARGET="${MACOSX_DEPLOYMENT_TARGET}" \
    -DCMAKE_PREFIX_PATH="${DEPS_ROOT};${QT_ROOT}" \
    -DNUNCHUK_BUILD_TESTS=OFF \
    -DOPENSSL_ROOT_DIR="${DEPS_ROOT}" \
    -DOPENSSL_USE_STATIC_LIBS=TRUE \
    -DQt6_DIR="${QT_ROOT}/lib/cmake/Qt6" \
    -DUR__DISABLE_TESTS=ON

for expected_library in \
    "${DEPS_ROOT}/lib/libcrypto.a" \
    "${DEPS_ROOT}/lib/libssl.a"; do
    if ! grep -Fq "=${expected_library}" "${app_build_dir}/CMakeCache.txt"; then
        echo "CMake did not select pinned library ${expected_library}." >&2
        exit 1
    fi
done

cmake --build "${app_build_dir}" --parallel "${JOBS}"

APP_PATH="${app_build_dir}/Nunchuk.app" \
HWI_BINARY="${hwi_binary}" \
PROJECT_DIR="${PROJECT_DIR}" \
QT_ROOT="${QT_ROOT}" \
OUTPUT_DIR="${OUTPUT_DIR}" \
ARCH="${ARCH}" \
TAG="${TAG}" \
SOURCE_DATE_EPOCH="${SOURCE_DATE_EPOCH}" \
    bash "${PROJECT_DIR}/reproducible-builds/package_macos.sh"
