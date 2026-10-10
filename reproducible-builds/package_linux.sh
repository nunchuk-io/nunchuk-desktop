#!/usr/bin/env bash
set -euo pipefail

prepare_package() {
    PROJECT_DIR="${PROJECT_DIR:-/project}"
    TAG="${TAG:-0.0.0}"
    ARCH="${ARCH:?ARCH must be set to x86_64 or aarch64}"

    if [[ "${PROJECT_DIR}" != /* || ! -f "${PROJECT_DIR}/CMakeLists.txt" ]]; then
        echo "PROJECT_DIR must be an absolute Nunchuk source directory: ${PROJECT_DIR}" >&2
        exit 1
    fi
    if [[ ! "${TAG}" =~ ^[0-9]+\.[0-9]+\.[0-9]+([.-][0-9A-Za-z]+)*$ ]]; then
        echo "Invalid TAG: ${TAG}" >&2
        exit 1
    fi
    if [[ "${ARCH}" != x86_64 && "${ARCH}" != aarch64 ]]; then
        echo "Unsupported ARCH: ${ARCH}" >&2
        exit 1
    fi
    if [[ ! "${SOURCE_DATE_EPOCH:-}" =~ ^[0-9]+$ ]]; then
        echo "Invalid SOURCE_DATE_EPOCH: ${SOURCE_DATE_EPOCH:-unset}" >&2
        exit 1
    fi

    export ARCH SOURCE_DATE_EPOCH
    export TZ=UTC LANG=C.UTF-8 LC_ALL=C.UTF-8
    export ZERO_AR_DATE=1 APPIMAGE_EXTRACT_AND_RUN=1

    PACKAGE_NAME="nunchuk-linux-v${TAG}-${ARCH}"
    PACKAGE_DIR="${PROJECT_DIR}/${PACKAGE_NAME}"
    APP_DIR="${PACKAGE_DIR}/Appdir"
    APPIMAGE_NAME="${PACKAGE_NAME}.AppImage"
    APPIMAGE_PATH="${PACKAGE_DIR}/${APPIMAGE_NAME}"
    DESKTOP_FILE="${PACKAGE_DIR}/nunchuk.desktop"
    CUSTOM_APPRUN="${PACKAGE_DIR}/nunchuk.AppRun"
    OPENSSL_LIB_DIR="/usr/lib/$(dpkg-architecture -qDEB_HOST_MULTIARCH)"

    cmake -E remove_directory "${PACKAGE_DIR}"
    mkdir -p "${APP_DIR}/usr/bin"
    install -m 0755 /opt/hwi/hwi "${APP_DIR}/usr/bin/hwi"
}

write_launcher_files() {
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

    cat > "${CUSTOM_APPRUN}" <<'EOF'
#!/bin/sh
# nunchuk-custom-apprun
set -eu
APPDIR=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
export QTWEBENGINE_DISABLE_SANDBOX=1
export QT_MEDIA_BACKEND=ffmpeg
export OPENSSL_MODULES="$APPDIR/usr/lib/ossl-modules"
# Prefer the running system's own, OS-maintained CA store (kept current by
# the distro's security updates, and honours any locally trusted enterprise
# root); fall back to the bundle frozen at build time only if the host has
# none of the well-known bundle paths used by major distro families
# (Debian/Ubuntu/Arch, RHEL/Fedora, openSUSE).
NUNCHUK_HOST_CA_BUNDLE=""
for candidate in \
    /etc/ssl/certs/ca-certificates.crt \
    /etc/pki/tls/certs/ca-bundle.crt \
    /etc/ssl/ca-bundle.pem \
    /etc/pki/tls/cacert.pem; do
    if [ -s "$candidate" ]; then
        NUNCHUK_HOST_CA_BUNDLE="$candidate"
        break
    fi
done
export SSL_CERT_FILE="${NUNCHUK_HOST_CA_BUNDLE:-$APPDIR/usr/resources/ca-certificates.crt}"
export PATH="$APPDIR/usr/bin:$PATH"
exec "$APPDIR/usr/bin/nunchuk-qt" "$@"
EOF
    chmod 0755 "${CUSTOM_APPRUN}"
}

prepare_qml_sources() {
    # Scan only compiled resources, excluding extracted AppImages and old builds.
    QML_SOURCES_PATHS="$(mktemp -d)"
    trap 'rm -rf -- "${QML_SOURCES_PATHS}"' EXIT
    python3 - "${PROJECT_DIR}" "${QML_SOURCES_PATHS}" <<'PY'
from pathlib import Path
import shutil
import sys
import xml.etree.ElementTree as ET

project, destination = map(Path, sys.argv[1:])
for entry in ET.parse(project / "qml.qrc").iter("file"):
    relative_path = Path(entry.text.strip())
    if relative_path.suffix not in (".qml", ".js", ".mjs") and relative_path.name != "qmldir":
        continue
    target = destination / relative_path
    target.parent.mkdir(parents=True, exist_ok=True)
    shutil.copyfile(project / relative_path, target)
PY
    export QML_SOURCES_PATHS
}

restrict_qt_plugins() {
    local plugin_root="$1"
    local plugin

    # Exclude unused backends before linuxdeploy tries to bundle their dependencies.
    while IFS= read -r -d '' plugin; do
        case "${plugin#"${plugin_root}/"}" in
            platforms/libqxcb.so|\
            xcbglintegrations/libqxcb-egl-integration.so|\
            xcbglintegrations/libqxcb-glx-integration.so|\
            imageformats/libqjpeg.so|imageformats/libqsvg.so|\
            iconengines/libqsvgicon.so|\
            multimedia/libffmpegmediaplugin.so|\
            sqldrivers/libqsqlite.so|tls/libqopensslbackend.so|\
            platforminputcontexts/libcomposeplatforminputcontextplugin.so|\
            platforminputcontexts/libibusplatforminputcontextplugin.so|\
            platformthemes/libqxdgdesktopportal.so)
                ;;
            *)
                rm -f -- "${plugin}"
                ;;
        esac
    done < <(find "${plugin_root}" \( -type f -o -type l \) -name '*.so*' -print0)
}

bundle_tls_runtime() {
    # Qt loads OpenSSL dynamically; AppRun uses the CA bundle as a host fallback.
    mkdir -p "${APP_DIR}/usr/lib/ossl-modules" "${APP_DIR}/usr/resources"
    install -m 0644 "$(readlink -f "${OPENSSL_LIB_DIR}/libssl.so.3")" "${APP_DIR}/usr/lib/libssl.so.3"
    install -m 0644 "$(readlink -f "${OPENSSL_LIB_DIR}/libcrypto.so.3")" "${APP_DIR}/usr/lib/libcrypto.so.3"
    install -m 0755 "${OPENSSL_LIB_DIR}/ossl-modules/legacy.so" "${APP_DIR}/usr/lib/ossl-modules/legacy.so"
    install -m 0644 /etc/ssl/certs/ca-certificates.crt "${APP_DIR}/usr/resources/ca-certificates.crt"
}

deploy_runtime() {
    local minizip_library
    local candidate
    local -a elf_args=()

    QMAKE="$(command -v qmake6 || command -v qmake)"
    export QMAKE
    export PATH="/usr/local/bin:${QT_INSTALLED_PREFIX}/bin:${PATH}"
    export NO_STRIP=1 EXTRA_QT_MODULES=svg
    unset EXTRA_QT_PLUGINS EXTRA_PLATFORM_PLUGINS
    export LD_LIBRARY_PATH="${QT_INSTALLED_PREFIX}/lib${LD_LIBRARY_PATH:+:${LD_LIBRARY_PATH}}"

    # ShaderTools and minizip are not discovered by the automatic dependency scan.
    minizip_library="$(ldconfig -p | awk '$1 == "libminizip.so.1" { print $NF }' | sort -u | head -n1)"
    restrict_qt_plugins "${QT_INSTALLED_PREFIX}/plugins"
    linuxdeploy \
        --appdir "${APP_DIR}" \
        --executable "${PROJECT_DIR}/build/nunchuk-qt" \
        --library "${QT_INSTALLED_PREFIX}/lib/libQt6ShaderTools.so.6" \
        --library "${minizip_library}" \
        --desktop-file "${DESKTOP_FILE}" \
        --icon-file "${PROJECT_DIR}/deploy/nunchuk-qt.png" \
        --custom-apprun "${CUSTOM_APPRUN}" \
        --plugin qt

    restrict_qt_plugins "${APP_DIR}/usr/plugins"
    bundle_tls_runtime

    # Bundle dependencies of the QML and plugin ELFs added by the Qt deploy plugin.
    export LD_LIBRARY_PATH="${APP_DIR}/usr/lib:${QT_INSTALLED_PREFIX}/lib"
    while IFS= read -r -d '' candidate; do
        if [[ "$(file -b "${candidate}")" == *ELF* ]]; then
            elf_args+=(--deploy-deps-only "${candidate}")
        fi
    done < <(find "${APP_DIR}" -type f -print0 | LC_ALL=C sort -z)

    rm -f -- "${APP_DIR}/AppRun" "${APP_DIR}/AppRun.wrapped"
    linuxdeploy \
        --appdir "${APP_DIR}" \
        "${elf_args[@]}" \
        --custom-apprun "${CUSTOM_APPRUN}"

    # Preserve the selected plugins and pinned TLS runtime after deployment.
    restrict_qt_plugins "${APP_DIR}/usr/plugins"
    bundle_tls_runtime
    ln -sfn -- nunchuk-qt.png "${APP_DIR}/.DirIcon"
}

create_appimage() {
    # Normalize permissions and timestamps before creating the SquashFS.
    find "${APP_DIR}" -type d -exec chmod 0755 -- {} +
    find "${APP_DIR}" -type f -exec chmod 0644 -- {} +
    chmod 0755 "${APP_DIR}/AppRun" \
        "${APP_DIR}/usr/bin/nunchuk-qt" "${APP_DIR}/usr/bin/hwi" \
        "${APP_DIR}/usr/libexec/QtWebEngineProcess"
    if [[ -e "${APP_DIR}/AppRun.wrapped" ]]; then
        chmod 0755 "${APP_DIR}/AppRun.wrapped"
    fi
    find "${APP_DIR}" -print0 \
        | LC_ALL=C sort -z \
        | xargs -0r touch --no-dereference --date="@${SOURCE_DATE_EPOCH}"

    (
        cd "${PACKAGE_DIR}"
        # Package directly with the pinned runtime. Version aliases would make
        # appimagetool rewrite the desktop file after timestamp normalization.
        env -u VERSION -u LDAI_VERSION -u LINUXDEPLOY_OUTPUT_VERSION \
            LDAI_RUNTIME_FILE="${APPIMAGE_RUNTIME_FILE}" \
            LDAI_OUTPUT="${APPIMAGE_PATH}" \
            LDAI_NO_APPSTREAM=1 \
            linuxdeploy-plugin-appimage --appdir "${APP_DIR}"

        chmod 0755 "${APPIMAGE_NAME}"
        touch --no-dereference --date="@${SOURCE_DATE_EPOCH}" "${APPIMAGE_NAME}"
        sha256sum "${APPIMAGE_NAME}" > "${APPIMAGE_NAME}.sha256"
    )
}

main() {
    prepare_package
    write_launcher_files
    prepare_qml_sources
    deploy_runtime
    create_appimage
}

main
