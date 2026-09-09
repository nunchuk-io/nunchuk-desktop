This prerelease contains the Qt 6 reference artifacts for Linux x86_64,
macOS Intel and Apple Silicon, and Windows x64.

- Every workflow validates the source version against the release tag before
  an artifact can be released. macOS additionally builds twice per
  architecture and gates signing on the two builds comparing byte-for-byte;
  Linux and Windows build once by default, with an opt-in
  `check_reproducible_build` option to verify byte-for-byte reproducibility
  on a manual run (never on a tag push, and never a release gate for those
  two platforms).
- Linux publishes the byte-reproducible AppImage ZIP and its SHA-256 checksum.
- macOS publishes Developer ID signed, outer-DMG notarized and stapled images.
  Each release manifest links the signed DMG to its reproducible unsigned
  payload digest.
- Windows publishes the unsigned reference ZIP and Inno Setup installer from
  a single pinned-toolchain build (see `reproducible-builds/README-windows.md`).
  Authenticode signing is not yet part of this pipeline; filenames are
  suffixed `-unsigned` accordingly.
- Release assets are immutable. A rerun accepts an existing byte-identical
  asset and refuses to replace an asset with different bytes.
