#!/usr/bin/env bash
# reproducible_zip.sh — make a reproducible ZIP from a directory
set -e

[ $# -eq 2 ] || { echo "Usage: $0 <directory> <output.zip>"; exit 1; }
DIR=$1
OUT=$2
[ -d "$DIR" ] || { echo "Error: $DIR is not a directory"; exit 1; }

# 1) freeze mtimes to $SOURCE_DATE_EPOCH (fallback to 1980-01-01 for ZIP)
TZ=UTC find "$DIR" -exec touch -h -d "@${SOURCE_DATE_EPOCH:-315532800}" {} +

# 2) feed a sorted file list to zip for deterministic order
find $DIR -type f -print0 | LC_ALL=C sort -z | xargs -0 zip -X "$OUT"
