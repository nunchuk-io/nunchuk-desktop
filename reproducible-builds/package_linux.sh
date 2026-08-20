#!/usr/bin/env bash
set -euo pipefail

TAG="${TAG:-0.0.0}"
case "$TAG" in
    ''|*[!A-Za-z0-9._+-]*)
        echo "Invalid package version: $TAG" >&2
        exit 1
        ;;
esac
case "${SOURCE_DATE_EPOCH:-}" in
    ''|*[!0-9]*)
        echo "Invalid SOURCE_DATE_EPOCH: ${SOURCE_DATE_EPOCH:-unset}" >&2
        exit 1
        ;;
esac

PROJECT_ROOT="$(CDPATH='' cd -- "$(dirname -- "$0")/.." && pwd -P)"
RELEASE_ROOT="$PROJECT_ROOT/nunchuk-linux-v$TAG"
APPDIR="$RELEASE_ROOT/Appdir"
NUNCHUK_BINARY="$PROJECT_ROOT/build/nunchuk-qt"
NUNCHUK_ICON="$PROJECT_ROOT/deploy/nunchuk-qt.png"
APPDIR_TOOL="$PROJECT_ROOT/reproducible-builds/appdir_tool.sh"
OPENSSL_LIB_DIR="$OPENSSL_ROOT_DIR/lib"
APPIMAGE="$RELEASE_ROOT/nunchuk-linux-v$TAG.AppImage"
ARCHIVE="$RELEASE_ROOT/nunchuk-linux-v$TAG.zip"
APPIMAGETOOL="${APPIMAGETOOL:-/usr/local/bin/appimagetool}"
APPIMAGE_RUNTIME="${APPIMAGE_RUNTIME:-/usr/local/lib/appimage/runtime-x86_64}"
TEMP_ROOT="$(mktemp -d)"
trap 'rm -rf -- "$TEMP_ROOT"' EXIT

for command_name in \
    cqtdeployer curl file find ldd readelf patchelf sha256sum touch 7z; do
    command -v "$command_name" >/dev/null || {
        echo "Missing packaging tool: $command_name" >&2
        exit 1
    }
done

[[ -x "$NUNCHUK_BINARY" ]] || {
    echo "Nunchuk binary was not found: $NUNCHUK_BINARY" >&2
    exit 1
}
file -Lb "$NUNCHUK_BINARY" | grep -q 'ELF 64-bit.*x86-64' || {
    echo "Nunchuk binary is not a Linux x86_64 ELF file" >&2
    exit 1
}
[[ -x "$QT_INSTALLED_PREFIX/bin/qmake" ]] || {
    echo "Qt qmake was not found under $QT_INSTALLED_PREFIX" >&2
    exit 1
}
[[ -f "$NUNCHUK_ICON" ]] || {
    echo "Nunchuk icon was not found: $NUNCHUK_ICON" >&2
    exit 1
}
[[ -x "$APPDIR_TOOL" ]] || {
    echo "AppDir helper is not executable: $APPDIR_TOOL" >&2
    exit 1
}
[[ -x "$APPIMAGETOOL" ]] || {
    echo "Pinned appimagetool was not found: $APPIMAGETOOL" >&2
    exit 1
}
[[ -x "$APPIMAGE_RUNTIME" ]] || {
    echo "Pinned AppImage runtime was not found: $APPIMAGE_RUNTIME" >&2
    exit 1
}

rm -rf -- "$RELEASE_ROOT"
mkdir -p "$APPDIR"

cqtdeployer \
    -bin "$NUNCHUK_BINARY" \
    -qmake "$QT_INSTALLED_PREFIX/bin/qmake" \
    -qmlDir "$PROJECT_ROOT" \
    -targetDir "$APPDIR" \
    -icon "$NUNCHUK_ICON" \
    noTranslation \
    noStrip

[[ -f "$APPDIR/plugins/platforms/libqxcb.so" ]] || {
    echo "CQtDeployer did not create the Qt XCB platform plugin" >&2
    exit 1
}

# CQtDeployer includes every backend of a discovered Qt module. Retain the
# runtime backends used by Nunchuk so the dependency collector does not pull in
# unused Wayland, SQL, GTK, or device stacks.
while IFS= read -r -d '' qt_plugin; do
    relative_plugin="${qt_plugin#"$APPDIR/plugins/"}"
    case "$relative_plugin" in
        platforms/libqxcb.so|\
        xcbglintegrations/libqxcb-egl-integration.so|\
        xcbglintegrations/libqxcb-glx-integration.so|\
        imageformats/libqjpeg.so|\
        imageformats/libqsvg.so|\
        iconengines/libqsvgicon.so|\
        mediaservice/libgstcamerabin.so|\
        mediaservice/libgstmediaplayer.so|\
        mediaservice/libgstaudiodecoder.so|\
        audio/libqtaudio_alsa.so|\
        audio/libqtmedia_pulse.so|\
        playlistformats/libqtmultimedia_m3u.so|\
        sqldrivers/libqsqlite.so|\
        bearer/libqgenericbearer.so|\
        platforminputcontexts/libcomposeplatforminputcontextplugin.so|\
        platforminputcontexts/libibusplatforminputcontextplugin.so|\
        platforminputcontexts/libqtvirtualkeyboardplugin.so)
            ;;
        *)
            echo "Removing unused Qt plugin: $relative_plugin"
            rm -f -- "$qt_plugin"
            ;;
    esac
