#!/usr/bin/env bash
set -euo pipefail

if [[ "$#" -ne 3 ]]; then
    echo "Usage: $0 <linux|mac> <x86_64|aarch64|arm64> <destination>" >&2
    exit 1
fi

# shellcheck source=hwi.lock.env
source "$(dirname "${BASH_SOURCE[0]}")/hwi.lock.env"
platform="$1"
arch="$2"
destination="$3"
[[ "${arch}" != arm64 ]] || arch=aarch64
case "${platform}-${arch}" in
    linux-x86_64) sha256="${HWI_LINUX_X86_64_SHA256}" ;;
    linux-aarch64) sha256="${HWI_LINUX_AARCH64_SHA256}" ;;
    mac-x86_64) sha256="${HWI_MAC_X86_64_SHA256}" ;;
    mac-aarch64) sha256="${HWI_MAC_AARCH64_SHA256}" ;;
    *) echo "Unsupported HWI platform/architecture: ${platform}-${arch}" >&2; exit 1 ;;
esac

work_dir="$(mktemp -d)"
trap 'rm -rf -- "${work_dir}"' EXIT
asset="hwi-${HWI_VERSION}-${platform}-${arch}.tar.gz"
curl --fail --location --retry 3 --show-error \
    --output "${work_dir}/${asset}" \
    "https://github.com/nogibi/HWI/releases/download/${HWI_VERSION}/${asset}"
(
    cd "${work_dir}"
    if command -v sha256sum >/dev/null 2>&1; then
        printf '%s  %s\n' "${sha256}" "${asset}" | sha256sum --check --strict
    else
        printf '%s  %s\n' "${sha256}" "${asset}" | shasum -a 256 --check
    fi
    tar -xzf "${asset}" hwi
)
mkdir -p "$(dirname "${destination}")"
install -m 0755 "${work_dir}/hwi" "${destination}"
