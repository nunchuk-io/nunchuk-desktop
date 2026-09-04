#!/usr/bin/env bash
set -euo pipefail

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
    -qmldir="${PROJECT_DIR}" \
    -always-overwrite \
    -verbose=2

# Qt's deployment scanner can miss this transitive dependency of
# Qt5Compat.GraphicalEffects.private. Keep the framework layout and symlinks.
shader_tools_source="${QT_ROOT}/lib/QtShaderTools.framework"
shader_tools_destination="${APP_PATH}/Contents/Frameworks/QtShaderTools.framework"
if [[ -d "${shader_tools_source}" && ! -d "${shader_tools_destination}" ]]; then
    /usr/bin/ditto --noextattr --noqtn \
        "${shader_tools_source}" \
        "${shader_tools_destination}"
fi

install -m 0755 "${HWI_BINARY}" "${APP_PATH}/Contents/MacOS/hwi"

is_macho() {
    /usr/bin/file -b "$1" | grep -q 'Mach-O'
}

strip_all_signatures() {
    local candidate
    local code_directory
    local signature_directory

    while IFS= read -r -d '' candidate; do
        if is_macho "${candidate}"; then
            codesign --remove-signature "${candidate}" >/dev/null 2>&1 || true
        fi
    done < <(find "${APP_PATH}" -type f -print0)

    while IFS= read -r -d '' code_directory; do
        if codesign --display "${code_directory}" >/dev/null 2>&1; then
            codesign --remove-signature "${code_directory}" >/dev/null 2>&1 || true
        fi
    done < <(find "${APP_PATH}" -depth -type d \
        \( -name '*.app' -o -name '*.framework' -o -name '*.bundle' -o -name '*.plugin' -o -name '*.xpc' \) \
        -print0)

    while IFS= read -r -d '' signature_directory; do
        rm -rf "${signature_directory}"
    done < <(find "${APP_PATH}" -depth -type d -name _CodeSignature -print0)
    xattr -cr "${APP_PATH}"
}

# Strip valid upstream/ad-hoc signatures before changing load commands or
# thinning universal binaries, both of which would invalidate those signatures.
strip_all_signatures

webengine_helper="${APP_PATH}/Contents/Frameworks/QtWebEngineCore.framework/Versions/A/Helpers/QtWebEngineProcess.app/Contents/MacOS/QtWebEngineProcess"
if [[ -f "${webengine_helper}" ]]; then
    required_helper_rpath='@executable_path/../../../../../../../../Frameworks'
    if ! otool -l "${webengine_helper}" \
        | awk '/cmd LC_RPATH/{read_path=1; next} read_path && /path /{print $2; read_path=0}' \
        | grep -Fqx "${required_helper_rpath}"; then
        install_name_tool -add_rpath "${required_helper_rpath}" "${webengine_helper}"
    fi
fi

# The Qt archives are universal on macOS. Each official artifact is native to
# its matrix architecture, including all helpers and plug-ins.
while IFS= read -r -d '' candidate; do
    if ! is_macho "${candidate}"; then
        continue
    fi
    if ! lipo "${candidate}" -verify_arch "${ARCH}"; then
        echo "Mach-O file does not contain ${ARCH}: ${candidate}" >&2
        exit 1
    fi
    macho_arches="$(lipo -archs "${candidate}")"
    if [[ "${macho_arches}" == *" "* ]]; then
        original_mode="$(stat -f '%Lp' "${candidate}")"
        lipo "${candidate}" -thin "${ARCH}" -output "${candidate}.thin"
        chmod "${original_mode}" "${candidate}.thin"
        mv "${candidate}.thin" "${candidate}"
    fi
done < <(find "${APP_PATH}" -type f -print0)

# Ensure transformations did not preserve or reintroduce any signature.
strip_all_signatures

while IFS= read -r -d '' candidate; do
    if is_macho "${candidate}" && codesign --display "${candidate}" >/dev/null 2>&1; then
        echo "Unsigned payload still contains a code signature: ${candidate}" >&2
        exit 1
    fi
done < <(find "${APP_PATH}" -type f -print0)

