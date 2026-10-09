# Windows release-build notes

**Reproducible-build status: standby.** These are developer build instructions;
independent reproducible-build verification is not yet supported.

The Qt6/MSVC pipeline uses `build_windows.ps1`, `package_windows.ps1` and
`windows-dependencies.lock.json`. HWI 3.2.2 is downloaded and checksum-verified
automatically. Releases contain an unsigned Inno Setup installer (`-setup-unsigned.exe`)
and portable ZIP, with a payload manifest and build metadata. Use the installer
for normal installation, shortcuts and uninstall support. Use the portable ZIP
to extract and run `nunchuk-qt.exe` without installing.

## GitHub Actions

Run **Build windows release** on a branch or tag to download artifacts without
publishing.
Pushing a numeric tag `X.Y.Z` publishes a prerelease; its version must match
`CMakeLists.txt` and `app/main.cpp`.

## Local build

Requires a Windows machine with: MSVC toolset `14.44.35207` / SDK `10.0.22621.0` on `PATH` (via `vcvarsall.bat x64` or equivalent), Python 3.12, NASM.

```powershell
$commit = (git rev-parse HEAD).Trim()
$epoch  = (git show -s --format=%ct HEAD).Trim()
$version = "X.Y.Z"   # must match CMakeLists.txt / main.cpp

.\reproducible-builds\build_windows.ps1 `
  -Replica a `
  -ExpectedCommit $commit `
  -ReleaseVersion $version `
  -SourceDateEpoch ([long]$epoch)

.\reproducible-builds\package_windows.ps1 `
  -ExpectedCommit $commit `
  -ReleaseVersion $version `
  -SourceDateEpoch ([long]$epoch) `
  -ArtifactFlavor unsigned `
  -ApplicationExe C:\nunchuk-repro\build-output\nunchuk-qt.exe `
  -BuildInfoFile C:\nunchuk-repro\build-output\build-info.json `
  -TlsProbeExe C:\nunchuk-repro\build-output\qt-tls-probe.exe `
  -SmokeTest
```

Output is written to `C:\nunchuk-repro\artifacts`.
For version 2.9.0, the downloads are `nunchuk-windows-v2.9.0-x64-setup-unsigned.exe`
and `nunchuk-windows-v2.9.0-x64-portable-unsigned.zip`.

Published Windows artifacts are unsigned; Authenticode signing is not configured.
`sign_windows.ps1` is retained for the planned signing flow, but no current
workflow invokes it.
