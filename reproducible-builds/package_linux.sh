#!/bin/bash
set -euo pipefail

# Determine version tag
TAG=${TAG:-0.0.0}
OUTDIR=nunchuk-linux-v${TAG}/Appdir
mkdir -p "$OUTDIR"

set -x
# Deploy with CQtDeployer
EXEC="build/nunchuk-qt"
ICON_PATH="/project/deploy/nunchuk-qt.png"
cqtdeployer -bin "build/nunchuk-qt" \
            -qmake "$QT_INSTALLED_PREFIX/bin/qmake" \
            -qmlDir . \
            -targetDir "$OUTDIR" \
            -icon "$ICON_PATH" \
            noTranslation \
            noStrip

# Bundle libs
mkdir -p "$OUTDIR/lib"
BIN="$OUTDIR/bin/nunchuk-qt"
while IFS= read -r line; do
    if [[ "$line" == *"not found"* ]]; then
        echo "Missing library: $line"
    else
        lib=$(echo "$line" | awk '{print $3}')
        if [[ "$lib" == /* ]]; then
            cp -L "$lib" "$OUTDIR/lib/"
        fi
    fi
done < <(ldd "$BIN")

# OpenSSL libraries
cp -L $OPENSSL_ROOT_DIR/lib/libssl.so* $OUTDIR/lib/
cp -L $OPENSSL_ROOT_DIR/lib/libcrypto.so* $OUTDIR/lib/

# Patchelf rpath
patchelf --set-rpath '$ORIGIN/../lib' "$BIN"

# Install HWI
#wget -q https://github.com/bitcoin-core/HWI/releases/download/3.1.0/hwi-3.1.0-linux-x86_64.tar.gz
wget -q https://github.com/giahuy98/HWI/releases/download/3.1.0/hwi-3.1.0-mac-x86_64.tar.gz
mkdir -p hwi-extracted && tar -xzf hwi-3.1.0-linux-x86_64.tar.gz -C hwi-extracted
cp hwi-extracted/hwi "$OUTDIR/bin/"
chmod +x "$OUTDIR/bin/hwi"

# Desktop and AppRun
cat <<EOF > $OUTDIR/nunchuk.desktop
[Desktop Entry]
Type=Application
Name=Nunchuk
Exec=AppRun
Icon=nunchuk-qt
Categories=Utility;
EOF
cp -L "deploy/nunchuk-qt.png" "$OUTDIR"

cat <<'EOF' > $OUTDIR/AppRun
#!/bin/bash
HERE="$(dirname "$(readlink -f "$0")")"
export QTWEBENGINE_DISABLE_SANDBOX=1
exec "$HERE/nunchuk-qt.sh" "$@"
EOF
chmod +x $OUTDIR/AppRun

# Create AppImage
cd nunchuk-linux-v${TAG}
ARCH=x86_64 appimagetool --appimage-extract-and-run Appdir "nunchuk-linux-v${TAG}.AppImage"
