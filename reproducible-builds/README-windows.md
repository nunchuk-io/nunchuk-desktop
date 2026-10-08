# Windows Build Guide

Pinned toolchain (Qt 6.9.3, MSVC 14.44.35207 / SDK 10.0.22621.0, OpenSSL 3.5.7, vcpkg, etc.) is defined in `windows-dependencies.lock.json`. CI runs on `windows-2022` via `.github/workflows/build-windows.yml`, calling `reproducible-builds/build_windows.ps1` then `package_windows.ps1`.

## 1. Release build (tag push)

1. Bump the version in `CMakeLists.txt` (`project(... VERSION X.Y.Z)`) and in `app/main.cpp` (`QCoreApplication::setApplicationVersion()`) — both must match.
2. Commit the bump.
3. Create and push a tag `X.Y.Z` (no leading `v`) on that commit.
4. CI builds, packages, and publishes the unsigned ZIP + installer to a GitHub prerelease named `X.Y.Z` automatically.

## 2. Manual build (no release)

1. GitHub → **Actions** → **Build windows release** → **Run workflow**.
2. Select the branch or tag to build.
3. Run. Download the artifact from the run page once it finishes.

Artifact naming:

| Trigger | Artifact name |
| --- | --- |
| Tag push | `nunchuk-windows-x64-v<tag>` |
| Manual, off a branch | `nunchuk-windows-x64-<branch>-manual` |
| Manual, off a tag | `nunchuk-windows-x64-v<tag>-manual` |

Each artifact contains the unsigned ZIP, the Inno Setup installer (`*-setup.exe`), `payload-manifest.sha256`, and `build-info.json`.

## 3. Running the scripts locally

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

## 4. Signing

Not wired into CI yet. `sign_windows.ps1` exists but no workflow invokes it, and the `release-signing` environment secrets (`AZURE_TENANT_ID`, `AZURE_CLIENT_ID`, `AZURE_CLIENT_SECRET`, `AZURE_CODE_SIGNING_NAME`, `AZURE_CERT_PROFILE_NAME`) are not yet configured. Published releases are unsigned.
