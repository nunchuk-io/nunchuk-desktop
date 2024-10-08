cmake_minimum_required( VERSION 3.1 )
#set(CMAKE_OSX_DEPLOYMENT_TARGET "10.14" CACHE STRING "Minimum OS X deployment version")
if(UNIX AND APPLE)
    project(Nunchuk VERSION 1.9.38)
else()
    project(nunchuk-qt VERSION 1.9.38)
endif()

set( CMAKE_CXX_STANDARD 20 )
set( CMAKE_CXX_STANDARD_REQUIRED ON)
set( CMAKE_INCLUDE_CURRENT_DIR ON )
set( CMAKE_AUTOMOC ON )
set( CMAKE_AUTORCC ON )
set( CMAKE_AUTOUIC ON )
set( CXX_STANDARD "-std=c++20")
if (WIN32)
    set(CXX_STANDARD "/std:c++20")
elseif (NOT APPLE)
    set( CMAKE_CXX_FLAGS "-march=native -O3")
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    add_definitions ( -DRELEASE_MODE )
endif()

if(WIN32)
    set( CMAKE_CXX_FLAGS_RELEASE "/MT" )
    set( CMAKE_CXX_FLAGS_DEBUG "/MTd" )
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

add_definitions ( -DSIGNET_SUPPORT )
add_definitions ( -Wall )

set( DQLIMIT_ONE OFF )

if(DQLIMIT_ONE)
    add_definitions ( -DENABLE_LIMIT )
endif(DQLIMIT_ONE)

find_package(Qt5 COMPONENTS Qml Quick Core Gui Multimedia Sql Concurrent Svg PrintSupport REQUIRED)

set(QAppEngine_SRCS
    QAppEngine/QOutlog/QOutlog.cpp
    QAppEngine/QOutlog/QPingThread.cpp
    QAppEngine/QOutlog/QPDFPrinter.cpp
    QAppEngine/QEventProcessor/QEventProcessor.cpp
    QAppEngine/QEventProcessor/QPopupDelegate/QPopupDelegate.cpp
    QAppEngine/QEventProcessor/QScreenDelegate/QScreenDelegate.cpp
    QAppEngine/QEventProcessor/QScreenDelegate/QScreenQueue.cpp
    )

set(QAppEngine_MOCS
    QAppEngine/QOutlog/QOutlog.h
    QAppEngine/QOutlog/QPingThread.h
    QAppEngine/QOutlog/QPDFPrinter.h
    QAppEngine/QEventProcessor/QEventProcessor.h
    QAppEngine/QEventProcessor/QPopupDelegate/QPopupDelegate.h
    QAppEngine/QEventProcessor/QScreenDelegate/QScreenQueue.h
    )
QT5_WRAP_CPP(MOCS_APPENGINE ${QAppEngine_MOCS})

set(QRScranner_SRCS
    QRScanner/QBarcodeDecoder.cpp
    QRScanner/QBarcodeFilter.cpp
    QRScanner/QBarcodeGenerator.cpp
    )

set(QRScranner_MOCS
    QRScanner/QBarcodeDecoder.h
    QRScanner/QBarcodeFilter.h
    QRScanner/QBarcodeGenerator.h
    )
