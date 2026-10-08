#!/usr/bin/env bash
set -euo pipefail

die()
{
    echo "ERROR: $*" >&2
    exit 1
}

is_elf()
{
    file -Lb -- "$1" 2>/dev/null | grep -q '^ELF '
}

is_host_runtime()
{
    # These libraries form part of the host ABI, graphics/audio stack, or
    # hardware drivers. Bundling them makes an AppImage less portable and, for
    # glibc, can mix a foreign libc with the host ELF loader.
    case "$1" in
        linux-vdso.so.*|ld-linux*.so.*|ld-*.so|\
        libc.so.*|libc-*.so|libdl.so.*|libdl-*.so|\
        libm.so.*|libm-*.so|libmvec.so.*|libmvec-*.so|\
        libpthread.so.*|libpthread-*.so|\
        librt.so.*|librt-*.so|libresolv.so.*|libresolv-*.so|\
        libutil.so.*|libutil-*.so|libanl.so.*|libanl-*.so|\
        libBrokenLocale.so.*|libBrokenLocale-*.so|\
        libthread_db.so.*|libthread_db-*.so|libcidn.so.*|libnss_*.so.*|\
        libGL.so.*|libGLX.so.*|libGLdispatch.so.*|\
        libEGL.so.*|libOpenGL.so.*|libdrm.so.*|\
        libdrm_*.so.*|libglapi.so.*|libgbm.so.*|libvulkan.so.*|\
        libcuda.so.*|libnvidia-*.so.*|\
        libxcb.so.*|libX11.so.*|libX11-xcb.so.*|libwayland-client.so.*|\
        libasound.so.*|libfontconfig.so.*|libfreetype.so.*|libharfbuzz.so.*|\
        libcom_err.so.*|libexpat.so.*|libgpg-error.so.*|\
        libICE.so.*|libSM.so.*|libusb-1.0.so.*|libuuid.so.*|libz.so.*|\
        libjack.so.*|libpipewire-0.3.so.*|libxcb-dri2.so.*|\
        libxcb-dri3.so.*|libfribidi.so.*|libgmp.so.*)
            return 0
            ;;
        *)
            return 1
            ;;
    esac
}

remove_host_runtimes()
{
    local appdir="$1"
    local entry

    while IFS= read -r -d '' entry; do
        if is_host_runtime "$(basename -- "$entry")"; then
            echo "Removing host runtime: $entry"
            rm -f -- "$entry"
        fi
    done < <(find "$appdir" \( -type f -o -type l \) -print0)
}

