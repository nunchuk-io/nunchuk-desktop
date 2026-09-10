# Windows Qt 6 reproducible release

This document describes the full intended design: the same source commit
built twice on independent `windows-2022` runners with native MSVC 2022,
where both unsigned ZIP files, Inno Setup installers, payload manifests and
build metadata must compare byte-for-byte before a protected signing job
runs. `sign_windows.ps1` implements the signing half of that design but is
not yet invoked by any workflow.

**Current CI status:** `.github/workflows/build-windows.yml` builds once per
tag push or manual run (matching `build-linux.yml`'s own single-build UX) and
publishes the resulting **unsigned** ZIP/installer as the GitHub release
asset directly -- a tag build is never gated on a second build comparing
byte-for-byte. A manual `workflow_dispatch` run can tick the
"check_reproducible_build" checkbox to additionally rebuild the same commit a
second time in the same job and diff the two unsigned payloads; this is an
opt-in diagnostic for validating pipeline changes, not a release gate, and it
never runs for a tag push. Neither `sign_windows.ps1` nor the
`release-signing` environment described under "Signing and publication" is
invoked yet. That signing wiring, and the NASM version pin noted in the
workflow's own comments (NASM is installed via Chocolatey without a hash
pin, unlike the rest of the toolchain below), are open follow-ups.

Qt is pinned to 6.9.3 (matching Linux and macOS) rather than a newer 6.11.x
release: aqtinstall 3.3.0 (the only tagged release) has a confirmed,
still-unreleased-fix bug that prevents it from installing Qt 6.11.x on
Windows (it assumes the pre-6.11 repository folder layout). Qt 6.9.3 still
uses that pre-6.11 layout, so the plain, unpatched `aqtinstall==3.3.0`
release installed from PyPI works correctly -- the same Qt version and
install method as the proven manual reference workflow this pipeline is
based on.

## Immutable inputs

`windows-dependencies.lock.json` pins Qt 6.9.3 and its required modules,
QtKeychain 0.15.0, Olm, vcpkg, HWI, CMake, Ninja, aqt, Inno Setup and the exact
OpenSSL 3.5.7 source archive. The same OpenSSL source is built twice:

- `no-shared` archives are selected explicitly by the application CMake cache;
- shared `libssl-3-x64.dll` and `libcrypto-3-x64.dll` provide the runtime used
  by Qt's `qopensslbackend` plugin.

The build rejects any vcpkg-provided OpenSSL package. A small Qt program is also
compiled and run from the staged payload to require the `openssl` TLS backend,
`QSslSocket::supportsSsl()`, and the exact OpenSSL 3.5.7 runtime version.

The release tag must be a numeric version such as `2.6.7`, without a leading
`v`. CI requires that tag, both CMake `project(... VERSION ...)` declarations,
and `QCoreApplication::setApplicationVersion()` all contain the same version and
that the tag points exactly to the checked-out commit. `SOURCE_DATE_EPOCH` is
the commit timestamp.

## Build boundary

`build_windows.ps1` copies the clean recursive checkout to a fixed build path
before configuring it. This protects the original checkout from dependencies
whose CMake configure steps rename tracked files. It checks the original source
again after the build.

The native build uses `/MD`, `/Brepro`, deterministic PDB paths and fixed path
maps. The scripts reject `/MT`, `/MTd`, `/NODEFAULTLIB`, a mismatched OpenSSL
CMake cache, debug Qt DLLs, non-x64 PE files and unresolved non-system DLL
dependencies. ZIP entries use ordinal path order, no compression and the commit
timestamp. Inno Setup receives an ordinal explicit file list with `notimestamp`
and single-threaded compression.

## Signing and publication

The `release-signing` GitHub environment must define these secrets:

- `AZURE_TENANT_ID`
- `AZURE_CLIENT_ID`
- `AZURE_CLIENT_SECRET`
- `AZURE_CODE_SIGNING_NAME`
- `AZURE_CERT_PROFILE_NAME`

Set the GitHub Actions configuration variable `WINDOWS_SIGNER_SUBJECT` (at the
repository or `release-signing` environment level) to the exact Authenticode
subject expected on Nunchuk release files.

Once wired up (not yet implemented in CI, per "Current CI status" above),
after the two-replica gate passes, Azure Artifact Signing would sign exactly:

- `nunchuk-qt.exe`
- `qt6keychain.dll`
- `hwi.exe`

The signed payload is repackaged, then the outer Inno Setup installer is signed.
`Get-AuthenticodeSignature` and `signtool /pa /all` must both pass with the exact
configured subject and an RFC 3161 timestamp certificate.

The public release contains only the signed ZIP, signed installer, release
manifest, checksum file and GitHub provenance bundle. Unsigned packages remain
one-day internal Actions artifacts. The release manifest records SHA-256 hashes
for the canonical unsigned ZIP, unsigned installer and unsigned payload
manifest so the signed output remains bound to the successful reproducibility
gate.

Release assets are immutable: an existing complete byte-identical Windows asset
set is a no-op; a partial set or a differing file fails for manual investigation.
Because Authenticode timestamps change between signing attempts, a failure after
a partial upload cannot be repaired by overwriting assets. Remove the incomplete
release only after explicit investigation, then rerun the protected job.

## Remaining external inputs

The dependency versions and downloaded bytes are locked, but the
`windows-2022` hosted image, MSVC/Windows SDK installation, Qt distribution
service and upstream archive availability are external inputs. The two replicas
prove determinism for the selected runner/toolchain fingerprint; long-term
independent rebuilding requires mirrored dependencies and a versioned runner
image.
