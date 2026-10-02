# Reproducible builds

## Linux prerequisites

- Git
- Docker with Buildx
- A host able to run `linux/amd64` and/or `linux/arm64` containers (natively or emulated)
- Access to the recursive source submodules

## Build Linux

Numeric tag, no leading `v` (e.g. `2.9.0`). Set `ARCH` to `x86_64` or `aarch64`.

```bash
export PROJECT_DIR="$HOME/nunchuk-desktop"
export VERSION="2.9.0"
export ARCH="x86_64"   # or: aarch64

case "$ARCH" in
  x86_64)  PLATFORM=linux/amd64; QT_HOST=linux;      QT_ARCH=linux_gcc_64;      QT_DIR_NAME=gcc_64 ;;
  aarch64) PLATFORM=linux/arm64; QT_HOST=linux_arm64; QT_ARCH=linux_gcc_arm64;  QT_DIR_NAME=gcc_arm64 ;;
  *) echo "ARCH must be x86_64 or aarch64" >&2; exit 1 ;;
esac

git clone https://github.com/nunchuk-io/nunchuk-desktop "$PROJECT_DIR"
cd "$PROJECT_DIR"
git checkout --detach "$VERSION"
git submodule update --init --recursive

docker buildx build \
  --platform "$PLATFORM" \
  --load \
  --file reproducible-builds/Dockerfile.linux \
  --build-arg APPIMAGE_ARCH="$ARCH" \
  --build-arg QT_HOST="$QT_HOST" \
  --build-arg QT_ARCH="$QT_ARCH" \
  --build-arg QT_DIR_NAME="$QT_DIR_NAME" \
  --tag "nunchuk-builder-linux-$ARCH:qt6" \
  .
```

### Build HWI (required, both architectures built from source)

```bash
git clone --branch 3.2.0-displayaddress --depth 1 \
  https://github.com/nogibi/HWI.git /tmp/hwi-src
docker buildx build \
  --platform "$PLATFORM" --load \
  --file /tmp/hwi-src/contrib/build.Dockerfile \
  --tag hwi-builder:local \
  /tmp/hwi-src
mkdir -p "$PROJECT_DIR/hwi-prebuilt"
docker run --platform "$PLATFORM" --rm \
  --volume "$PROJECT_DIR/hwi-prebuilt:/out" \
  hwi-builder:local \
  bash -c 'cd "$(dirname "$(dirname "$(find / -xdev -maxdepth 8 -path "*/contrib/build_bin.sh" -print -quit)")")" \
    && bash contrib/build_bin.sh --without-gui \
    && install -m 0755 "$(find dist -type f -name hwi -print -quit)" /out/hwi'
```

### Build app

`SOURCE_DATE_EPOCH` is derived from the commit automatically; no need to set it manually.

```bash
docker run --platform "$PLATFORM" --rm \
  --env TAG="$VERSION" \
  --env ARCH="$ARCH" \
  --volume "$PROJECT_DIR:/project" \
  --workdir /project \
  "nunchuk-builder-linux-$ARCH:qt6" \
  bash reproducible-builds/build_linux.sh
```

Output:

```text
nunchuk-linux-$ARCH-v<VERSION>/nunchuk-linux-$ARCH-v<VERSION>.zip
nunchuk-linux-$ARCH-v<VERSION>/nunchuk-linux-$ARCH-v<VERSION>.zip.sha256
```

Verify checksum:

```bash
cd "$PROJECT_DIR/nunchuk-linux-$ARCH-v$VERSION"
sha256sum --check "nunchuk-linux-$ARCH-v$VERSION.zip.sha256"
```

## macOS / Windows

See `reproducible-builds/README-macos.md` and `reproducible-builds/README-windows.md`.