done < <(find "$APPDIR/plugins" \( -type f -o -type l \) -name '*.so*' -print0)

for required_qt_plugin in \
    platforms/libqxcb.so \
    imageformats/libqjpeg.so \
    imageformats/libqsvg.so \
    iconengines/libqsvgicon.so \
    mediaservice/libgstcamerabin.so \
    sqldrivers/libqsqlite.so; do
    [[ -f "$APPDIR/plugins/$required_qt_plugin" ]] || {
        echo "Required Qt plugin is missing: $required_qt_plugin" >&2
        exit 1
    }
done

rm -rf -- "$APPDIR/qml/QtWayland"
find "$APPDIR/lib" "$APPDIR/bin" -maxdepth 1 \
    \( -type f -o -type l \) -name 'libQt5Wayland*.so*' -print -delete

(
    cd "$APPDIR/plugins"
    find . \( -type f -o -type l \) -name '*.so*' \
        -printf '%P\n' | LC_ALL=C sort
) > "$APPDIR/qt-plugin-manifest.txt"

mkdir -p \
    "$APPDIR/bin" \
    "$APPDIR/lib" \
    "$APPDIR/libexec" \
    "$APPDIR/resources" \
    "$APPDIR/translations/qtwebengine_locales"

for ssl_library in libssl.so.1.1 libcrypto.so.1.1; do
    [[ -f "$OPENSSL_LIB_DIR/$ssl_library" ]] || {
        echo "OpenSSL runtime is missing: $OPENSSL_LIB_DIR/$ssl_library" >&2
        exit 1
    }
    install -m 0755 "$OPENSSL_LIB_DIR/$ssl_library" "$APPDIR/lib/$ssl_library"
done
ln -sfn libssl.so.1.1 "$APPDIR/lib/libssl.so"
ln -sfn libcrypto.so.1.1 "$APPDIR/lib/libcrypto.so"

[[ -x "$QT_INSTALLED_PREFIX/libexec/QtWebEngineProcess" ]] || {
    echo "QtWebEngineProcess is missing from Qt" >&2
    exit 1
}
[[ -d "$QT_INSTALLED_PREFIX/resources" ]] || {
    echo "Qt WebEngine resources are missing from Qt" >&2
    exit 1
}
[[ -d "$QT_INSTALLED_PREFIX/translations/qtwebengine_locales" ]] || {
    echo "Qt WebEngine locales are missing from Qt" >&2
    exit 1
}
install -m 0755 \
    "$QT_INSTALLED_PREFIX/libexec/QtWebEngineProcess" \
    "$APPDIR/libexec/QtWebEngineProcess"
cp -a "$QT_INSTALLED_PREFIX/resources/." "$APPDIR/resources/"
cp -a \
    "$QT_INSTALLED_PREFIX/translations/qtwebengine_locales/." \
    "$APPDIR/translations/qtwebengine_locales/"

[[ -s /etc/ssl/certs/ca-certificates.crt ]] || {
    echo "System CA certificate bundle was not found" >&2
    exit 1
}
install -m 0644 \
    /etc/ssl/certs/ca-certificates.crt \
    "$APPDIR/resources/ca-certificates.crt"

# Qt WebEngine's NSS backend loads these files dynamically, so ldd alone cannot
# discover them.
while IFS= read -r nss_file; do
    [[ -f "$nss_file" ]] || continue
    case "$(basename -- "$nss_file")" in
        libsoftokn3.so|libsoftokn3.chk|libfreebl*.so|libfreebl*.chk|\
        libnssckbi.so|libnssdbm3.so|libnssdbm3.chk|libnsssysinit.so)
            nss_destination="$APPDIR/lib/$(basename -- "$nss_file")"
            if [[ -e "$nss_destination" || -L "$nss_destination" ]]; then
                cmp -s -- "$nss_file" "$nss_destination" || {
                    echo "NSS runtime collision: $nss_destination" >&2
                    exit 1
                }
            else
                cp -L --preserve=mode,timestamps -- "$nss_file" "$nss_destination"
                chmod a+rX "$nss_destination"
            fi
            ;;
    esac
done < <(dpkg-query -L libnss3 2>/dev/null || true)

HWI_ARCHIVE="$TEMP_ROOT/hwi-3.2.0-linux-x86_64.tar.gz"
HWI_EXTRACTED="$TEMP_ROOT/hwi-extracted"
HWI_SHA256="ee7cf2afe085128c55ebaa1fb1da45831ae82a02f5b8c462c1dc95e6e606220a"
curl --fail --location --silent --show-error \
    --retry 5 --retry-delay 2 --retry-all-errors \
    "https://github.com/nogibi/HWI/releases/download/3.2.0-displayaddress/hwi-3.2.0-linux-x86_64.tar.gz" \
    --output "$HWI_ARCHIVE"
