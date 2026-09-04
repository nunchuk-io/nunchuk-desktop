#!/usr/bin/env bash
set -euo pipefail

required_variables=(
    PROJECT_DIR
    PAYLOAD_DIR
    OUTPUT_DIR
    ARCH
    TAG
    SOURCE_COMMIT
    SOURCE_DATE_EPOCH
    UNSIGNED_PAYLOAD_SHA256
    MACOS_CERTIFICATE
    MACOS_CERTIFICATE_PWD
    MACOS_CERTIFICATE_NAME
    MACOS_CI_KEYCHAIN_PWD
    PROD_MACOS_NOTARIZATION_APPLE_ID
    PROD_MACOS_NOTARIZATION_TEAM_ID
    PROD_MACOS_NOTARIZATION_PWD
)
for variable_name in "${required_variables[@]}"; do
    if [[ -z "${!variable_name:-}" ]]; then
        echo "Missing required environment variable: ${variable_name}" >&2
        exit 1
    fi
done

case "${ARCH}" in
    x86_64|arm64) ;;
    *) echo "Unsupported macOS architecture: ${ARCH}" >&2; exit 1 ;;
esac
if [[ ! "${TAG}" =~ ^[0-9]+\.[0-9]+\.[0-9]+([.-][0-9A-Za-z]+)*$ ]]; then
    echo "Invalid release version: ${TAG}" >&2
    exit 1
fi
if [[ ! "${SOURCE_COMMIT}" =~ ^[0-9a-f]{40}$ ]]; then
    echo "Invalid source commit: ${SOURCE_COMMIT}" >&2
    exit 1
fi
if [[ ! "${SOURCE_DATE_EPOCH}" =~ ^[0-9]+$ ]]; then
    echo "Invalid source epoch: ${SOURCE_DATE_EPOCH}" >&2
    exit 1
fi
if [[ ! "${UNSIGNED_PAYLOAD_SHA256}" =~ ^[0-9a-f]{64}$ ]]; then
    echo "Invalid unsigned payload digest." >&2
    exit 1
