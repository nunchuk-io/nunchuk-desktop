cmake_minimum_required( VERSION 3.1 )
if(UNIX AND APPLE)
    project(Nunchuk VERSION 1.9.49)
else()
    project(nunchuk-qt VERSION 1.9.49)
endif()

# nunchuk
if(MSVC AND WIN32)
    set(CMAKE_CXX_FLAGS_RELEASE "/MT")
    set(CMAKE_CXX_FLAGS_DEBUG "/MTd")
    set(VCPKG_TARGET_ARCHITECTURE x64)
    set(VCPKG_CRT_LINKAGE static)
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

# Compiler Opt
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_AUTOMOC ON )
set(CMAKE_AUTORCC ON )
set(CMAKE_AUTOUIC ON )

if(DEFINED ENV{OAUTH_CLIENT_ID})
    set(OAUTH_CLIENT_ID $ENV{OAUTH_CLIENT_ID})
else()
    set(OAUTH_CLIENT_ID "") # fallback to runtime
endif()

if(DEFINED ENV{OAUTH_CLIENT_SECRET})
    set(OAUTH_CLIENT_SECRET $ENV{OAUTH_CLIENT_SECRET})
else()
    set(OAUTH_CLIENT_SECRET "") # fallback to runtime
endif()

if(DEFINED ENV{OAUTH_REDIRECT_URI})
    set(OAUTH_REDIRECT_URI $ENV{OAUTH_REDIRECT_URI})
else()
    set(OAUTH_REDIRECT_URI "") # fallback to runtime
endif()

add_definitions(-DOAUTH_CLIENT_ID="${OAUTH_CLIENT_ID}")
add_definitions(-DOAUTH_CLIENT_SECRET="${OAUTH_CLIENT_SECRET}")
add_definitions(-DOAUTH_REDIRECT_URI="${OAUTH_REDIRECT_URI}")

# ZXing Opt
set(ZXING_UNIT_TESTS OFF)
set(ZXING_BLACKBOX_TESTS OFF)
set(ZXING_EXAMPLES OFF)
set(BUILD_SHARED_LIBS OFF)
find_package(Qt5 COMPONENTS Widgets REQUIRED)
add_subdirectory(contrib/zxing)

# Quotient
set(Quotient_ENABLE_E2EE ON CACHE INTERNAL "" FORCE)
set(BUILD_TESTING OFF)
add_subdirectory(contrib/quotient)

if ((UNIX AND NOT APPLE) OR WIN32)
    message("---------------------------------> Backward compatibility BOOST_ASIO_NO_DEPRECATED=0")
    add_compile_definitions(BOOST_ASIO_NO_DEPRECATED=0)
endif()

if( NOT CMAKE_BUILD_TYPE )
    set( CMAKE_BUILD_TYPE Debug )
endif()

# Definations
option(LIMIT_SW_SIGNER "Enable Software signer" OFF)
option(USING_WEBENGINE "Enable WebEngine" ON)
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

# Dependences mandatory
find_package(Qt5 COMPONENTS Qml Quick Core Gui Multimedia Concurrent Svg PrintSupport Network REQUIRED)
if(USING_WEBENGINE)
    message(STATUS "---------------------------------> USING WEBENGINE")
    find_package(Qt5 COMPONENTS Widgets WebEngineWidgets REQUIRED)
    if (Qt5WebEngineWidgets_FOUND)
        message( STATUS "Using WebEngineWidgets ${Qt5WebEngineWidgets_VERSION} at ${Qt5WebEngineWidgets_DIR}")
    endif()
    add_definitions (-DENABLE_WEBVIEW_SIGIN)
else()
    message(STATUS "---------------------------------> DISABLE WEBENGINE")
endif()


# Dependences additional
find_package(Qt5Keychain REQUIRED)
if (Qt5Keychain_FOUND)
    message( STATUS "Using Qt Keychain ${Qt5Keychain_VERSION} at ${Qt5Keychain_DIR}")
endif()

if(WIN32)
    find_package(Boost REQUIRED)
else()
    find_package(Boost REQUIRED )
endif()

FIND_PACKAGE(OpenGL REQUIRED)
if(OPENGL_FOUND) # never true, but printed as true
    message(STATUS "---------------------------------> OPENGL: " ${OPENGL_LIBRARIES})
    message(STATUS "---------------------------------> OPENGL: " ${OPENGL_INCLUDE_DIR})
    link_directories(${OPENGL_LIBRARIES})
    include_directories(${OPENGL_INCLUDE_DIR})
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
)

