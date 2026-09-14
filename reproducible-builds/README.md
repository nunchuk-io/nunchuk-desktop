# Reproducible builds

This directory contains the reference build environment used to verify that a
release artifact was produced from the published source.

## Current status

| Platform | Status | Reference artifact |
| --- | --- | --- |
| Linux x86_64/arm64 | Qt 6 reference build (opt-in reproducibility check) | `nunchuk-linux-<ARCH>-v<VERSION>.zip` |
| macOS x86_64/arm64 | Single build per arch, Developer ID signed and notarized (opt-in reproducibility check, not gated) | `nunchuk-macos-<ARCH>-v<VERSION>.dmg` |
| Windows x64 | Single build, unsigned (opt-in reproducibility check, not gated); Authenticode signing not yet wired into CI | `nunchuk-windows-x64-v<VERSION>-unsigned-setup.exe` |

The Linux builder uses Qt 6.9.3 and OpenSSL 3.5.7 LTS (matching macOS and
Windows's pinned Qt version -- see README-windows.md). It pins
Qt module set,
source dependency commits, linuxdeploy/linuxdeploy-plugin-qt/linuxdeploy-plugin-appimage
bytes and the AppImage runtime, per architecture. Like main's Linux builder, it
does not pin the Ubuntu base image digest or individual apt package versions —
an accepted, documented gap (see "Reproducibility boundary" below), not
something this port tightens beyond main's own baseline. `SOURCE_DATE_EPOCH` is
derived from the source commit.

macOS and Windows now share the same Qt version (6.9.3) as Linux, though not
the same OpenSSL sourcing: Windows pins Qt 6.9.3 and OpenSSL 3.5.7 LTS
(`windows-dependencies.lock.json`), while macOS was re-derived from a
separately proven reference workflow and pins Qt 6.9.3, with OpenSSL sourced
from the `contrib/libnunchuk` submodule instead of an independent download
(see `reproducible-builds/macos.lock.env`). Both
still build once per architecture by default and only reproducibility-check
on an opt-in basis (see "Reproducibility boundary" below) -- an earlier
version of both workflows instead built two clean unsigned replicas
unconditionally and gated everything downstream on them comparing
byte-for-byte; that design was replaced with the single-build-plus-opt-in-
checkbox model to match `build-linux.yml`'s own UX. Service-issued signing
timestamps and Apple notarization tickets mean a signed macOS DMG is not
expected to be byte-identical across separate signing runs of the same
unsigned input; published release manifests bind each signed artifact to the
digest of the unsigned payload it was signed from.

As of this writing, `build-windows.yml` builds once per tag/manual run and
publishes the unsigned Windows payload only; it does not yet call
`sign_windows.ps1` or the `release-signing` environment. Windows Authenticode
signing wiring remains a follow-up.

`build-macos.yml` builds once per architecture and calls `sign_macos.sh` in
the `release-signing` environment for both x86_64 and arm64 immediately
afterward, using the secrets listed in `reproducible-builds/README-macos.md`.
Neither workflow has yet been exercised on a real GitHub Actions run against
`nunchuk-io/nunchuk-desktop`, so the toolchain/runner-image assumptions
documented in each workflow's own header comment, and whether the macOS
signing secrets already exist on this repository, both need confirmation on
the first tag build or manual `workflow_dispatch` run.

## Linux prerequisites

- Git
- Docker with Buildx
- A host able to run `linux/amd64` and/or `linux/arm64` containers (natively
  or emulated)
- Access to the recursive source submodules

## Build an exact Linux version

The release tag is expected to use the numeric form `2.6.7`, without a leading
`v`. Set `ARCH` to `x86_64` or `aarch64` for the architecture you want; the
other variables below follow from it (see `build-linux.yml`'s matrix for the
same mapping).

```bash
export PROJECT_DIR="$HOME/nunchuk-desktop"
export VERSION="2.6.7"
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

HWI has no prebuilt binary for arm64 upstream, so both architectures are built
from source and bind-mounted in, matching the "Build HWI from source" step in
`build-linux.yml`:

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

Then build the application. `SOURCE_DATE_EPOCH` is derived from the checked-out
commit by `build_linux.sh` itself; do not override it unless you are
deliberately reproducing a build made against a different `SOURCE_DATE_EPOCH`.

```bash
docker run --platform "$PLATFORM" --rm \
  --env TAG="$VERSION" \
  --env ARCH="$ARCH" \
  --volume "$PROJECT_DIR:/project" \
  --workdir /project \
  "nunchuk-builder-linux-$ARCH:qt6" \
  bash reproducible-builds/build_linux.sh
```

The build produces:

```text
nunchuk-linux-$ARCH-v<VERSION>/nunchuk-linux-$ARCH-v<VERSION>.zip
nunchuk-linux-$ARCH-v<VERSION>/nunchuk-linux-$ARCH-v<VERSION>.zip.sha256
```

Verify the checksum generated by the build:

```bash
cd "$PROJECT_DIR/nunchuk-linux-$ARCH-v$VERSION"
sha256sum --check "nunchuk-linux-$ARCH-v$VERSION.zip.sha256"
```

## Compare the Linux release

Download the release ZIP and its `.sha256` file from GitHub for your
architecture. First validate the downloaded checksum, then compare the
official and locally produced files:

```bash
sha256sum --check "nunchuk-linux-$ARCH-v$VERSION.zip.sha256"
cmp \
  "nunchuk-linux-$ARCH-v$VERSION.zip" \
  "$PROJECT_DIR/nunchuk-linux-$ARCH-v$VERSION/nunchuk-linux-$ARCH-v$VERSION.zip"
```

No output from `cmp` means that the two ZIP files are byte-for-byte identical.
For a mismatch, retain both ZIP files and inspect them with `diffoscope`.

## Signed macOS and Windows releases

The macOS workflow builds native Intel and Apple Silicon payloads, one build
per architecture. It signs nested code from the inside out, signs the outer
DMG, submits that DMG to Apple, staples the accepted ticket and runs Gatekeeper
checks. See `reproducible-builds/README-macos.md` for the exact input lock,
entitlements and verification boundary.

The Windows workflow builds with native MSVC 2022 because Qt WebEngine is not
supported by the MinGW build used by the old Qt 5 container. It packages the
canonical unsigned ZIP and Inno Setup installer, and once
`sign_windows.ps1`/`release-signing` are wired in (still a follow-up, see
above) is intended to sign the selected application payload with Azure
Artifact Signing, rebuild the installer from that signed payload, then
Authenticode-sign and verify the installer, recording the unsigned payload
digest, signer and signed artifact hashes in the published release manifest.
See `reproducible-builds/README-windows.md` for the dependency lock and
signing configuration.

Only tag builds publish GitHub release assets. All three publish jobs share one
queued concurrency group and never replace an existing asset with different
bytes. GitHub artifact attestations are created only after the immutable asset
gate accepts the release files.

## Reproducibility boundary

For Linux, the release workflow builds each architecture once per tag and does
not itself rebuild and diff a second copy before publishing; a tag build is
never gated on a byte-for-byte comparison. Reproducibility for a published
Linux release is instead something anyone can check themselves after the fact,
per "Compare the Linux release" above: build from the same tag, download the
official release, and `cmp` the two ZIPs. This mirrors main's Linux workflow,
which uses the same single-build-then-manually-verify model (see its own
`reproducible-builds/README.md`).

Separately, a manual (`workflow_dispatch`) build can opt into an in-CI version
of that same check by ticking the "check_reproducible_build" checkbox on the
"Run workflow" form. When enabled, the workflow rebuilds the exact same commit
from scratch a second time in the same job and reports PASSED/NOT PASSED for
each architecture, with a file-level diff of the two AppDirs when it does not
match. This exists to validate changes to the build/packaging pipeline itself
(e.g. this arm64 port); it never runs for a tag push, and never gates
publishing a release.

`build-windows.yml` and `build-macos.yml` now follow this exact same
single-build-plus-opt-in-checkbox model (each has its own
"check_reproducible_build" input, diffing the unsigned ZIP/installer or the
unsigned macOS payload tar respectively). This replaced an earlier design
where both workflows always built two replicas and gated everything
downstream -- for Windows, publishing the unsigned payload; for macOS, calling
`sign_macos.sh` -- on those replicas matching byte-for-byte. Neither workflow
gates on that comparison automatically any more; see
`reproducible-builds/README-macos.md` "Reproducibility model" for the
trade-off this accepts specifically for macOS signing.

Packaging uses a two-pass Qt 6 deployment. The first pass resolves the
application and QML imports; the second pass closes the dependency graph for
every ELF added by the Qt plugin. Before publication, CI extracts the final
AppImage and checks its desktop entry, custom launcher, Qt plugin allowlist,
Qt5Compat/ShaderTools pairing, WebEngine resources, pinned OpenSSL runtime, HWI,
XCB platform backend, minizip, and unresolved ELF dependencies.

The build still obtains Ubuntu packages, Qt archives, and the linuxdeploy /
linuxdeploy-plugin-qt / linuxdeploy-plugin-appimage assets from upstream
repositories while creating the builder image, separately per architecture.
Downloaded deployment tools are rejected unless their SHA-256 hashes match the
recorded commits. `linuxdeploy`, `linuxdeploy-plugin-qt` and
`linuxdeploy-plugin-appimage` are pinned to numbered release tags (not the
upstream `continuous` tag, which upstream's own CI rewrites on every push and
is therefore unsuitable for a stable pin), but those numbered release assets
are still ordinary GitHub release uploads, not archival mirrors. Long-term
verification therefore depends on those exact inputs remaining available. A
fully hermetic archive would require mirroring every Debian/Qt package and
deployment-tool binary used by the image.

HWI is built from source for both architectures (upstream only publishes a
prebuilt x86_64 binary), from a Dockerfile in the `nogibi/HWI` fork that is not
pinned or hash-checked by this repository the way the rest of the toolchain
is. Unlike the rest of this pipeline, its build environment is not frozen from
here, so it is the least-verified input in the chain — the
`check_reproducible_build` option above is the way to empirically confirm it
is not introducing non-determinism, rather than a pin proving it in advance.

The macOS/Windows lock files pin dependency versions, commits and downloaded
archive hashes, and both workflows record their native toolchain fingerprint.
GitHub-hosted runner images, Qt/PyPI distribution services and Windows SDK
components are nevertheless external inputs rather than an archived hermetic
toolchain. Determinism for the selected CI inputs is something each opt-in
`check_reproducible_build` run demonstrates for that specific commit, not
something proven automatically on every run; long-term independent
rebuilding additionally requires mirroring those inputs or using versioned
self-hosted runner images.
