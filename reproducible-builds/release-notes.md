Qt 6 release artifacts for Linux x86_64/aarch64, macOS Intel/Apple Silicon and Windows x64.

Download names follow `nunchuk-<os>-v<version>-<arch>`, with `-setup` or `-portable`
for Windows packages and `-unsigned` last when applicable, before the file extension.

- **Linux:** AppImage for x86-64 and ARM64.
  For either Linux architecture, rebuild the tag and compare the AppImage with the published artifact.
  Enable executable permission on the downloaded AppImage before launching it.
- **macOS:** DMG containing a Developer ID signed, notarized and stapled app.
- **Windows:** unsigned setup EXE for normal installation and a portable ZIP to run without installing.

For download verification, use `SHA256SUMS` and its detached GPG signature,
`SHA256SUMS.asc`.

Reproducible-build verification is active for Linux x86-64 and ARM64.
Windows and macOS remain standby.

Release builds run once per architecture.

Reproducible-build guide: [Linux](https://github.com/nunchuk-io/nunchuk-desktop/blob/main/reproducible-builds/README.md).
Developer build notes: [macOS](https://github.com/nunchuk-io/nunchuk-desktop/blob/main/reproducible-builds/README-macos.md),
[Windows](https://github.com/nunchuk-io/nunchuk-desktop/blob/main/reproducible-builds/README-windows.md).
