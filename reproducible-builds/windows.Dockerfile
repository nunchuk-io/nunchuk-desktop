FROM ubuntu:24.04

RUN set -ex; \
  export DEBIAN_FRONTEND=noninteractive && \
  apt-get update && \
  apt-get install -y \
    tzdata \
    cmake \
    make \
    ninja-build \
    git \
    wget \
    file \
    zip \
    curl \
    gcc-mingw-w64-x86-64 \
    g++-mingw-w64-x86-64 \
    pip \
    pkg-config \
    autoconf \
    automake \
	osslsigncode \
	nsis \
	qt5-qmake qtbase5-dev qttools5-dev-tools
RUN rm -rf /var/lib/apt/lists/*

RUN pip install aqtinstall --break-system-packages

ENV LC_ALL=C
ENV TZ=UTC
ENV SOURCE_DATE_EPOCH=1604304302

# Install QT5
ARG QT_VERSION=5.15.2
RUN aqt install-qt windows desktop ${QT_VERSION} win64_mingw81 --timeout 10 --outputdir /qt

# Install openssl
ARG OPENSSL_VERSION=OpenSSL_1_1_1j
RUN set -ex; \
    git clone https://github.com/openssl/openssl -b ${OPENSSL_VERSION} --depth 1 && \
	cd openssl && \
	./Configure mingw64 --cross-compile-prefix=x86_64-w64-mingw32- --prefix=/usr/x86_64-w64-mingw32 && \
	make -j$(nproc) && \
	make install_dev && \
	rm -rf /openssl

# Install boost
ARG BOOST_VERSION=1_81_0
RUN set -ex; \
  BOOST_VERSION_DOT="$(echo "$BOOST_VERSION" | sed 's/_/./g')" && \
  wget "https://archives.boost.io/release/$BOOST_VERSION_DOT/source/boost_${BOOST_VERSION}.tar.gz" && \
  tar -xzf "boost_${BOOST_VERSION}.tar.gz" --strip-components=1 -C /usr/x86_64-w64-mingw32/include boost_${BOOST_VERSION}/boost/ && \
  rm -rf "boost_${BOOST_VERSION}.tar.gz"

# Copy our toolchain file
COPY reproducible-builds/toolchain-mingw64.cmake /toolchain-mingw64.cmake

# Install libevent
ARG LIBEVENT_VERSION=release-2.1.12-stable
RUN set -ex; \
  git clone https://github.com/libevent/libevent -b $LIBEVENT_VERSION --depth 1 && \
  cd libevent && \
  cmake -S . -B build \
  -DCMAKE_TOOLCHAIN_FILE=/toolchain-mingw64.cmake \
  -DCMAKE_POLICY_VERSION_MINIMUM=3.5 \
  -DCMAKE_INSTALL_PREFIX=/usr/x86_64-w64-mingw32 \
  -DEVENT__DISABLE_BENCHMARK=On \
  -DEVENT__DISABLE_TESTS=On \
  -DEVENT__DISABLE_REGRESS=On \
  -DEVENT__DISABLE_SAMPLES=On && \
  cmake --build build --parallel $(nproc) && \
  cmake --install build && \
  rm -rf /libevent
  
# Install lib olm
ARG OLM_VERSION=3.2.16
RUN set -ex; \
  git clone https://gitlab.matrix.org/matrix-org/olm -b $OLM_VERSION --depth=1 && \
  cd olm && \
  cmake -S . -B build \
  -DCMAKE_TOOLCHAIN_FILE=/toolchain-mingw64.cmake \
  -DCMAKE_POLICY_VERSION_MINIMUM=3.5 \
  -DCMAKE_INSTALL_PREFIX=/usr/x86_64-w64-mingw32 \
  -DBUILD_SHARED_LIBS=Off \
  -DCMAKE_BUILD_TYPE=Release \
  -DOLM_TESTS=Off && \
  cmake --build build --parallel $(nproc) && \
  cmake --install build && \
  rm -rf /olm  

# Fix missing include
RUN cp /usr/x86_64-w64-mingw32/include/windows.h /usr/x86_64-w64-mingw32/include/Windows.h

# Install qtkeychain
ARG QTKEYCHAIN_COMMIT=35c4f40186a6ac3260982256e846bb0774ea91de
RUN \
  git clone https://github.com/frankosterfeld/qtkeychain && \
  cd qtkeychain && \
  git checkout ${QTKEYCHAIN_COMMIT} && \
  cmake -S . -B build \
  -DCMAKE_TOOLCHAIN_FILE=/toolchain-mingw64.cmake \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_INSTALL_PREFIX=/usr/x86_64-w64-mingw32 \
  -DBUILD_SHARED_LIBS=Off \
  -DQt5_DIR=/qt/${QT_VERSION}/mingw81_64/lib/cmake/Qt5 && \
  cmake --build build --parallel $(nproc) && \
  cmake --install build && \
  rm -rf /qtkeychain
  
# Fix Qt build with new GCC https://codereview.qt-project.org/c/qt/qtbase/+/339417
COPY reproducible-builds/patch/qtconcurrentthreadengine.h \
    /qt/${QT_VERSION}/mingw81_64/include/QtConcurrent/qtconcurrentthreadengine.h
	
# Install CQtDeployer
ARG CQTDEPLOYER_VERSION=1.6.2365
ARG CQTDEPLOYER_COMMIT=7cce7f3
ARG ARCH=x86_64
RUN set -ex; \
  if [ "$ARCH" = "x86_64" ]; then \
    wget https://github.com/QuasarApp/CQtDeployer/releases/download/v${CQTDEPLOYER_VERSION}/CQtDeployer_${CQTDEPLOYER_VERSION}.${CQTDEPLOYER_COMMIT}_Linux_${ARCH}.deb && \
    dpkg -i CQtDeployer_${CQTDEPLOYER_VERSION}.${CQTDEPLOYER_COMMIT}_Linux_${ARCH}.deb && \
    rm CQtDeployer_${CQTDEPLOYER_VERSION}.${CQTDEPLOYER_COMMIT}_Linux_${ARCH}.deb; \
  else \
    git clone https://github.com/QuasarApp/CQtDeployer.git -b v${CQTDEPLOYER_VERSION} --depth=1 && \
    cd CQtDeployer && \
    git submodule update --init --recursive --depth=1 && \
    cmake -S . -B build -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Release -DCQT_DEPLOYER_TESTS=Off -DCQT_DEPLOYER_TOOL=Off && \
    cmake --build build --parallel $(nproc) && \
    cmake --install build && \
    ln -s $PWD/build/src/CQtDeployer/CQtDeployer /usr/bin/cqtdeployer; \
  fi

# Download HWI
ARG HWI_VERSION=3.1.0
RUN set -ex; \
  wget https://github.com/bitcoin-core/HWI/releases/download/${HWI_VERSION}/hwi-${HWI_VERSION}-windows-${ARCH}.zip && \
  unzip hwi-${HWI_VERSION}-windows-${ARCH}.zip && \
  rm hwi-${HWI_VERSION}-windows-${ARCH}.zip && \
  rm hwi-qt.exe || true

# Install Microsoft CA
RUN set -ex; \
  curl -L -o /tmp/Microsoft-Identity-Verification-Root-CA-2020.der \
  "https://www.microsoft.com/pkiops/certs/Microsoft%20Identity%20Verification%20Root%20Certificate%20Authority%202020.crt" && \
  openssl x509 -inform DER -in /tmp/Microsoft-Identity-Verification-Root-CA-2020.der \ 
  -outform PEM -out /usr/local/share/ca-certificates/Microsoft-Identity-Verification-Root-CA-2020.crt && \
  rm /tmp/Microsoft-Identity-Verification-Root-CA-2020.der && \
  update-ca-certificates
