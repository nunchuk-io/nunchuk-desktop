#!/bin/bash
set -e

# Build project
cd /nunchuk-desktop
git config --global --add safe.directory /nunchuk-desktop
export SOURCE_DATE_EPOCH=$(git log -1 --pretty=%ct)
mkdir build || { echo "Failed to create build directory"; exit 1; }
cd build
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
for lib in libcrypto libssl libmd4c libpcre2-16 libdouble-conversion; do
  cp /usr/lib/${ARCH}-linux-gnu/${lib}.so* output/appdir/lib
  chmod 775 output/appdir/lib/${lib}.so*
done

libavcodec_path=$(find /usr/lib /usr/lib64 /usr/local/lib -type f -name "libavcodec.so.58" 2>/dev/null | head -n 1)
if [ -n "$libavcodec_path" ]; then
    echo "Found libavcodec.so.58 at: $libavcodec_path"
    cp "$libavcodec_path" output/appdir/lib/
    chmod 775 output/appdir/lib/libavcodec.so.58
else
    echo "libavcodec.so.58 not found"
fi

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
