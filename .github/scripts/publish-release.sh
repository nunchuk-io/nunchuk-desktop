#!/usr/bin/env bash
set -euo pipefail

: "${BUILD_VERSION:?BUILD_VERSION is required}"
if [[ "$#" -eq 0 ]]; then
    echo "No release assets were downloaded" >&2
    exit 1
fi

options=(
    --title "${BUILD_VERSION}"
    --notes-file reproducible-builds/release-notes.md
    --prerelease
)

if gh release view "${BUILD_VERSION}" >/dev/null 2>&1; then
    gh release edit "${BUILD_VERSION}" "${options[@]}" --draft=false
    gh release upload "${BUILD_VERSION}" "$@" --clobber
else
    gh release create "${BUILD_VERSION}" "$@" "${options[@]}" \
        --verify-tag --latest=false
fi
