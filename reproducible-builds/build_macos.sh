#!/usr/bin/env bash
set -euo pipefail

# Re-derived from the manually-run reference workflow at
# https://github.com/tongvanlinh/macdeploynunchuk/blob/main/.github/workflows/nunchuk-macos_matrix.yml
# (proven to build, sign and notarize successfully today), per an explicit
# instruction not to trust the previous version of this script -- it was an
# unverified draft, never exercised in CI. Every dependency build step below
# mirrors a command that reference workflow actually runs (same versions,
# same flags, same install layout where it matters for correctness -- e.g.
# OpenSSL's in-tree contrib/openssl/lib install path, required by
# contrib/libnunchuk/CMakeLists.txt's own `EXISTS .../contrib/openssl/lib`
# check). Deliberate deviations from that reference, and why, are called out
# inline as they occur; nothing else here should be assumed to differ.
#
# Known deviations from the reference workflow:
# - The reference's "Set compiler" step exports CC=gcc-14/CXX=g++-14 into
#   ~/.bashrc, which only two later steps (`source ~/.bashrc`) actually pick
#   up: building the main app, and building OpenSSL. No step ever installs a
#   `gcc-14`/`g++-14` toolchain, and every other native dependency (Boost,
#   libevent, Berkeley DB, Olm) explicitly and consistently uses Xcode's own
#   clang via `xcrun --find clang`. GNU GCC cannot compile Objective-C++,
#   which Qt's Cocoa platform plugin relies on, making a real switch to GCC
#   for the app build implausible. This script therefore does not use GCC
#   anywhere and always builds with Xcode's clang, confirmed on request.
# - The reference's "Build nunchuk-qt" step appends `-O0` to a build
#   otherwise configured as CMAKE_BUILD_TYPE=Release. Confirmed on request:
#   this script does not carry that over, and lets CMake's normal Release
#   optimization flags apply, so the produced binary is a real optimized
#   Release build (and therefore not byte-identical to whatever the
#   reference workflow has been publishing).
# - qtkeychain installs into a private prefix here instead of the
#   reference's `sudo cmake --install . --prefix "$BREW_PREFIX"` (which
#   mutates the runner's shared Homebrew installation). This changes nothing
#   about how qtkeychain itself is built, only where the result is installed
#   and looked up from (via CMAKE_PREFIX_PATH).
# - DMG creation uses `hdiutil` instead of the reference's `appdmg` (an npm
#   package). This drops the custom icon-layout background image but avoids
#   adding a Node/npm dependency to a Bash pipeline; it does not affect the
#   signed/notarized .app inside the DMG.
#
# Reproducibility additions layered on top of that proven baseline (per
# instruction to add these once the build itself matches a known-working
# reference): a clean-checkout assertion before any build starts, SHA-256
# verification of every downloaded archive, exact commit verification for
# Olm and HWI (kept because the reference already does this for those two;
# not added for qtkeychain, which the reference clones by tag only, since no
# commit hash was available to verify against without inventing one),
# SOURCE_DATE_EPOCH-driven deterministic packaging (handled by
# package_macos.sh), and a canonical hash-checked output archive. Compiler
# flags such as -ffile-prefix-map (present in git history of this file) are
# deliberately NOT reinstated in this pass -- they touch every dependency's
# build and were never validated against this from-scratch rewrite; adding
# them is a candidate follow-up once this simpler version is confirmed to
# build successfully in CI.

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
# Deliberately outside PROJECT_DIR (unlike an earlier version of this
# default): this script's own clean-tree check below fails if anything is
# left untracked inside PROJECT_DIR, and a prior build's own output sitting
# there would fail that check on a second invocation in the same workspace
# (e.g. the opt-in CI reproducibility-check rebuild).
OUTPUT_DIR="${OUTPUT_DIR:-${BUILD_ROOT}/output}"
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
export TZ=UTC
export LANG=C
export LC_ALL=C
export MACOSX_DEPLOYMENT_TARGET
export PATH="${QT_ROOT}/bin:${PATH}"
# CMake 4.0 removed support for cmake_minimum_required()/cmake_policy(VERSION)
# calls below 3.5, and several vendored dependencies below (libevent, Olm,
# qtkeychain) pin an older minimum in their own CMakeLists.txt that this
# script does not control. The top-level app's own CMakeLists.txt already
# works around this for itself (see its CMAKE_VERSION >= 3.25 check setting
# this same variable), but that only takes effect once CMake is already
# inside that project's own configure step -- it cannot help a *different*
# project's cmake_minimum_required() call. Setting this as an environment
# variable is CMake's own documented mechanism for exactly this situation:
# it seeds the CMAKE_POLICY_VERSION_MINIMUM cache entry for every new build
# tree this script configures (the app and every dependency), without
# patching any dependency's source.
export CMAKE_POLICY_VERSION_MINIMUM=3.5
umask 022