while IFS= read -r -d '' code_directory; do
    if codesign --display "${code_directory}" >/dev/null 2>&1; then
        echo "Unsigned payload still contains a bundle signature: ${code_directory}" >&2
        exit 1
    fi
done < <(find "${APP_PATH}" -type d \
    \( -name '*.app' -o -name '*.framework' -o -name '*.bundle' -o -name '*.plugin' -o -name '*.xpc' \) \
    -print0)

require_path() {
    local description="$1"
    local path_pattern="$2"
    if [[ -z "$(find "${APP_PATH}" -path "${path_pattern}" -print -quit)" ]]; then
        echo "Missing packaged ${description}: ${path_pattern}" >&2
        exit 1
    fi
}

require_path 'HWI executable' '*/Contents/MacOS/hwi'
require_path 'Qt NetworkAuth framework' '*/QtNetworkAuth.framework'
require_path 'Qt WebEngine process' '*/QtWebEngineProcess.app/Contents/MacOS/QtWebEngineProcess'
require_path 'Qt WebEngine resources' '*/qtwebengine_resources.pak'
require_path 'Qt WebEngine 100 percent resources' '*/qtwebengine_resources_100p.pak'
require_path 'Qt WebEngine 200 percent resources' '*/qtwebengine_resources_200p.pak'
require_path 'Qt WebEngine locales' '*/qtwebengine_locales/*.pak'
require_path 'Qt WebEngine ICU data' '*/icudtl.dat'
require_path 'Qt WebEngine V8 snapshot' "*/v8_context_snapshot.${ARCH}.bin"
require_path 'Qt ShaderTools framework' '*/QtShaderTools.framework'
require_path 'Qt5Compat GraphicalEffects plug-in' '*/Qt5Compat/GraphicalEffects/libqtgraphicaleffectsplugin.dylib'
require_path 'Qt5Compat GraphicalEffects private plug-in' '*/Qt5Compat/GraphicalEffects/private/libqtgraphicaleffectsprivateplugin.dylib'
require_path 'multimedia FFmpeg plug-in' '*/multimedia/libffmpegmediaplugin.dylib'
require_path 'Cocoa platform plug-in' '*/platforms/libqcocoa.dylib'

bundle_version="$(plutil -extract CFBundleShortVersionString raw \
    -o - "${APP_PATH}/Contents/Info.plist")"
if [[ "${bundle_version}" != "${TAG}" ]]; then
    echo "Packaged version ${bundle_version} does not match requested ${TAG}." >&2
    exit 1
fi
if ! otool -L "${APP_PATH}/Contents/MacOS/Nunchuk" \
    | grep -Fq 'QtNetworkAuth.framework'; then
    echo "The application does not declare Qt NetworkAuth as a runtime dependency." >&2
    exit 1
fi

