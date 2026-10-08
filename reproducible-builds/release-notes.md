This prerelease contains the Qt 6 reference artifacts for Linux x86_64,
macOS Intel and Apple Silicon, and Windows x64.

- Every workflow validates the source version against the release tag before
  an artifact can be released. All three platforms build once per
  architecture by default; each has an opt-in `check_reproducible_build`
  option (manual `workflow_dispatch` runs only) that rebuilds the same commit
  a second time in the same job and reports whether the two builds match
  byte-for-byte. This is a diagnostic for validating pipeline changes, not a
  release gate: it never runs on a tag push, and a mismatch never blocks or
  delays a release on any of the three platforms.
- Linux publishes the byte-reproducible AppImage ZIP and its SHA-256 checksum
  directly -- the published file is itself the artifact to rebuild and
  compare against.
- macOS publishes Developer ID signed, outer-DMG notarized and stapled
  images. Apple's signing timestamp and notarization ticket make the signed
  DMG itself non-reproducible by design; each release manifest instead links
  the signed DMG to the SHA-256 digest of the reproducible unsigned payload
  it was signed from.
- Windows publishes the unsigned reference ZIP and Inno Setup installer from
  a single pinned-toolchain build (see `reproducible-builds/README-windows.md`).
  Authenticode signing is not yet part of this pipeline; filenames are
  suffixed `-unsigned` accordingly.
- Release assets are immutable. A rerun accepts an existing byte-identical
  asset and refuses to replace an asset with different bytes.

Build instructions: `reproducible-builds/README.md` (Linux),
`reproducible-builds/README-macos.md` (macOS), `reproducible-builds/README-windows.md`
(Windows).