readonly JOBS="$(sysctl -n hw.ncpu)"
readonly CLANG="$(xcrun --sdk macosx --find clang)"
readonly CLANGXX="$(xcrun --sdk macosx --find clang++)"
readonly SDKROOT="$(xcrun --sdk macosx --show-sdk-path)"

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

# The checked-out source (including the openssl submodule this script builds
# in place, see below) must start clean so a repeated invocation in the same
# workspace behaves the same as a first run. Any build residue left inside
# the openssl submodule by a previous run of this exact script is removed on
# exit, not just checked for up front.
openssl_source_dir="${PROJECT_DIR}/contrib/libnunchuk/contrib/openssl"
cleanup_openssl_build_residue() {
    # `checkout -- .` first reverts any tracked file `./config`/`make
    # install_dev` rewrote in place (e.g. permission bits, generated headers
    # tracked upstream) -- `git clean` alone cannot touch modified tracked
    # files, only untracked ones. `-ff` (double force, not `-f`) is required
    # for `clean` to also remove untracked directories that themselves look
    # like git repositories, which OpenSSL's own build can leave behind.
    # Without both, this nested tree can stay dirty after a build even
    # though this cleanup ran, which fails the very next clean-tree check
    # below on a second invocation in the same checkout (e.g. the opt-in
    # reproducibility-check rebuild in the same CI job).
    git -C "${openssl_source_dir}" checkout --quiet -- . >/dev/null 2>&1 || true
    git -C "${openssl_source_dir}" clean -ffdx --quiet >/dev/null 2>&1 || true
}
# Run once up front too, not only on exit: if a previous run of this script
# was killed before its own EXIT trap fired (runner timeout, cancelled job),
# this is the only thing that can still recover a clean tree for this run.
cleanup_openssl_build_residue
trap cleanup_openssl_build_residue EXIT

if [[ -n "$(git -C "${PROJECT_DIR}" status --porcelain --untracked-files=all)" ]]; then
    echo "The reproducible macOS builder requires a completely clean source tree." >&2
    git -C "${PROJECT_DIR}" status --short --untracked-files=all >&2
    # The line(s) above only report *that* a submodule is dirty (e.g. " M
    # contrib/libnunchuk") -- git's top-level status never expands into a
    # submodule's own working tree, so a dirty-submodule failure alone gives
    # no actionable detail. Two independent things can each cause that " M":
    # (1) modified/untracked content inside the submodule's own working
    # tree, or (2) the submodule's checked-out commit itself having moved
    # away from what the superproject's index recorded, with no local
    # content changes at all -- e.g. something ran `git checkout`/`git
    # submodule update` to a different ref during configure/build. Case (2)
    # produces no output from a plain `git status` inside the submodule, so
    # both are checked and reported separately below.
    echo "--- git submodule status --recursive (a leading +/- means a checked-out commit changed) ---" >&2
    git -C "${PROJECT_DIR}" submodule status --recursive >&2
    git -C "${PROJECT_DIR}" submodule foreach --quiet --recursive '
        if test -n "$(git status --porcelain --untracked-files=all)"; then
            echo "--- dirty submodule content: $displaypath ---" >&2
            git status --short --untracked-files=all >&2
        fi
    ' >&2
    exit 1
