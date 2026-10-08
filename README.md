# Nunchuk Desktop

## Build

Follow the [reproducible builds guide](reproducible-builds/README.md) to verify
Linux x86-64 and ARM64 releases. Windows and macOS reproducible builds are
**standby** and not yet supported for independent verification.

Developer build notes:

- [macOS release build](reproducible-builds/README-macos.md)
- [Windows release build](reproducible-builds/README-windows.md)
- [Windows manual development setup](README-WINDOWS-QT6.md)

## Hardware wallets on Linux

Install the bundled [udev rules](deploy/nunchuk-linux/udev/README.md) to give your
user access to connected hardware wallets. Log out and back in after adding your
user to the `plugdev` group.

## License

[GNU GPL v3](LICENSE).
