# Reproducible builds

Verify that your installed binary matches the source code.

## Status

| Platform | Reproducible builds |
| --- | --- |
| Linux x86-64 | Active |
| Linux ARM64 | Active |
| Windows | Standby |
| macOS | Standby |

The instructions below cover both Linux architectures. Windows and macOS
reproducible-build verification is not yet supported.

## 1. Get the source

You need Git, Docker with Buildx, and `diff`.

Find your app version under **Profile → Settings → About**. Set `VERSION` to that
numeric tag, without a leading `v`, and follow the guide from that same tag.

```bash
export VERSION="<release-tag>"   # e.g. 2.9.0
git clone https://github.com/nunchuk-io/nunchuk-desktop
cd nunchuk-desktop
git checkout --detach "$VERSION"
git submodule update --init --recursive
```

## 2. Build with Docker

Choose the architecture of the release you want to verify:

```bash
export PLATFORM=linux/amd64   # x86-64; use linux/arm64 for ARM64

docker buildx build --platform "$PLATFORM" --load \
  -t nunchuk-builder -f reproducible-builds/Dockerfile.linux .

docker run --platform "$PLATFORM" --rm \
  -e TAG="$VERSION" -v "$PWD:/project" nunchuk-builder
```

The builder handles dependencies and downloads the checksum-verified
[HWI 3.2.4 binary](https://github.com/nogibi/HWI/releases/tag/3.2.4) automatically.
OpenSSL comes from the same frozen Ubuntu snapshot as the system packages.

Output: `nunchuk-linux-v$VERSION-x86_64/nunchuk-linux-v$VERSION-x86_64.AppImage`.
For ARM64, the directory and AppImage use `aarch64` instead of `x86_64`.

## 3. Compare with the release

Download the matching `.AppImage` from [GitHub Releases](https://github.com/nunchuk-io/nunchuk-desktop/releases),
then compare it with your build:

```bash
diff "/path/to/download/nunchuk-linux-v$VERSION-x86_64.AppImage" \
  "nunchuk-linux-v$VERSION-x86_64/nunchuk-linux-v$VERSION-x86_64.AppImage"
```

For ARM64, replace `x86_64` with `aarch64` in the comparison paths.
No output means the files are byte-for-byte identical.

Before launching the downloaded AppImage, enable **Allow executing file as
program** in your file manager's permissions.
