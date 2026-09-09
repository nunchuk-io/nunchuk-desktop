#!/usr/bin/env bash
set -euo pipefail

# Deployment steps re-derived from the manually-run reference workflow (see
# build_macos.sh's header for the link and the rationale). macdeployqt
# invocation, the QtShaderTools.framework copy fix, and the
# QtWebEngineProcess rpath/dependency patch below all mirror that reference
# workflow's own steps -- none of the previous draft's architecture-thinning,
# signature-stripping or exhaustive Mach-O dependency-closure checks are
# carried over, since those were tuned for a different Qt version/module set
# that was never actually validated. Signature removal now happens in
# sign_macos.sh immediately before each codesign call (`codesign
# --remove-signature` per file), matching how the reference workflow's own
# signing step does it, rather than as a separate packaging-time pass.
#
# Reproducibility additions kept from the previous draft (generic packaging
# mechanics, not build-specific, so not affected by the "don't trust the old
# build steps" instruction): a payload manifest recording every file's mode,
# type and SHA-256; SOURCE_DATE_EPOCH-normalized timestamps; and a
# deterministic GNU-format tar so the canonical archive's bytes depend only
# on file contents, not on incidental filesystem/tar metadata.

required_variables=(
    APP_PATH
    HWI_BINARY
    PROJECT_DIR
    QT_ROOT
    OUTPUT_DIR
    ARCH
    TAG
    SOURCE_DATE_EPOCH
)
for variable_name in "${required_variables[@]}"; do
    if [[ -z "${!variable_name:-}" ]]; then
        echo "Missing required environment variable: ${variable_name}" >&2
        exit 1
    fi
done

# shellcheck source=macos.lock.env
source "${PROJECT_DIR}/reproducible-builds/macos.lock.env"

if [[ ! -d "${APP_PATH}" || ! -x "${APP_PATH}/Contents/MacOS/Nunchuk" ]]; then
    echo "Invalid application bundle: ${APP_PATH}" >&2
    exit 1
fi
if [[ ! -x "${HWI_BINARY}" ]]; then
    echo "Invalid HWI executable: ${HWI_BINARY}" >&2
    exit 1
fi

export TZ=UTC
export LANG=C
export LC_ALL=C
umask 022

"${QT_ROOT}/bin/macdeployqt" "${APP_PATH}" \
    -executable="${APP_PATH}/Contents/MacOS/Nunchuk" \
    -qmldir="${PROJECT_DIR}" \
    -always-overwrite

# Qt's deployment scanner can miss this transitive dependency of
# Qt5Compat.GraphicalEffects.private (same fix the reference workflow
# applies after its own macdeployqt call).
shader_tools_source="${QT_ROOT}/lib/QtShaderTools.framework"
shader_tools_destination="${APP_PATH}/Contents/Frameworks/QtShaderTools.framework"
if [[ -d "${shader_tools_source}" && ! -d "${shader_tools_destination}" ]]; then
    cp -R "${shader_tools_source}" "${APP_PATH}/Contents/Frameworks/"
fi

install -m 0755 "${HWI_BINARY}" "${APP_PATH}/Contents/MacOS/hwi"

# QtWebEngineProcess helper rpath/dependency patch, ported as-is from the
# reference workflow's own step of the same name. Best-effort/non-fatal by
# design there (a missing helper, or an rpath/dependency already in the
# expected state, are not errors) -- kept that way here rather than making it
# stricter, since that leniency has not been re-validated against Qt 6.9.3's
# actual deployed layout in this pipeline yet.
helper_path="${APP_PATH}/Contents/Frameworks/QtWebEngineCore.framework/Versions/A/Helpers/QtWebEngineProcess.app"
helper_executable="${helper_path}/Contents/MacOS/QtWebEngineProcess"
if [[ -f "${helper_executable}" ]]; then
    current_rpaths="$(otool -l "${helper_executable}" \
        | awk '/cmd LC_RPATH/{read_path=1; next} read_path && /path /{print $2; read_path=0}')"
    old_rpath='@executable_path/../Frameworks'
    new_rpath='@executable_path/../../../../../../../../Frameworks'

    if grep -Fqx "${old_rpath}" <<< "${current_rpaths}"; then
        install_name_tool -delete_rpath "${old_rpath}" "${helper_executable}" || true
    fi
    if ! grep -Fqx "${new_rpath}" <<< "${current_rpaths}"; then
        install_name_tool -add_rpath "${new_rpath}" "${helper_executable}" || true
    fi

    qt_dependencies="$(otool -L "${helper_executable}" | awk '{ print $1 }' | grep -E 'Qt[^/]+\.framework' || true)"
    if [[ -n "${qt_dependencies}" ]]; then
        while IFS= read -r dependency; do
            [[ -z "${dependency}" ]] && continue
            framework="$(sed -E 's|.*/(Qt[^/]+)\.framework.*|\1|' <<< "${dependency}")"
            new_path="@executable_path/../../../../../../../${framework}.framework/Versions/A/${framework}"
            install_name_tool -change "${dependency}" "${new_path}" "${helper_executable}" || true
        done <<< "${qt_dependencies}"
    fi
