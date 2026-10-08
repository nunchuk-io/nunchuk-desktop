#!/usr/bin/env bash
set -euo pipefail

# Codesign/notarize sequence re-derived from the manually-run reference
# workflow (see build_macos.sh's header for the link and rationale). This
# intentionally mirrors that reference's actual, proven behavior rather than
# the previous draft's independently-invented signing design, including two
# points where they differ:
# - The reference signs every dylib/so/bundle/plugin and the main executable
#   with `codesign --deep`, and only signs the outer .app bundle without
#   --deep at the very end. Apple's own guidance discourages --deep, and the
#   previous draft avoided it entirely -- but --deep on individual files is
#   what the reference workflow actually runs today and notarizes
#   successfully with, so it is kept here rather than "corrected" to a
#   design that was never validated.
# - The reference never signs or notarizes the outer DMG at all -- only the
#   .app bundle inside it is signed and notarized (as a zip), stapled, and
#   then packaged into a DMG afterward. The DMG wrapper itself carries no
#   signature or notarization ticket. This script does the same; DMG
#   creation uses `hdiutil` instead of the reference's `appdmg` (npm
#   package) purely to avoid adding a Node dependency -- it does not sign or
#   notarize the DMG either.

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
original_keychain_list=()
original_default_keychain=""

cleanup() {
    if (( mounted == 1 )); then
        hdiutil detach "${MOUNT_POINT}" -force >/dev/null 2>&1 || true
    fi
    rm -f "${CERTIFICATE_PATH}"
    # Restore the process's keychain search list/default before deleting the
    # signing keychain, not after -- otherwise the (now-deleted) signing
    # keychain would linger in the search list until this shell exits.
    if [[ -n "${original_default_keychain}" ]]; then
        security default-keychain -s "${original_default_keychain}" >/dev/null 2>&1 || true
    fi
    if (( ${#original_keychain_list[@]} > 0 )); then
        security list-keychains -d user -s "${original_keychain_list[@]}" >/dev/null 2>&1 || true
    fi
    security delete-keychain "${KEYCHAIN_PATH}" >/dev/null 2>&1 || true
}
trap cleanup EXIT

if [[ ! -d "${APP_PATH}" || ! -x "${MAIN_EXECUTABLE}" || ! -x "${HWI_PATH}" ]]; then
    echo "The gated payload does not contain the complete Nunchuk app." >&2
    exit 1
fi
plutil -lint "${ENTITLEMENTS}" "${ENTITLEMENTS_HWI}"

mkdir -p "${OUTPUT_DIR}"
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

# `security create-keychain` does not add the new keychain to the process's
# keychain SEARCH LIST or make it the default. `security find-identity
# <path>` (the self-check just below) takes an explicit path and so finds
# the identity regardless -- but `codesign -s "<name>"` resolves identities
# by searching the keychain search list / default keychain only, not an
# arbitrary path on disk. Skipping this step is what previously produced
# "error: The specified item could not be found in the keychain" on the very
# first codesign call below, even though the identity had just been
# confirmed present in this exact keychain. The original list/default are
# captured here and restored by the cleanup() trap.
while IFS= read -r existing_keychain; do
    original_keychain_list+=("${existing_keychain}")
done < <(security list-keychains -d user | sed -E 's/^[[:space:]]*"(.*)"[[:space:]]*$/\1/')
original_default_keychain="$(security default-keychain -d user | sed -E 's/^[[:space:]]*"(.*)"[[:space:]]*$/\1/')"
security list-keychains -d user -s "${KEYCHAIN_PATH}" "${original_keychain_list[@]}"
security default-keychain -s "${KEYCHAIN_PATH}"

if ! security find-identity -v -p codesigning "${KEYCHAIN_PATH}" \
    | grep -Fq "${MACOS_CERTIFICATE_NAME}"; then
    echo "Developer ID identity was not imported: ${MACOS_CERTIFICATE_NAME}" >&2
    exit 1
fi
umask 022

codesign_deep() {
    codesign --remove-signature "$1" >/dev/null 2>&1 || true
    codesign --deep --force --verify --options=runtime --verbose --timestamp \
        --entitlements "$2" -s "${MACOS_CERTIFICATE_NAME}" --keychain "${KEYCHAIN_PATH}" "$1"
}

echo "Signing internal components (dylib/so/bundle/plugin)..."
while IFS= read -r -d '' candidate; do
    codesign_deep "${candidate}" "${ENTITLEMENTS}"
done < <(find "${APP_PATH}" -type f \
    \( -name '*.dylib' -o -name '*.so' -o -name '*.bundle' -o -name '*.plugin' \) \
    -print0)

echo "Signing frameworks and Qt components..."
while IFS= read -r -d '' candidate; do
    codesign_deep "${candidate}" "${ENTITLEMENTS}"
done < <(find "${APP_PATH}" -name 'Qt*' -type f -print0)

echo "Signing Nunchuk main executable..."
codesign_deep "${MAIN_EXECUTABLE}" "${ENTITLEMENTS}"

echo "Signing HWI..."
codesign --remove-signature "${HWI_PATH}" >/dev/null 2>&1 || true
codesign --force --options=runtime --timestamp \
    --entitlements "${ENTITLEMENTS_HWI}" -s "${MACOS_CERTIFICATE_NAME}" \
    --keychain "${KEYCHAIN_PATH}" "${HWI_PATH}"
codesign --verify --strict --verbose=4 "${HWI_PATH}"

echo "Signing QtWebEngineProcess helper..."
webengine_helper_app="${APP_PATH}/Contents/Frameworks/QtWebEngineCore.framework/Versions/A/Helpers/QtWebEngineProcess.app"
if [[ -d "${webengine_helper_app}" ]]; then
    webengine_helper_entitlements="${webengine_helper_app}/Contents/Resources/QtWebEngineProcess.entitlements"
    if [[ ! -f "${webengine_helper_entitlements}" ]]; then
        echo "QtWebEngineProcess entitlements are missing from the deployed helper." >&2
        exit 1
    fi
    plutil -lint "${webengine_helper_entitlements}"
    codesign_deep "${webengine_helper_app}" "${webengine_helper_entitlements}"
fi

echo "Signing the app bundle..."
codesign --remove-signature "${APP_PATH}" >/dev/null 2>&1 || true
codesign --force --verify --options=runtime --verbose --timestamp \
    --entitlements "${ENTITLEMENTS}" -s "${MACOS_CERTIFICATE_NAME}" \
    --keychain "${KEYCHAIN_PATH}" "${APP_PATH}"

echo "Verifying signature..."
codesign --verify --deep --strict --verbose=4 "${APP_PATH}"
codesign -dvvvv "${APP_PATH}"

echo "Creating notarization profile..."
xcrun notarytool store-credentials "${NOTARY_PROFILE}" \
    --keychain "${KEYCHAIN_PATH}" \
    --apple-id "${PROD_MACOS_NOTARIZATION_APPLE_ID}" \
    --team-id "${PROD_MACOS_NOTARIZATION_TEAM_ID}" \
    --password "${PROD_MACOS_NOTARIZATION_PWD}"

notarization_zip="${OUTPUT_DIR}/notarization-${ARCH}.zip"
rm -f "${notarization_zip}"
/usr/bin/ditto -c -k --keepParent "${APP_PATH}" "${notarization_zip}"

echo "Submitting app bundle for notarization..."
notary_result="${OUTPUT_DIR}/notary-result.json"
notarized=0
for attempt in 1 2 3; do
    if xcrun notarytool submit "${notarization_zip}" \
        --keychain-profile "${NOTARY_PROFILE}" \
        --keychain "${KEYCHAIN_PATH}" \
        --wait \
        --timeout 30m \
        --output-format json > "${notary_result}"; then
        notarized=1
        break
    fi
    if (( attempt < 3 )); then
        sleep $((attempt * 60))
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

echo "Stapling notarization ticket to the app bundle..."
xcrun stapler staple "${APP_PATH}"
xcrun stapler validate "${APP_PATH}"
codesign --verify --deep --strict --verbose=4 "${APP_PATH}"
spctl --assess --type execute --verbose=4 "${APP_PATH}"

echo "Packaging the notarized app bundle into a DMG..."
dmg_stage="${OUTPUT_DIR}/dmg-stage"
rm -rf "${dmg_stage}"
mkdir -p "${dmg_stage}"
/usr/bin/ditto "${APP_PATH}" "${dmg_stage}/Nunchuk.app"
ln -s /Applications "${dmg_stage}/Applications"

DMG_PATH="${OUTPUT_DIR}/nunchuk-macos-${ARCH}-v${TAG}.dmg"
rm -f "${DMG_PATH}"
# hdiutil create intermittently fails with "Resource busy" -- a known,
# transient macOS disk-arbitration/Spotlight race against the just-written
# dmg_stage directory or the just-removed DMG_PATH, not a real conflict with
# this script's own state (nothing else here holds either path open at this
# point). Retried a few times with a short backoff rather than failing the
# whole signing job on what is reliably a one-shot flake.
dmg_created=0
for attempt in 1 2 3 4 5; do
    if hdiutil create \
        -volname Nunchuk \
        -srcfolder "${dmg_stage}" \
        -fs HFS+ \
        -format UDZO \
        -imagekey zlib-level=9 \
        "${DMG_PATH}"; then
        dmg_created=1
        break
    fi
    echo "hdiutil create failed (attempt ${attempt}/5), retrying..." >&2
    rm -f "${DMG_PATH}"
    if (( attempt < 5 )); then
        sleep $((attempt * 5))
    fi
done
if (( dmg_created != 1 )); then
    echo "hdiutil create failed after five attempts." >&2
    exit 1
fi

echo "Verifying the mounted app inside the DMG..."
rm -rf "${MOUNT_POINT}"
mkdir -p "${MOUNT_POINT}"
# Same transient "Resource busy" risk as the hdiutil create above -- retry
# rather than fail outright.
dmg_attached=0
for attempt in 1 2 3 4 5; do
    if hdiutil attach -readonly -nobrowse -mountpoint "${MOUNT_POINT}" "${DMG_PATH}" >/dev/null; then
        dmg_attached=1
        break
    fi
    echo "hdiutil attach failed (attempt ${attempt}/5), retrying..." >&2
    if (( attempt < 5 )); then
        sleep $((attempt * 5))
    fi
done
if (( dmg_attached != 1 )); then
    echo "hdiutil attach failed after five attempts." >&2
    exit 1
fi
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

printf 'Signed, notarized and stapled: %s (DMG wrapper is unsigned, matching the reference workflow)\n' "${DMG_PATH}"