QT5_WRAP_CPP(MOCS_QRSCANNER ${QRScranner_MOCS})

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
    Views/STATE_ID_SCR_UNLOCK_DB.cpp
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
    Views/STATE_ID_SCR_LOGIN_ONLINE.cpp
    Views/STATE_ID_SCR_HOME_ONLINE.cpp
    Views/STATE_ID_SCR_ONLINE_ADD_CONTACTS.cpp
    Views/STATE_ID_SCR_PENDING_REQUEST.cpp
    Views/STATE_ID_SCR_REVIEW_SHARED_WALLET.cpp
    Views/STATE_ID_SCR_CREATE_SHARED_WALLET.cpp
    Views/STATE_ID_SCR_CONFIGURE_SHARED_WALLET.cpp
    Views/STATE_ID_SCR_ASSIGN_SIGNER_TO_SHARED_WALLET.cpp
    Views/STATE_ID_SCR_SHARED_WL_DEVICE_REGISTRATION.cpp
    Views/STATE_ID_SCR_SHARED_WALLET_CONFIGURE.cpp
    Views/STATE_ID_SCR_BACKUP_SHARED_WALLET.cpp
    Views/STATE_ID_SCR_STARTING_CREATE_SIGNER.cpp
    Views/STATE_ID_TOAST_MESSAGE_DISPLAY.cpp
    Views/STATE_ID_SCR_UPDATE_PROFILE.cpp
    Views/STATE_ID_SCR_RECOVER_SHARED_WALLET.cpp
    Views/STATE_ID_SCR_CREATE_ACCOUNT.cpp
    Views/STATE_ID_SCR_CHANGE_PASSWORD.cpp
    Views/STATE_ID_SCR_PRIMARY_KEY_ACCOUNT.cpp
    Views/STATE_ID_SCR_SIGN_IN_BY_IMPORTING_THE_PRIMARY_KEY.cpp
    Views/STATE_ID_SCR_SIGN_IN_MANUALLY.cpp
    Views/STATE_ID_SCR_LOGIN_WITH_SOFTWARE_KEY.cpp
    Views/STATE_ID_SCR_REPLACE_PRIMARY_KEY.cpp
    Views/STATE_ID_SCR_PRIMARY_KEY_CONFIGURATION.cpp
    Views/STATE_ID_SCR_SIGN_IN.cpp
    Views/STATE_ID_SCR_SERVICE_SETTINGS.cpp
    Views/STATE_ID_SCR_LOCKDOWN_SUCCESS.cpp
    Views/STATE_ID_SCR_REENTER_YOUR_PASSWORD.cpp
    Views/STATE_ID_SCR_KEY_RECOVERY.cpp
    Views/STATE_ID_SCR_SELECT_YOUR_LOCKDOWN_PERIOD.cpp
    Views/STATE_ID_SCR_KEY_RECOVERY_ANSER_SECURITY_QUESTION.cpp
    Views/STATE_ID_SCR_LOCKDOWN_ANSER_SECURITY_QUESTION.cpp
    Views/STATE_ID_SCR_ENTER_BACKUP_PASSWORD.cpp
    Views/STATE_ID_SCR_LOCKDOWN_SUCCESS.cpp
    Views/STATE_ID_SCR_TAPSIGNER_RECOVERED_SUCCESS.cpp
    Views/STATE_ID_SCR_DUMMY_TRANSACTION_INFO.cpp
    Views/STATE_ID_SCR_INHERITANCE_CONFIRM_TRANSACTION.cpp
    Views/STATE_ID_SCR_INHERITANCE_TRANSACTION_DETAILS.cpp
    Views/STATE_ID_SCR_INHERITANCE_WITHDRAW_BALANCE.cpp
    Views/STATE_ID_SCR_SELECT_WALLET_CO_SIGN_POLICE.cpp
    Views/STATE_ID_SCR_EDIT_YOUR_INHERITANCE_PLAN.cpp
    Views/STATE_ID_SCR_SHARE_YOUR_SECRETS.cpp
    Views/STATE_ID_SCR_ADD_HARDWARE.cpp
    Views/STATE_ID_SCR_ADD_HARDWARE_ASK.cpp
    Views/STATE_ID_SCR_ADD_HARDWARE_EXIST.cpp
    Views/STATE_ID_SCR_DASHBOARD_ALERT_INFO.cpp
    Views/STATE_ID_SCR_HEALTH_CHECK_STARTING.cpp
    Views/STATE_ID_SCR_KEY_HEALTH_CHECK_STATUS.cpp
    Views/STATE_ID_SCR_DASHBOARD_ALERT_SUCCESS.cpp
    Views/STATE_ID_SCR_SETUP_ANSWER_SECURITY_QUESTION.cpp
    Views/STATE_ID_SCR_SETUP_SECURITY_QUESTION.cpp
    Views/STATE_ID_SCR_RECURRING_PAYMENTS.cpp
    Views/STATE_ID_SCR_ONBOARDING.cpp
    Views/STATE_ID_SCR_SELECT_SERVER.cpp
    Views/STATE_ID_SCR_CHANGE_EMAIL.cpp
    Views/STATE_ID_SCR_REPLACE_SELECT_KEY.cpp
    Views/STATE_ID_SCR_REPLACE_KEYS.cpp
    Views/STATE_ID_SCR_SIGN_IN_VIA_XPUB.cpp
    Views/STATE_ID_SCR_EDIT_MEMBERS.cpp
    )