while IFS= read -r -d '' candidate; do
    if ! is_macho "${candidate}"; then
        continue
    fi
    if ! lipo "${candidate}" -verify_arch "${ARCH}"; then
        echo "Wrong packaged architecture: ${candidate}" >&2
        exit 1
    fi

    while IFS= read -r dependency; do
        dependency="${dependency%% (*}"
        case "${dependency}" in
            @*|/System/Library/*|/usr/lib/*) ;;
            '') ;;
            *)
                echo "External Mach-O dependency in ${candidate}: ${dependency}" >&2
                exit 1
                ;;
        esac
    done < <(otool -L "${candidate}" | tail -n +2 | sed -E 's/^[[:space:]]+//')

    while IFS= read -r runtime_path; do
        case "${runtime_path}" in
            @*) ;;
            /System/Library/*|/usr/lib/*) ;;
            *)
                echo "External LC_RPATH in ${candidate}: ${runtime_path}" >&2
                exit 1
                ;;
        esac
    done < <(otool -l "${candidate}" \
        | awk '/cmd LC_RPATH/{read_path=1; next} read_path && /path /{print $2; read_path=0}')
done < <(find "${APP_PATH}" -type f -print0)

# Resolve every relocatable Mach-O install name against the loader, the
# nearest containing executable bundle, and its declared runpaths. Merely
# accepting an @rpath string would allow a missing framework to pass the
# packaging gate and fail only on a user's machine.
python3 - "${APP_PATH}" <<'PY'
import os
import subprocess
import sys

app_root = os.path.abspath(sys.argv[1])
app_real = os.path.realpath(app_root)


def command_output(*arguments):
    result = subprocess.run(
        arguments,
        check=True,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True,
    )
    return result.stdout


def is_macho(path):
    return "Mach-O" in command_output("/usr/bin/file", "-b", path)


def dependencies(path):
    lines = command_output("/usr/bin/otool", "-L", path).splitlines()[1:]
    return [line.strip().split(" (", 1)[0] for line in lines if line.strip()]


def runpaths(path):
    values = []
    expect_path = False
    for line in command_output("/usr/bin/otool", "-l", path).splitlines():
        stripped = line.strip()
        if stripped == "cmd LC_RPATH":
            expect_path = True
        elif expect_path and stripped.startswith("path "):
            values.append(stripped.split()[1])
            expect_path = False
    return values


def bundle_container(path):
    container = app_root
    relative = os.path.relpath(path, app_root)
    current = app_root
    for component in relative.split(os.sep)[:-1]:
        current = os.path.join(current, component)
        if component.endswith(".app"):
            container = current
    return container


def bundle_executable(path):
    container = bundle_container(path)
    executable_dir = os.path.join(container, "Contents", "MacOS")
    if (
        os.path.dirname(path) == executable_dir
        and os.path.isfile(path)
        and os.access(path, os.X_OK)
    ):
        return path
    bundle_name = os.path.basename(container)[: -len(".app")]
    return os.path.join(executable_dir, bundle_name)


def expand_token(value, loader_dir, executable_dir):
    substitutions = (
        ("@loader_path", loader_dir),
        ("@executable_path", executable_dir),
    )
    for token, base in substitutions:
        if value == token:
            return base
        if value.startswith(token + "/"):
            return os.path.join(base, value[len(token) + 1 :])
    if value.startswith("@"):
        return None
    return value


def inside_bundle(path):
    candidate_real = os.path.realpath(path)
    try:
        return os.path.commonpath((app_real, candidate_real)) == app_real
    except ValueError:
        return False


missing = []
macho_files = []
for current_root, directories, files in os.walk(app_root, followlinks=False):
    directories.sort(key=os.fsencode)
    files.sort(key=os.fsencode)
    for name in files:
        path = os.path.join(current_root, name)
        # Framework convenience links point at a real binary that is already
        # checked below. Resolving from the symlink's directory would give
        # @loader_path the wrong base.
        if not os.path.islink(path) and is_macho(path):
            macho_files.append(path)

for path in macho_files:
    loader_dir = os.path.dirname(path)
    executable_path = bundle_executable(path)
    executable_dir = os.path.dirname(executable_path)
    expanded_runpaths = []
    runpath_sources = [path]
    if executable_path != path and os.path.isfile(executable_path):
        runpath_sources.append(executable_path)
    for runpath_source in runpath_sources:
        source_loader_dir = os.path.dirname(runpath_source)
        source_executable_dir = os.path.dirname(bundle_executable(runpath_source))
        for runtime_path in runpaths(runpath_source):
            expanded = expand_token(
                runtime_path, source_loader_dir, source_executable_dir
            )
            if expanded is None:
                missing.append(
                    f"{runpath_source}: unsupported LC_RPATH {runtime_path}"
                )
                continue
            expanded = os.path.normpath(expanded)
            if not (
                inside_bundle(expanded)
                or expanded.startswith("/System/Library/")
                or expanded.startswith("/usr/lib/")
            ):
                missing.append(
                    f"{runpath_source}: LC_RPATH escapes bundle {runtime_path}"
                )
                continue
            if expanded not in expanded_runpaths:
                expanded_runpaths.append(expanded)

    for dependency in dependencies(path):
        if dependency.startswith("/System/Library/") or dependency.startswith(
            "/usr/lib/"
        ):
            continue

        candidates = []
        if dependency.startswith("@rpath/"):
            suffix = dependency[len("@rpath/") :]
            candidates.extend(os.path.join(base, suffix) for base in expanded_runpaths)
        else:
            expanded = expand_token(dependency, loader_dir, executable_dir)
            if expanded is not None:
                candidates.append(expanded)

        resolved = next(
            (
                candidate
                for candidate in candidates
                if os.path.exists(candidate) and inside_bundle(candidate)
            ),
            None,
        )
        if resolved is None:
            missing.append(f"{path}: unresolved dependency {dependency}")

if missing:
    raise SystemExit(
        "Packaged Mach-O dependency closure is incomplete:\n" + "\n".join(missing)
    )
print(f"Verified Mach-O dependency closure for {len(macho_files)} files")
PY

payload_root="${OUTPUT_DIR}/payload"
cmake -E remove_directory "${payload_root}"
mkdir -p "${payload_root}"
/usr/bin/ditto --noextattr --noqtn "${APP_PATH}" "${payload_root}/Nunchuk.app"

source_commit="$(git -C "${PROJECT_DIR}" rev-parse HEAD)"
{
    printf 'schema=nunchuk-macos-build-inputs-v1\n'
    printf 'arch=%s\n' "${ARCH}"
    printf 'version=%s\n' "${TAG}"
    printf 'source_commit=%s\n' "${source_commit}"
    printf 'source_date_epoch=%s\n' "${SOURCE_DATE_EPOCH}"
    printf 'qt=%s\n' "${QT_VERSION}"
    printf 'aqt=%s\n' "${AQT_VERSION}"
    printf 'qt_modules=%s\n' "${QT_MODULES}"
    printf 'qtkeychain=%s@%s\n' "${QTKEYCHAIN_VERSION}" "${QTKEYCHAIN_COMMIT}"
    printf 'boost=%s@sha256:%s\n' "${BOOST_VERSION}" "${BOOST_SHA256}"
    printf 'libevent=%s@sha256:%s\n' "${LIBEVENT_VERSION}" "${LIBEVENT_SHA256}"
    printf 'olm=%s@%s\n' "${OLM_VERSION}" "${OLM_COMMIT}"
    printf 'openssl=%s@sha256:%s\n' "${OPENSSL_VERSION}" "${OPENSSL_SHA256}"
    printf 'hwi_version=%s\n' "${HWI_VERSION}"
    if [[ "${ARCH}" == 'x86_64' ]]; then
        printf 'hwi_input=official-asset@sha256:%s\n' "${HWI_MACOS_X86_64_SHA256}"
    else
        printf 'hwi_input=source@%s\n' "${HWI_COMMIT}"
        printf 'libusb=%s@sha256:%s\n' "${LIBUSB_VERSION}" "${LIBUSB_SHA256}"
        hwi_build_root="$(cd "$(dirname "${HWI_BINARY}")/.." && pwd)"
        printf 'hwi_poetry_lock_sha256=%s\n' "$(shasum -a 256 "${hwi_build_root}/sources/hwi/poetry.lock" | awk '{ print $1 }')"
    fi
    printf 'hwi_binary_sha256=%s\n' "$(shasum -a 256 "${payload_root}/Nunchuk.app/Contents/MacOS/hwi" | awk '{ print $1 }')"
    printf 'python=%s\n' "${PYTHON_VERSION}"
    printf 'cmake=%s\n' "$(cmake --version | awk 'NR == 1 { print $3 }')"
    printf 'ninja=%s\n' "$(ninja --version)"
    printf 'xcode=%s\n' "$(xcodebuild -version | paste -sd ' ' -)"
    printf 'clang=%s\n' "$(clang --version | sed -n '1p')"
    printf '%s\n' 'submodules:'
    git -C "${PROJECT_DIR}" submodule status --recursive | LC_ALL=C sort
} > "${payload_root}/build-inputs.txt"

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
# the canonical archive contains only source-controlled values. GNU tar format
# keeps long Qt/WebEngine paths deterministic without PAX metadata records.
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
