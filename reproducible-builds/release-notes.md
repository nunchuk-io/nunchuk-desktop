This prerelease contains the Qt 6 reference artifacts for Linux x86_64,
macOS Intel and Apple Silicon, and Windows x64.

- Every workflow validates the source version against the release tag and
  compares two clean unsigned builds before an artifact can be released.
- Linux publishes the byte-reproducible AppImage ZIP and its SHA-256 checksum.
- macOS publishes Developer ID signed, outer-DMG notarized and stapled images.
  Each release manifest links the signed DMG to its reproducible unsigned
  payload digest.
- Windows publishes an Authenticode timestamped installer and a ZIP whose
  approved executable allowlist passed signing verification. Its release
  manifest links the signed outputs to the reproducible unsigned payload.
- Release assets are immutable. A rerun accepts an existing byte-identical
  asset and refuses to replace an asset with different bytes.