fi

source_commit="$(git -C "${PROJECT_DIR}" rev-parse HEAD)"
{
    printf 'schema=nunchuk-macos-build-inputs-v1\n'
    printf 'arch=%s\n' "${ARCH}"
    printf 'version=%s\n' "${TAG}"
    printf 'source_commit=%s\n' "${source_commit}"
    printf 'source_date_epoch=%s\n' "${SOURCE_DATE_EPOCH}"
    printf 'qt=%s\n' "${QT_VERSION}"
    printf 'qt_modules=%s\n' "${QT_MODULES}"
    printf 'qtkeychain=%s\n' "${QTKEYCHAIN_VERSION}"
    printf 'boost=%s@sha256:%s\n' "${BOOST_VERSION}" "${BOOST_SHA256}"
    printf 'libevent=%s@sha256:%s\n' "${LIBEVENT_VERSION}" "${LIBEVENT_SHA256}"
    printf 'olm=%s@%s\n' "${OLM_VERSION}" "${OLM_COMMIT}"
    printf 'hwi=%s@%s\n' "${HWI_VERSION}" "${HWI_COMMIT}"
    printf 'hwi_binary_sha256=%s\n' "$(shasum -a 256 "${APP_PATH}/Contents/MacOS/hwi" | awk '{ print $1 }')"
    printf 'python=%s\n' "${PYTHON_VERSION}"
    printf 'xcode=%s\n' "$(xcodebuild -version | paste -sd ' ' -)"
    printf 'clang=%s\n' "$(clang --version | sed -n '1p')"
    printf '%s\n' 'submodules:'
    git -C "${PROJECT_DIR}" submodule status --recursive | LC_ALL=C sort
} > "${OUTPUT_DIR}/build-inputs.txt"

payload_root="${OUTPUT_DIR}/payload"
cmake -E remove_directory "${payload_root}"
mkdir -p "${payload_root}"
/usr/bin/ditto --noextattr --noqtn "${APP_PATH}" "${payload_root}/Nunchuk.app"
cp "${OUTPUT_DIR}/build-inputs.txt" "${payload_root}/build-inputs.txt"

# Normalize the tree before hashing it into the manifest.
find "${payload_root}" -type d -exec chmod 0755 {} +
while IFS= read -r -d '' payload_file; do
    if [[ -x "${payload_file}" ]]; then
        chmod 0755 "${payload_file}"
    else
        chmod 0644 "${payload_file}"
    fi
done < <(find "${payload_root}" -type f -print0)
xattr -cr "${payload_root}"

python3 - "${payload_root}/Nunchuk.app" "${payload_root}/payload-manifest.json" \
    "${ARCH}" "${TAG}" "${source_commit}" "${SOURCE_DATE_EPOCH}" <<'PY'
import hashlib
import json
import os
import stat
import sys

app_path, output_path, arch, version, commit, epoch = sys.argv[1:]
root = os.path.dirname(app_path)
paths = [app_path]
for current_root, directories, files in os.walk(app_path, topdown=True, followlinks=False):
    directories.sort()
    files.sort()
    paths.extend(os.path.join(current_root, name) for name in directories)
    paths.extend(os.path.join(current_root, name) for name in files)

