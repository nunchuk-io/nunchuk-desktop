#!/bin/bash
NUNCHUK_SRC=$PWD
cd $NUNCHUK_SRC/build
make -j8 VERBOSE=1
cd $NUNCHUK_SRC
