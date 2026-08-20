# Reproducible Builds Guide

Easily verify that the binary you installed **really** comes from the open‑source code on GitHub.

## Status

- **Linux x86‑64** — active
- **Linux ARM64** — standby
- **Windows** — standby
- **macOS** — standby

---

## 0 — Prerequisites

| Tool                | Tested version | Purpose                                       |
| ------------------- | -------------- | --------------------------------------------- |
| **git**             | any            | Clone the source repository and tags          |
| **Docker + Buildx** | 24.0+          | Provides a deterministic build environment    |
| **diff**            | any            | Byte‑for‑byte comparison of two files         |

---

## 1 — Find the version running on your device

1. Open **Nunchuk → Profile → Settings → About**.
2. Note the version shown (e.g. **2.6.6**). You will use this as the Git tag.

---

## 2 — Clone the source at that exact tag

``` bash
export PROJECT_DIR=$HOME/nunchuk-desktop
export VERSION=2.6.6 # Replace with the version you want to check

git clone https://github.com/nunchuk-io/nunchuk-desktop "$PROJECT_DIR"
cd "$PROJECT_DIR"
git checkout $VERSION
git submodule update --init --recursive
```

---

## 3 — Reproducible build inside Docker

### Linux

```bash
# Build the builder image
docker buildx build \
  --platform linux/amd64 \
  -t nunchuk-builder \
  -f reproducible-builds/Dockerfile.linux \
  --load .

# Build the app
docker run --platform linux/amd64 --rm \
  -e TAG="$VERSION" \
  -v "$PROJECT_DIR":/project \
  -w /project \
  nunchuk-builder \
  bash ./reproducible-builds/build_linux.sh
```

The Linux package includes
[HWI 3.2.0-displayaddress](https://github.com/nogibi/HWI/releases/tag/3.2.0-displayaddress),
verified with its pinned SHA-256 checksum during packaging.
The deploy process also:

- uses pinned, checksum-verified CQtDeployer, appimagetool, and AppImage runtime
  inputs;
- bundles the Qt NetworkAuth, WebEngine, OpenSSL 1.1.1, NSS, CA certificate, and
  selected Qt plugin runtimes required by Nunchuk;
- computes the complete ELF dependency closure while excluding host ABI,
  graphics-driver, and audio-driver libraries;
- deliberately uses the host system's glibc and never bundles `libc.so.6` or
  its ELF loader; and
- extracts and verifies the generated AppImage, then runs it for 30 seconds in
  a virtual X11/DBus session to catch loader, Qt plugin, QML, WebEngine, and TLS
  startup failures before publishing.

The final output is `nunchuk-linux-v$VERSION/nunchuk-linux-v$VERSION.zip`.
When a tag is pushed, the Linux workflow creates or updates the matching GitHub
Release as a pre-release and uploads this ZIP. Manual builds do not publish a
GitHub Release.

---

## 4 — Byte‑for‑byte Verification

Ensure your local build matches the official release exactly by comparing the final output files.

1. **Download** the official Linux release from the [GitHub releases page](https://github.com/nunchuk-io/nunchuk-desktop/releases).

2. **Compare** it to your local build:

   ```bash
   diff /path/to/download/nunchuk-linux-v$VERSION.zip "$PROJECT_DIR/nunchuk-linux-v$VERSION/nunchuk-linux-v$VERSION.zip"
   ```

---

### Result

If the `diff` command returns **no output**, your local build is **byte-for-byte identical** to the official release — congratulations!

If you see any differences, something is off — refer to the next section for troubleshooting tips.

---

## Troubleshooting
TBD

---

> *Verified builds keep everyone safer.* Thank you for taking the time to reproduce the binaries you run!
