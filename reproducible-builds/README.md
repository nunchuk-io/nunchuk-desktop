
# Reproducible Builds Guide

Easily verify that the binary you installed **really** comes from the open‑source code on GitHub.

> **Status**: Linux (x86‑64 & ARM64) supported. Windows and macOS instructions coming soon.

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

```bash
cd "$PROJECT_DIR/reproducible-builds"

# Build the builder image
export ARCH=$(uname -m)
docker buildx build -t nunchuk-builder --build-arg ARCH=$ARCH -f linux.Dockerfile .

# Build the app
docker run --rm -v "$PROJECT_DIR":/nunchuk-desktop -w /nunchuk-desktop nunchuk-builder bash ./reproducible-builds/linux.sh
```

When the script completes, your zip lives in:

```
$PROJECT_DIR/build/artifacts/nunchuk-linux-v$VERSION-$ARCH.zip

# For example:
/home/ubuntu/nunchuk-desktop/build/artifacts/nunchuk-linux-v1.9.46-x86_64.zip
```

---

## 4 — Byte‑for‑byte verification

1. Download the official release for your platform from the [GitHub releases page](https://github.com/nunchuk-io/nunchuk-desktop/releases).
2. Compare your locally built ZIP with the one you downloaded:

```bash
diff /path/to/download/nunchuk-linux-v$VERSION-$ARCH.zip "$PROJECT_DIR/build/artifacts/nunchuk-linux-v$VERSION-$ARCH.zip"
```

If the `diff` command prints **no output**, the builds are identical—congratulations! Any difference indicates something went wrong; see the next section.

---

## Troubleshooting
TBD

---

> *Verified builds keep everyone safer.* Thank you for taking the time to reproduce the binaries you run!
