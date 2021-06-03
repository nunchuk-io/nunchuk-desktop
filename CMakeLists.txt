cmake_minimum_required( VERSION 3.1 )
set(CMAKE_OSX_DEPLOYMENT_TARGET "10.9" CACHE STRING "Minimum OS X deployment version")
project(nunchuck-client-qt VERSION 0.1.0)
set( CMAKE_CXX_STANDARD 11 )
set( CMAKE_CXX_STANDARD_REQUIRED ON)
set( CMAKE_INCLUDE_CURRENT_DIR ON )
set( CMAKE_AUTOMOC ON )
set( CMAKE_AUTORCC ON )
set( CMAKE_AUTOUIC ON )

if(WIN32)
    set(VCPKG_TARGET_ARCHITECTURE x64)
    set(VCPKG_CRT_LINKAGE static)
    set(VCPKG_LIBRARY_LINKAGE static)
    set(VCPKG_BUILD_TYPE Release)
endif()

# NO PIE memory
if(UNIX AND NOT APPLE)
    set ( CMAKE_CXX_FLAGS "-no-pie -s" )
endif()

if( NOT CMAKE_BUILD_TYPE )
    set( CMAKE_BUILD_TYPE Debug )
endif()

add_definitions ( -Wall )
add_definitions ( -DQT_USE_QSTRINGBUILDER )

set( DQLIMIT_ONE OFF )
set( QUSING_STUB_API OFF )

if(DQLIMIT_ONE)
    add_definitions ( -DENABLE_LIMIT )
endif(QUSING_STUB_API)

if(QUSING_STUB_API)
    add_definitions ( -fno-stack-protector )
    add_definitions ( -DUSING_STUB_API )
endif(QUSING_STUB_API)

find_package(Qt5 REQUIRED Qml Quick Core Gui Multimedia)
if(APPLE)
    find_package(Iconv)
endif()

set(QAppEngine_SRCS
    QAppEngine/QOutlog/QOutlog.cpp
    QAppEngine/QQuickViewer/QQuickViewer.cpp
    QAppEngine/QQuickViewer/PopupManager/QPopupManager.cpp
    QAppEngine/QQuickViewer/ScreenManager/QScreenManager.cpp
    QAppEngine/QQuickViewer/ScreenManager/QScreenQueue.cpp
    )

set(QAppEngine_MOCS
    QAppEngine/QQuickViewer/QQuickViewer.h
    QAppEngine/QQuickViewer/PopupManager/QPopupManager.h
    QAppEngine/QQuickViewer/ScreenManager/QScreenQueue.h
    )
QT5_WRAP_CPP(MOCS_APPENGINE ${QAppEngine_MOCS})

set(Views_SRCS
    Views/STATE_ID_ROOT.cpp
    Views/STATE_ID_SCR_ADD_HARDWARE_SIGNER.cpp
    Views/STATE_ID_SCR_SOFTWARE_SIGNER_CONFIGURATION.cpp
    Views/STATE_ID_SCR_RECOVER_SOFTWARE_SIGNER.cpp
    Views/STATE_ID_SCR_CREATE_NEW_SEED.cpp
    Views/STATE_ID_SCR_ADD_NEW_SOFTWARE_SIGNER.cpp
    Views/STATE_ID_SCR_ADD_NEW_SIGNER.cpp
    Views/STATE_ID_SCR_ADD_NEW_SIGNER_TO_WALLET.cpp
    Views/STATE_ID_SCR_ADD_MASTER_SIGNER_RESULT.cpp
    Views/STATE_ID_SCR_ADD_REMOTE_SIGNER_RESULT.cpp
    Views/STATE_ID_SCR_ADD_HARDWARE_SIGNER_TO_WALLET.cpp
    Views/STATE_ID_SCR_ADD_WALLET.cpp
    Views/STATE_ID_SCR_ADD_WALLET_CONFIRMATION.cpp
    Views/STATE_ID_SCR_ADD_WALLET_SIGNER_CONFIGURATION.cpp
    Views/STATE_ID_SCR_APP_SETTINGS.cpp
    Views/STATE_ID_SCR_CONSOLIDATE_OUTPUT.cpp
    Views/STATE_ID_SCR_CREATE_TRANSACTION.cpp
    Views/STATE_ID_SCR_EXPORT_WALLET.cpp
    Views/STATE_ID_SCR_HOME.cpp
    Views/STATE_ID_SCR_LOGIN.cpp
    Views/STATE_ID_SCR_MASTER_SIGNER_INFO.cpp
    Views/STATE_ID_SCR_RECEIVE.cpp
    Views/STATE_ID_SCR_REMOTE_SIGNER_INFO.cpp
    Views/STATE_ID_SCR_SEND.cpp
    Views/STATE_ID_SCR_TRANSACTION_HISTORY.cpp
    Views/STATE_ID_SCR_TRANSACTION_INFO.cpp
    Views/STATE_ID_SCR_UTXOS.cpp
    Views/STATE_ID_SCR_UTXO_OUTPUT.cpp
    Views/STATE_ID_SCR_WALLET_CHANGE_ADDRESSES.cpp
    Views/STATE_ID_SCR_WALLET_INFO.cpp
    Views/STATE_ID_SCR_INPUT_PIN.cpp
    Views/STATE_ID_SCR_INPUT_PASSPHRASE.cpp
    )

