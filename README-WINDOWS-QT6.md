# Build guideline — Windows (Qt6, native MSVC + vcpkg)

Step-by-step instructions to build `nunchuk-qt` natively on Windows against **Qt6**,
matching the current root [`CMakeLists.txt`](./CMakeLists.txt).

## Step 1 — Install prerequisites

| Tool | Version | Notes |
| --- | --- | --- |
| **Visual Studio 2022** | 17.8+ | Workload: "Desktop development with C++" (project builds with `/std:c++23`). |
| **Qt6** | 6.5+ | [Qt online/offline installer](https://www.qt.io/offline-installers). See checklist below — the default install is **not** enough. |
| **CMake** | 3.28+ | The one bundled with VS2022 works, or install standalone from [cmake.org](https://cmake.org/download/). |
| **Git** | any | To clone the repo and submodules. |
| **Python 3** | 3.9+ | Used by Bitcoin Core's build and a couple of helper scripts. |
| **vcpkg** | latest | For Boost headers, libevent, sqlite3, OpenSSL. |

**Qt Maintenance Tool / installer component checklist** — under your Qt6 version node,
tick:
- ☑ **MSVC 2022 64-bit** (the compiler kit itself — not MinGW, not 32-bit). This
  already bundles Core, Gui, Widgets, Qml, Quick, Network, Sql, PrintSupport,
  Concurrent, Svg, QuickControls2 — no separate checkboxes needed for those.
- ☑ **Qt Multimedia** — needed for `Qt6::Multimedia`.
- ☑ **Qt WebEngine** — needed for `Qt6::WebEngineWidgets` (`USING_WEBENGINE` defaults
  `ON` in `CMakeLists.txt`). This one is large/slow to download.

Everything else (Qt Quick 3D, Sensors, Positioning, mobile/WASM kits, WebEngine
"Sources", etc.) is not used by this project — skip it.

Note the install path you choose (default `C:\Qt\<version>\msvc2022_64`) — you'll
reuse it verbatim in Steps 5 and 6.

## Step 2 — Clone the project and submodules

```powershell
git clone https://gitlab.com/nunchuck/nunchuck-qt
cd nunchuck-qt
git submodule update --init --recursive
```

This fetches `contrib/libnunchuk`, `contrib/quotient`, and `contrib/zxing`.

## Step 3 — Set up vcpkg

The project forces the static CRT on MSVC (`/MT` / `/MTd`), so use the **static**
triplet `x64-windows-static` (not the default dynamic `x64-windows`).

```powershell
git clone https://github.com/Microsoft/vcpkg.git C:\vcpkg
C:\vcpkg\bootstrap-vcpkg.bat
```

Install the dependencies:

```powershell
C:\vcpkg\vcpkg install boost:x64-windows-static
C:\vcpkg\vcpkg install libevent:x64-windows-static
C:\vcpkg\vcpkg install sqlite3:x64-windows-static
C:\vcpkg\vcpkg install openssl:x64-windows-static
C:\vcpkg\vcpkg integrate install
```

- `boost` — only headers are used (`multi_index`, `signals2`, `asio`, `process`,
  `algorithm`, `tokenizer`, `format`, `bind`, `tuple`); no compiled Boost libraries
  are linked.
- `libevent` — required by Bitcoin Core's CMake (`find_package(Libevent 2.1.8 MODULE REQUIRED)`).
- `sqlite3` — required by Bitcoin Core's wallet CMake code
  (`find_package(unofficial-sqlite3 CONFIG REQUIRED)`).
- `openssl` — required by both `contrib/libnunchuk` and `contrib/quotient`.

`sqlcipher` does **not** need to come from vcpkg — `libnunchuk/CMakeLists.txt`
builds it in-tree from `contrib/libnunchuk/contrib/sqlite`.

## Step 4 — Build Olm (required by `contrib/quotient`)

```powershell
git clone https://gitlab.matrix.org/matrix-org/olm.git
cd olm
Remove-Item -Recurse -Force build
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=OFF -DOLM_TESTS=OFF `
  -DCMAKE_INSTALL_PREFIX=C:\deps `
  -DCMAKE_POLICY_DEFAULT_CMP0091=NEW -DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreaded
cmake --build build --config Release -j8
cmake --install build --config Release
cd ..
```

## Step 5 — Build QtKeychain against your Qt6 install

Build from source (rather than a prebuilt package) so it links against the exact
same Qt6 as the app:

```powershell
git clone https://github.com/frankosterfeld/qtkeychain.git
cd qtkeychain
Remove-Item -Recurse -Force build
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=OFF `
  -DCMAKE_INSTALL_PREFIX=C:\deps `
  -DQt6_DIR="C:\Qt\6.11.1\msvc2022_64\lib\cmake\Qt6" `
  -DCMAKE_POLICY_DEFAULT_CMP0091=NEW -DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreaded
cmake --build build --config Release -j8
cmake --install build --config Release
cd ..
```

Adjust `Qt6_DIR` to your actual Qt install path/version.

## Step 6 — Configure `nunchuk-qt`

This guide assumes an out-of-source build directory as a **sibling** of the project
folder, e.g.:

```
D:\Workspace\nunchuck-qt   <- source (this repo)
D:\Workspace\build         <- build output
```

Adjust the paths below if you'd rather nest `build\` inside `nunchuck-qt\` instead —
the only thing that matters is that the first `cmake ..` argument points at the
`nunchuck-qt` source directory.

**6.1 — Create the build directory**

```powershell
Remove-Item -Recurse -Force D:\Workspace\build
cmake -E make_directory D:\Workspace\build
cd D:\Workspace\build
```

**6.2 — Run CMake configure**

```powershell
cmake D:\Workspace\nunchuck-qt `
  -DCMAKE_BUILD_TYPE=Release `
  -DCMAKE_TOOLCHAIN_FILE=C:\vcpkg\scripts\buildsystems\vcpkg.cmake `
  -DVCPKG_TARGET_TRIPLET=x64-windows-static `
  -DCMAKE_PREFIX_PATH="C:\Qt\6.11.1\msvc2022_64;C:\deps" `
  -DBUILD_BITCOIN_BIN=OFF `
  -DBUILD_UTIL=OFF `
  -DBUILD_WALLET_TOOL=OFF
```

What each flag does:

| Flag | Purpose |
| --- | --- |
| `-DCMAKE_BUILD_TYPE=Release` | Build type; must match the config used in steps 4–5 (Release deps + Release app, or Debug + Debug — don't mix, `/MT` vs `/MTd` won't link together). |
| `-DCMAKE_TOOLCHAIN_FILE=...\vcpkg.cmake` | Routes `find_package()` calls through vcpkg. |
| `-DVCPKG_TARGET_TRIPLET=x64-windows-static` | Use the static-CRT triplet, matching the project's `/MT` setting. |
| `-DCMAKE_PREFIX_PATH="<Qt6 path>;C:\deps"` | Lets CMake find your Qt6 install *and* the Olm/QtKeychain you installed to `C:\deps` in steps 4–5. |
| `-DBUILD_BITCOIN_BIN=OFF -DBUILD_UTIL=OFF -DBUILD_WALLET_TOOL=OFF` | Skip building the standalone `bitcoin-cli`/`bitcoin-util`/`bitcoin-wallet` executables — only the `nunchuk` static library is needed. |

If configure succeeds, you'll see Qt6, Boost, OpenSSL, Libevent, Olm and Qt6Keychain
all reported as found in the CMake output.

## Step 7 — Build

Still inside `D:\Workspace\build`:

```powershell
cmake --build . --config Release -j12
```

The resulting binary is `D:\Workspace\build\Release\nunchuk-qt.exe` (or
`D:\Workspace\build\nunchuk-qt.exe`, depending on the CMake generator VS2022 picked).

## Step 8 — Run the application

`nunchuk-qt.exe` needs the Qt6 DLLs (and QML modules, WebEngine process, etc.) next
to it. Deploy them with Qt's own tool, pointing `--qmldir` at the source tree's
`Qml` folder:

```powershell
C:\Qt\6.11.1\msvc2022_64\bin\windeployqt.exe --release --qmldir D:\Workspace\nunchuck-qt\Qml D:\Workspace\build\Release\nunchuk-qt.exe
```

Then run `nunchuk-qt.exe` from `D:\Workspace\build\Release\`.

## Troubleshooting

- **CRT/linker mismatches (`LNK4098` or similar)** — every vcpkg package must be
  installed for the `x64-windows-static` triplet, and Olm/QtKeychain must be built
  with the same `CMAKE_BUILD_TYPE` as the app (Release ↔ Release, Debug ↔ Debug).
- **`Qt6WebEngineWidgets` not found** — re-run the Qt Maintenance Tool and make sure
  "Qt WebEngine" is installed for your Qt6 kit. Alternatively configure with
  `-DUSING_WEBENGINE=OFF` if you don't need the embedded web sign-in flow.
- **`find_package(unofficial-sqlite3)` or `find_package(Libevent)` fails** — confirm
  `vcpkg integrate install` ran, and that `CMAKE_TOOLCHAIN_FILE` /
  `VCPKG_TARGET_TRIPLET` were both passed in step 6.2.
- **`find_package(Olm)` / `find_package(Qt6Keychain)` fails** — confirm
  `CMAKE_PREFIX_PATH` includes `C:\deps` (or wherever you installed them in steps
  4–5).
