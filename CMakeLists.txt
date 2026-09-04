if(WIN32)
    cmake_minimum_required( VERSION 3.5 )
else()
    cmake_minimum_required( VERSION 3.16 )
endif()
if(CMAKE_VERSION VERSION_GREATER_EQUAL "3.25")
    set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
endif()

# Let CMake select MSVC's runtime through MSVC_RUNTIME_LIBRARY instead of
# injecting legacy /MD or /MT flags into CMAKE_CXX_FLAGS.
if(POLICY CMP0091)
    cmake_policy(SET CMP0091 NEW)
endif()

if(UNIX AND APPLE)
    project(Nunchuk VERSION 2.6.7)
else()
    project(nunchuk-qt VERSION 2.6.7)
endif()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_AUTOMOC ON )
set(CMAKE_AUTORCC ON )
set(CMAKE_AUTOUIC ON )

# nunchuk
if(MSVC AND WIN32)
    # Qt's official MSVC binaries use the dynamic CRT. Keep every target in
    # this process on the same runtime to avoid allocator/ABI mismatches.
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
    set(VCPKG_TARGET_ARCHITECTURE x64)
    set(VCPKG_CRT_LINKAGE dynamic)
    set(VCPKG_LIBRARY_LINKAGE static)
    set(VCPKG_BUILD_TYPE Release)
    set(Boost_USE_STATIC_LIBS OFF)

    # For embeded
    set(HAVE_EVHTTP_CONNECTION_GET_PEER_CONST_CHAR ON)
endif()

if (WIN32)
    add_definitions("-DWIN32")
    add_definitions("-DBOOST_PROCESS_USE_STD_FS")
endif()
add_subdirectory(contrib/libnunchuk)


set(CMAKE_AUTOMOC ON )
set(CMAKE_AUTORCC ON )
set(CMAKE_AUTOUIC ON )

# ZXing Opt
set(ZXING_UNIT_TESTS OFF)
set(ZXING_BLACKBOX_TESTS OFF)
set(ZXING_EXAMPLES OFF)
set(BUILD_SHARED_LIBS OFF)
# find Qt6 Widgets for ZXing if required
find_package(Qt6 COMPONENTS Widgets REQUIRED)
add_subdirectory(contrib/zxing)

# Quotient

find_package(Qt6 COMPONENTS CorePrivate REQUIRED)
set(Quotient_ENABLE_E2EE ON CACHE INTERNAL "" FORCE)
set(BUILD_TESTING OFF)
add_subdirectory(contrib/quotient)

if ((UNIX AND NOT APPLE) OR WIN32)
    # NOTE: Do NOT add_compile_definitions(BOOST_ASIO_NO_DEPRECATED=...) here.
    # Boost.Asio's io_service.hpp gates the boost::asio::io_service backward-compat
    # typedef on `#if !defined(BOOST_ASIO_NO_DEPRECATED)` - it only checks whether the
    # macro is defined at all, not its value. Defining it (even as 0) strips io_service,
    # which src/backend/synchronizer.h in libnunchuk still relies on.
    message("---------------------------------> Backward compatibility: keeping boost::asio::io_service (BOOST_ASIO_NO_DEPRECATED left undefined)")
endif()

if( NOT CMAKE_BUILD_TYPE )
    set( CMAKE_BUILD_TYPE Debug )
endif()

# Definations
option(LIMIT_SW_SIGNER "Enable Software signer" OFF)
# Social OAuth uses the system browser. WebEngine remains required by CAPTCHA.
option(USING_WEBENGINE "Build with WebEngine (currently required by CAPTCHA)" ON)
option(USING_AUTO_FEE  "Enable Auto fee selection" ON)

if(USING_AUTO_FEE)
    message(STATUS "---------------------------------> USING USING_AUTO_FEE")
    add_definitions (-DENABLE_AUTO_FEE)
else()
    message(STATUS "---------------------------------> DISABLE USING_AUTO_FEE")
endif()

if(LIMIT_SW_SIGNER)
    message(STATUS "---------------------------------> USING LIMIT_SW_SIGNER")
    add_definitions (-DENABLE_LIMIT)
else()
    message(STATUS "---------------------------------> DISABLE LIMIT_SW_SIGNER")
endif()
add_definitions (-DSIGNET_SUPPORT)

# Dependences mandatory (Qt6)
find_package(Qt6 COMPONENTS Core CorePrivate Qml Quick Gui Multimedia Concurrent Svg PrintSupport Network NetworkAuth Sql QuickControls2 REQUIRED)

if(USING_WEBENGINE)
    message(STATUS "---------------------------------> USING WEBENGINE")
    find_package(Qt6 COMPONENTS WebEngineWidgets REQUIRED)
    if (Qt6WebEngineWidgets_FOUND)
        message( STATUS "Using WebEngineWidgets ${Qt6WebEngineWidgets_VERSION} at ${Qt6WebEngineWidgets_DIR}")
    endif()
