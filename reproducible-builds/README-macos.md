# Reproducible macOS release

The macOS workflow builds native Intel and Apple Silicon releases, one build
per architecture, and signs that single build directly -- see
"Reproducibility model" below for why there is no byte-for-byte replica gate
in front of signing (a deliberate change from an earlier version of this
workflow, made to match `build-linux.yml`'s single-build UX instead).

## Release flow

1. Checkout the exact tag commit and all recursive submodules.
2. Select Xcode 16.4 and install Qt 6.9.3 via `jurplel/install-qt-action`,
   including Qt Network Authorization and Qt WebEngine. CMake uses its
   default Unix Makefiles generator on macOS (no Ninja). `build_macos.sh`
   installs its own pyenv-managed Python 3.9 internally, scoped only to
   building HWI from source.
3. Build locked Boost, libevent, Berkeley DB and Olm inputs, plus QtKeychain
   0.14.2 into a private prefix. OpenSSL is not an independently-locked
   input: it is built in place from the copy already vendored inside the
   `contrib/libnunchuk` submodule (pinned by that submodule's own commit),
   because `contrib/libnunchuk/CMakeLists.txt` requires it at that exact
   path. HWI is built from source for **both** architectures from the exact
   locked commit -- unlike an earlier version of this pipeline, Intel HWI is
   not taken from a prebuilt upstream release asset.
4. Deploy Qt, copy the QtShaderTools framework (a transitive dependency Qt's
   deployment scanner can miss), install HWI into the bundle, and patch the
   QtWebEngineProcess helper's rpath/dependencies. Produce one canonical
   unsigned `.tar` payload (no architecture-thinning or signature-stripping
   at this stage) with its SHA256 checksum and a payload manifest.
5. The signing job downloads that unsigned payload, re-verifies its checksum
   (protects against artifact-transport corruption only, not against
   non-reproducibility -- see below), and extracts it.
6. Sign nested Mach-O code and bundles from the inside out: every
   dylib/so/bundle/plugin and Qt component, then the main executable, each
   with `codesign --deep` and the app's entitlements; HWI with its own
   PyInstaller entitlements (no `--deep`); the QtWebEngineProcess helper with
   its own bundled entitlements (`--deep`); and finally the outer `.app`
   bundle with the app's entitlements, deliberately **without** `--deep`.
7. Zip and notarize the signed `.app` bundle (not the DMG), staple the
   accepted ticket to the `.app`, and verify it with Gatekeeper. Only then
   package the notarized `.app` into a DMG with `hdiutil` -- the DMG wrapper
   itself is never separately signed or notarized, matching the reference
   workflow.
8. Publish the DMG (containing the signed/notarized/stapled `.app`), its
   checksum and a release manifest linking it to the unsigned payload's
   SHA256. Existing release assets are immutable: identical bytes are a
   no-op and different bytes fail.

The unsigned archive is an internal GitHub Actions artifact retained for
traceability; it is not a release download. Apple signing timestamps and the
notarization ticket intentionally make the final DMG non-reproducible at the
byte level.

## Reproducibility model

By default -- both a tag push and a plain manual `workflow_dispatch` run --
each architecture is built **once**, and that single unsigned payload is
signed directly. There is no automatic byte-for-byte comparison gating
signing, unlike `build-windows.yml`'s always-two-replica design. This mirrors
`build-linux.yml`'s own model: Linux has always published a single build per
architecture with reproducibility left as something anyone can verify after
the fact (see `build-linux.yml`'s "Compare the Linux release" section), and
this workflow now takes the same posture for macOS.

Ticking "Check reproducible build" on a manual `workflow_dispatch` run adds an
opt-in diagnostic: the same commit is rebuilt from scratch a second time in
the same job, and the two unsigned tars are compared byte-for-byte, reporting
PASSED/NOT PASSED in the job's step summary -- exactly like `build-linux.yml`'s
own `check_reproducible_build` checkbox. This is useful for validating changes
to the build/packaging pipeline itself. It is diagnostic only: a NOT PASSED
result does not stop or affect the sign jobs, which always proceed from the
single build produced earlier in the same job run. This checkbox does not
exist on a tag push, so it can never gate or delay a release either.

**Accepted trade-off:** an earlier version of this workflow built each
architecture twice on independent runners and only allowed `sign_macos.sh` to
run once both replicas matched byte-for-byte, so every signed/published DMG
came with a cryptographic guarantee that its unsigned input was
reproducible. That guarantee no longer exists here. To recover an equivalent
level of assurance for a specific tag, rebuild that tag with
`workflow_dispatch` + "Check reproducible build" checked before (or after)
relying on the published release.

## Locked inputs and secrets

Build inputs are recorded in `reproducible-builds/macos.lock.env`. Signing uses
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

## CI signing setup checklist

`build-macos.yml`'s `sign-x86_64` / `sign-arm64` jobs run in a GitHub
Environment named `release-signing` and read the seven secrets above by name.
Referencing an environment that does not exist yet does not fail the run --
GitHub creates it automatically, with no protection rules, the first time a
job requests it -- so nothing below blocks a first run by itself. What is not
independently confirmed from this repository's evidence is *where* the seven
secret values currently live:

- They are the same secret names already used, and already working, by the
  manually-run reference workflow at
  `tongvanlinh/macdeploynunchuk/.github/workflows/nunchuk-macos_matrix.yml`
  (a personal fork of `nunchuk-io/macdeploynunchuk`). That confirms a valid
  Developer ID Application certificate, keychain password and Apple
  notarization credential exist somewhere and are usable -- it does not by
  itself confirm they are also configured on `nunchuk-io/nunchuk-desktop`
  (this repository). GitHub Actions secrets are never shared between
  repositories automatically, including between a fork and its upstream,
  unless they were deliberately set at the `nunchuk-io` organization level
  with this repository included in that secret's access list.
- To check: repository Settings -> Secrets and variables -> Actions (and,
  separately, Settings -> Environments -> `release-signing` if that
  environment is created ahead of time) should list all seven names above.
  Organization-level secrets shared with this repository show up the same
  way, under "Secrets" with an "Organization" origin badge.
- If they are missing here, they need to be added as repository secrets (or
  org secrets scoped to include this repository) using the same values
  already in use for `macdeploynunchuk`, namely:
  - `PROD_MACOS_CERTIFICATE`: the Developer ID Application (.p12) certificate,
    base64-encoded (`base64 -i DeveloperID.p12 | pbcopy`).
  - `PROD_MACOS_CERTIFICATE_PWD`: the export password chosen when creating
    that .p12.
  - `PROD_MACOS_CERTIFICATE_NAME`: the exact signing identity string, e.g.
    `Developer ID Application: <Name/Org> (<TEAMID>)`, as shown by
    `security find-identity -v -p codesigning`.
  - `PROD_MACOS_CI_KEYCHAIN_PWD`: any password used only to protect the
    temporary CI keychain created by `sign_macos.sh`; it does not need to
    match anything else.
  - `IO_APPLE_ID`: the Apple ID email used for notarization.
  - `IO_APPLE_TEAM_ID`: the Apple Developer Team ID (also visible inside
    `PROD_MACOS_CERTIFICATE_NAME`'s parentheses).
  - `IO_APPLE_APP_PASSWORD`: an app-specific password for that Apple ID
    (generated at appleid.apple.com, not the Apple ID's account password),
    or an App Store Connect API key exchanged for one -- whichever
    `nunchuk-macos_matrix.yml` already uses today.
- Adding required reviewers or a branch/tag restriction on the
  `release-signing` environment (Settings -> Environments -> New environment
  or edit the existing one) is the only gate in front of signing today --
  see "Reproducibility model" above for why there is no automatic
  byte-for-byte check performing that role instead.

## Reproducibility boundary

`package_macos.sh` normalizes file contents, modes, symlink targets and the
canonical archive (deterministic GNU tar, SOURCE_DATE_EPOCH-driven timestamps,
a SHA-256 payload manifest) so that two from-scratch builds of the same commit
are *capable* of matching byte-for-byte -- and the opt-in "Check reproducible
build" run (see
"Reproducibility model" above) is how that is actually confirmed for a given
commit, since it no longer happens automatically on every run. The hosted
macOS runner image and upstream distribution services are not immutable
archives. The workflow pins/asserts their versions and records the build
fingerprints, but long-term hermetic rebuilding still requires mirroring the
Qt, PyPI and source archives and using a snapshotted self-hosted runner.
