FROM ubuntu:24.04

RUN apt-get update;
RUN apt-get install -y \
  cmake \
  g++ \
  make \
  ninja-build \
  libevent-dev \
  qtbase5-dev \
  qtdeclarative5-dev \
  qtquickcontrols2-5-dev \
  qtmultimedia5-dev \
  qttools5-dev \
  qttools5-dev-tools \
  qt5-qmake \
  qtchooser \
  libqt5svg5-dev \
  libqt5networkauth5-dev \
  libsecret-1-dev

RUN apt-get install -y git


# Build & Install qtkeychain
ENV QTKEYCHAIN_VERSION=0.15.0

RUN set -ex; \
  apt install -y mingw-w64

ENV TOOLCHAIN_PREFIX=x86_64-w64-mingw32
ENV CMAKE_C_COMPILER=${TOOLCHAIN_PREFIX}-gcc
ENV CMAKE_CXX_COMPILER=${TOOLCHAIN_PREFIX}-g++
ENV CMAKE_RC_COMPILER=${TOOLCHAIN_PREFIX}-windres
ENV CMAKE_SYSTEM_NAME=Windows
ENV CMAKE_SYSTEM_PROCESSOR=x86_64
ENV CMAKE_FIND_ROOT_PATH=/usr/${TOOLCHAIN_PREFIX}
ENV CMAKE_FIND_ROOT_PATH_MODE_PROGRAM=NEVER
ENV CMAKE_FIND_ROOT_PATH_MODE_LIBRARY=ONLY
ENV CMAKE_FIND_ROOT_PATH_MODE_INCLUDE=ONLY
ENV CC=${TOOLCHAIN_PREFIX}-gcc
ENV CXX=${TOOLCHAIN_PREFIX}-g++
ENV AR=${TOOLCHAIN_PREFIX}-ar
ENV LD=${TOOLCHAIN_PREFIX}-ld
ENV STRIP=${TOOLCHAIN_PREFIX}-strip
ENV WINDRES=${TOOLCHAIN_PREFIX}-windres
ENV RANLIB=${TOOLCHAIN_PREFIX}-ranlib
ENV NM=${TOOLCHAIN_PREFIX}-nm

RUN set -ex; \
  git clone https://github.com/frankosterfeld/qtkeychain -b $QTKEYCHAIN_VERSION && \
  cd qtkeychain; \
  cmake -S . -B build -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Release && \
  cmake --build build -- -j$(nproc) && \
  cmake --install build

# Build & Install Olm
ENV OLM_VERSION=3.2.16
RUN set -ex; \
  git clone https://gitlab.matrix.org/matrix-org/olm -b $OLM_VERSION;

RUN set -ex; \
  cd olm && \
  cmake -S . -B build -DCMAKE_POLICY_VERSION_MINIMUM=3.5 -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Release && \
  cmake --build build -- -j$(nproc) && \
  cmake --install build

# Install boosts
RUN set -ex; \
  wget https://archives.boost.io/release/1.81.0/source/boost_1_81_0.tar.gz && \
  tar -xvzf boost_1_81_0.tar.gz

ENV BOOST_ROOT=/root/boost_1_81_0

# Build openssl
#RUN set -ex; \
#  cd /nunchuk-desktop/contrib/libnunchuk/contrib/openssl && \
#  ./Configure --prefix="$PWD/lib" mingw64 no-shared && \
#  make -j$(nproc) && \
#  make install_dev
#./Configure --prefix="$PWD/lib" mingw64 shared --cross-compile-prefix=x86_64-w64-mingw32-

# Build nunchuk
#cd /nunchuk-desktop
#mkdir build && cd build
#cmake .. -DCMAKE_BUILD_TYPE=Release -DBOOST_ROOT=/boost_1_81_0