# Source
file(GLOB_RECURSE SOURCES_IFACES	CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/ifaces/*.cpp		${CMAKE_SOURCE_DIR}/ifaces/*.h)
file(GLOB_RECURSE SOURCES_MODELS	CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/Models/*.cpp		${CMAKE_SOURCE_DIR}/Models/*.h)
file(GLOB_RECURSE SOURCES_QAPPENGINE	CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/QAppEngine/*.cpp		${CMAKE_SOURCE_DIR}/QAppEngine/*.h)
file(GLOB_RECURSE SOURCES_QSCANNER	CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/QRScanner/*.cpp		${CMAKE_SOURCE_DIR}/QRScanner/*.h)
file(GLOB_RECURSE SOURCES_REGISTERTYPE	CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/RegisterTypes/*.cpp	${CMAKE_SOURCE_DIR}/RegisterTypes/*.h)
file(GLOB_RECURSE SOURCES_VIEWS		CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/Views/*.cpp		${CMAKE_SOURCE_DIR}/Views/*.h)
file(GLOB_RECURSE FEDERATEDAUTH		CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/FederatedAuth/*.cpp	${CMAKE_SOURCE_DIR}/FederatedAuth/*.h)

QT5_ADD_RESOURCES(SOURCE_RSCS
    qml.qrc
    fonts_Lato.qrc
    fonts_Montserrat.qrc
)

# Add include path
include_directories(${Nunchuk_INC_PATH})
include_directories("${PROJECT_SOURCE_DIR}/contrib/quotient/lib")
include_directories(${QTKEYCHAIN_INCLUDE_DIRS})

# Executable
if(APPLE)
    # And this part tells CMake where to find and install the file itself
    set(APP_ICON_MACOSX ${CMAKE_CURRENT_SOURCE_DIR}/Icon.icns)
    set_source_files_properties(${APP_ICON_MACOSX} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
    add_executable (${PROJECT_NAME} MACOSX_BUNDLE main.cpp ${SOURCES_IFACES} ${SOURCES_MODELS} ${SOURCES_QAPPENGINE} ${SOURCES_QSCANNER} ${SOURCES_REGISTERTYPE} ${SOURCES_VIEWS} ${FEDERATEDAUTH} ${SOURCE_RSCS} ${APP_ICON_MACOSX})
elseif(UNIX AND NOT APPLE)
    add_executable (${PROJECT_NAME} main.cpp ${SOURCES_IFACES} ${SOURCES_MODELS} ${SOURCES_QAPPENGINE} ${SOURCES_QSCANNER} ${SOURCES_REGISTERTYPE} ${SOURCES_VIEWS} ${FEDERATEDAUTH} ${SOURCE_RSCS})
else()
    add_executable (${PROJECT_NAME} WIN32 main.cpp ${SOURCES_IFACES} ${SOURCES_MODELS} ${SOURCES_QAPPENGINE} ${SOURCES_QSCANNER} ${SOURCES_REGISTERTYPE} ${SOURCES_VIEWS} ${FEDERATEDAUTH} ${SOURCE_RSCS})
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
message(STATUS "---------------------------------> OAUTH_CLIENT_ID = ${OAUTH_CLIENT_ID}")
message(STATUS "---------------------------------> OAUTH_CLIENT_SECRET = ${OAUTH_CLIENT_SECRET}")
message(STATUS "---------------------------------> OAUTH_REDIRECT_URI = ${OAUTH_REDIRECT_URI}")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DENABLE_OUTLOG)
    # add_definitions(-DENABLE_BACKTRACE)
    # add_definitions(-DENABLE_THREAD_MONITOR)

elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    # Apply flags for Release with debug info
else()
    # Apply flags for else with debug info
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    message(STATUS "---------------------------------> MSVC")
    target_compile_options(${PROJECT_NAME} PUBLIC /std:c++20)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
    message(STATUS "---------------------------------> AppleClang")
    target_compile_options(${PROJECT_NAME} PUBLIC -std=c++20)
else()
    message(STATUS "---------------------------------> GCC and either")
    target_compile_options(${PROJECT_NAME} PUBLIC -std=c++20 -fno-extended-identifiers -fstack-reuse=none)
endif()

# Target link options
if(WIN32 AND MSVC)
    target_link_options(${PROJECT_NAME} PUBLIC /INCREMENTAL:NO /NODEFAULTLIB:MSVCRT)
endif()

# Target link libraries
target_link_libraries (${PROJECT_NAME} PUBLIC Qt5::Core Qt5::Gui Qt5::Qml Qt5::Quick Qt5::Concurrent Qt5::Svg Qt5::PrintSupport Qt5::Multimedia Qt5::Network)
if(USING_WEBENGINE)
    target_link_libraries (${PROJECT_NAME} PUBLIC Qt5::Widgets Qt5::WebEngineWidgets)
endif()

target_link_libraries (${PROJECT_NAME} PUBLIC nunchuk)
target_include_directories("${PROJECT_NAME}" PUBLIC
    "${PROJECT_SOURCE_DIR}/contrib/libnunchuk/include"
    ${Boost_INCLUDE_DIRS}
)
target_link_libraries (${PROJECT_NAME} PUBLIC ${QTKEYCHAIN_LIBRARIES})
target_include_directories("${PROJECT_NAME}" PUBLIC "${QTKEYCHAIN_INCLUDE_DIRS}")

target_link_libraries (${PROJECT_NAME} PUBLIC Quotient)
target_include_directories("${PROJECT_NAME}" PUBLIC "${PROJECT_SOURCE_DIR}/contrib/quotient/lib" )

target_link_libraries (${PROJECT_NAME} PUBLIC ZXing)
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    message(STATUS "---------------------------------> MSVC -> link SQLite::SQLite3")
	target_link_libraries (${PROJECT_NAME} PRIVATE SQLite::SQLite3)
endif()
