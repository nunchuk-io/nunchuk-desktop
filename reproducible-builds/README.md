
# Reproducible Builds Guide

Easily verify that the binary you installed **really** comes from the open‑source code on GitHub.

> **Status**  
> **Linux** (x86‑64 & ARM64) — supported from version **1.9.46+**  
> **Windows** — supported from version **1.9.47+**  
> **macOS** — support coming soon
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
2. Note the version shown (e.g. **1.9.46**). You will use this as the Git tag.

---

## 2 — Clone the source at that exact tag

``` bash
export PROJECT_DIR=$HOME/nunchuk-desktop
export VERSION=1.9.46 # Replace with the version you want to check

git clone https://github.com/nunchuk-io/nunchuk-desktop "$PROJECT_DIR"
cd "$PROJECT_DIR"
git checkout $VERSION
git submodule update --init --recursive
```

---

## 3 — Reproducible build inside Docker

### Linux

```bash
export ARCH=$(uname -m)
# Build the builder image
docker buildx build -t nunchuk-builder --build-arg ARCH=$ARCH -f reproducible-builds/linux.Dockerfile .
# Build the app
docker run --rm -v "$PROJECT_DIR":/nunchuk-desktop -w /nunchuk-desktop nunchuk-builder bash ./reproducible-builds/linux.sh
```
---

### Windows
```bash
export ARCH=$(uname -m)
# Build the builder image
docker buildx build -t nunchuk-builder --build-arg ARCH=$ARCH -f reproducible-builds/windows.Dockerfile .
# Build the app
docker run --rm -v "$PROJECT_DIR":/nunchuk-desktop -w /nunchuk-desktop nunchuk-builder bash ./reproducible-builds/windows.sh
```
---
When the script completes, your build output will be located in:

```
$PROJECT_DIR/build/artifacts/

# For example:
~/nunchuk-desktop/build/artifacts/nunchuk-linux-v1.9.46-x86_64.zip
~/nunchuk-desktop/build/artifacts/nunchuk-windows-v1.9.47-x64-setup-unsigned.exe
```

---

## 4 — Byte‑for‑byte Verification

Ensure your local build matches the official release exactly by comparing the final output files.

---

### Linux

1. **Download** the official Linux release from the [GitHub releases page](https://github.com/nunchuk-io/nunchuk-desktop/releases).

2. **Compare** it to your local build:

   ```bash
   diff /path/to/download/nunchuk-linux-v$VERSION-$ARCH.zip "$PROJECT_DIR/build/artifacts/nunchuk-linux-v$VERSION-$ARCH.zip"
   ```

---

### Windows

1. **Download** both of the following from the [GitHub releases page](https://github.com/nunchuk-io/nunchuk-desktop/releases):

   * The `.exe` installer (e.g., `nunchuk-windows-v1.9.67-x64-setup.exe`)
   * Its corresponding **PEM signature file** (e.g., `nunchuk-windows-v1.9.67-x64-setup.exe.pem`)

2. **Attach the signature** to your locally built unsigned executable:

   ```bash
   docker run --rm \
     -v "$PROJECT_DIR/build/artifacts:/artifacts" \
     -v "/path/to/download:/signed" \
     nunchuk-builder \
     osslsigncode attach-signature \
     -in /artifacts/nunchuk-windows-v$VERSION-x64-setup-unsigned.exe \
     -out /artifacts/nunchuk-windows-v$VERSION-x64-setup.exe \
     -sigin /signed/nunchuk-windows-v$VERSION-x64-setup.exe.pem
   ```

3. **Compare** the signed executable with the official release:

   ```bash
   diff /path/to/download/nunchuk-windows-v$VERSION-x64-setup.exe "$PROJECT_DIR/build/artifacts/nunchuk-windows-v$VERSION-x64-setup.exe"
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