fi
if [[ "${PAYLOAD_DIR}" != /* || "${OUTPUT_DIR}" != /* \
    || "${PAYLOAD_DIR}" == / || "${OUTPUT_DIR}" == / \
    || "${OUTPUT_DIR}" == "${PROJECT_DIR}" ]]; then
    echo "Refusing unsafe payload/output directory." >&2
    exit 1
fi

APP_PATH="${PAYLOAD_DIR}/Nunchuk.app"
ENTITLEMENTS="${PROJECT_DIR}/entitlements.plist"
ENTITLEMENTS_HWI="${PROJECT_DIR}/entitlements_hwi.plist"
HWI_PATH="${APP_PATH}/Contents/MacOS/hwi"
MAIN_EXECUTABLE="${APP_PATH}/Contents/MacOS/Nunchuk"
KEYCHAIN_PATH="${RUNNER_TEMP:-${OUTPUT_DIR}}/nunchuk-signing-${ARCH}.keychain-db"
CERTIFICATE_PATH="${RUNNER_TEMP:-${OUTPUT_DIR}}/nunchuk-signing-${ARCH}.p12"
NOTARY_PROFILE="nunchuk-notary-${ARCH}"
MOUNT_POINT="${OUTPUT_DIR}/mounted-dmg"
mounted=0

cleanup() {
    if (( mounted == 1 )); then
        hdiutil detach "${MOUNT_POINT}" -force >/dev/null 2>&1 || true
    fi
    rm -f "${CERTIFICATE_PATH}"
    security delete-keychain "${KEYCHAIN_PATH}" >/dev/null 2>&1 || true
}
trap cleanup EXIT

if [[ ! -d "${APP_PATH}" || ! -x "${MAIN_EXECUTABLE}" || ! -x "${HWI_PATH}" ]]; then
    echo "The gated payload does not contain the complete Nunchuk app." >&2
    exit 1
fi
plutil -lint "${ENTITLEMENTS}" "${ENTITLEMENTS_HWI}"
if codesign --display "${APP_PATH}" >/dev/null 2>&1; then
    echo "The signing job received an already-signed app instead of the gated unsigned payload." >&2
    exit 1
fi

mkdir -p "${OUTPUT_DIR}"
# The decoded Developer ID certificate is a short-lived secret on disk. Keep
# it readable only by the runner account, independently of the host umask.
umask 077
printf '%s' "${MACOS_CERTIFICATE}" | base64 --decode > "${CERTIFICATE_PATH}"
chmod 0600 "${CERTIFICATE_PATH}"
security create-keychain -p "${MACOS_CI_KEYCHAIN_PWD}" "${KEYCHAIN_PATH}"
security set-keychain-settings -lut 21600 "${KEYCHAIN_PATH}"
security unlock-keychain -p "${MACOS_CI_KEYCHAIN_PWD}" "${KEYCHAIN_PATH}"
security import "${CERTIFICATE_PATH}" \
    -k "${KEYCHAIN_PATH}" \
    -P "${MACOS_CERTIFICATE_PWD}" \
    -T /usr/bin/codesign \
    -T /usr/bin/security
security set-key-partition-list \
    -S apple-tool:,apple:,codesign: \
    -s \
    -k "${MACOS_CI_KEYCHAIN_PWD}" \
    "${KEYCHAIN_PATH}"
if ! security find-identity -v -p codesigning "${KEYCHAIN_PATH}" \
    | grep -Fq "${MACOS_CERTIFICATE_NAME}"; then
    echo "Developer ID identity was not imported: ${MACOS_CERTIFICATE_NAME}" >&2
    exit 1
fi
umask 022

codesign_item() {
    local target="$1"
    local entitlements_file="${2:-}"
    local arguments=(
        --force
        --options runtime
        --timestamp
        --keychain "${KEYCHAIN_PATH}"
        --sign "${MACOS_CERTIFICATE_NAME}"
    )
    if [[ -n "${entitlements_file}" ]]; then
        arguments+=(--entitlements "${entitlements_file}")
    fi
    codesign "${arguments[@]}" "${target}"
}

codesign_disk_image() {
    codesign \
        --force \
        --timestamp \
        --keychain "${KEYCHAIN_PATH}" \
        --sign "${MACOS_CERTIFICATE_NAME}" \
        "$1"
}

is_macho() {
    /usr/bin/file -b "$1" | grep -q 'Mach-O'
}

webengine_helper_executable="$(find "${APP_PATH}" \
    -path '*/QtWebEngineProcess.app/Contents/MacOS/QtWebEngineProcess' \
    -type f -print -quit)"
webengine_entitlements=''
if [[ -n "${webengine_helper_executable}" ]]; then
    webengine_helper_app="${webengine_helper_executable%/Contents/MacOS/QtWebEngineProcess}"
    bundled_webengine_entitlements="${webengine_helper_app}/Contents/Resources/QtWebEngineProcess.entitlements"
    if [[ ! -f "${bundled_webengine_entitlements}" ]]; then
        echo "QtWebEngineProcess entitlements are missing from the deployed helper." >&2
        exit 1
    fi
    plutil -lint "${bundled_webengine_entitlements}"
    webengine_entitlements="${bundled_webengine_entitlements}"
fi

# Sign every raw Mach-O first. Special executables are signed immediately below
# with their own entitlements. No --deep is used for signing.
while IFS= read -r candidate; do
    if [[ "${candidate}" == "${MAIN_EXECUTABLE}" \
        || "${candidate}" == "${HWI_PATH}" \
        || "${candidate}" == "${webengine_helper_executable}" ]]; then
        continue
    fi
    if is_macho "${candidate}"; then
        codesign_item "${candidate}"
    fi
done < <(find "${APP_PATH}" -type f -print \
    | awk '{ print length($0) "\t" $0 }' \
    | LC_ALL=C sort -rn \
    | cut -f2-)

codesign_item "${HWI_PATH}" "${ENTITLEMENTS_HWI}"
if [[ -n "${webengine_helper_executable}" ]]; then
    codesign_item "${webengine_helper_executable}" "${webengine_entitlements}"
fi

# Sign nested code containers from deepest to shallowest. The WebEngine helper
# app keeps the entitlements shipped with this exact Qt archive.
while IFS= read -r code_container; do
    if [[ "${code_container}" == "${APP_PATH}" ]]; then
        continue
    fi
    if [[ -n "${webengine_helper_executable}" \
        && "${code_container}" == "${webengine_helper_app}" ]]; then
        codesign_item "${code_container}" "${webengine_entitlements}"
    else
        codesign_item "${code_container}"
    fi
done < <(find "${APP_PATH}" -type d \
    \( -name '*.app' -o -name '*.framework' -o -name '*.bundle' -o -name '*.plugin' -o -name '*.xpc' \) \
    -print \
    | awk '{ print length($0) "\t" $0 }' \
    | LC_ALL=C sort -rn \
    | cut -f2-)

codesign_item "${MAIN_EXECUTABLE}" "${ENTITLEMENTS}"
codesign_item "${APP_PATH}" "${ENTITLEMENTS}"

codesign --verify --deep --strict --verbose=4 "${APP_PATH}"
codesign --verify --strict --verbose=4 "${HWI_PATH}"

dmg_stage="${OUTPUT_DIR}/dmg-stage"
rm -rf "${dmg_stage}"
mkdir -p "${dmg_stage}"
/usr/bin/ditto "${APP_PATH}" "${dmg_stage}/Nunchuk.app"
ln -s /Applications "${dmg_stage}/Applications"

DMG_PATH="${OUTPUT_DIR}/nunchuk-macos-${ARCH}-v${TAG}.dmg"
rm -f "${DMG_PATH}"
hdiutil create \
    -volname Nunchuk \
    -srcfolder "${dmg_stage}" \
    -fs HFS+ \
    -format UDZO \
    -imagekey zlib-level=9 \
    "${DMG_PATH}"
codesign_disk_image "${DMG_PATH}"
codesign --verify --strict --verbose=4 "${DMG_PATH}"

xcrun notarytool store-credentials "${NOTARY_PROFILE}" \
    --keychain "${KEYCHAIN_PATH}" \
    --apple-id "${PROD_MACOS_NOTARIZATION_APPLE_ID}" \
    --team-id "${PROD_MACOS_NOTARIZATION_TEAM_ID}" \
    --password "${PROD_MACOS_NOTARIZATION_PWD}"

notary_result="${OUTPUT_DIR}/notary-result.json"
notarized=0
for attempt in 1 2 3; do
    if xcrun notarytool submit "${DMG_PATH}" \
        --keychain-profile "${NOTARY_PROFILE}" \
        --keychain "${KEYCHAIN_PATH}" \
        --wait \
        --timeout 30m \
        --output-format json > "${notary_result}"; then
        notarized=1
        break
    fi
    if (( attempt < 3 )); then
        sleep $((attempt * 30))
    fi
done
if (( notarized != 1 )); then
    echo "Apple notarization failed after three attempts." >&2
    exit 1
fi

python3 - "${notary_result}" <<'PY'
import json
import sys

with open(sys.argv[1], "r", encoding="utf-8") as source:
    result = json.load(source)
if result.get("status") != "Accepted":
    raise SystemExit(f"Notarization was not accepted: {result}")
PY

xcrun stapler staple "${DMG_PATH}"
xcrun stapler validate "${DMG_PATH}"
codesign --verify --strict --verbose=4 "${DMG_PATH}"
spctl --assess --type open --context context:primary-signature --verbose=4 "${DMG_PATH}"

rm -rf "${MOUNT_POINT}"
mkdir -p "${MOUNT_POINT}"
hdiutil attach -readonly -nobrowse -mountpoint "${MOUNT_POINT}" "${DMG_PATH}" >/dev/null
mounted=1
codesign --verify --deep --strict --verbose=4 "${MOUNT_POINT}/Nunchuk.app"
spctl --assess --type execute --verbose=4 "${MOUNT_POINT}/Nunchuk.app"
hdiutil detach "${MOUNT_POINT}" >/dev/null
mounted=0

dmg_sha256="$(shasum -a 256 "${DMG_PATH}" | awk '{ print $1 }')"
printf '%s  %s\n' "${dmg_sha256}" "$(basename "${DMG_PATH}")" > "${DMG_PATH}.sha256"

RELEASE_MANIFEST="${OUTPUT_DIR}/nunchuk-macos-${ARCH}-v${TAG}.release.json"
python3 - "${RELEASE_MANIFEST}" \
    "${ARCH}" "${TAG}" "${SOURCE_COMMIT}" "${SOURCE_DATE_EPOCH}" \
    "${UNSIGNED_PAYLOAD_SHA256}" "${dmg_sha256}" \
    "${MACOS_CERTIFICATE_NAME}" "$(basename "${DMG_PATH}")" <<'PY'
import json
import sys

(
    output,
    arch,
    version,
    commit,
    epoch,
    unsigned_sha256,
    signed_sha256,
    identity,
    filename,
) = sys.argv[1:]
manifest = {
    "arch": arch,
    "format": "nunchuk-macos-signed-release-v1",
    "notarization": "Accepted",
    "signed_dmg": {"filename": filename, "sha256": signed_sha256},
    "signing_identity": identity,
    "source_commit": commit,
    "source_date_epoch": int(epoch),
    "unsigned_payload_sha256": unsigned_sha256,
    "version": version,
}
with open(output, "w", encoding="utf-8", newline="\n") as destination:
    json.dump(manifest, destination, ensure_ascii=True, indent=2, sort_keys=True)
    destination.write("\n")
PY

printf 'Signed, notarized and stapled DMG: %s\n' "${DMG_PATH}"
