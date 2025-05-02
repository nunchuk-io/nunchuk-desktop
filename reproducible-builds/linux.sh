#!/bin/bash
set -e

# Build project
cd /nunchuk-desktop
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=Off -DBUILD_CLI=Off -DCMAKE_AUTORCC_OPTIONS="--format-version;1"
cmake --build . --parallel $(nproc)

# Get version
VERSION=$(grep 'setApplicationVersion' /nunchuk-desktop/main.cpp | sed -E 's/.*setApplicationVersion\("([^"]+)"\).*/\1/')
ARCH=$(uname -m)

# Deploy
cqtdeployer -bin /nunchuk-desktop/build/nunchuk-qt -qmlDir /nunchuk-desktop noTranslations -verbose 3 -disablePlugins sqldrivers -targetDir output/appdir

# Fix permissions
find /nunchuk-desktop/deploy -exec chmod 0755 {} +

# Copy resources
cp /nunchuk-desktop/deploy/AppRun output/
cp /nunchuk-desktop/deploy/nunchuk-qt.desktop output/
cp /nunchuk-desktop/deploy/nunchuk-qt.png output/

# Copy extra libs
for lib in libcrypto libssl libolm libqt5keychain libmd4c libpcre2-16 libdouble-conversion; do
  cp /usr/lib/${ARCH}-linux-gnu/${lib}.so* output/appdir/lib
  chmod 775 output/appdir/lib/${lib}.so*
done

# Copy hwi
cp /hwi output/appdir/bin

# Make appimage
/appimagetool-${ARCH}.AppImage --appimage-extract-and-run --runtime-file=/runtime-${ARCH} output nunchuk-linux-v${VERSION}-${ARCH}.AppImage

# Zip the result
mkdir nunchuk-linux-v${VERSION}-${ARCH}
mv nunchuk-linux-v${VERSION}-${ARCH}.AppImage nunchuk-linux-v${VERSION}-${ARCH}
cp -r /nunchuk-desktop/deploy/nunchuk-linux nunchuk-linux-v${VERSION}-${ARCH}
bash /nunchuk-desktop/reproducible-builds/reproducible_zip.sh nunchuk-linux-v${VERSION}-${ARCH} nunchuk-linux-v${VERSION}-${ARCH}.zip
mkdir artifacts
mv nunchuk-linux-v${VERSION}-${ARCH}.zip artifacts/

# error while loading shared libraries: libxcb-xinput.so.0
# sudo apt-get install libxcb-xinput0
