#!/bin/bash
set -e

# Build project
cd /nunchuk-desktop
git config --global --add safe.directory /nunchuk-desktop
export SOURCE_DATE_EPOCH=$(git log -1 --pretty=%ct)
mkdir build || { echo "Failed to create build directory"; exit 1; }
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=../reproducible-builds/toolchain-mingw64.cmake \
	-DCMAKE_BUILD_TYPE=Release \
	-DBUILD_SHARED_LIBS=Off \
	-DQt5_DIR=/qt/5.15.2/mingw81_64/lib/cmake/Qt5 \
	-DCMAKE_AUTORCC_OPTIONS="--format-version;1"
cmake --build . --parallel $(nproc)

# Get version
VERSION=$(grep 'setApplicationVersion' /nunchuk-desktop/main.cpp | sed -E 's/.*setApplicationVersion\("([^"]+)"\).*/\1/')
ARCH=$(uname -m)

# Deploy
cqtdeployer -bin /nunchuk-desktop/build/nunchuk-qt.exe \
	-qmlDir /nunchuk-desktop \
	noTranslations \
	-disablePlugins sqldrivers \
	-libDir /qt/5.15.2/mingw81_64/bin/ \
	-qmake /qt/5.15.2/mingw81_64/bin/qmake.exe  \
	-targetDir output \
	-qmlOut qml \
	-libOut . \
	-trOut . \
	-pluginOut plugins \
	-binOut . \
	-recOut . \
	-verbose 3
	
# Remove unwanted files
rm output/qt.conf
rm output/nunchuk-qt.bat

# Copy hwi
cp /hwi.exe output

# Copy extra libs
cp /usr/x86_64-w64-mingw32/bin/libcrypto* output/
cp /usr/x86_64-w64-mingw32/bin/libssl* output/
cp /qt/5.15.2/mingw81_64/bin/libEGL* output/
cp /qt/5.15.2/mingw81_64/bin/libGLESv2* output/

# Make installer
mkdir artifacts
makensis -DVERSION=$VERSION -DPROJECT_DIR=/nunchuk-desktop /nunchuk-desktop/deploy/setup.nsi