fi
git -C "${PROJECT_DIR}" submodule foreach --quiet --recursive '
    if test -n "$(git status --porcelain --untracked-files=all)"; then
        echo "Dirty source in submodule: $displaypath" >&2
        git status --short --untracked-files=all >&2
        exit 1
    fi
'

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

# --- HWI, built from source for both architectures (matching the reference
# workflow's own approach; the previous draft used a prebuilt x86_64 release
# asset instead, which was never actually validated).
export HOMEBREW_NO_AUTO_UPDATE=1
brew install pyenv libusb >/dev/null

# Matches the reference workflow exactly: `brew install pyenv` only installs
# the pyenv command itself; PYENV_ROOT ($HOME/.pyenv) is created lazily by
# pyenv on first use, not by this script.
export PYENV_ROOT="${HOME}/.pyenv"
export PATH="${PYENV_ROOT}/bin:${PATH}"
eval "$(pyenv init --path)"
pyenv install --skip-existing "${PYTHON_VERSION}"
pyenv global "${PYTHON_VERSION}"
python3 --version

hwi_source_dir="${SOURCE_ROOT}/hwi"
checkout_commit "https://github.com/nogibi/HWI.git" "${HWI_COMMIT}" "${hwi_source_dir}"
(
    cd "${hwi_source_dir}"
    ./contrib/build_bin.sh --without-gui
)
hwi_built_binary="$(find "${hwi_source_dir}/dist" -path '*.tar.gz.dir/hwi' -type f -perm -111 -print -quit)"
if [[ -z "${hwi_built_binary}" ]]; then
    echo "HWI build did not produce the expected dist/*.tar.gz.dir/hwi executable." >&2
    exit 1
fi
hwi_binary="${BUILD_ROOT}/hwi/hwi"
mkdir -p "$(dirname "${hwi_binary}")"
install -m 0755 "${hwi_built_binary}" "${hwi_binary}"
if ! lipo "${hwi_binary}" -verify_arch "${ARCH}"; then
    echo "HWI does not contain the requested architecture ${ARCH}." >&2
    exit 1
fi
"${hwi_binary}" --version

# --- Boost ${BOOST_VERSION} (static, matching the reference workflow's b2
# options; version itself is not from the reference -- see macos.lock.env's
# BOOST_VERSION comment).
boost_archive="${DOWNLOAD_DIR}/boost_${BOOST_VERSION//./_}.tar.gz"
download_checked \
    "https://archives.boost.io/release/${BOOST_VERSION}/source/boost_${BOOST_VERSION//./_}.tar.gz" \
    "${BOOST_SHA256}" \
    "${boost_archive}"