entries = []
for path in sorted(paths, key=lambda item: os.path.relpath(item, root)):
    metadata = os.lstat(path)
    entry = {
        "mode": format(stat.S_IMODE(metadata.st_mode), "04o"),
        "path": os.path.relpath(path, root),
    }
    if stat.S_ISLNK(metadata.st_mode):
        entry.update(type="symlink", target=os.readlink(path))
    elif stat.S_ISDIR(metadata.st_mode):
        entry.update(type="directory")
    elif stat.S_ISREG(metadata.st_mode):
        digest = hashlib.sha256()
        with open(path, "rb") as source:
            for chunk in iter(lambda: source.read(1024 * 1024), b""):
                digest.update(chunk)
        entry.update(type="file", size=metadata.st_size, sha256=digest.hexdigest())
    else:
        raise SystemExit(f"Unsupported payload entry: {path}")
    entries.append(entry)

manifest = {
    "arch": arch,
    "entries": entries,
    "format": "nunchuk-macos-unsigned-payload-v1",
    "source_commit": commit,
    "source_date_epoch": int(epoch),
    "version": version,
}
with open(output_path, "w", encoding="utf-8", newline="\n") as destination:
    json.dump(manifest, destination, ensure_ascii=True, separators=(",", ":"), sort_keys=True)
    destination.write("\n")
PY

chmod 0644 "${payload_root}/payload-manifest.json"

normalized_timestamp="$(date -u -r "${SOURCE_DATE_EPOCH}" '+%Y%m%d%H%M.%S')"
find "${payload_root}" -exec touch -h -t "${normalized_timestamp}" {} +

archive="${OUTPUT_DIR}/nunchuk-macos-${ARCH}-v${TAG}-unsigned.tar"
# Apple's bsdtar writes PAX ctime/atime records and can emit AppleDouble
# metadata even after xattr cleanup. Build each header explicitly instead so
# the canonical archive contains only source-controlled values. GNU tar
# format keeps long Qt/WebEngine paths deterministic without PAX metadata
# records.
python3 - "${payload_root}" "${archive}" "${SOURCE_DATE_EPOCH}" <<'PY'
import os
import stat
import sys
import tarfile

payload_root, archive_path, epoch_text = sys.argv[1:]
epoch = int(epoch_text)

paths = [payload_root]
for current_root, directories, files in os.walk(
    payload_root, topdown=True, followlinks=False
):
    directories.sort(key=os.fsencode)
    files.sort(key=os.fsencode)
    paths.extend(os.path.join(current_root, name) for name in directories)
    paths.extend(os.path.join(current_root, name) for name in files)

paths.sort(
    key=lambda path: os.fsencode(os.path.relpath(path, payload_root))
)

with tarfile.open(
    archive_path,
    mode="w",
    format=tarfile.GNU_FORMAT,
    dereference=False,
) as archive:
    for path in paths:
        relative = os.path.relpath(path, payload_root)
        archive_name = "." if relative == "." else f"./{relative}"
        metadata = os.lstat(path)

        info = tarfile.TarInfo(archive_name)
        info.uid = 0
        info.gid = 0
        info.uname = "root"
        info.gname = "wheel"
        info.mode = stat.S_IMODE(metadata.st_mode)
        info.mtime = epoch

        if stat.S_ISDIR(metadata.st_mode):
            info.type = tarfile.DIRTYPE
            archive.addfile(info)
        elif stat.S_ISLNK(metadata.st_mode):
            info.type = tarfile.SYMTYPE
            info.linkname = os.readlink(path)
            archive.addfile(info)
        elif stat.S_ISREG(metadata.st_mode):
            info.type = tarfile.REGTYPE
            info.size = metadata.st_size
            with open(path, "rb") as payload_file:
                archive.addfile(info, payload_file)
        else:
            raise SystemExit(f"Unsupported payload entry: {path}")
PY

(
    cd "${OUTPUT_DIR}"
    shasum -a 256 "$(basename "${archive}")" > "$(basename "${archive}").sha256"
    shasum -a 256 --check "$(basename "${archive}").sha256"
)

printf 'Created canonical unsigned macOS payload: %s\n' "${archive}"
