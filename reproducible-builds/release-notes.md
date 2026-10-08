Qt 6 release artifacts for Linux x86_64/aarch64, macOS Intel/Apple Silicon and Windows x64.

- **Linux:** AppImage ZIP, checksum, build-input inventory and exact CI builder image digest.
  For either Linux architecture, rebuild the tag and compare the complete ZIP with the published artifact.
- **macOS:** DMG containing a Developer ID signed, notarized and stapled app.
  The release manifest records the unsigned payload hash for diagnostics.
- **Windows:** unsigned ZIP and Inno Setup installer, with payload manifest and build metadata.

Reproducible-build verification is active for Linux x86-64 and ARM64.
Windows and macOS remain standby.

All platforms use checksum-verified HWI 3.2.1 release binaries. Release builds run
once per architecture. Manual workflows offer an optional second build to check
reproducibility; it is not a release gate.

Reproducible-build guide: [Linux](https://github.com/nunchuk-io/nunchuk-desktop/blob/main/reproducible-builds/README.md).
Developer build notes: [macOS](https://github.com/nunchuk-io/nunchuk-desktop/blob/main/reproducible-builds/README-macos.md),
[Windows](https://github.com/nunchuk-io/nunchuk-desktop/blob/main/reproducible-builds/README-windows.md).
