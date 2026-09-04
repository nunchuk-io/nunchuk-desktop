# Reproducible macOS release

The macOS workflow builds native Intel and Apple Silicon releases. For each
architecture it runs two clean builds on separate runners and compares a
canonical, unsigned payload archive byte-for-byte. Signing is allowed only for
the payload that passes that gate.

## Release flow

1. Checkout the exact tag commit and all recursive submodules.
2. Select Xcode 16.4 and install the locked Python, CMake, Ninja and Qt 6.11.1
   toolchain, including Qt Network Authorization and Qt WebEngine.
3. Build locked Boost, libevent, Olm, QtKeychain 0.15.0 and OpenSSL 3.5.7 LTS
   inputs. Intel HWI comes from the release asset whose SHA256 is in
   `macos.lock.env`; Apple Silicon HWI is built from the exact locked commit.
4. Deploy Qt, explicitly require the Qt5Compat private GraphicalEffects plug-in
   and QtShaderTools, thin every Mach-O to the matrix architecture, remove
   ad-hoc signatures and normalize metadata.
5. Compare the two unsigned `.tar` payloads with `cmp`.
6. Sign nested Mach-O code and bundles from the inside out, using the app,
   PyInstaller HWI and QtWebEngine helper entitlements as appropriate. The
   workflow never uses `codesign --deep` to sign.
7. Build and sign the DMG, submit that outer DMG to Apple, staple the accepted
   ticket, then verify both the DMG and its mounted app with Gatekeeper.
8. Publish only the signed/notarized/stapled DMG, its checksum and a release
   manifest linking it to the gated unsigned SHA256. Existing release assets
   are immutable: identical bytes are a no-op and different bytes fail.

The unsigned archive is an internal GitHub Actions artifact retained for
verification; it is not a release download. Apple signing timestamps and the
notarization ticket intentionally make the final DMG non-reproducible at the
byte level.

## Locked inputs and secrets

Build inputs are recorded in `reproducible-builds/macos.lock.env`. The workflow
keeps compatibility with `GITLAB_PAT` and the legacy `GITLAB_PASS`. Signing uses
the existing secrets:

- `PROD_MACOS_CERTIFICATE`
- `PROD_MACOS_CERTIFICATE_PWD`
- `PROD_MACOS_CERTIFICATE_NAME`
- `PROD_MACOS_CI_KEYCHAIN_PWD`
- `IO_APPLE_ID`
- `IO_APPLE_TEAM_ID`
- `IO_APPLE_APP_PASSWORD`

`entitlements_hwi.plist` restores the HWI-specific PyInstaller entitlement set
recorded by commit `d0e55370d`: JIT, unsigned executable memory and disabled
library validation. These broader runtime permissions are applied only to the
bundled HWI executable, not to the whole application.

## Reproducibility boundary

The two-replica gate covers the native application, HWI, deployed Qt runtime,
file contents, modes, symlink targets and canonical archive. The hosted macOS
runner image and upstream distribution services are not immutable archives.
The workflow pins/asserts their versions and records the build fingerprints,
but long-term hermetic rebuilding still requires mirroring the Qt, PyPI and
source archives and using a snapshotted self-hosted runner.
