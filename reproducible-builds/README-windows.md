# Windows Qt 6 reproducible release

The Windows release workflow builds the same source commit twice on independent
`windows-2022` runners with native MSVC 2022. Both unsigned ZIP files, Inno
Setup installers, payload manifests and build metadata must compare
byte-for-byte before the protected signing job can run.

## Immutable inputs

`windows-dependencies.lock.json` pins Qt 6.11.1 and its required modules,
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

After the two-replica gate passes, Azure Artifact Signing signs exactly:

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