printf '%s  %s\n' "$HWI_SHA256" "$HWI_ARCHIVE" | sha256sum --check --strict
mkdir -p "$HWI_EXTRACTED"
tar -xzf "$HWI_ARCHIVE" -C "$HWI_EXTRACTED"
install -m 0755 "$HWI_EXTRACTED/hwi" "$APPDIR/bin/hwi"

cat > "$APPDIR/bin/qt.conf" <<'QT_CONF'
[Paths]
Prefix=..
Libraries=lib
Plugins=plugins
Qml2Imports=qml
Translations=translations
QT_CONF
cp -a "$APPDIR/bin/qt.conf" "$APPDIR/libexec/qt.conf"

cat > "$APPDIR/nunchuk.desktop" <<'DESKTOP_FILE'
[Desktop Entry]
Type=Application
Name=Nunchuk
Exec=AppRun
Icon=nunchuk-qt
Categories=Utility;
DESKTOP_FILE
install -m 0644 "$NUNCHUK_ICON" "$APPDIR/nunchuk-qt.png"
ln -sfn nunchuk-qt.png "$APPDIR/.DirIcon"

cat > "$APPDIR/AppRun" <<'APP_RUN'
#!/bin/sh
set -eu

HERE=$(CDPATH='' cd -- "$(dirname -- "$0")" && pwd -P)
export APPDIR="$HERE"
export PATH="$HERE/bin${PATH:+:$PATH}"
export LD_LIBRARY_PATH="$HERE/lib:$HERE/bin${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"
export QT_PLUGIN_PATH="$HERE/plugins:$HERE/bin/plugins:$HERE/bin${QT_PLUGIN_PATH:+:$QT_PLUGIN_PATH}"
export QML_IMPORT_PATH="$HERE/qml:$HERE/bin/qml${QML_IMPORT_PATH:+:$QML_IMPORT_PATH}"
export QML2_IMPORT_PATH="$HERE/qml:$HERE/bin/qml${QML2_IMPORT_PATH:+:$QML2_IMPORT_PATH}"
export QT_QPA_PLATFORM_PLUGIN_PATH="$HERE/plugins/platforms"
export QTWEBENGINEPROCESS_PATH="$HERE/libexec/QtWebEngineProcess"
export QTWEBENGINE_RESOURCES_PATH="$HERE/resources"
export QTWEBENGINE_LOCALES_PATH="$HERE/translations/qtwebengine_locales"
export SSL_CERT_FILE="$HERE/resources/ca-certificates.crt"
export QTWEBENGINE_DISABLE_SANDBOX=1

exec "$HERE/bin/nunchuk-qt" "$@"
APP_RUN
chmod 0755 "$APPDIR/AppRun"

# patchelf needs the literal $ORIGIN string.
patchelf --set-rpath '$ORIGIN/../lib' "$APPDIR/bin/nunchuk-qt"

"$APPDIR_TOOL" collect "$APPDIR" "$QT_INSTALLED_PREFIX" "$OPENSSL_LIB_DIR"
# Keep build-path-dependent diagnostics out of the release artifacts.
"$APPDIR_TOOL" verify "$APPDIR" /dev/null
sh -n "$APPDIR/AppRun"
if command -v desktop-file-validate >/dev/null; then
    desktop-file-validate "$APPDIR/nunchuk.desktop"
fi

APPIMAGE_EXTRACT_AND_RUN=1 \
ARCH=x86_64 \
VERSION="$TAG" \
    "$APPIMAGETOOL" \
        --no-appstream \
        --runtime-file "$APPIMAGE_RUNTIME" \
        "$APPDIR" \
        "$APPIMAGE"
chmod 0755 "$APPIMAGE"
# 7z stores this mtime in the ZIP entry but does not read SOURCE_DATE_EPOCH.
touch --date="@$SOURCE_DATE_EPOCH" -- "$APPIMAGE"

VERIFY_ROOT="$TEMP_ROOT/appimage-verify"
mkdir -p "$VERIFY_ROOT"
(
    cd "$VERIFY_ROOT"
    "$APPIMAGE" --appimage-extract >/dev/null
)
EXTRACTED_APPDIR="$VERIFY_ROOT/squashfs-root"
[[ -d "$EXTRACTED_APPDIR" ]] || {
    echo "The generated AppImage could not be extracted" >&2
    exit 1
}
"$APPDIR_TOOL" verify "$EXTRACTED_APPDIR" /dev/null

(
    cd "$RELEASE_ROOT"
    TZ=UTC 7z a -tzip -mx=9 \
        "$(basename -- "$ARCHIVE")" \
        "$(basename -- "$APPIMAGE")"
)
7z t "$ARCHIVE"

echo "Linux package created: $ARCHIVE"
