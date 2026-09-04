#!/usr/bin/env bash
set -euo pipefail

PROJECT_DIR="${PROJECT_DIR:-/project}"
TAG="${TAG:-0.0.0}"
HWI_VERSION="3.2.0-displayaddress"
HWI_ARCHIVE="hwi-3.2.0-linux-x86_64.tar.gz"
HWI_SHA256="ee7cf2afe085128c55ebaa1fb1da45831ae82a02f5b8c462c1dc95e6e606220a"

if [[ "${PROJECT_DIR}" != /* || ! -f "${PROJECT_DIR}/CMakeLists.txt" ]]; then
    echo "PROJECT_DIR must be an absolute Nunchuk source directory: ${PROJECT_DIR}" >&2
    exit 1
fi

if [[ ! "${TAG}" =~ ^[0-9]+\.[0-9]+\.[0-9]+([.-][0-9A-Za-z]+)*$ ]]; then
    echo "Invalid TAG: ${TAG}" >&2
    exit 1
fi

SOURCE_DATE_EPOCH="${SOURCE_DATE_EPOCH:-$(git -c safe.directory="${PROJECT_DIR}" -C "${PROJECT_DIR}" log -1 --pretty=%ct)}"
if [[ ! "${SOURCE_DATE_EPOCH}" =~ ^[0-9]+$ ]]; then
    echo "Invalid SOURCE_DATE_EPOCH: ${SOURCE_DATE_EPOCH}" >&2
    exit 1
fi

export SOURCE_DATE_EPOCH
export TZ=UTC
export LANG=C.UTF-8
export LC_ALL=C.UTF-8
export ZERO_AR_DATE=1
export APPIMAGE_EXTRACT_AND_RUN=1
umask 022

PACKAGE_NAME="nunchuk-linux-v${TAG}"
PACKAGE_DIR="${PROJECT_DIR}/${PACKAGE_NAME}"
APP_DIR="${PACKAGE_DIR}/Appdir"
APPIMAGE_NAME="${PACKAGE_NAME}.AppImage"
APPIMAGE_PATH="${PACKAGE_DIR}/${APPIMAGE_NAME}"
ZIP_NAME="${PACKAGE_NAME}.zip"
NUNCHUK_BINARY="${PROJECT_DIR}/build/nunchuk-qt"
DESKTOP_FILE="${PACKAGE_DIR}/nunchuk.desktop"
CUSTOM_APPRUN="${PACKAGE_DIR}/nunchuk.AppRun"

ALLOWED_QT_PLUGINS=(
    platforms/libqxcb.so
    xcbglintegrations/libqxcb-egl-integration.so
    xcbglintegrations/libqxcb-glx-integration.so
    imageformats/libqjpeg.so
    imageformats/libqsvg.so
    iconengines/libqsvgicon.so
    multimedia/libffmpegmediaplugin.so
    sqldrivers/libqsqlite.so
    tls/libqopensslbackend.so
    platforminputcontexts/libcomposeplatforminputcontextplugin.so
    platforminputcontexts/libibusplatforminputcontextplugin.so
    platformthemes/libqxdgdesktopportal.so
)

REQUIRED_QT_PLUGINS=(
    platforms/libqxcb.so
    imageformats/libqjpeg.so
    imageformats/libqsvg.so
    iconengines/libqsvgicon.so
    multimedia/libffmpegmediaplugin.so
    sqldrivers/libqsqlite.so
    tls/libqopensslbackend.so
)

check_ldd() {
    local candidate="$1"
    local search_path="$2"
    local output
    local status

    set +e
    output="$(LD_LIBRARY_PATH="${search_path}" ldd "${candidate}" 2>&1)"
    status=$?
    set -e
    if (( status != 0 )) \
        && [[ "${output}" != *"not a dynamic executable"* ]] \
        && [[ "${output}" != *"statically linked"* ]]; then
        echo "ldd failed for ${candidate}:" >&2
        echo "${output}" >&2
        exit 1
    fi
    if grep -q 'not found' <<< "${output}"; then
        echo "Unresolved runtime dependency in ${candidate}:" >&2
        echo "${output}" >&2
        exit 1
    fi
}

restrict_qt_plugins() {
    local plugin_root="$1"
    local plugin
    local relative_plugin
    local allowed
    local keep

    if [[ ! -d "${plugin_root}" ]]; then
        echo "Qt plugin root was not deployed: ${plugin_root}" >&2
        exit 1
    fi

    while IFS= read -r -d '' plugin; do
        relative_plugin="${plugin#"${plugin_root}/"}"
        keep=0
        for allowed in "${ALLOWED_QT_PLUGINS[@]}"; do
            if [[ "${relative_plugin}" == "${allowed}" ]]; then
                keep=1
                break
            fi
        done
        if (( keep == 0 )); then
            echo "Removing unused Qt plugin: ${relative_plugin}"
            rm -f -- "${plugin}"
        fi
    done < <(
        find "${plugin_root}" \( -type f -o -type l \) -name '*.so*' -print0
    )

    for relative_plugin in "${REQUIRED_QT_PLUGINS[@]}"; do
        if [[ ! -f "${plugin_root}/${relative_plugin}" ]]; then
            echo "Required Qt plugin was not deployed: ${relative_plugin}" >&2
            exit 1
        fi
    done
}

verify_custom_apprun() {
    local app_root="$1"
    local candidate
    local selected=""

    for candidate in "${app_root}/AppRun" "${app_root}/AppRun.wrapped"; do
        if [[ -f "${candidate}" ]] && grep -Fq '# nunchuk-custom-apprun' "${candidate}"; then
            selected="${candidate}"
            break
        fi
    done
    if [[ -z "${selected}" ]]; then
        echo "linuxdeploy did not preserve the Nunchuk AppRun" >&2
        find "${app_root}" -maxdepth 1 -name 'AppRun*' -print >&2
        exit 1
    fi

    grep -Fqx 'export QTWEBENGINE_DISABLE_SANDBOX=1' "${selected}"
    grep -Fqx 'export QT_MEDIA_BACKEND=ffmpeg' "${selected}"
    grep -Fqx 'export OPENSSL_MODULES="$APPDIR/usr/lib/ossl-modules"' "${selected}"
    grep -Fqx 'exec "$APPDIR/usr/bin/nunchuk-qt" "$@"' "${selected}"

    if [[ "${selected}" == "${app_root}/AppRun.wrapped" ]] \
        && ! grep -Fq 'AppRun.wrapped' "${app_root}/AppRun"; then
        echo "Generated AppRun does not forward to AppRun.wrapped" >&2
        exit 1
    fi
}

verify_desktop_metadata() {
    local desktop_file="$1"
    local version_key_count

    if [[ ! -f "${desktop_file}" ]]; then
        echo "Desktop metadata was not deployed: ${desktop_file}" >&2
        exit 1
    fi

    desktop-file-validate --no-hints "${desktop_file}"
    version_key_count="$(grep -c '^X-AppImage-Version=' "${desktop_file}" || true)"
    if [[ "${version_key_count}" != 1 ]] \
        || ! grep -Fqx "X-AppImage-Version=${TAG}" "${desktop_file}"; then
        echo "Desktop metadata has an unexpected AppImage version: ${desktop_file}" >&2
        grep '^X-AppImage-Version=' "${desktop_file}" >&2 || true
        exit 1
    fi
}

normalize_appdir_permissions() {
    local requested_appdir="$1"
    local appdir
    local unexpected
    local candidate
    local target
    local entry
    local mode
    local expected_mode
    local executable_target
    local -a executable_candidates
    local -a executable_targets=()

    appdir="$(readlink -f -- "${requested_appdir}" 2>/dev/null || true)"
    if [[ -z "${appdir}" || ! -d "${appdir}/usr" ]]; then
        echo "Invalid AppDir for permission normalization: ${requested_appdir}" >&2
        exit 1
    fi

    unexpected="$(find "${appdir}" ! -type d ! -type f ! -type l -print -quit)"
    if [[ -n "${unexpected}" ]]; then
        echo "Unexpected AppDir entry type: ${unexpected}" >&2
        exit 1
    fi

    # Canonical modes must not depend on archive, checkout or deploy-tool modes.
    # Shared objects only need to be readable; only process entrypoints need +x.
    find "${appdir}" -type d -exec chmod 0755 -- {} +
    find "${appdir}" -type f -exec chmod 0644 -- {} +

    executable_candidates=(
        "${appdir}/AppRun"
        "${appdir}/usr/bin/nunchuk-qt"
        "${appdir}/usr/bin/hwi"
        "${appdir}/usr/libexec/QtWebEngineProcess"
    )
    if [[ -e "${appdir}/AppRun.wrapped" || -L "${appdir}/AppRun.wrapped" ]]; then
        executable_candidates+=("${appdir}/AppRun.wrapped")
    fi

    for candidate in "${executable_candidates[@]}"; do
        if [[ ! -e "${candidate}" && ! -L "${candidate}" ]]; then
            echo "Required AppDir executable is missing: ${candidate}" >&2
            exit 1
        fi
        target="$(readlink -f -- "${candidate}" 2>/dev/null || true)"
        if [[ -z "${target}" || ! -f "${target}" ]]; then
            echo "Invalid AppDir executable: ${candidate}" >&2
            exit 1
        fi
        case "${target}" in
            "${appdir}"/*)
                ;;
            *)
                echo "AppDir executable escapes the bundle: ${candidate} => ${target}" >&2
                exit 1
                ;;
        esac
        chmod 0755 -- "${target}"
        executable_targets+=("${target}")
    done

    while IFS= read -r -d '' entry; do
        mode="$(stat -c '%a' -- "${entry}")"
        if [[ "${mode}" != 755 ]]; then
            echo "Unexpected AppDir directory mode ${mode}: ${entry}" >&2
            exit 1
        fi
    done < <(find "${appdir}" -type d -print0 | LC_ALL=C sort -z)

    while IFS= read -r -d '' entry; do
        expected_mode=644
        for executable_target in "${executable_targets[@]}"; do
            if [[ "${entry}" == "${executable_target}" ]]; then
                expected_mode=755
                break
            fi
        done
        mode="$(stat -c '%a' -- "${entry}")"
        if [[ "${mode}" != "${expected_mode}" ]]; then
            echo "Unexpected AppDir file mode ${mode}; expected ${expected_mode}: ${entry}" >&2
            exit 1
        fi
    done < <(find "${appdir}" -type f -print0 | LC_ALL=C sort -z)
}

write_appdir_manifest() {
    local requested_appdir="$1"
    local output_file="$2"
    local appdir
    local entry
    local relative_path
    local entry_type
    local mode
    local uid
    local gid
    local mtime
    local size
    local payload

    appdir="$(readlink -f -- "${requested_appdir}" 2>/dev/null || true)"
    if [[ -z "${appdir}" || ! -d "${appdir}" ]]; then
        echo "Invalid AppDir for metadata manifest: ${requested_appdir}" >&2
        exit 1
    fi

    : > "${output_file}"
    while IFS= read -r -d '' entry; do
        if [[ "${entry}" == "${appdir}" ]]; then
            relative_path=.
        else
            relative_path="${entry#"${appdir}/"}"
        fi
        mode="$(stat -c '%a' -- "${entry}")"
        uid="$(stat -c '%u' -- "${entry}")"
        gid="$(stat -c '%g' -- "${entry}")"
        mtime="$(stat -c '%Y' -- "${entry}")"
        size=-
        payload=-

        if [[ -L "${entry}" ]]; then
            entry_type=symlink
            payload="$(readlink -- "${entry}")"
        elif [[ -d "${entry}" ]]; then
            entry_type=directory
        elif [[ -f "${entry}" ]]; then
            entry_type=file
            size="$(stat -c '%s' -- "${entry}")"
            payload="$(sha256sum -- "${entry}" | awk '{ print $1 }')"
        else
            echo "Unexpected AppDir entry while creating manifest: ${entry}" >&2
            exit 1
        fi

        printf '%q\t%s\t%s\t%s\t%s\t%s\t%s\t%q\n' \
            "${relative_path}" \
            "${entry_type}" \
            "${mode}" \
            "${uid}" \
            "${gid}" \
            "${mtime}" \
            "${size}" \
            "${payload}" \
            >> "${output_file}"
    done < <(
        printf '%s\0' "${appdir}"
        find "${appdir}" -mindepth 1 -print0 | LC_ALL=C sort -z
    )
}

if [[ ! -x "${NUNCHUK_BINARY}" ]]; then
    echo "Application binary does not exist: ${NUNCHUK_BINARY}" >&2
    exit 1
fi

QMAKE="$(command -v qmake6 || command -v qmake)"
QT_QML_PATH="$(${QMAKE} -query QT_INSTALL_QML)"
QT_SHADER_TOOLS_LIB="${QT_INSTALLED_PREFIX}/lib/libQt6ShaderTools.so.6"
GRAPHICAL_EFFECTS_SOURCE="${QT_QML_PATH}/Qt5Compat/GraphicalEffects/private/libqtgraphicaleffectsprivateplugin.so"
WEBENGINE_CORE="$(find "${QT_INSTALLED_PREFIX}/lib" -maxdepth 1 -type f -name 'libQt6WebEngineCore.so.*' -print -quit)"
QXCB_PLUGIN="${QT_INSTALLED_PREFIX}/plugins/platforms/libqxcb.so"

for required_file in \
    "${QT_SHADER_TOOLS_LIB}" \
    "${GRAPHICAL_EFFECTS_SOURCE}" \
    "${WEBENGINE_CORE}" \
    "${QXCB_PLUGIN}"; do
    if [[ -z "${required_file}" || ! -f "${required_file}" ]]; then
        echo "Required Qt 6 runtime input is missing: ${required_file}" >&2
        exit 1
    fi
    check_ldd "${required_file}" "${QT_INSTALLED_PREFIX}/lib:${OPENSSL_ROOT_DIR}/lib"
done

cmake -E remove_directory "${PACKAGE_DIR}"
mkdir -p "${APP_DIR}/usr/bin"

HWI_DOWNLOAD="${PACKAGE_DIR}/${HWI_ARCHIVE}"
HWI_EXTRACT_DIR="${PACKAGE_DIR}/hwi-extracted"
curl --fail --location --show-error \
    --output "${HWI_DOWNLOAD}" \
    "https://github.com/nogibi/HWI/releases/download/${HWI_VERSION}/${HWI_ARCHIVE}"
echo "${HWI_SHA256}  ${HWI_DOWNLOAD}" | sha256sum --check --strict
mkdir -p "${HWI_EXTRACT_DIR}"
tar -xzf "${HWI_DOWNLOAD}" -C "${HWI_EXTRACT_DIR}"
install -m 0755 "${HWI_EXTRACT_DIR}/hwi" "${APP_DIR}/usr/bin/hwi"
rm -rf -- "${HWI_DOWNLOAD}" "${HWI_EXTRACT_DIR}"

cat > "${DESKTOP_FILE}" <<'EOF'
[Desktop Entry]
Type=Application
Name=Nunchuk
Comment=Bitcoin multisig wallet
Exec=nunchuk-qt
Icon=nunchuk-qt
Categories=Utility;
Terminal=false
EOF
printf 'X-AppImage-Version=%s\n' "${TAG}" >> "${DESKTOP_FILE}"
verify_desktop_metadata "${DESKTOP_FILE}"

cat > "${CUSTOM_APPRUN}" <<'EOF'
#!/bin/sh
# nunchuk-custom-apprun
set -eu
APPDIR=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
export QTWEBENGINE_DISABLE_SANDBOX=1
export QT_MEDIA_BACKEND=ffmpeg
export OPENSSL_MODULES="$APPDIR/usr/lib/ossl-modules"
export PATH="$APPDIR/usr/bin:$PATH"
exec "$APPDIR/usr/bin/nunchuk-qt" "$@"
EOF
chmod 0755 "${CUSTOM_APPRUN}"

export PATH="/usr/local/bin:${QT_INSTALLED_PREFIX}/bin:${PATH}"
export QMAKE
export QML_SOURCES_PATHS="${PROJECT_DIR}"
export NO_STRIP=1
export EXTRA_QT_MODULES=svg
unset EXTRA_QT_PLUGINS EXTRA_PLATFORM_PLUGINS
export LD_LIBRARY_PATH="${OPENSSL_ROOT_DIR}/lib:${QT_INSTALLED_PREFIX}/lib${LD_LIBRARY_PATH:+:${LD_LIBRARY_PATH}}"
export VERSION="${TAG}"
export ARCH=x86_64

# First pass deploys the executable, its QML imports and the selected Qt 6
# runtime. ShaderTools is explicit because it is loaded by Qt5Compat's private
# GraphicalEffects plugin and therefore is not visible in the main ELF graph.
linuxdeploy \
    --appdir "${APP_DIR}" \
    --executable "${NUNCHUK_BINARY}" \
    --library "${QT_SHADER_TOOLS_LIB}" \
    --desktop-file "${DESKTOP_FILE}" \
    --icon-file "${PROJECT_DIR}/deploy/nunchuk-qt.png" \
    --custom-apprun "${CUSTOM_APPRUN}" \
    --plugin qt

PLUGIN_ROOT="${APP_DIR}/usr/plugins"
restrict_qt_plugins "${PLUGIN_ROOT}"

GRAPHICAL_EFFECTS_PLUGIN="${APP_DIR}/usr/qml/Qt5Compat/GraphicalEffects/private/libqtgraphicaleffectsprivateplugin.so"
if [[ ! -f "${GRAPHICAL_EFFECTS_PLUGIN}" ]]; then
    echo "Qt5Compat GraphicalEffects private plugin was not deployed" >&2
    exit 1
fi
if [[ -z "$(find "${APP_DIR}/usr/lib" -maxdepth 1 -name 'libQt6ShaderTools.so*' -print -quit)" ]]; then
    echo "Qt6ShaderTools was not deployed with Qt5Compat GraphicalEffects" >&2
    exit 1
fi

# Qt's OpenSSL backend is loaded dynamically. Force the pinned OpenSSL 3.5.7
# runtime and provider modules into the bundle even if the host also supplies a
# compatible libssl.so.3.
mkdir -p "${APP_DIR}/usr/lib/ossl-modules"
install -m 0644 "$(readlink -f "${OPENSSL_ROOT_DIR}/lib/libssl.so.3")" "${APP_DIR}/usr/lib/libssl.so.3"
install -m 0644 "$(readlink -f "${OPENSSL_ROOT_DIR}/lib/libcrypto.so.3")" "${APP_DIR}/usr/lib/libcrypto.so.3"
install -m 0755 "${OPENSSL_ROOT_DIR}/lib/ossl-modules/legacy.so" "${APP_DIR}/usr/lib/ossl-modules/legacy.so"

# The Qt deploy plugin adds QML/plugin ELFs after linuxdeploy's initial scan.
# Feed every resulting ELF back through linuxdeploy to close their dependency
# graphs before producing the deterministic squashfs.
export LD_LIBRARY_PATH="${APP_DIR}/usr/lib:${OPENSSL_ROOT_DIR}/lib:${QT_INSTALLED_PREFIX}/lib"
APPDIR_ELF_ARGS=()
while IFS= read -r -d '' candidate; do
    if [[ "$(file -b "${candidate}")" == *ELF* ]]; then
        APPDIR_ELF_ARGS+=(--deploy-deps-only "${candidate}")
    fi
done < <(find "${APP_DIR}" -type f -print0 | LC_ALL=C sort -z)

if (( ${#APPDIR_ELF_ARGS[@]} == 0 )); then
    echo "No ELF files were found in AppDir for dependency closure" >&2
    exit 1
fi

rm -f -- "${APP_DIR}/AppRun" "${APP_DIR}/AppRun.wrapped"
linuxdeploy \
    --appdir "${APP_DIR}" \
    "${APPDIR_ELF_ARGS[@]}" \
    --custom-apprun "${CUSTOM_APPRUN}"

# Re-assert security- and compatibility-critical contents after the dependency
# closure pass, which is allowed to update AppDir.
restrict_qt_plugins "${PLUGIN_ROOT}"
install -m 0644 "$(readlink -f "${OPENSSL_ROOT_DIR}/lib/libssl.so.3")" "${APP_DIR}/usr/lib/libssl.so.3"
install -m 0644 "$(readlink -f "${OPENSSL_ROOT_DIR}/lib/libcrypto.so.3")" "${APP_DIR}/usr/lib/libcrypto.so.3"
install -m 0755 "${OPENSSL_ROOT_DIR}/lib/ossl-modules/legacy.so" "${APP_DIR}/usr/lib/ossl-modules/legacy.so"
if [[ ! -e "${APP_DIR}/nunchuk-qt.png" ]]; then
    echo "Root AppImage icon was not deployed: ${APP_DIR}/nunchuk-qt.png" >&2
    exit 1
fi
ln -sfn -- nunchuk-qt.png "${APP_DIR}/.DirIcon"
normalize_appdir_permissions "${APP_DIR}"
verify_custom_apprun "${APP_DIR}"
verify_desktop_metadata "${APP_DIR}/nunchuk.desktop"
verify_desktop_metadata "${APP_DIR}/usr/share/applications/nunchuk.desktop"

app_runpath="$(patchelf --print-rpath "${APP_DIR}/usr/bin/nunchuk-qt")"
if [[ "${app_runpath}" != *'$ORIGIN/../lib'* ]]; then
    echo "Packaged application has an unexpected RUNPATH: ${app_runpath}" >&2
    exit 1
fi

# Prove that the deployed Qt TLS plugin loads the pinned OpenSSL runtime rather
# than merely checking that files with the expected names exist.
TLS_PROBE_SOURCE="${PACKAGE_DIR}/qt-tls-probe.cpp"
TLS_PROBE_BINARY="${PACKAGE_DIR}/qt-tls-probe"
cat > "${TLS_PROBE_SOURCE}" <<'EOF'
#include <QCoreApplication>
#include <QSslSocket>
#include <QString>

#include <iostream>

int main(int argc, char **argv) {
    QCoreApplication app(argc, argv);
    const bool supported = QSslSocket::supportsSsl();
    const QString runtime = QSslSocket::sslLibraryVersionString();
    std::cout << runtime.toStdString() << '\n';
    return supported ? 0 : 1;
}
EOF

"${CXX:-g++}" \
    -std=c++17 \
    -fPIC \
    -pthread \
    -I"${QT_INSTALLED_PREFIX}/include" \
    -I"${QT_INSTALLED_PREFIX}/include/QtCore" \
    -I"${QT_INSTALLED_PREFIX}/include/QtNetwork" \
    "${TLS_PROBE_SOURCE}" \
    -L"${QT_INSTALLED_PREFIX}/lib" \
    -lQt6Network \
    -lQt6Core \
    -o "${TLS_PROBE_BINARY}"

TLS_PROBE_OUTPUT="$(
    LD_LIBRARY_PATH="${APP_DIR}/usr/lib" \
    QT_PLUGIN_PATH="${PLUGIN_ROOT}" \
    OPENSSL_MODULES="${APP_DIR}/usr/lib/ossl-modules" \
    "${TLS_PROBE_BINARY}"
)"
if [[ "${TLS_PROBE_OUTPUT}" != *"OpenSSL ${OPENSSL_VERSION}"* ]]; then
    echo "Unexpected packaged TLS runtime: ${TLS_PROBE_OUTPUT}" >&2
    exit 1
fi
rm -f -- "${TLS_PROBE_SOURCE}" "${TLS_PROBE_BINARY}"

library_path="$(
    find "${APP_DIR}" -type f -name '*.so*' -exec dirname {} \; \
        | LC_ALL=C sort -u \
        | paste -sd: -
)"
while IFS= read -r -d '' candidate; do
    if [[ "$(file -b "${candidate}")" != *ELF* ]]; then
        continue
    fi
    check_ldd "${candidate}" "${library_path}"
done < <(find "${APP_DIR}" -type f -print0)

APPDIR_VERIFICATION_REPORT="${PACKAGE_DIR}/appdir-verification.txt"
"${PROJECT_DIR}/reproducible-builds/appdir_tool.sh" \
    verify \
    "${APP_DIR}" \
    "${APPDIR_VERIFICATION_REPORT}"
grep -E '^max_glibc(xx)?=' "${APPDIR_VERIFICATION_REPORT}"
rm -f -- "${APPDIR_VERIFICATION_REPORT}"

# Normalize all input metadata before squashfs creation. The separately pinned
# runtime prevents appimagetool from fetching anything from a moving channel.
find "${APP_DIR}" -print0 \
    | LC_ALL=C sort -z \
    | xargs -0r touch --no-dereference --date="@${SOURCE_DATE_EPOCH}"

APPDIR_MANIFEST="${PACKAGE_DIR}/appdir-metadata.manifest"
APPDIR_AFTER_MANIFEST="${PACKAGE_DIR}/appdir-metadata.after-appimagetool.manifest"
write_appdir_manifest "${APP_DIR}" "${APPDIR_MANIFEST}"

env -u VERSION \
    ARCH=x86_64 \
    appimagetool \
    --appimage-extract-and-run \
    --runtime-file "${APPIMAGE_RUNTIME_FILE}" \
    "${APP_DIR}" \
    "${APPIMAGE_PATH}"
write_appdir_manifest "${APP_DIR}" "${APPDIR_AFTER_MANIFEST}"
if ! cmp -s "${APPDIR_MANIFEST}" "${APPDIR_AFTER_MANIFEST}"; then
    diff -u "${APPDIR_MANIFEST}" "${APPDIR_AFTER_MANIFEST}" >&2 || true
    echo "appimagetool modified normalized AppDir metadata" >&2
    exit 1
fi
rm -f -- "${APPDIR_AFTER_MANIFEST}"
chmod 0755 "${APPIMAGE_PATH}"
touch --no-dereference --date="@${SOURCE_DATE_EPOCH}" "${APPIMAGE_PATH}"

(
    cd "${PACKAGE_DIR}"
    # The AppImage payload is already compressed. Store mode avoids coupling
    # release bytes to a particular zlib implementation.
    zip -X -0 "${ZIP_NAME}" "${APPIMAGE_NAME}"
    sha256sum "${ZIP_NAME}" > "${ZIP_NAME}.sha256"
)