tar -xzf "${boost_archive}" -C "${SOURCE_ROOT}"
boost_source="${SOURCE_ROOT}/boost_${BOOST_VERSION//./_}"
boost_prefix="${DEPS_ROOT}/boost"
(
    cd "${boost_source}"
    ./bootstrap.sh --prefix="${boost_prefix}"
    ./b2 install \
        --prefix="${boost_prefix}" \
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

# --- libevent 2.1.12, built explicitly with Xcode's clang (matching the
# reference workflow, which never sources the CC=gcc-14 override for this
# dependency either).
libevent_archive="${DOWNLOAD_DIR}/libevent-${LIBEVENT_VERSION}.tar.gz"
download_checked \
    "https://github.com/libevent/libevent/releases/download/release-${LIBEVENT_VERSION}-stable/libevent-${LIBEVENT_VERSION}-stable.tar.gz" \
    "${LIBEVENT_SHA256}" \
    "${libevent_archive}"
tar -xzf "${libevent_archive}" -C "${SOURCE_ROOT}"
libevent_prefix="${DEPS_ROOT}/libevent"
cmake -S "${SOURCE_ROOT}/libevent-${LIBEVENT_VERSION}-stable" \
    -B "${BUILD_ROOT}/libevent-build" \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX="${libevent_prefix}" \
    -DCMAKE_C_COMPILER="${CLANG}" \
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
lipo "${libevent_prefix}/lib/libevent.a" -verify_arch "${ARCH}"

# --- Berkeley DB 4.8.30.NC via Bitcoin Core's installer script, with the
# same macOS-specific patches the reference workflow applies (GNU-style
# sha256sum/-c isn't the CLI this installer expects on modern macOS; tar -v
# isn't needed; only the static libs/headers are required, not the full
# `make install`).
bdb_build_root="${BUILD_ROOT}/bdb-4.8.30.NC"
bdb_installer="${DOWNLOAD_DIR}/install_db4.sh"
bdb_prefix="${bdb_build_root}/db4"
download_checked \
    "https://raw.githubusercontent.com/bitcoin/bitcoin/${BDB_INSTALLER_COMMIT}/contrib/install_db4.sh" \
    "${BDB_INSTALLER_SHA256}" \
    "${bdb_installer}"
chmod +x "${bdb_installer}"
sed -i '' \
    -e 's/if check_exists sha256sum; then/if false; then/' \
    -e 's/elif check_exists sha256; then/elif false; then/' \
    -e 's/tar -xzvf/tar -xzf/' \
    -e 's/^make install$/make libdb_cxx-4.8.a libdb-4.8.a install_lib install_include/' \
    -e "s|^CONFIG_GUESS_URL=.*|CONFIG_GUESS_URL='https://raw.githubusercontent.com/bitcoin/bitcoin/${BDB_INSTALLER_COMMIT}/depends/config.guess'|" \
    -e "s|^CONFIG_SUB_URL=.*|CONFIG_SUB_URL='https://raw.githubusercontent.com/bitcoin/bitcoin/${BDB_INSTALLER_COMMIT}/depends/config.sub'|" \
    "${bdb_installer}"
mkdir -p "${bdb_build_root}"
(
    # install_db4.sh downloads "db-4.8.30.NC.tar.gz" with `curl -O` and
    # extracts/builds it into whatever directory it is run from -- it does
    # not confine that to its <base-dir> argument. Without this `cd`, both
    # land in this script's own working directory (PROJECT_DIR, when invoked
    # from a CI step with a repo-rooted working-directory default), leaving
    # the checkout dirty and failing the clean-tree check on any later
    # invocation in the same checkout (e.g. the opt-in
    # reproducibility-check rebuild in the same CI job).
    cd "${bdb_build_root}"
    unset CPATH C_INCLUDE_PATH CPLUS_INCLUDE_PATH LIBRARY_PATH \
        OBJC_INCLUDE_PATH OBJCPLUS_INCLUDE_PATH
    export CC="${CLANG}"
    export CXX="${CLANGXX}"
    export AR="$(xcrun --sdk macosx --find ar)"
    export RANLIB="$(xcrun --sdk macosx --find ranlib)"
    # -DSTDC_HEADERS=1: dbinc/db_int.h picks <stdarg.h> vs. the legacy
    # <varargs.h> based on whether autoconf's STDC_HEADERS macro got defined.
    # BDB 4.8's own `configure` (this installer's target) is not reliably
    # setting it on this toolchain, so mut_tas.c falls through to
    # `#include <varargs.h>` -- and Xcode 16.4/clang 17 turns that header
    # into a hard `#error "Please use <stdarg.h> instead of <varargs.h>"`
    # (older Xcode versions shipped a harmless deprecated stub instead, which
    # is presumably why this was not caught before). This is not something
    # `-Wno-error=...` can paper over: it is a preprocessor #error, not a
    # compiler warning. Forcing STDC_HEADERS=1 is factually correct on any
    # modern macOS toolchain (fully conformant ANSI C headers) and matches
    # what autoconf's own AC_HEADER_STDC test is trying to detect in the
    # first place -- this just supplies the answer directly instead of
    # relying on that 2010-era test still working on today's toolchain.
    export CFLAGS="-arch ${ARCH} -mmacosx-version-min=${MACOSX_DEPLOYMENT_TARGET} -isysroot ${SDKROOT} -DSTDC_HEADERS=1 -Wno-error=implicit-function-declaration -Wno-error=format-security -Wno-error=implicit-int -Wno-deprecated-non-prototype"
    export CXXFLAGS="-arch ${ARCH} -mmacosx-version-min=${MACOSX_DEPLOYMENT_TARGET} -isysroot ${SDKROOT} -stdlib=libc++"
    export LDFLAGS="-arch ${ARCH} -mmacosx-version-min=${MACOSX_DEPLOYMENT_TARGET} -isysroot ${SDKROOT}"
    export MAKEFLAGS="-j1 -s"
    "${bdb_installer}" "${bdb_build_root}"
)
if [[ ! -f "${bdb_prefix}/lib/libdb_cxx-4.8.a" ]]; then
    echo "Berkeley DB build did not produce libdb_cxx-4.8.a." >&2
    exit 1
fi
lipo "${bdb_prefix}/lib/libdb_cxx-4.8.a" -verify_arch "${ARCH}"

# --- Olm 3.2.16, with the same clang-17 const-correctness patch the
# reference workflow applies before building.
olm_source_dir="${SOURCE_ROOT}/olm"
checkout_commit \
    "https://gitlab.matrix.org/matrix-org/olm.git" \
    "${OLM_COMMIT}" \
    "${olm_source_dir}"
sed -i '' \
    's/T \* const other_pos = other\._data;/T const * other_pos = other._data;/' \
    "${olm_source_dir}/include/olm/list.hh"
if ! grep -Fq 'T const * other_pos = other._data;' "${olm_source_dir}/include/olm/list.hh"; then
    echo "Failed to apply the Olm const-correctness patch." >&2
    exit 1
fi
olm_prefix="${DEPS_ROOT}/olm"
cmake -S "${olm_source_dir}" \
    -B "${BUILD_ROOT}/olm-build" \
    -DBUILD_SHARED_LIBS=OFF \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX="${olm_prefix}" \
    -DCMAKE_OSX_ARCHITECTURES="${ARCH}" \
    -DCMAKE_OSX_DEPLOYMENT_TARGET="${MACOSX_DEPLOYMENT_TARGET}" \
    -DOLM_TESTS=OFF
cmake --build "${BUILD_ROOT}/olm-build" --parallel "${JOBS}"
cmake --install "${BUILD_ROOT}/olm-build"

# --- QtKeychain 0.14.2. Installed into a private prefix instead of the
# reference workflow's `sudo cmake --install . --prefix "$BREW_PREFIX"`,
# which would mutate the runner's shared Homebrew installation; the build
# itself is otherwise identical.
qtkeychain_source_dir="${SOURCE_ROOT}/qtkeychain"
git clone --depth 1 --branch "${QTKEYCHAIN_VERSION}" \
    https://github.com/frankosterfeld/qtkeychain.git "${qtkeychain_source_dir}"
qtkeychain_prefix="${DEPS_ROOT}/qtkeychain"
cmake -S "${qtkeychain_source_dir}" \
    -B "${BUILD_ROOT}/qtkeychain-build" \
    -DBUILD_WITH_QT6=ON \
    -DCMAKE_INSTALL_PREFIX="${qtkeychain_prefix}" \
    -DCMAKE_OSX_ARCHITECTURES="${ARCH}" \
    -DCMAKE_OSX_DEPLOYMENT_TARGET="${MACOSX_DEPLOYMENT_TARGET}" \
    -DCMAKE_PREFIX_PATH="${QT_ROOT}/lib/cmake"
cmake --build "${BUILD_ROOT}/qtkeychain-build" --parallel "${JOBS}"
cmake --install "${BUILD_ROOT}/qtkeychain-build"

# --- OpenSSL: NOT downloaded. This builds the copy already vendored inside
# the libnunchuk submodule at contrib/openssl, installing to contrib/openssl/lib
# because contrib/libnunchuk/CMakeLists.txt hard-requires that exact in-tree
# path (`if(EXISTS ".../contrib/openssl/lib") set(OPENSSL_ROOT_DIR ...)`) --
# an out-of-tree build here would leave the app unable to find OpenSSL at
# all. Build residue inside this submodule is removed on exit (see the
# cleanup_openssl_build_residue trap above) so a repeated run starts clean.
(
    cd "${openssl_source_dir}"
    ./config --prefix="${openssl_source_dir}/lib"
    make -j"${JOBS}"
    make install_dev
)
if [[ ! -f "${openssl_source_dir}/lib/lib/libssl.a" && ! -f "${openssl_source_dir}/lib/lib/libssl.dylib" ]]; then
    echo "OpenSSL build did not produce a lib/lib/libssl artifact." >&2
    exit 1
fi

export BOOST_ROOT="${boost_prefix}"
export PATH="${QT_ROOT}/bin:${PATH}"

app_build_dir="${BUILD_ROOT}/app-build"
# ${boost_prefix} is in CMAKE_PREFIX_PATH (in addition to -DBOOST_ROOT below)
# because two independent Boost lookups happen during this configure, not
# one: the top-level app's own find_package(Boost) (classic MODULE mode via
# CMake's bundled FindBoost.cmake, which honors BOOST_ROOT) and, separately,
# contrib/libnunchuk/contrib/bitcoin/cmake/module/AddBoostIfNeeded.cmake's
# find_package(Boost 1.73.0 REQUIRED CONFIG) (CONFIG mode, which does not
# consult BOOST_ROOT at all, only Boost_ROOT/CMAKE_PREFIX_PATH/registry).
# Without this, the second lookup either fails outright or -- worse, on a
# machine with Homebrew's own boost installed -- silently resolves to
# whatever version Homebrew currently ships instead of this pinned build
# (see macos.lock.env's BOOST_VERSION comment for why that already broke
# once). ${boost_prefix} satisfies both: CMake's CONFIG search checks
# <prefix>/lib/cmake/Boost-${BOOST_VERSION}/ for every entry in
# CMAKE_PREFIX_PATH, which is where `b2 install` places the package-config
# files this Boost version generates.
cmake -S "${PROJECT_DIR}" -B "${app_build_dir}" \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_OSX_DEPLOYMENT_TARGET="${MACOSX_DEPLOYMENT_TARGET}" \
    -DCMAKE_PREFIX_PATH="${QT_ROOT}/lib/cmake;${olm_prefix};${libevent_prefix};${bdb_prefix};${qtkeychain_prefix};${boost_prefix}" \
    -DBOOST_ROOT="${boost_prefix}" \
    -Devent_lib:FILEPATH="${libevent_prefix}/lib/libevent.a" \
    -DAPPEND_CPPFLAGS="-I${libevent_prefix}/include" \
    -DUR__DISABLE_TESTS=ON
# Retried rather than dropped to -j1: a transient host-level I/O flake has
# been observed here ("ranlib: can't write to output file (Input/output
# error)"), consistent with the same kind of macOS-runner disk-arbitration
# hiccup already retried for hdiutil in sign_macos.sh, not a determinism bug
# in this build. A retry is cheap -- cmake/make only recompile/relink
# whatever object failed to write, not a full rebuild -- and does not cost
# extra time on the (normal) non-flaky run, unlike permanently lowering
# --parallel, which would slow down every single build to guard against a
# rare event.
app_build_succeeded=0
for attempt in 1 2 3; do
    if cmake --build "${app_build_dir}" --parallel "${JOBS}"; then
        app_build_succeeded=1
        break
    fi
    echo "cmake --build (app) failed (attempt ${attempt}/3), retrying..." >&2
    if (( attempt < 3 )); then
        sleep $((attempt * 10))
    fi
done
if (( app_build_succeeded != 1 )); then
    echo "cmake --build (app) failed after three attempts." >&2
    exit 1
fi

APP_PATH="${app_build_dir}/Nunchuk.app" \
HWI_BINARY="${hwi_binary}" \
PROJECT_DIR="${PROJECT_DIR}" \
QT_ROOT="${QT_ROOT}" \
OUTPUT_DIR="${OUTPUT_DIR}" \
ARCH="${ARCH}" \
TAG="${TAG}" \
SOURCE_DATE_EPOCH="${SOURCE_DATE_EPOCH}" \
    bash "${PROJECT_DIR}/reproducible-builds/package_macos.sh"
