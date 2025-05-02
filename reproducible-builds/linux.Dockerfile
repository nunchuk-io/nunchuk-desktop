FROM ubuntu:22.04

# ARCH: x86_64 or arm64
ARG ARCH=x86_64
RUN echo "Building for arch: $ARCH"

RUN set -ex; \
  export DEBIAN_FRONTEND=noninteractive && \
  apt-get update && \
  apt-get install -y \
    tzdata \
    qtbase5-dev \
    g++ \
    cmake \
    make \
    ninja-build \
    libevent-dev \
    libssl-dev \
    qtdeclarative5-dev \
    qtquickcontrols2-5-dev \
    qtmultimedia5-dev \
    qttools5-dev \
    qttools5-dev-tools \
    qt5-qmake \
    qtchooser \
    qml-module-qtquick2 \
    qml-module-qtquick-controls \
    qml-module-qtquick-controls2 \
    qml-module-qtquick-layouts \
    qml-module-qtquick-dialogs \
    qml-module-qtgraphicaleffects \
    qml-module-qtmultimedia \
    qml-module-qtqml-models2 \
    qml-module-qt-labs-platform \
    qml-module-qt-labs-settings \
    libqt5svg5-dev \
    libqt5networkauth5-dev \
    libqt5multimedia5-plugins \
    libsecret-1-dev \
    libssl-dev \
    git \
    wget \
    file \
    zip
RUN rm -rf /var/lib/apt/lists/*

ENV SOURCE_DATE_EPOCH=1604304302
ENV LC_ALL=C
ENV TZ=UTC

# Build & Install qtkeychain
ARG QTKEYCHAIN_VERSION=0.15.0
RUN set -ex; \
  git clone https://github.com/frankosterfeld/qtkeychain -b $QTKEYCHAIN_VERSION --depth=1 && \
  cd qtkeychain && \
  cmake -S . -B build -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Release && \
  cmake --build build --parallel $(nproc) && \
  cmake --install build && \
  rm -rf qtkeychain

# Build & Install Olm
ARG OLM_VERSION=3.2.16
RUN set -ex; \
  git clone https://gitlab.matrix.org/matrix-org/olm -b $OLM_VERSION --depth=1 && \
  cd olm && \
  cmake -S . -B build -DCMAKE_POLICY_VERSION_MINIMUM=3.5 -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Release -DOLM_TESTS=Off && \
  cmake --build build --parallel $(nproc) && \
  cmake --install build && \
  rm -rf olm

# Install boosts
ARG BOOST_VERSION=1.81.0
ARG BOOST_TAR_VERSION=1_81_0
RUN set -ex; \
  wget https://archives.boost.io/release/${BOOST_VERSION}/source/boost_${BOOST_TAR_VERSION}.tar.gz && \
  tar -xvzf boost_${BOOST_TAR_VERSION}.tar.gz && \
  rm boost_${BOOST_TAR_VERSION}.tar.gz
ENV BOOST_ROOT=/boost_${BOOST_TAR_VERSION}

# Download HWI
ARG HWI_VERSION=3.1.0
RUN set -ex; \
  wget https://github.com/bitcoin-core/HWI/releases/download/${HWI_VERSION}/hwi-${HWI_VERSION}-linux-${ARCH}.tar.gz && \
  tar -xvzf hwi-${HWI_VERSION}-linux-${ARCH}.tar.gz && \
  rm hwi-${HWI_VERSION}-linux-${ARCH}.tar.gz && \
  rm hwi-qt || true

# Install CQtDeployer
ARG CQTDEPLOYER_VERSION=1.6.2365
ARG CQTDEPLOYER_COMMIT=7cce7f3
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

# Install appimagetool
RUN set -ex; \
  wget https://github.com/AppImage/appimagetool/releases/download/continuous/appimagetool-${ARCH}.AppImage && \
  chmod u+x appimagetool-${ARCH}.AppImage && \
  wget https://github.com/AppImage/type2-runtime/releases/download/continuous/runtime-${ARCH}