else()
    message(STATUS "---------------------------------> DISABLE WEBENGINE")
endif()


# Dependences additional
# NOTE: package name for keychain binding for Qt6 may differ depending on how it's installed;
# try find_package(Qt6Keychain CONFIG REQUIRED) or adapt to your environment.
find_package(Qt6Keychain REQUIRED)
if (Qt6Keychain_FOUND)
    message( STATUS "Using Qt Keychain ${Qt6Keychain_VERSION} at ${Qt6Keychain_DIR}")
endif()

if(WIN32)
    find_package(Boost REQUIRED)
else()
    find_package(Boost REQUIRED )
endif()

# Include path
set(Nunchuk_INC_PATH
    ${PROJECT_SOURCE_DIR}
    QAppEngine/QOutlog
    QAppEngine/QEventProcessor
    QAppEngine/QEventProcessor/QScreenDelegate
    QAppEngine/QEventProcessor/QPopupDelegate
    QAppEngine/QEventProcessor/Common
    Views
    Views/Common
    ifaces
    Models
    Models/Printer
    RegisterTypes
    QRScanner
    QRScanner/private
    core
    features
    app
)

# Source
file(GLOB_RECURSE SOURCES_IFACES	CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/ifaces/*.cpp		${CMAKE_SOURCE_DIR}/ifaces/*.h)
file(GLOB_RECURSE SOURCES_MODELS	CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/Models/*.cpp		${CMAKE_SOURCE_DIR}/Models/*.h)
file(GLOB_RECURSE SOURCES_QAPPENGINE	CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/QAppEngine/*.cpp		${CMAKE_SOURCE_DIR}/QAppEngine/*.h)
file(GLOB_RECURSE SOURCES_QSCANNER	CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/QRScanner/*.cpp		${CMAKE_SOURCE_DIR}/QRScanner/*.h)
file(GLOB_RECURSE SOURCES_REGISTERTYPE	CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/RegisterTypes/*.cpp	${CMAKE_SOURCE_DIR}/RegisterTypes/*.h)
file(GLOB_RECURSE SOURCES_VIEWS		CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/Views/*.cpp		${CMAKE_SOURCE_DIR}/Views/*.h)
file(GLOB_RECURSE FEDERATEDAUTH		CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/FederatedAuth/*.cpp	${CMAKE_SOURCE_DIR}/FederatedAuth/*.h)
file(GLOB_RECURSE SOURCES_CORE		CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/core/*.cpp                ${CMAKE_SOURCE_DIR}/core/*.h)
file(GLOB_RECURSE SOURCES_FEATURES	CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/features/*.cpp            ${CMAKE_SOURCE_DIR}/features/*.h)
file(GLOB_RECURSE SOURCES_APP           CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/app/*.cpp                 ${CMAKE_SOURCE_DIR}/app/*.h)

# Qt6 resources -- use qt_add_resources
qt_add_resources(SOURCE_RSCS
    qml.qrc
    fonts_Lato.qrc
    fonts_Montserrat.qrc
)

# Add include path
include_directories(${Nunchuk_INC_PATH})
include_directories("${PROJECT_SOURCE_DIR}/contrib/quotient")
include_directories("${PROJECT_SOURCE_DIR}/contrib/quotient/Quotient")
include_directories(${Qt6Keychain_INCLUDE_DIRS})

# Executable
# qt_add_executable replaces add_executable for Qt6 projects.
# It automatically calls qt_finalize_executable which:
#   - processes .prl files to find and link static plugins (including
#     QDarwinCameraPermissionPlugin on macOS) without needing explicit CMake targets
#   - generates Q_IMPORT_PLUGIN calls for all required static plugins
#   - sets MACOSX_BUNDLE on Apple, WIN32 on Windows automatically
if(APPLE)
    set(APP_ICON_MACOSX ${CMAKE_CURRENT_SOURCE_DIR}/Icon.icns)
    set_source_files_properties(${APP_ICON_MACOSX} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
    qt_add_executable(${PROJECT_NAME} ${SOURCES_APP} ${SOURCES_IFACES} ${SOURCES_MODELS} ${SOURCES_QAPPENGINE} ${SOURCES_QSCANNER} ${SOURCES_REGISTERTYPE} ${SOURCES_VIEWS} ${FEDERATEDAUTH} ${SOURCES_CORE} ${SOURCES_FEATURES} ${SOURCE_RSCS} ${APP_ICON_MACOSX})
elseif(UNIX AND NOT APPLE)
    qt_add_executable(${PROJECT_NAME} ${SOURCES_APP} ${SOURCES_IFACES} ${SOURCES_MODELS} ${SOURCES_QAPPENGINE} ${SOURCES_QSCANNER} ${SOURCES_REGISTERTYPE} ${SOURCES_VIEWS} ${FEDERATEDAUTH} ${SOURCES_CORE} ${SOURCES_FEATURES} ${SOURCE_RSCS})
else()
    qt_add_executable(${PROJECT_NAME} WIN32 ${SOURCES_APP} ${SOURCES_IFACES} ${SOURCES_MODELS} ${SOURCES_QAPPENGINE} ${SOURCES_QSCANNER} ${SOURCES_REGISTERTYPE} ${SOURCES_VIEWS} ${FEDERATEDAUTH} ${SOURCES_CORE} ${SOURCES_FEATURES} ${SOURCE_RSCS})
endif()

if(WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        PROJECT_VERSION_MAJOR=${PROJECT_VERSION_MAJOR}
        PROJECT_VERSION_MINOR=${PROJECT_VERSION_MINOR}
        PROJECT_VERSION_PATCH=${PROJECT_VERSION_PATCH}
    )
    target_sources(${PROJECT_NAME} PRIVATE res/nunchuk.rc)
endif()

# Target properties
if(APPLE)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_ICON_FILE "Icon"
        MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/Info.plist"
    )
endif()

# Compile options
target_compile_definitions(${PROJECT_NAME} PRIVATE USE_KEYCHAIN)
target_compile_options(${PROJECT_NAME} PRIVATE -Wall)
if (NOT WIN32)
    target_compile_options(${PROJECT_NAME} PRIVATE -Werror=return-type)
endif()

message(STATUS "---------------------------------> CMAKE_SYSTEM_NAME: ${CMAKE_SYSTEM_NAME}")
message(STATUS "---------------------------------> CMAKE_SYSTEM_PROCESSOR: ${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS "---------------------------------> CMAKE_OSX_ARCHITECTURES: ${CMAKE_OSX_ARCHITECTURES}")
message(STATUS "---------------------------------> C++ Compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "---------------------------------> C++ Compiler ID: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "---------------------------------> C++ Compiler Version: ${CMAKE_CXX_COMPILER_VERSION}")
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DENABLE_OUTLOG)
    # add_definitions(-DENABLE_BACKTRACE)
    # add_definitions(-DENABLE_THREAD_MONITOR)

elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    # add_definitions(-DENABLE_OUTLOG)
    # Apply flags for Release with debug info
else()
    # Apply flags for else with debug info
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    message(STATUS "---------------------------------> MSVC")
    target_compile_options(${PROJECT_NAME} PUBLIC /std:c++23)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
    message(STATUS "---------------------------------> AppleClang")
    # target_compile_options(${PROJECT_NAME} PUBLIC -std=c++23)
else()
    message(STATUS "---------------------------------> GCC and either")
    target_compile_options(${PROJECT_NAME} PUBLIC -std=c++23)
endif()

# Target link options
if(WIN32 AND MSVC)
    set(NUNCHUK_WINDOWS_MANIFEST
        "${CMAKE_CURRENT_SOURCE_DIR}/res/nunchuk.exe.manifest")
    set_property(TARGET ${PROJECT_NAME} PROPERTY
        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
    set_property(TARGET ${PROJECT_NAME} APPEND PROPERTY
        LINK_DEPENDS "${NUNCHUK_WINDOWS_MANIFEST}")
    target_link_options(${PROJECT_NAME} PRIVATE
        /INCREMENTAL:NO
        /MANIFEST:EMBED
        "/MANIFESTINPUT:${NUNCHUK_WINDOWS_MANIFEST}"
    )
endif()

# Target link libraries (Qt6)
target_link_libraries (${PROJECT_NAME} PUBLIC
    Qt6::Core
    Qt6::CorePrivate
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick
    Qt6::Concurrent
    Qt6::Svg
    Qt6::PrintSupport
    Qt6::Multimedia
    Qt6::Network
    Qt6::NetworkAuth
    Qt6::Sql
    Qt6::Widgets
    Qt6::QuickControls2
)
if(USING_WEBENGINE)
    target_link_libraries (${PROJECT_NAME} PUBLIC Qt6::WebEngineWidgets)
endif()


target_link_libraries (${PROJECT_NAME} PUBLIC nunchuk)
target_include_directories("${PROJECT_NAME}" PUBLIC
    "${PROJECT_SOURCE_DIR}/contrib/libnunchuk/include"
    ${Boost_INCLUDE_DIRS}
)
target_link_libraries (${PROJECT_NAME} PUBLIC ${Qt6Keychain_LIBRARIES})
target_include_directories("${PROJECT_NAME}" PUBLIC "${Qt6Keychain_INCLUDE_DIRS}")

target_link_libraries (${PROJECT_NAME} PUBLIC QuotientQt6)
target_include_directories("${PROJECT_NAME}" PUBLIC "${PROJECT_SOURCE_DIR}/contrib/quotient/lib" )

target_link_libraries (${PROJECT_NAME} PUBLIC ZXing)
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    message(STATUS "---------------------------------> MSVC -> link SQLite::SQLite3")
        target_link_libraries (${PROJECT_NAME} PRIVATE SQLite::SQLite3)
endif()