collect_dependencies()
{
    local appdir="$1"
    local qt_prefix="$2"
    local openssl_lib="$3"
    local search_path
    local elf output soname arrow resolved destination
    local copied=0
    local index=0
    local entry

    search_path="$appdir/lib:$appdir/bin:$qt_prefix/lib:/usr/local/lib:/usr/local/lib/x86_64-linux-gnu:$openssl_lib"

    remove_host_runtimes "$appdir"

    mapfile -d '' -t queue < <(find "$appdir" -type f -print0)
    declare -A seen=()

    while (( index < ${#queue[@]} )); do
        elf="${queue[$index]}"
        index=$((index + 1))

        if [[ -n "${seen[$elf]+present}" ]]; then
            continue
        fi
        seen["$elf"]=1

        is_elf "$elf" || continue

        if ! output="$(env LD_LIBRARY_PATH="$search_path" ldd "$elf" 2>&1)"; then
            case "$output" in
                *'statically linked'*|*'not a dynamic executable'*)
                    continue
                    ;;
                *)
                    printf '%s\n' "$output" >&2
                    die "ldd failed while scanning $elf"
                    ;;
            esac
        fi

        while read -r soname arrow resolved _; do
            [[ "$arrow" == '=>' ]] || continue

            if [[ "$resolved" == 'not' ]]; then
                if is_host_runtime "$soname"; then
                    echo "Host runtime not bundled: $soname (required by $elf)"
                    continue
                fi
                die "Missing dependency $soname while scanning $elf"
            fi
            [[ -f "$resolved" ]] || die "Dependency does not exist: $resolved"
            is_host_runtime "$soname" && continue

            case "$resolved" in
                "$appdir"/*)
                    continue
                    ;;
            esac

            [[ "$soname" == "$(basename -- "$soname")" ]] ||
                die "Unsafe dependency SONAME: $soname"

            destination="$appdir/lib/$soname"
            if [[ -e "$destination" || -L "$destination" ]]; then
                cmp -s -- "$resolved" "$destination" ||
                    die "Different libraries share SONAME $soname"
                continue
            fi

            echo "Bundling $soname from $resolved"
            cp -L --preserve=mode,timestamps -- "$resolved" "$destination"
            chmod a+rX "$destination"
            queue+=("$destination")
            copied=$((copied + 1))
        done <<< "$output"
    done

    remove_host_runtimes "$appdir"
    echo "Bundled $copied additional runtime libraries."
}

verify_appdir()
{
    local appdir="$1"
    local report="$2"
    local search_path="$appdir/lib:$appdir/bin"
    local canonical_appdir
    local elf output soname arrow resolved canonical_resolved
    local version_info
    local failures=0
    local symbols_file
    local max_glibc max_glibcxx
    local entry
    local actual_qt_plugins expected_qt_plugins nss_pattern
    local dynamic_info

    canonical_appdir="$(readlink -f -- "$appdir")"
    symbols_file="$(mktemp)"

    for required in \
        "$appdir/AppRun" \
        "$appdir/bin/nunchuk-qt" \
        "$appdir/bin/hwi" \
        "$appdir/libexec/QtWebEngineProcess" \
        "$appdir/resources/icudtl.dat" \
        "$appdir/resources/qtwebengine_resources.pak" \
        "$appdir/resources/qtwebengine_resources_100p.pak" \
        "$appdir/resources/qtwebengine_resources_200p.pak" \
        "$appdir/resources/ca-certificates.crt" \
        "$appdir/lib/libssl.so.1.1" \
        "$appdir/lib/libcrypto.so.1.1" \
        "$appdir/qt-plugin-manifest.txt" \
        "$appdir/nunchuk.desktop" \
        "$appdir/nunchuk-qt.png"; do
        if [[ ! -e "$required" ]]; then
            echo "Missing AppDir runtime file: $required" >&2
            failures=$((failures + 1))
        fi
    done

    for executable in \
        "$appdir/AppRun" \
        "$appdir/bin/nunchuk-qt" \
        "$appdir/bin/hwi" \
        "$appdir/libexec/QtWebEngineProcess"; do
        if [[ ! -x "$executable" ]]; then
            echo "AppDir file is not executable: $executable" >&2
            failures=$((failures + 1))
        fi
    done

    find "$appdir/translations/qtwebengine_locales" \
        -type f -name '*.pak' -print -quit | grep -q . || {
        echo "Qt WebEngine locales are missing" >&2
        failures=$((failures + 1))
    }
    find "$appdir/plugins" -type f -name 'libqxcb.so' -print -quit | grep -q . || {
        echo "Qt XCB platform plugin is missing" >&2
        failures=$((failures + 1))
    }
    find "$appdir/lib" -maxdepth 1 -type f -name 'libQt5NetworkAuth.so*' \
        -print -quit | grep -q . || {
        echo "Qt NetworkAuth runtime is missing" >&2
        failures=$((failures + 1))
    }
    dynamic_info="$(readelf -d "$appdir/bin/nunchuk-qt" 2>/dev/null || true)"
    if [[ "$dynamic_info" != *'Shared library: [libQt5NetworkAuth.so'* ]]; then
        echo "Nunchuk does not declare Qt NetworkAuth as a runtime dependency" >&2
        failures=$((failures + 1))
    fi
    dynamic_info="$(readelf -d "$appdir/lib/libssl.so.1.1" 2>/dev/null || true)"
    if [[ "$dynamic_info" != *'Library soname: [libssl.so.1.1]'* ]]; then
        echo "Bundled OpenSSL runtime has an unexpected libssl SONAME" >&2
        failures=$((failures + 1))
    fi
    dynamic_info="$(readelf -d "$appdir/lib/libcrypto.so.1.1" 2>/dev/null || true)"
    if [[ "$dynamic_info" != *'Library soname: [libcrypto.so.1.1]'* ]]; then
        echo "Bundled OpenSSL runtime has an unexpected libcrypto SONAME" >&2
        failures=$((failures + 1))
    fi

    actual_qt_plugins="$({
        cd "$appdir/plugins"
        find . \( -type f -o -type l \) -name '*.so*' \
            -printf '%P\n' | LC_ALL=C sort
    })"
    expected_qt_plugins="$(cat "$appdir/qt-plugin-manifest.txt" 2>/dev/null || true)"
    if [[ "$actual_qt_plugins" != "$expected_qt_plugins" ]]; then
        echo "Qt plugin set does not match its deployment manifest" >&2
        failures=$((failures + 1))
    fi

    for nss_pattern in \
        'libsoftokn3.so' \
        'libsoftokn3.chk' \
        'libfreebl*.so' \
        'libfreebl*.chk' \
        'libnssckbi.so'; do
        find "$appdir/lib" -maxdepth 1 -type f -name "$nss_pattern" \
            -print -quit | grep -q . || {
            echo "NSS WebEngine runtime is missing: $nss_pattern" >&2
            failures=$((failures + 1))
        }
    done

    while IFS= read -r -d '' entry; do
        if is_host_runtime "$(basename -- "$entry")"; then
            echo "Host runtime must not be bundled: $entry" >&2
            failures=$((failures + 1))
        fi
    done < <(find "$appdir" \( -type f -o -type l \) -print0)

    {
        printf 'AppDir: %s\n' "$canonical_appdir"
        printf 'Dependency resolution:\n'
    } > "$report"

    while IFS= read -r -d '' elf; do
        is_elf "$elf" || continue

        version_info="$(readelf --version-info "$elf" 2>/dev/null || true)"
        grep -oE 'GLIBC(XX)?_[0-9]+(\.[0-9]+)*' \
            <<< "$version_info" >> "$symbols_file" || true

        if [[ "$version_info" == *GLIBC_PRIVATE* ]]; then
            echo "$elf requires GLIBC_PRIVATE" >&2
            failures=$((failures + 1))
        fi

        if ! output="$(env LD_LIBRARY_PATH="$search_path" ldd "$elf" 2>&1)"; then
            case "$output" in
                *'statically linked'*|*'not a dynamic executable'*)
                    continue
                    ;;
                *)
                    printf '%s\n' "$output" >&2
                    echo "ldd failed while verifying $elf" >&2
                    failures=$((failures + 1))
                    continue
                    ;;
            esac
        fi

        while read -r soname arrow resolved _; do
            [[ "$arrow" == '=>' ]] || continue

            if [[ "$resolved" == 'not' ]]; then
                if is_host_runtime "$soname"; then
                    printf '%s -> host runtime (not present on build host)\n' \
                        "$soname" >> "$report"
                    continue
                fi
                echo "$elf is missing $soname" >&2
                failures=$((failures + 1))
                continue
            fi

            printf '%s -> %s\n' "$soname" "$resolved" >> "$report"
            is_host_runtime "$soname" && continue

            canonical_resolved="$(readlink -f -- "$resolved")"
            case "$canonical_resolved" in
                "$canonical_appdir"/*)
                    ;;
                *)
                    echo "$elf uses a library outside AppDir: $soname => $resolved" >&2
                    failures=$((failures + 1))
                    ;;
            esac
        done <<< "$output"
    done < <(find "$appdir" -type f -print0)

    {
        printf '\nRequired symbol versions:\n'
        sort -Vu "$symbols_file"
    } >> "$report"

    max_glibc="$({ grep -oE '^GLIBC_[0-9.]+' "$symbols_file" || true; } |
        sort -V | tail -n 1)"
    max_glibcxx="$({ grep -oE '^GLIBCXX_[0-9.]+' "$symbols_file" || true; } |
        sort -V | tail -n 1)"
    printf '\nmax_glibc=%s\nmax_glibcxx=%s\n' \
        "${max_glibc:-unknown}" "${max_glibcxx:-unknown}" >> "$report"

    if (( failures > 0 )); then
        rm -f -- "$symbols_file"
        die "AppDir verification failed with $failures error(s)"
    fi
    env LD_LIBRARY_PATH="$search_path" "$appdir/bin/hwi" --version
    rm -f -- "$symbols_file"
    echo "AppDir verification passed."
}

case "${1:-}" in
    collect)
        [[ $# -eq 4 ]] || die "Usage: $0 collect APPDIR QT_PREFIX OPENSSL_LIB"
        collect_dependencies "$2" "$3" "$4"
        ;;
    verify)
        [[ $# -eq 3 ]] || die "Usage: $0 verify APPDIR REPORT"
        verify_appdir "$2" "$3"
        ;;
    *)
        die "Unknown mode: ${1:-}"
        ;;
esac
