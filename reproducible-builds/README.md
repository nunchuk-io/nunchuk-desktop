# Linux reproducible build

The Linux x86_64 AppImage is built in a pinned Ubuntu 24.04 container. The
recipe fixes the base image, Ubuntu package snapshot, dependency revisions,
download checksums, locale, build paths and file timestamps. CI builds the
AppImage twice and requires both files to be byte-for-byte identical.

## Requirements

- Git
- Docker 24+
- An x86_64 Linux Docker host (or an x86_64 runner)

## Build an exact release

```bash
export VERSION=2.6.5
export PROJECT_DIR="$HOME/nunchuk-desktop"

git clone https://github.com/nunchuk-io/nunchuk-desktop "$PROJECT_DIR"
cd "$PROJECT_DIR"
git checkout --detach "$VERSION"
git submodule update --init --recursive

export TAG="$VERSION"
export SOURCE_COMMIT="$(git rev-parse HEAD)"
export SOURCE_DATE_EPOCH="$(git log -1 --pretty=%ct)"

docker build --no-cache \
  --file reproducible-builds/Dockerfile.linux \
  --tag nunchuk-linux-reproducible:local \
  .

docker run --rm \
  -e SOURCE_COMMIT \
  -e SOURCE_DATE_EPOCH \
  -e TAG \
  -e RUN_SMOKE_TEST=1 \
  -v "$PROJECT_DIR:/project" \
  nunchuk-linux-reproducible:local
```

Outputs are written to `dist/`:

- `nunchuk-linux-v<VERSION>.AppImage`
- `nunchuk-linux-v<VERSION>.AppImage.sha256`
- `AppDir.sha256` and `AppDir.symlinks`
- `build-info.txt`

`build-info.txt` records the release-specific inputs. OAuth configuration is
deliberately excluded from the compiled artifact, so a verifier does not need
access to any Google OAuth value.

## Verify reproducibility

Keep the first AppImage outside `dist/`, run the same container command again,
then compare the files:

```bash
cp "dist/nunchuk-linux-v$TAG.AppImage" /tmp/nunchuk-first.AppImage

docker run --rm \
  -e SOURCE_COMMIT \
  -e SOURCE_DATE_EPOCH \
  -e TAG \
  -e RUN_SMOKE_TEST=0 \
  -v "$PROJECT_DIR:/project" \
  nunchuk-linux-reproducible:local

cmp /tmp/nunchuk-first.AppImage "dist/nunchuk-linux-v$TAG.AppImage"
sha256sum /tmp/nunchuk-first.AppImage "dist/nunchuk-linux-v$TAG.AppImage"
```

No output from `cmp` means both AppImages are identical.

## OAuth and reproducibility

The application source is not changed by this recipe. Its three OAuth macros
fall back to runtime environment variables when the compile-time values are
empty. The reproducible build therefore forces these compile-time inputs to an
empty string:

- `OAUTH_CLIENT_ID`
- `OAUTH_CLIENT_SECRET`
- `OAUTH_REDIRECT_URI`

This makes the AppImage independent of repository secrets and of the builder's
local environment. Missing or different OAuth values on a verifier's machine
cannot change the output hash.

To exercise the existing sign-in implementation, provide all three values only
when launching the already-built AppImage:

```bash
OAUTH_CLIENT_ID='...' \
OAUTH_CLIENT_SECRET='...' \
OAUTH_REDIRECT_URI='...' \
./dist/nunchuk-linux-v"$TAG".AppImage
```

Those runtime values do not modify the AppImage. The current Google flow still
uses the application's embedded Qt WebEngine implementation; this build recipe
only ensures that its required WebEngine process and resources are packaged.
Any Google policy or OAuth-flow change requires a separate source-code change.

## TLS compatibility note

The Qt 5.15.2 binary distribution uses the OpenSSL 1.1 ABI, so this recipe pins
the final 1.1.1w release instead of the older 1.1.1g from the legacy workflow.
OpenSSL 1.1.1 is end-of-life; moving the application to a supported Qt/OpenSSL
combination remains a separate migration task.
