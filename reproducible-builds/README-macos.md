# Build macOS

## Prerequisites

- macOS, native architecture (Intel or Apple Silicon) -- no cross-compiling
- Xcode 16.4
- Homebrew
- Git

## Build

Tag is numeric, no leading `v` (e.g. `2.9.0`). `ARCH` is taken from the host.

```bash
export PROJECT_DIR="$HOME/nunchuk-desktop"
export VERSION="2.9.0"
export ARCH="$(uname -m)"   # x86_64 or arm64

git clone https://github.com/nunchuk-io/nunchuk-desktop "$PROJECT_DIR"
cd "$PROJECT_DIR"
git checkout --detach "$VERSION"
git submodule update --init --recursive

sudo xcode-select --switch /Applications/Xcode_16.4.app
```

### Install Qt 6.9.3

```bash
pip install aqtinstall --break-system-packages
aqt install-qt mac desktop 6.9.3 clang_64 \
  -m qtmultimedia qtnetworkauth qtwebengine qtwebchannel qtpositioning qt5compat qtshadertools \
  -O "$HOME/Qt"
export QT_ROOT="$HOME/Qt/6.9.3/macos"
```

### Build app

`SOURCE_DATE_EPOCH` is derived from the commit automatically. `build_macos.sh`
installs its own pyenv/Python internally to build HWI -- nothing else to install.

```bash
ARCH="$ARCH" TAG="$VERSION" QT_ROOT="$QT_ROOT" \
  bash reproducible-builds/build_macos.sh
```

Output:

```text
/private/tmp/nunchuk-macos-reproducible/$ARCH/output/nunchuk-macos-$ARCH-v$VERSION-unsigned.tar
/private/tmp/nunchuk-macos-reproducible/$ARCH/output/nunchuk-macos-$ARCH-v$VERSION-unsigned.tar.sha256
```

Verify checksum:

```bash
cd /private/tmp/nunchuk-macos-reproducible/$ARCH/output
shasum -a 256 --check "nunchuk-macos-$ARCH-v$VERSION-unsigned.tar.sha256"
```
