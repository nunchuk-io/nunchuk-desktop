#!/usr/bin/env bash
set -euo pipefail

PROJECT_ROOT=${PROJECT_ROOT:-/project}
TAG=${TAG:?TAG is required}
SOURCE_COMMIT=${SOURCE_COMMIT:-$(git -C "$PROJECT_ROOT" rev-parse HEAD)}
SOURCE_DATE_EPOCH=${SOURCE_DATE_EPOCH:-$(git -C "$PROJECT_ROOT" log -1 --pretty=%ct)}
RELEASE_ROOT="$PROJECT_ROOT/nunchuk-linux-v$TAG"
APPDIR="$RELEASE_ROOT/Appdir"
DIST_DIR="$PROJECT_ROOT/dist"
APPIMAGE="$DIST_DIR/nunchuk-linux-v$TAG.AppImage"
APPIMAGE_RUNTIME=/usr/local/lib/appimage-runtime-x86_64
HWI_URL=https://github.com/nogibi/HWI/releases/download/3.2.0-displayaddress/hwi-3.2.0-linux-x86_64.tar.gz
HWI_SHA256=ee7cf2afe085128c55ebaa1fb1da45831ae82a02f5b8c462c1dc95e6e606220a

case "$TAG" in
    *[!A-Za-z0-9._+-]*)
        echo "ERROR: invalid build tag: $TAG" >&2
        exit 1
        ;;
esac

for command_name in appimagetool cqtdeployer curl file ldd patchelf readelf sha256sum; do
    command -v "$command_name" >/dev/null || {
        echo "ERROR: missing packaging command: $command_name" >&2
        exit 1
    }
done

is_elf()
{
    file -Lb -- "$1" 2>/dev/null | grep -q '^ELF '
}

is_host_runtime()
{
    case "$1" in
        linux-vdso.so.*|ld-linux*.so.*|ld-*.so|\
        libc.so.*|libc-*.so|libdl.so.*|libdl-*.so|\
        libm.so.*|libm-*.so|libmvec.so.*|libmvec-*.so|\
        libpthread.so.*|libpthread-*.so|librt.so.*|librt-*.so|\
        libresolv.so.*|libresolv-*.so|libutil.so.*|libutil-*.so|\
        libanl.so.*|libBrokenLocale.so.*|libthread_db.so.*|\
        libcidn.so.*|libnss_*.so.*|libGL.so.*|libGLX.so.*|\
        libGLdispatch.so.*|libEGL.so.*|libOpenGL.so.*|libdrm.so.*|\
        libdrm_*.so.*|libglapi.so.*|libgbm.so.*|libvulkan.so.*|\
        libcuda.so.*|libnvidia-*.so.*|libxcb.so.*|libX11.so.*|\
        libX11-xcb.so.*|libwayland-client.so.*|libasound.so.*|\
        libfontconfig.so.*|libfreetype.so.*|libharfbuzz.so.*|\
        libcom_err.so.*|libexpat.so.*|libgpg-error.so.*|libICE.so.*|\
        libSM.so.*|libusb-1.0.so.*|libuuid.so.*|libz.so.*|\
        libjack.so.*|libpipewire-0.3.so.*|libxcb-dri2.so.*|\
        libxcb-dri3.so.*|libfribidi.so.*|libgmp.so.*)
            return 0
            ;;
        *)
            return 1
            ;;
    esac
}

