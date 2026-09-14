#!/usr/bin/env bash
set -euo pipefail

die() {
    echo "ERROR: $*" >&2
    exit 1
}

failures=0
symbols_file_to_clean=""

record_failure() {
    echo "ERROR: $*" >&2
    failures=$((failures + 1))
}

cleanup() {
    if [[ -n "${symbols_file_to_clean}" ]]; then
        rm -f -- "${symbols_file_to_clean}"
    fi
}

is_elf() {
    readelf -h -- "$1" >/dev/null 2>&1
}

is_host_runtime() {
    # Keep the host ABI, graphics/audio stack and hardware drivers outside the
    # AppImage. libstdc++ and libgcc_s intentionally follow linuxdeploy's
    # AppImage excludelist; their maximum required versions are reported below.
    # All other dependencies must resolve from inside the AppDir.
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
        libgcc_s.so.*|libstdc++.so.*|\
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

require_file() {
    local path="$1"
    local description="$2"

    if [[ ! -f "${path}" ]]; then
        record_failure "Missing ${description}: ${path}"
    fi
}

require_executable() {
    local path="$1"
    local description="$2"

    if [[ ! -x "${path}" ]]; then
        record_failure "Missing or non-executable ${description}: ${path}"
    fi
}

require_match() {
    local root="$1"
    local pattern="$2"
    local description="$3"
    local match

    if [[ ! -d "${root}" ]]; then
        record_failure "Missing directory for ${description}: ${root}"
        return
    fi

    match="$(find "${root}" -type f -name "${pattern}" -print -quit)"
    if [[ -z "${match}" ]]; then
        record_failure "Missing ${description} matching ${root}/${pattern}"
    fi
}

verify_no_bundled_host_runtimes() {
    local appdir="$1"
    local entry

    while IFS= read -r -d '' entry; do
        if is_host_runtime "$(basename -- "${entry}")"; then
            record_failure "Host runtime must not be bundled: ${entry}"
        fi
    done < <(
        find "${appdir}" \( -type f -o -type l \) -print0 \
            | LC_ALL=C sort -z
    )
}

build_library_path() {
    local appdir="$1"

    # Mirror the runtime library roots. Adding plugin or QML directories here
    # could hide a broken RUNPATH by letting ldd resolve a dependency from a
    # directory the packaged process would never search.
    printf '%s\n' "${appdir}/usr/lib"
}

verify_dynamic_identity() {
    local appdir="$1"
    local application="${appdir}/usr/bin/nunchuk-qt"
    local ssl_library="${appdir}/usr/lib/libssl.so.3"
    local crypto_library="${appdir}/usr/lib/libcrypto.so.3"
    local dynamic_info

    if [[ -f "${application}" ]]; then
        dynamic_info="$(readelf -d -- "${application}" 2>/dev/null || true)"
        if [[ "${dynamic_info}" != *'Shared library: [libQt6Core.so.6]'* ]]; then
            record_failure "Nunchuk does not declare Qt6 Core as a runtime dependency"
        fi
        if [[ "${dynamic_info}" != *'Shared library: [libQt6NetworkAuth.so.6]'* ]]; then
            record_failure "Nunchuk does not declare Qt6 NetworkAuth as a runtime dependency"
        fi
        if [[ "${dynamic_info}" == *'Shared library: [libQt5'* ]]; then
            record_failure "Nunchuk unexpectedly declares a Qt5 runtime dependency"
        fi
    fi

    if [[ -f "${ssl_library}" ]]; then
        dynamic_info="$(readelf -d -- "${ssl_library}" 2>/dev/null || true)"
        if [[ "${dynamic_info}" != *'Library soname: [libssl.so.3]'* ]]; then
            record_failure "Bundled OpenSSL runtime has an unexpected libssl SONAME"
        fi
    fi

    if [[ -f "${crypto_library}" ]]; then
        dynamic_info="$(readelf -d -- "${crypto_library}" 2>/dev/null || true)"
        if [[ "${dynamic_info}" != *'Library soname: [libcrypto.so.3]'* ]]; then
            record_failure "Bundled OpenSSL runtime has an unexpected libcrypto SONAME"
        fi
    fi
}

verify_ldd_entry() {
    local appdir="$1"
    local report="$2"
    local elf="$3"
    local line="$4"
    local soname
    local resolved
    local remainder
    local canonical_resolved

    line="${line#"${line%%[![:space:]]*}"}"
    [[ -n "${line}" ]] || return
    if [[ "${line}" == 'ldd: warning: you do not have execution permission for '* ]]; then
        return
    fi

    if [[ "${line}" == *'=>'* ]]; then
        soname="${line%%[[:space:]]*}"
        remainder="${line#*=>}"
        remainder="${remainder#"${remainder%%[![:space:]]*}"}"

        if [[ "${remainder}" == 'not found'* ]]; then
            if is_host_runtime "${soname}"; then
                printf '%s -> host runtime (not present on build host)\n' \
                    "${soname}" >> "${report}"
                return
            fi
            record_failure "${elf} is missing ${soname}"
            printf '%s -> not found\n' "${soname}" >> "${report}"
            return
        fi

        resolved="${remainder%%[[:space:]]*}"
        if [[ -z "${resolved}" || "${resolved}" != /* ]]; then
            record_failure "Could not parse dependency path for ${soname} in ${elf}: ${line}"
            return
        fi
    else
        resolved="${line%%[[:space:]]*}"
        soname="$(basename -- "${resolved}")"

        if [[ "${resolved}" != /* ]]; then
            if is_host_runtime "${soname}"; then
                printf '%s -> host runtime\n' "${soname}" >> "${report}"
                return
            fi
            record_failure "Could not resolve dependency entry in ${elf}: ${line}"
            return
        fi
    fi

    if [[ ! -e "${resolved}" ]]; then
        record_failure "Resolved dependency does not exist for ${elf}: ${soname} => ${resolved}"
        return
    fi

    canonical_resolved="$(readlink -f -- "${resolved}" 2>/dev/null || true)"
    if [[ -z "${canonical_resolved}" ]]; then
        record_failure "Could not canonicalize dependency for ${elf}: ${soname} => ${resolved}"
        return
    fi

    printf '%s -> %s\n' "${soname}" "${canonical_resolved}" >> "${report}"

    if is_host_runtime "${soname}"; then
        case "${canonical_resolved}" in
            "${appdir}"/*)
                record_failure \
                    "${elf} resolves host runtime from AppDir: ${soname} => ${resolved}"
                ;;
        esac
        return
    fi

    case "${canonical_resolved}" in
        "${appdir}"/*)
            ;;
        *)
            record_failure "${elf} uses a non-host library outside AppDir: ${soname} => ${resolved}"
            ;;
    esac
}

verify_elf_dependencies() {
    local appdir="$1"
    local report="$2"
    local library_path="$3"
    local symbols_file="$4"
    local elf
    local output
    local parsed_output
    local status
    local version_info
    local needs_info
    local line
    local non_dynamic_output_regex='^[[:blank:]]*(statically linked|not a dynamic executable)[[:blank:]]*$'

    while IFS= read -r -d '' elf; do
        is_elf "${elf}" || continue

        version_info="$(LC_ALL=C readelf --version-info -- "${elf}" 2>/dev/null || true)"
        needs_info="$(sed -n '/Version needs section/,$p' <<< "${version_info}")"
        grep -oE 'GLIBC(XX)?_[0-9]+(\.[0-9]+)*' \
            <<< "${needs_info}" >> "${symbols_file}" || true

        if [[ "${needs_info}" == *GLIBC_PRIVATE* ]]; then
            record_failure "${elf} requires GLIBC_PRIVATE"
        fi

        set +e
        output="$(LC_ALL=C LD_LIBRARY_PATH="${library_path}" ldd -- "${elf}" 2>&1)"
        status=$?
        set -e

        parsed_output="$(sed -E \
            '/^[[:blank:]]*ldd: warning: you do not have execution permission for /d' \
            <<< "${output}")"
        if [[ "${parsed_output}" =~ ${non_dynamic_output_regex} ]]; then
            continue
        fi

        if (( status != 0 )); then
            printf '%s\n' "${output}" >&2
            record_failure "ldd failed while verifying ${elf}"
            continue
        fi

        while IFS= read -r line; do
            verify_ldd_entry "${appdir}" "${report}" "${elf}" "${line}"
        done <<< "${parsed_output}"
    done < <(find "${appdir}" -type f -print0 | LC_ALL=C sort -z)
}

verify_hwi() {
    local appdir="$1"
    local report="$2"
    local library_path="$3"
    local hwi="${appdir}/usr/bin/hwi"
    local output

    [[ -x "${hwi}" ]] || return

    if output="$(LD_LIBRARY_PATH="${library_path}" "${hwi}" --version 2>&1)"; then
        printf '\nHWI version:\n%s\n' "${output}" >> "${report}"
    else
        printf '%s\n' "${output}" >&2
        record_failure "Bundled HWI failed to execute with --version"
    fi
}

verify_appdir() {
    local appdir="$1"
    local report="$2"
    local canonical_appdir
    local library_path
    local symbols_file
    local max_glibc
    local max_glibcxx

    for command_name in basename find grep ldd mktemp readelf readlink rm sed sort tail; do
        command -v "${command_name}" >/dev/null \
            || die "Missing verification tool: ${command_name}"
    done

    [[ -d "${appdir}" ]] || die "AppDir does not exist: ${appdir}"
    [[ -d "${appdir}/usr" ]] || die "AppDir does not use the expected /usr layout: ${appdir}"

    canonical_appdir="$(readlink -f -- "${appdir}")"
    [[ -n "${canonical_appdir}" ]] || die "Could not canonicalize AppDir: ${appdir}"

    symbols_file="$(mktemp)"
    symbols_file_to_clean="${symbols_file}"
    trap cleanup EXIT

    {
        printf 'AppDir: %s\n' "${canonical_appdir}"
        printf 'Dependency resolution:\n'
    } > "${report}"

    require_executable "${canonical_appdir}/AppRun" "AppRun"
    require_executable "${canonical_appdir}/usr/bin/nunchuk-qt" "Nunchuk binary"
    require_executable "${canonical_appdir}/usr/bin/hwi" "HWI binary"
    require_executable \
        "${canonical_appdir}/usr/libexec/QtWebEngineProcess" \
        "QtWebEngineProcess"

    require_file \
        "${canonical_appdir}/usr/share/applications/nunchuk.desktop" \
        "desktop entry"
    require_file "${canonical_appdir}/usr/resources/icudtl.dat" "Qt WebEngine ICU data"
    require_file \
        "${canonical_appdir}/usr/resources/v8_context_snapshot.bin" \
        "Qt WebEngine V8 snapshot"
    require_file "${canonical_appdir}/usr/lib/libssl.so.3" "OpenSSL libssl 3 runtime"
    require_file "${canonical_appdir}/usr/lib/libcrypto.so.3" "OpenSSL libcrypto 3 runtime"
    require_file \
        "${canonical_appdir}/usr/lib/ossl-modules/legacy.so" \
        "OpenSSL legacy provider"

    require_file \
        "${canonical_appdir}/usr/resources/qtwebengine_resources.pak" \
        "Qt WebEngine resources"
    require_file \
        "${canonical_appdir}/usr/resources/qtwebengine_resources_100p.pak" \
        "Qt WebEngine 100-percent resources"
    require_file \
        "${canonical_appdir}/usr/resources/qtwebengine_resources_200p.pak" \
        "Qt WebEngine 200-percent resources"
    require_file \
        "${canonical_appdir}/usr/resources/ca-certificates.crt" \
        "CA certificate bundle"
    require_match \
        "${canonical_appdir}/usr/translations/qtwebengine_locales" \
        '*.pak' \
        "Qt WebEngine locales"
    require_match "${canonical_appdir}/usr/lib" 'libQt6Core.so*' "Qt6 Core runtime"
    require_match \
        "${canonical_appdir}/usr/lib" \
        'libQt6NetworkAuth.so*' \
        "Qt6 NetworkAuth runtime"
    require_match \
        "${canonical_appdir}/usr/lib" \
        'libQt6WebEngineCore.so*' \
        "Qt6 WebEngineCore runtime"
    require_match \
        "${canonical_appdir}/usr/lib" \
        'libQt6ShaderTools.so*' \
        "Qt6 ShaderTools runtime"
    require_match "${canonical_appdir}/usr/lib" 'libminizip.so.1*' "minizip runtime"

    require_file \
        "${canonical_appdir}/usr/qml/Qt5Compat/GraphicalEffects/qmldir" \
        "Qt5Compat GraphicalEffects metadata"
    require_file \
        "${canonical_appdir}/usr/qml/Qt5Compat/GraphicalEffects/private/libqtgraphicaleffectsprivateplugin.so" \
        "Qt5Compat GraphicalEffects private plugin"

    for plugin in \
        platforms/libqxcb.so \
        imageformats/libqjpeg.so \
        imageformats/libqsvg.so \
        iconengines/libqsvgicon.so \
        multimedia/libffmpegmediaplugin.so \
        sqldrivers/libqsqlite.so \
        tls/libqopensslbackend.so; do
        require_file \
            "${canonical_appdir}/usr/plugins/${plugin}" \
            "required Qt6 plugin ${plugin}"
    done

    verify_no_bundled_host_runtimes "${canonical_appdir}"
    verify_dynamic_identity "${canonical_appdir}"

    library_path="$(build_library_path "${canonical_appdir}")"
    verify_elf_dependencies \
        "${canonical_appdir}" \
        "${report}" \
        "${library_path}" \
        "${symbols_file}"
    verify_hwi "${canonical_appdir}" "${report}" "${library_path}"

    {
        printf '\nRequired symbol versions:\n'
        LC_ALL=C sort -Vu "${symbols_file}"
    } >> "${report}"

    max_glibc="$({ grep -oE '^GLIBC_[0-9.]+' "${symbols_file}" || true; } \
        | LC_ALL=C sort -V | tail -n 1)"
    max_glibcxx="$({ grep -oE '^GLIBCXX_[0-9.]+' "${symbols_file}" || true; } \
        | LC_ALL=C sort -V | tail -n 1)"
    printf '\nmax_glibc=%s\nmax_glibcxx=%s\n' \
        "${max_glibc:-unknown}" \
        "${max_glibcxx:-unknown}" >> "${report}"

    if (( failures > 0 )); then
        die "AppDir verification failed with ${failures} error(s)"
    fi

    rm -f -- "${symbols_file}"
    symbols_file_to_clean=""
    trap - EXIT
    echo "AppDir verification passed."
}

case "${1:-}" in
    verify)
        [[ $# -eq 3 ]] || die "Usage: $0 verify APPDIR REPORT"
        verify_appdir "$2" "$3"
        ;;
    *)
        die "Unknown mode: ${1:-}"
        ;;
esac