set(Views_MOCS
    Views/Common/ViewsEnums.h
    )
QT5_WRAP_CPP(MOCS_VIEWS ${Views_MOCS})

set(${PROJECT_NAME}_SRCS
    contrib/nayuki/QrCode.cpp
    Models/AppSetting.cpp
    Models/AppModel.cpp
    Models/DeviceModel.cpp
    Models/Localization.cpp
    Models/MasterSignerModel.cpp
    Models/SingleSignerModel.cpp
    Models/TransactionModel.cpp
    Models/UTXOModel.cpp
    Models/WalletModel.cpp
    Models/Worker.cpp
    Models/QWarningMessage.cpp
    ifaces/QRCodeItem.cpp
    ifaces/QRGenerator.cpp
    ifaces/bridgeifaces.cpp
    ifaces/nunchuckstub.cpp
    ifaces/nunchuckiface.cpp
    ifaces/nunchucklistener.cpp
    ifaces/qUtils.cpp
    main.cpp
    )

set(${PROJECT_NAME}_RSCS
    qml.qrc
    fonts_Lato.qrc
    fonts_Montserrat.qrc
#    localization/localization.qrc
    )

QT5_ADD_RESOURCES(RSCS ${${PROJECT_NAME}_RSCS})

set(${PROJECT_NAME}_INC_PATH
    QAppEngine/QOutlog
    QAppEngine/QQuickViewer
    QAppEngine/QQuickViewer/ScreenManager
    QAppEngine/QQuickViewer/PopupManager
    QAppEngine/QQuickViewer/Common
    Views/Common
    Views
    ifaces
    Models
    contrib/libnunchuk/src
    )
include_directories(${${PROJECT_NAME}_INC_PATH})

set(${PROJECT_NAME}_MOCS
    Models/AppModel.h
    Models/DeviceModel.h
    Models/MasterSignerModel.h
    Models/SingleSignerModel.h
    Models/TransactionModel.h
    Models/UTXOModel.h
    Models/WalletModel.h
    Models/Worker.h
    ifaces/QRCodeItem.h
    ifaces/bridgeifaces.h
    )
QT5_WRAP_CPP(MOCS ${${PROJECT_NAME}_MOCS})

set( QRCODE_SCANNER ON )
if(QRCODE_SCANNER)
    SET(QZXING_MULTIMEDIA ON)
    SET(QZXING_USE_QML ON)
    add_definitions( -DQZXING_QML )
    add_definitions( -DQZXING_MULTIMEDIA )
    add_definitions( -DENABLE_DECODER_QR_CODE )
    add_subdirectory(contrib/qzxing/src qzxing)
endif(QRCODE_SCANNER)

add_subdirectory(contrib/libnunchuk lib EXCLUDE_FROM_ALL)

add_executable ( ${PROJECT_NAME} WIN32 ${QAppEngine_SRCS} ${Views_SRCS} ${${PROJECT_NAME}_SRCS} ${UIS} ${RSCS} ${MOCS_APPENGINE} ${MOCS_VIEWS} ${MOCS})

if(WIN32)
    target_compile_options("${PROJECT_NAME}" PUBLIC /MT)
    target_link_options("${PROJECT_NAME}" PUBLIC /INCREMENTAL:NO /NODEFAULTLIB:MSVCRT)
endif()

target_link_libraries (${PROJECT_NAME} Qt5::Core Qt5::Gui Qt5::Qml Qt5::Quick )

if(UNIX AND NOT APPLE)
    target_link_libraries("${PROJECT_NAME}" -Wl,--start-group nunchuk)
else()
    target_link_libraries("${PROJECT_NAME}" nunchuk)
endif()

if(QRCODE_SCANNER)
    if(APPLE)
        target_link_libraries (${PROJECT_NAME} qzxing Iconv::Iconv)
    else()
        target_link_libraries (${PROJECT_NAME} qzxing)
    endif()

    target_link_libraries (${PROJECT_NAME} Qt5::Multimedia )
endif(QRCODE_SCANNER)

if (APPLE AND CMAKE_BUILD_TYPE MATCHES Release)
    SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Info.plist)
endif()
