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
  DMG itself non-reproducible by design (see "How to verify this release"
  below); each release manifest instead links the signed DMG to the SHA-256
  digest of the reproducible unsigned payload it was signed from.
- Windows publishes the unsigned reference ZIP and Inno Setup installer from
  a single pinned-toolchain build (see `reproducible-builds/README-windows.md`).
  Authenticode signing is not yet part of this pipeline; filenames are
  suffixed `-unsigned` accordingly. Like Linux, the published file is itself
  the artifact to rebuild and compare against.
- Release assets are immutable. A rerun accepts an existing byte-identical
  asset and refuses to replace an asset with different bytes.

## How to verify this release

Anyone can rebuild this exact tag from public source and check it against
what was published here, without needing to trust this repository's own CI
run.

**Linux** (`nunchuk-linux-<ARCH>-v<VERSION>.zip`): build the tag with
`reproducible-builds/Dockerfile.linux` + `build_linux.sh`, then compare your
resulting ZIP directly against the one on this release with `cmp` (after
checking both against their `.sha256` files). Full commands: "Build an exact
Linux version" and "Compare the Linux release" in
`reproducible-builds/README.md`.

**Windows** (`nunchuk-windows-x64-v<VERSION>-unsigned-setup.exe` and its
ZIP): build the tag with `build_windows.ps1` on a matching Windows/MSVC
toolchain, then compare your resulting ZIP/installer directly against the
ones on this release (again after checking both against their `.sha256`
files). See `reproducible-builds/README-windows.md`.

**macOS** (`nunchuk-macos-<ARCH>-v<VERSION>.dmg`): the published DMG cannot
be reproduced byte-for-byte by anyone, including Nunchuk re-signing its own
unchanged build -- Apple's notarization ticket and codesign timestamp are
issued fresh by Apple's servers on every signing run. What is reproducible
is the *unsigned* payload the DMG was signed from. To verify:

1. Check out this tag on the matching architecture (Intel or Apple Silicon)
   and build it with `reproducible-builds/build_macos.sh` -- see
   `reproducible-builds/README-macos.md` for the exact toolchain.
2. Compute the SHA-256 of the canonical unsigned `.tar` your build produces.
3. Download this release's `nunchuk-macos-<ARCH>-v<VERSION>.release.json` and
   compare your hash against its `unsigned_payload_sha256` field.

A match means: the DMG published here really was signed from this exact,
independently-rebuildable source -- Apple's signature only vouches for
*who* published it, not for reproducibility, which this hash comparison
provides instead.