collect_dependencies()
{
    local search_path="$APPDIR/lib:$APPDIR/bin:$QT_INSTALLED_PREFIX/lib:/usr/local/lib:/usr/local/lib/x86_64-linux-gnu:$OPENSSL_ROOT_DIR/lib"
    local index=0
    local elf output soname arrow resolved destination entry
    local -a queue
    declare -A seen=()

    while IFS= read -r -d '' entry; do
        if is_host_runtime "$(basename -- "$entry")"; then
            rm -f -- "$entry"
        fi
    done < <(find "$APPDIR" \( -type f -o -type l \) -print0)

    mapfile -d '' -t queue < <(find "$APPDIR" -type f -print0)
    while (( index < ${#queue[@]} )); do
        elf=${queue[$index]}
        index=$((index + 1))
        [[ -z "${seen["$elf"]+present}" ]] || continue
        seen["$elf"]=1
        is_elf "$elf" || continue

        if ! output=$(env LD_LIBRARY_PATH="$search_path" ldd "$elf" 2>&1); then
            case "$output" in
                *'statically linked'*|*'not a dynamic executable'*)
                    continue
                    ;;
                *)
                    printf '%s\n' "$output" >&2
                    echo "ERROR: ldd failed while scanning $elf" >&2
                    exit 1
                    ;;
            esac
        fi

        while read -r soname arrow resolved _; do
            [[ "$arrow" == '=>' ]] || continue
            if [[ "$resolved" == 'not' ]]; then
                is_host_runtime "$soname" && continue
                echo "ERROR: unresolved dependency $soname required by $elf" >&2
                exit 1
            fi
            is_host_runtime "$soname" && continue
            [[ -f "$resolved" ]] || {
                echo "ERROR: dependency path does not exist: $resolved" >&2
                exit 1
            }
            case "$resolved" in
                "$APPDIR"/*)
                    continue
                    ;;
            esac
            [[ "$soname" == "$(basename -- "$soname")" ]] || {
                echo "ERROR: unsafe dependency name: $soname" >&2
                exit 1
            }

            destination="$APPDIR/lib/$soname"
            if [[ -e "$destination" || -L "$destination" ]]; then
                cmp -s -- "$resolved" "$destination" || {
                    echo "ERROR: different libraries share SONAME $soname" >&2
                    exit 1
                }
                continue
            fi
            cp -L --preserve=mode,timestamps -- "$resolved" "$destination"
            chmod a+rX "$destination"
            queue+=("$destination")
        done <<< "$output"
    done
}

verify_dependency_closure()
{
    local root=$1
    local canonical_root search_path elf output soname arrow resolved canonical_resolved
    canonical_root=$(readlink -f -- "$root")
    search_path="$root/lib:$root/bin"

    while IFS= read -r -d '' elf; do
        is_elf "$elf" || continue
        if ! output=$(env LD_LIBRARY_PATH="$search_path" ldd "$elf" 2>&1); then
            case "$output" in
                *'statically linked'*|*'not a dynamic executable'*)
                    continue
                    ;;
                *)
                    printf '%s\n' "$output" >&2
                    echo "ERROR: ldd failed while verifying $elf" >&2
                    exit 1
                    ;;
            esac
        fi
        while read -r soname arrow resolved _; do
            [[ "$arrow" == '=>' ]] || continue
            if [[ "$resolved" == 'not' ]]; then
                is_host_runtime "$soname" && continue
                echo "ERROR: extracted AppImage is missing $soname" >&2
                exit 1
            fi
            is_host_runtime "$soname" && continue
            canonical_resolved=$(readlink -f -- "$resolved")
            case "$canonical_resolved" in
                "$canonical_root"/*)
                    ;;
                *)
                    echo "ERROR: $elf resolves $soname outside AppDir: $resolved" >&2
                    exit 1
                    ;;
            esac
        done <<< "$output"
    done < <(find "$root" -type f -print0)
}

rm -rf -- "$RELEASE_ROOT" "$DIST_DIR"
mkdir -p "$APPDIR" "$DIST_DIR"

NUNCHUK_BINARY="$PROJECT_ROOT/build/nunchuk-qt"
test -x "$NUNCHUK_BINARY" || {
    echo "ERROR: missing built executable: $NUNCHUK_BINARY" >&2
    exit 1
}
file -Lb "$NUNCHUK_BINARY" | grep -q 'ELF 64-bit.*x86-64' || {
    echo "ERROR: nunchuk-qt is not a Linux x86_64 ELF executable" >&2
    exit 1
}
test -x "$QT_INSTALLED_PREFIX/bin/qmake" || {
    echo "ERROR: qmake was not found in $QT_INSTALLED_PREFIX" >&2
    exit 1
}

cqtdeployer \
    -bin "$NUNCHUK_BINARY" \
    -qmake "$QT_INSTALLED_PREFIX/bin/qmake" \
    -qmlDir "$PROJECT_ROOT" \
    -targetDir "$APPDIR" \
    -icon "$PROJECT_ROOT/deploy/nunchuk-qt.png" \
    noTranslation \
    noStrip

test -x "$APPDIR/bin/nunchuk-qt" || {
    echo "ERROR: CQtDeployer did not create Appdir/bin/nunchuk-qt" >&2
    exit 1
}
test -f "$APPDIR/plugins/platforms/libqxcb.so" || {
    echo "ERROR: CQtDeployer did not include the XCB platform plugin" >&2
    exit 1
}

mkdir -p "$APPDIR/lib" "$APPDIR/libexec" "$APPDIR/resources" \
         "$APPDIR/translations/qtwebengine_locales"

for ssl_library in libssl.so.1.1 libcrypto.so.1.1; do
    test -f "$OPENSSL_ROOT_DIR/lib/$ssl_library" || {
        echo "ERROR: missing OpenSSL runtime: $ssl_library" >&2
        exit 1
    }
    install -m 0755 "$OPENSSL_ROOT_DIR/lib/$ssl_library" "$APPDIR/lib/$ssl_library"
done
ln -sfn libssl.so.1.1 "$APPDIR/lib/libssl.so"
ln -sfn libcrypto.so.1.1 "$APPDIR/lib/libcrypto.so"

test -x "$QT_INSTALLED_PREFIX/libexec/QtWebEngineProcess" || {
    echo "ERROR: QtWebEngineProcess was not installed" >&2
    exit 1
}
test -d "$QT_INSTALLED_PREFIX/resources" || {
    echo "ERROR: Qt WebEngine resources were not installed" >&2
    exit 1
}
test -d "$QT_INSTALLED_PREFIX/translations/qtwebengine_locales" || {
    echo "ERROR: Qt WebEngine locales were not installed" >&2
    exit 1
}
install -m 0755 "$QT_INSTALLED_PREFIX/libexec/QtWebEngineProcess" \
    "$APPDIR/libexec/QtWebEngineProcess"
cp -a "$QT_INSTALLED_PREFIX/resources/." "$APPDIR/resources/"
cp -a "$QT_INSTALLED_PREFIX/translations/qtwebengine_locales/." \
    "$APPDIR/translations/qtwebengine_locales/"
install -m 0644 /etc/ssl/certs/ca-certificates.crt \
    "$APPDIR/resources/ca-certificates.crt"

# Chromium loads these NSS modules dynamically, so ldd cannot discover them.
while IFS= read -r nss_file; do
    [[ -f "$nss_file" ]] || continue
    case "$(basename -- "$nss_file")" in
        libsoftokn3.so|libsoftokn3.chk|libfreebl*.so|libfreebl*.chk|\
        libnssckbi.so|libnssdbm3.so|libnssdbm3.chk|libnsssysinit.so)
            destination="$APPDIR/lib/$(basename -- "$nss_file")"
            if [[ -e "$destination" || -L "$destination" ]]; then
                cmp -s -- "$nss_file" "$destination" || {
                    echo "ERROR: NSS runtime collision: $destination" >&2
                    exit 1
                }
            else
                cp -L --preserve=mode,timestamps -- "$nss_file" "$destination"
                chmod a+rX "$destination"
            fi
            ;;
    esac
done < <(dpkg-query -L libnss3)

HWI_ARCHIVE=/tmp/nunchuk-hwi-3.2.0-linux-x86_64.tar.gz
HWI_EXTRACT=/tmp/nunchuk-hwi-3.2.0-linux-x86_64
rm -rf -- "$HWI_ARCHIVE" "$HWI_EXTRACT"
curl --fail --location --retry 5 --retry-all-errors \
    --output "$HWI_ARCHIVE" "$HWI_URL"
printf '%s  %s\n' "$HWI_SHA256" "$HWI_ARCHIVE" | sha256sum --check --strict
mkdir -p "$HWI_EXTRACT"
tar -xzf "$HWI_ARCHIVE" -C "$HWI_EXTRACT"
test -x "$HWI_EXTRACT/hwi" || {
    echo "ERROR: HWI archive did not contain executable 'hwi'" >&2
    exit 1
}
install -m 0755 "$HWI_EXTRACT/hwi" "$APPDIR/bin/hwi"
rm -rf -- "$HWI_ARCHIVE" "$HWI_EXTRACT"

cat > "$APPDIR/bin/qt.conf" <<'EOF_QT_CONF'
[Paths]
Prefix=..
Libraries=lib
Plugins=plugins
Qml2Imports=qml
Translations=translations
EOF_QT_CONF
cp -a "$APPDIR/bin/qt.conf" "$APPDIR/libexec/qt.conf"

cat > "$APPDIR/nunchuk.desktop" <<'EOF_DESKTOP'
[Desktop Entry]
Type=Application
Name=Nunchuk
Exec=AppRun
Icon=nunchuk-qt
Categories=Utility;
EOF_DESKTOP
install -m 0644 "$PROJECT_ROOT/deploy/nunchuk-qt.png" "$APPDIR/nunchuk-qt.png"
ln -sfn nunchuk-qt.png "$APPDIR/.DirIcon"

cat > "$APPDIR/AppRun" <<'EOF_APPRUN'
#!/bin/sh
set -eu

HERE=$(CDPATH='' cd -- "$(dirname -- "$0")" && pwd -P)
export APPDIR="$HERE"
export PATH="$HERE/bin${PATH:+:$PATH}"
export LD_LIBRARY_PATH="$HERE/lib:$HERE/bin${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"
export QT_PLUGIN_PATH="$HERE/plugins:$HERE/bin/plugins${QT_PLUGIN_PATH:+:$QT_PLUGIN_PATH}"
export QML_IMPORT_PATH="$HERE/qml:$HERE/bin/qml${QML_IMPORT_PATH:+:$QML_IMPORT_PATH}"
export QML2_IMPORT_PATH="$HERE/qml:$HERE/bin/qml${QML2_IMPORT_PATH:+:$QML2_IMPORT_PATH}"
export QT_QPA_PLATFORM_PLUGIN_PATH="$HERE/plugins/platforms"
export QTWEBENGINEPROCESS_PATH="$HERE/libexec/QtWebEngineProcess"
export QTWEBENGINE_RESOURCES_PATH="$HERE/resources"
export QTWEBENGINE_LOCALES_PATH="$HERE/translations/qtwebengine_locales"
export SSL_CERT_FILE="$HERE/resources/ca-certificates.crt"
export QTWEBENGINE_DISABLE_SANDBOX=1

exec "$HERE/bin/nunchuk-qt" "$@"
EOF_APPRUN
chmod 0755 "$APPDIR/AppRun"
patchelf --set-rpath '$ORIGIN/../lib' "$APPDIR/bin/nunchuk-qt"

collect_dependencies

for required_file in \
    "$APPDIR/AppRun" \
    "$APPDIR/bin/nunchuk-qt" \
    "$APPDIR/bin/hwi" \
    "$APPDIR/libexec/QtWebEngineProcess" \
    "$APPDIR/resources/icudtl.dat" \
    "$APPDIR/resources/qtwebengine_resources.pak" \
    "$APPDIR/resources/ca-certificates.crt"; do
    test -s "$required_file" || {
        echo "ERROR: AppDir is missing required file: $required_file" >&2
        exit 1
    }
done
for nss_pattern in libsoftokn3.so libsoftokn3.chk 'libfreebl*.so' 'libfreebl*.chk' libnssckbi.so; do
    find "$APPDIR/lib" -maxdepth 1 -type f -name "$nss_pattern" -print -quit \
        | grep -q . || {
            echo "ERROR: AppDir is missing NSS runtime: $nss_pattern" >&2
            exit 1
        }
done

sh -n "$APPDIR/AppRun"
desktop-file-validate "$APPDIR/nunchuk.desktop"
verify_dependency_closure "$APPDIR"
env LD_LIBRARY_PATH="$APPDIR/lib:$APPDIR/bin" "$APPDIR/bin/hwi" --version

# SquashFS and AppImage consume file mtimes. Normalize the full AppDir to the
# source commit time before hashing and packaging.
find "$APPDIR" -exec touch -h --date="@$SOURCE_DATE_EPOCH" {} +

(
    cd "$APPDIR"
    find . -type f -print0 | LC_ALL=C sort -z | xargs -0 sha256sum
) > "$DIST_DIR/AppDir.sha256"
(
    cd "$APPDIR"
    find . -type l -printf '%P -> %l\n' | LC_ALL=C sort
) > "$DIST_DIR/AppDir.symlinks"

cat > "$DIST_DIR/build-info.txt" <<EOF_BUILD_INFO
version=$TAG
source_commit=$SOURCE_COMMIT
source_date_epoch=$SOURCE_DATE_EPOCH
target=linux-x86_64
ubuntu_snapshot=${UBUNTU_SNAPSHOT:-20260818T000000Z}
qt=5.15.2
gcc=$(gcc-14 -dumpfullversion)
openssl=1.1.1w
hwi=3.2.0-displayaddress
oauth_compile_time_inputs=empty
oauth_runtime_fallback=OAUTH_CLIENT_ID,OAUTH_CLIENT_SECRET,OAUTH_REDIRECT_URI
EOF_BUILD_INFO

APPIMAGE_EXTRACT_AND_RUN=1 \
ARCH=x86_64 \
VERSION="$TAG" \
SOURCE_DATE_EPOCH="$SOURCE_DATE_EPOCH" \
    appimagetool \
        --no-appstream \
        --runtime-file "$APPIMAGE_RUNTIME" \
        "$APPDIR" \
        "$APPIMAGE"
chmod 0755 "$APPIMAGE"

VERIFY_ROOT=/tmp/nunchuk-appimage-verify
rm -rf -- "$VERIFY_ROOT"
mkdir -p "$VERIFY_ROOT"
(
    cd "$VERIFY_ROOT"
    "$APPIMAGE" --appimage-extract >/dev/null
)
EXTRACTED_APPDIR="$VERIFY_ROOT/squashfs-root"
test -d "$EXTRACTED_APPDIR" || {
    echo "ERROR: could not extract the generated AppImage" >&2
    exit 1
}
(
    cd "$EXTRACTED_APPDIR"
    find . -type f -print0 | LC_ALL=C sort -z | xargs -0 sha256sum
) > "$VERIFY_ROOT/AppDir.sha256"
cmp "$DIST_DIR/AppDir.sha256" "$VERIFY_ROOT/AppDir.sha256"
verify_dependency_closure "$EXTRACTED_APPDIR"

if [[ "${RUN_SMOKE_TEST:-1}" == 1 ]]; then
    SMOKE_LOG="$DIST_DIR/appimage-smoke.log"
    set +e
    APPIMAGE_EXTRACT_AND_RUN=1 \
    timeout --signal=TERM --kill-after=10s 20s \
        dbus-run-session -- \
        xvfb-run -a -s '-screen 0 1280x800x24' \
        "$APPIMAGE" >"$SMOKE_LOG" 2>&1
    smoke_status=$?
    set -e
    if [[ "$smoke_status" != 124 ]]; then
        cat "$SMOKE_LOG" >&2
        echo "ERROR: AppImage smoke test failed with status $smoke_status" >&2
        exit 1
    fi
    if grep -Eiq \
        'error while loading shared libraries|symbol lookup error|could not load the qt platform plugin|no qt platform plugin could be initialized|module .* is not installed|could not find qtwebengineprocess|qt webengine resources not found|no functional tls backend' \
        "$SMOKE_LOG"; then
        cat "$SMOKE_LOG" >&2
        echo "ERROR: AppImage smoke test reported a runtime packaging error" >&2
        exit 1
    fi
fi

rm -rf -- "$VERIFY_ROOT"
(
    cd "$DIST_DIR"
    sha256sum "$(basename -- "$APPIMAGE")" > "$(basename -- "$APPIMAGE").sha256"
)
ls -lh "$DIST_DIR"