set(Views_MOCS
    Views/Common/ViewsEnums.h
    )
QT5_WRAP_CPP(MOCS_VIEWS ${Views_MOCS})

set(${PROJECT_NAME}_SRCS
    Models/AppSetting.cpp
    Models/AppModel.cpp
    Models/ServiceSetting.cpp
    Models/ProfileSetting.cpp
    Models/AppOnlineModel.cpp
    Models/DeviceModel.cpp
    Models/Localization.cpp
    Models/MasterSignerModel.cpp
    Models/SingleSignerModel.cpp
    Models/TransactionModel.cpp
    Models/UTXOModel.cpp
    Models/WalletModel.cpp
    Models/Worker.cpp
    Models/QWarningMessage.cpp
    Models/OnBoardingModel.cpp
    ifaces/bridgeifaces.cpp
    ifaces/nunchuckiface.cpp
    ifaces/nunchucklistener.cpp
    ifaces/qUtils.cpp
    ifaces/Chats/matrixifaces.cpp
    ifaces/Chats/matrixbrigde.cpp
    ifaces/Chats/matrixlistener.cpp
    ifaces/Servers/Draco.cpp
    ifaces/Servers/QRest.cpp
    ifaces/Servers/Byzantine.cpp
    main.cpp
    Models/Chats/QConversationModel.cpp
    Models/Chats/QContactModel.cpp
    Models/Chats/ClientController.cpp
    Models/Chats/QLogginManager.cpp
    Models/Chats/QNunchukRoomModel.cpp
    Models/Chats/QNunchukMatrixEvent.cpp
    Models/Chats/QRoomWallet.cpp
    Models/Chats/QRoomTransaction.cpp
    Models/Chats/QNunchukRoomUsers.cpp
    Models/Chats/QRoomCustomEvent.cpp
    Models/Chats/QWalletSignersModel.cpp
    Models/Chats/QNunchukImageProvider.cpp
    Models/Chats/QLoggedInDeviceModel.cpp
    RegisterTypes/DashRectangle.cpp
    Models/Premiums/QUserWallets.cpp
    Models/Premiums/QGroupWallets.cpp
    Models/Premiums/QAssistedDraftWallets.cpp
    Models/Premiums/QWalletServicesTag.cpp
    Models/Premiums/QSwitchAPI.cpp
    Models/Premiums/QWalletManagement.cpp
    Models/Premiums/QGroupDashboard.cpp
    Models/Premiums/QGroupWalletHealthCheck.cpp
    Models/Premiums/QGroupWalletDummyTx.cpp
    Models/Premiums/QServerKey.cpp
    Models/Premiums/QInheritancePlan.cpp
    Models/Premiums/QKeyRecovery.cpp
    Models/Premiums/QWalletDummyTx.cpp
    Models/Premiums/QUserWalletDummyTx.cpp
    Models/Premiums/QBasePremium.cpp
    Models/Premiums/QRecurringPayment.cpp
    Models/Commons/WalletKeys.cpp
    Models/Commons/Slugs.cpp
    Models/Commons/SignInViaDummy.cpp
    Models/Commons/ReplaceKeyFreeUser.cpp
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
    QAppEngine/QEventProcessor
    QAppEngine/QEventProcessor/QScreenDelegate
    QAppEngine/QEventProcessor/QPopupDelegate
    QAppEngine/QEventProcessor/Common
    Views/Common
    Views
    ifaces
    Models
    RegisterTypes
    QRScanner
    QRScanner/private
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
    Models/OnBoardingModel.h
    ifaces/bridgeifaces.h
    RegisterTypes/DashRectangle.h
    ifaces/Servers/DracoDefines.h
    localization/STR_CPP.h
    )
