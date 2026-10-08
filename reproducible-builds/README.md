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
[HWI 3.2.1 binary](https://github.com/nogibi/HWI/releases/tag/3.2.1) automatically.

Output: `nunchuk-linux-x86_64-v$VERSION/nunchuk-linux-x86_64-v$VERSION.zip`.
For ARM64, the directory and ZIP use `aarch64` instead of `x86_64`.

## 3. Compare with the release

Download the matching Linux ZIP from [GitHub Releases](https://github.com/nunchuk-io/nunchuk-desktop/releases),
then compare it with your build:

```bash
diff "/path/to/download/nunchuk-linux-x86_64-v$VERSION.zip" \
  "nunchuk-linux-x86_64-v$VERSION/nunchuk-linux-x86_64-v$VERSION.zip"
```

For ARM64, replace `x86_64` with `aarch64` in the comparison paths.
No output means the files are byte-for-byte identical.
