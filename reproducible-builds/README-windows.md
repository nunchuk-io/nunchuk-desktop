# Windows release-build notes

**Reproducible-build status: standby.** These are developer build instructions;
independent reproducible-build verification is not yet supported.

The Qt6/MSVC pipeline uses `build_windows.ps1`, `package_windows.ps1` and
`windows-dependencies.lock.json`. HWI 3.2.1 is downloaded and checksum-verified
automatically. Releases contain an unsigned ZIP, Inno Setup installer, payload
manifest and build metadata.

## GitHub Actions

Run **Build windows release** on a branch or tag to download artifacts without
publishing. Enable **Check reproducible build** for an optional second build.
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

Published Windows artifacts are unsigned; Authenticode signing is not configured.
`sign_windows.ps1` is retained for the planned signing flow, but no current
workflow invokes it.