QT5_WRAP_CPP(MOCS ${${PROJECT_NAME}_MOCS})

set( QRCODE_SCANNER ON )
if(QRCODE_SCANNER)
    set(ZXING_UNIT_TESTS OFF)
    set(ZXING_BLACKBOX_TESTS OFF)
    set(ZXING_EXAMPLES OFF)
    set(BUILD_SHARED_LIBS OFF)
    # add_definitions ( -DQR_DFS_IMPROVE )
    # add_definitions ( -DQR_DFS_IMPROVE_TEST )
    add_subdirectory(contrib/zxing ZXing)
endif(QRCODE_SCANNER)

add_subdirectory(contrib/libnunchuk nunchuk EXCLUDE_FROM_ALL)

macro(SET_OPTION option value)
  set(${option} ${value} CACHE INTERNAL "" FORCE)
endmacro()

SET_OPTION(Quotient_ENABLE_E2EE  ON)
add_subdirectory(contrib/quotient quotient EXCLUDE_FROM_ALL)

if(APPLE)
    # And this part tells CMake where to find and install the file itself
    set(APP_ICON_MACOSX ${CMAKE_CURRENT_SOURCE_DIR}/Icon.icns)
    set_source_files_properties(${APP_ICON_MACOSX} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
    add_executable( ${PROJECT_NAME} MACOSX_BUNDLE ${QAppEngine_SRCS} ${QRScranner_SRCS} ${Views_SRCS} ${${PROJECT_NAME}_SRCS} ${UIS} ${RSCS} ${MOCS_APPENGINE} ${MOCS_QRSCANNER} ${MOCS_VIEWS} ${MOCS} ${APP_ICON_MACOSX})
else()
    add_executable ( ${PROJECT_NAME} WIN32 ${QAppEngine_SRCS} ${QRScranner_SRCS} ${Views_SRCS} ${${PROJECT_NAME}_SRCS} ${UIS} ${RSCS} ${MOCS_APPENGINE} ${MOCS_QRSCANNER} ${MOCS_VIEWS} ${MOCS})
endif()

if(APPLE)
    set_target_properties(${PROJECT_NAME}
        PROPERTIES MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/Info.plist")
endif()

if(WIN32)
#    set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE FALSE)
endif()

if (NOT WIN32)
    target_compile_options(${PROJECT_NAME} PRIVATE -Werror=return-type)
endif()

target_link_libraries (${PROJECT_NAME} PUBLIC Qt5::Core Qt5::Gui Qt5::Qml Qt5::Quick Qt5::Concurrent Qt5::Svg Qt5::PrintSupport)

if(UNIX AND NOT APPLE)
    target_link_libraries("${PROJECT_NAME}" PUBLIC -Wl,--start-group nunchuk)
else()
    target_link_libraries("${PROJECT_NAME}" PUBLIC nunchuk)
endif()

target_link_libraries("${PROJECT_NAME}" PUBLIC Quotient Qt5::Sql )
target_include_directories("${PROJECT_NAME}" PUBLIC "${PROJECT_SOURCE_DIR}/contrib/quotient/lib" )

if(QRCODE_SCANNER)
    target_link_libraries (${PROJECT_NAME} PUBLIC Qt5::Multimedia)
    target_link_libraries (${PROJECT_NAME} PUBLIC ZXing)
endif(QRCODE_SCANNER)

find_package(Qt5Keychain QUIET)
if (Qt5Keychain_FOUND)
    set(USE_KEYCHAIN ON)
endif()
if(USE_KEYCHAIN)
    message( STATUS "Using Qt Keychain ${Qt5Keychain_VERSION} at ${Qt5Keychain_DIR}")
    target_compile_definitions(${PROJECT_NAME} PRIVATE USE_KEYCHAIN)
    target_link_libraries(${PROJECT_NAME} PUBLIC ${QTKEYCHAIN_LIBRARIES})
    include_directories(${QTKEYCHAIN_INCLUDE_DIR})
endif()
