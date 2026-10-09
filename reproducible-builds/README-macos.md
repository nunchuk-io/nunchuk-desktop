# macOS release-build notes

**Reproducible-build status: standby.** These are developer build instructions;
independent reproducible-build verification is not yet supported.

Requires a native Intel or Apple Silicon Mac, Xcode 16.4, CMake, Ninja, Git and
Python 3. Use the instructions at the release tag, with a numeric `VERSION`
(without `v`).

## Build

```bash
export PROJECT_DIR="$HOME/nunchuk-desktop"
export VERSION="<release-tag>"
export ARCH="$(uname -m)"   # x86_64 or arm64

git clone https://github.com/nunchuk-io/nunchuk-desktop "$PROJECT_DIR"
cd "$PROJECT_DIR"
git checkout --detach "$VERSION"
git submodule update --init --recursive

sudo xcode-select --switch /Applications/Xcode_16.4.app

python3 -m pip install aqtinstall==3.3.0 --break-system-packages
aqt install-qt mac desktop 6.9.3 clang_64 \
  -m qtmultimedia qtnetworkauth qtwebengine qtwebchannel qtpositioning qt5compat qtshadertools \
  -O "$HOME/Qt"
export QT_ROOT="$HOME/Qt/6.9.3/macos"

ARCH="$ARCH" TAG="$VERSION" QT_ROOT="$QT_ROOT" \
  bash reproducible-builds/build_macos.sh
```

HWI 3.2.2 is downloaded and checksum-verified automatically. No separate HWI
build is needed. The source commit supplies `SOURCE_DATE_EPOCH`.

Packaging keeps only the selected architecture in bundled executables, frameworks
and plugins before creating the unsigned payload and signing the app.

## Build output

The unsigned payload and checksum are written to:

```text
/private/tmp/nunchuk-macos-reproducible/$ARCH/output/nunchuk-macos-v$VERSION-$ARCH-unsigned.tar
/private/tmp/nunchuk-macos-reproducible/$ARCH/output/nunchuk-macos-v$VERSION-$ARCH-unsigned.tar.sha256
```

CI records the unsigned payload hash in the release manifest for diagnostics.
The signed app includes timestamps and a notarization ticket, so the distributed
DMG is not a byte-for-byte comparison target.

CI signs and notarizes the app before packaging it in a DMG.
