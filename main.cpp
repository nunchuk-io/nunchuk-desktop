/**************************************************************************
 * This file is part of the Nunchuk software (https://nunchuk.io/)        *
 * Copyright (C) 2020-2022 Enigmo								          *
 * Copyright (C) 2022 Nunchuk								              *
 *                                                                        *
 * This program is free software; you can redistribute it and/or          *
 * modify it under the terms of the GNU General Public License            *
 * as published by the Free Software Foundation; either version 3         *
 * of the License, or (at your option) any later version.                 *
 *                                                                        *
 * This program is distributed in the hope that it will be useful,        *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 * GNU General Public License for more details.                           *
 *                                                                        *
 * You should have received a copy of the GNU General Public License      *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.  *
 *                                                                        *
 **************************************************************************/
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QScreen>
#include <QDir>
#include "QEventProcessor.h"
#include "Views/Views.h"
#include "Models/AppModel.h"
#include "Models/AppSetting.h"
#include "Models/ProfileSetting.h"
#include "Models/ServiceSetting.h"
#include "Models/QWarningMessage.h"
#include "Models/OnBoardingModel.h"
#include "QRScanner/QBarcodeGenerator.h"
#include "bridgeifaces.h"
#include "Servers/Draco.h"
#include "Chats/ClientController.h"
#include "Premiums/QUserWallets.h"
#include "Premiums/QGroupWallets.h"
#include "Premiums/QSharedWallets.h"
#include "Signers/QSignerManagement.h"
#include "contrib/libnunchuk/src/utils/loguru.hpp"
#include "RegisterTypes/DashRectangle.h"
#include "QRScanner/QBarcodeFilter.h"
#include "QPDFPrinter.h"

#ifdef ENABLE_THREAD_MONITOR
#include "QPingThread.h"
#endif

#ifdef ENABLE_BACKTRACE
#include <csignal>
#include <execinfo.h>
#include <fstream>
#include <iostream>
#endif

QStringList latoFonts = {
    ":/fonts/fonts/Lato/Lato-BlackItalic.ttf",
    ":/fonts/fonts/Lato/Lato-Black.ttf",
    ":/fonts/fonts/Lato/Lato-BoldItalic.ttf",
    ":/fonts/fonts/Lato/Lato-Bold.ttf",
    ":/fonts/fonts/Lato/Lato-Italic.ttf",
    ":/fonts/fonts/Lato/Lato-LightItalic.ttf",
    ":/fonts/fonts/Lato/Lato-Light.ttf",
    ":/fonts/fonts/Lato/Lato-Regular.ttf",
    ":/fonts/fonts/Lato/Lato-ThinItalic.ttf",
    ":/fonts/fonts/Lato/Lato-Thin.ttf"
};

QStringList montserratFonts = {
    ":/fonts/fonts/Montserrat/Montserrat-BlackItalic.ttf",
    ":/fonts/fonts/Montserrat/Montserrat-Black.ttf",
    ":/fonts/fonts/Montserrat/Montserrat-BoldItalic.ttf",
    ":/fonts/fonts/Montserrat/Montserrat-Bold.ttf",
    ":/fonts/fonts/Montserrat/Montserrat-ExtraBoldItalic.ttf",
    ":/fonts/fonts/Montserrat/Montserrat-ExtraBold.ttf",
    ":/fonts/fonts/Montserrat/Montserrat-ExtraLightItalic.ttf",
    ":/fonts/fonts/Montserrat/Montserrat-ExtraLight.ttf",
    ":/fonts/fonts/Montserrat/Montserrat-Italic.ttf",
    ":/fonts/fonts/Montserrat/Montserrat-LightItalic.ttf",
    ":/fonts/fonts/Montserrat/Montserrat-Light.ttf",
    ":/fonts/fonts/Montserrat/Montserrat-MediumItalic.ttf",
    ":/fonts/fonts/Montserrat/Montserrat-Medium.ttf",
    ":/fonts/fonts/Montserrat/Montserrat-Regular.ttf",
    ":/fonts/fonts/Montserrat/Montserrat-SemiBoldItalic.ttf",
    ":/fonts/fonts/Montserrat/Montserrat-SemiBold.ttf",
    ":/fonts/fonts/Montserrat/Montserrat-ThinItalic.ttf",
    ":/fonts/fonts/Montserrat/Montserrat-Thin.ttf"
};

#ifdef ENABLE_BACKTRACE
void writeBacktraceToFile() {
    std::time_t t = std::time(nullptr);
    std::tm tm = *std::localtime(&t);

    char filename[32];
    std::strftime(filename, sizeof(filename), "backtrace_%H%M%S.txt", &tm);

    const int max_frames = 256;
    void **buffer = (void **)malloc(max_frames * sizeof(void *));
    if (!buffer) {
        std::cerr << "Failed to allocate memory for backtrace buffer" << std::endl;
        return;
    }

    int nptrs = backtrace(buffer, max_frames);
    std::ofstream outFile(filename, std::ios::out | std::ios::app);
    if (!outFile) {
        std::cerr << "Could not open file: " << filename << std::endl;
        free(buffer);
        return;
    }

    outFile << "Backtrace (size = " << nptrs << "):" << std::endl;
    char **symbols = backtrace_symbols(buffer, nptrs);
    if (!symbols) {
        outFile << "Error retrieving symbols." << std::endl;
        free(buffer);
        return;
    }

    for (int i = 0; i < nptrs; i++) {
        outFile << symbols[i] << std::endl;
    }

    free(symbols);
    free(buffer);
}

// Signal handler
void signalHandler(int sig) {
    writeBacktraceToFile();
    exit(sig);
}
#endif


inline double calculateScaleFactor()
{
    static char  temp_arg[] = "";
    static char* temp_argv = temp_arg;
    static int   temp_argc = 1;
    // assumes that the default desktop resolution is 1080 (scale of 1)
    double scalePref = 1.0;

    QApplication* temp = new QApplication(temp_argc, &temp_argv);
    QScreen* primaryScr = QApplication::primaryScreen();
    if (primaryScr) {
        QRect rect = primaryScr->availableGeometry();
        int screenHeight = rect.height();
        int screenWidth = rect.width();
#if defined(Q_OS_LINUX) || defined (Q_OS_WIN)
        int appWidth = QAPP_WIDTH_EXPECTED + (QAPP_GAP_HEIGHT + rect.x()) * primaryScr->devicePixelRatio();
        int appHeight = QAPP_HEIGHT_EXPECTED + (QAPP_GAP_HEIGHT + rect.y()) * primaryScr->devicePixelRatio();
#else
        int appWidth = screenWidth = QAPP_WIDTH_EXPECTED;
        int appHeight = screenHeight = QAPP_HEIGHT_EXPECTED;
#endif
        if (screenHeight <= appHeight || screenWidth <= appWidth) {
            scalePref = min((double)screenHeight / appHeight, (double)screenWidth / appWidth);
        }
        std::string scaleAsString = QString::number(scalePref, 'f', 1).toStdString();
        DBG_INFO << screenHeight << screenWidth << appHeight << appWidth << scaleAsString << primaryScr->devicePixelRatio() ;
        QByteArray scaleAsQByteArray(scaleAsString.c_str(), scaleAsString.length());
        qputenv("QT_SCALE_FACTOR", scaleAsQByteArray);
    }
    delete temp;
    return scalePref;
}

int main(int argc, char* argv[])
{
    Q_UNUSED(argc);
    Q_UNUSED(argv);
#ifdef ENABLE_BACKTRACE
    signal(SIGSEGV, signalHandler);  // Segmentation fault
    signal(SIGABRT, signalHandler);  // Abort signal
#endif

#ifdef __linux__
    setenv("QTWEBENGINE_DISABLE_SANDBOX", "1", 1);
#endif

    DBG_INFO << "Build version:" << QSslSocket::sslLibraryBuildVersionString();
    DBG_INFO << "Runtime version:" << QSslSocket::sslLibraryVersionString();
    DBG_INFO << "Support SSL:" << QSslSocket::supportsSsl();

    double scale_factor = calculateScaleFactor();
    // static char  qt_arg[] = "";
    // static char* qt_argv = qt_arg;
    // static int   argc_own = 1;
    // QApplication app(argc_own, &qt_argv);

    QApplication app(argc, argv);
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    app.setWindowIcon(QIcon(":/Images/Images/logo-app.svg"));
    app.setOrganizationName("nunchuk");
    app.setOrganizationDomain("nunchuk.io");
    app.setApplicationName("NunchukClient");
    app.setApplicationVersion("2.0.0");
    app.setApplicationDisplayName(QString("%1 %2").arg("Nunchuk").arg(app.applicationVersion()));
    Draco::instance();
    AppModel::instance();
    QWalletManagement::instance();
    QUserWallets::instance();
    QGroupWallets::instance();
    QSharedWallets::instance();

#ifdef ENABLE_THREAD_MONITOR
   QPingThread objTracking;
   objTracking.startPing();
#endif

#ifdef ENABLE_OUTLOG
    loguru::g_stderr_verbosity = loguru::Verbosity_INFO;
#else
    loguru::g_stderr_verbosity = loguru::Verbosity_OFF;
#endif

    QEventProcessor::registerStates(STATE_ALL, ALEN(STATE_ALL));
    qmlRegisterType<E>("HMIEVENTS", 1, 0, "EVT");
    qmlRegisterType<QBarcodeGenerator>("QRCodeItem", 1, 0, "QRCodeItem");
    qmlRegisterType<DashRectangle>("RegisterTypes", 1, 0, "DashRectangle");
    qmlRegisterType<ENUNCHUCK>("NUNCHUCKTYPE", 1, 0, "NUNCHUCKTYPE");
    qmlRegisterType<ServiceSetting>("NUNCHUCKTYPE", 1, 0, "ServiceType");
    qmlRegisterType<EWARNING>("EWARNING", 1, 0, "EWARNING");
    qmlRegisterType<POPUP>("EWARNING", 1, 0, "Popup_t");
    qmlRegisterType<DRACO_CODE>("DRACO_CODE", 1, 0, "DRACO_CODE");
    qmlRegisterSingletonType(QUrl("qrc:/Qml/Global/QWalletData.qml"), "DataPool", 1, 0, "RoomWalletData");
    qmlRegisterSingletonType(QUrl("qrc:/Qml/Global/QGlobal.qml"), "DataPool", 1, 0, "GlobalData");
    qmlRegisterType<AlertEnum>("NUNCHUCKTYPE", 1, 0, "AlertType");
    qmlRegisterType<QBarcodeFilter>("QBarcodeFilter", 1, 0, "QBarcodeFilter");
    qmlRegisterType<ScriptNodeHelper>("NUNCHUCKTYPE", 1, 0, "ScriptNodeHelper");

    QEventProcessor::instance()->addImageProvider("nunchuk", CLIENT_INSTANCE->imageprovider());
    QEventProcessor::instance()->initialized();
    QEventProcessor::instance()->initFonts(latoFonts);
    QEventProcessor::instance()->initFonts(montserratFonts);

    // Handle window size
#if defined(Q_OS_LINUX) || defined (Q_OS_WIN)
    QEventProcessor::instance()->setContextProperty("QAPP_DEVICE_WIDTH", QAPP_WIDTH_EXPECTED);
    QEventProcessor::instance()->setContextProperty("QAPP_DEVICE_HEIGHT", QAPP_HEIGHT_EXPECTED);
    QEventProcessor::instance()->setContextProperty("QAPP_DEVICE_HEIGHT_RATIO", fmin(1.0, (double)QAPP_HEIGHT_EXPECTED/(double)QAPP_HEIGHT_EXPECTED));
    QEventProcessor::instance()->setViewerSize(QAPP_WIDTH_EXPECTED, QAPP_HEIGHT_EXPECTED);
#else
    DBG_INFO << scale_factor;
    QScreen* primaryScr = QApplication::primaryScreen();
    if (primaryScr) {
        QRect rect = primaryScr->availableGeometry();
        int screenHeight = rect.height();
        if(screenHeight < QAPP_HEIGHT_EXPECTED){
            QEventProcessor::instance()->setContextProperty("QAPP_DEVICE_WIDTH", QAPP_WIDTH_MIN);
            QEventProcessor::instance()->setContextProperty("QAPP_DEVICE_HEIGHT", QAPP_HEIGHT_MIN);
            QEventProcessor::instance()->setContextProperty("QAPP_DEVICE_HEIGHT_RATIO", fmin(1.0, (double)QAPP_HEIGHT_MIN/(double)QAPP_HEIGHT_EXPECTED));
            QEventProcessor::instance()->setViewerSize(QAPP_WIDTH_MIN, QAPP_HEIGHT_MIN);
        }
        else {
            QEventProcessor::instance()->setContextProperty("QAPP_DEVICE_WIDTH", QAPP_WIDTH_EXPECTED);
            QEventProcessor::instance()->setContextProperty("QAPP_DEVICE_HEIGHT", QAPP_HEIGHT_EXPECTED);
            QEventProcessor::instance()->setContextProperty("QAPP_DEVICE_HEIGHT_RATIO", fmin(1.0, (double)QAPP_HEIGHT_EXPECTED/(double)QAPP_HEIGHT_EXPECTED));
            QEventProcessor::instance()->setViewerSize(QAPP_WIDTH_EXPECTED, QAPP_HEIGHT_EXPECTED);
        }
    }
#endif
    AppSetting::instance();

    QEventProcessor::instance()->setContextProperty("MAINNET_SERVER", MAINNET_SERVER);
    QEventProcessor::instance()->setContextProperty("TESTNET_SERVER", TESTNET_SERVER);
#ifdef SIGNET_SUPPORT
    QEventProcessor::instance()->setContextProperty("SIGNET_SERVER", SIGNET_SERVER);
#else
    QEventProcessor::instance()->registerCtxProperty("SIGNET_SERVER", "");
#endif
    QEventProcessor::instance()->setContextProperty("EXPLORER_TESTNET", EXPLORER_TESTNET);
    QEventProcessor::instance()->setContextProperty("EXPLORER_MAINNET", EXPLORER_MAINNET);
    QEventProcessor::instance()->setContextProperty("EXPLORER_SIGNNET", EXPLORER_SIGNNET);
    QEventProcessor::instance()->setContextProperty("MAX_UNUSED_ADDR", MAX_UNUSED_ADDR);
    QEventProcessor::instance()->setContextProperty("AppModel", QVariant::fromValue(AppModel::instance()));
    QEventProcessor::instance()->setContextProperty("AppSetting", QVariant::fromValue(AppSetting::instance()));
    QEventProcessor::instance()->setContextProperty("Draco", QVariant::fromValue(Draco::instance()));
    QEventProcessor::instance()->setContextProperty("ClientController", QVariant::fromValue(CLIENT_INSTANCE));
    QEventProcessor::instance()->setContextProperty("qapplicationVersion", app.applicationVersion());
    QEventProcessor::instance()->setContextProperty("UserWallet", QVariant::fromValue(QUserWallets::instance()));
    QEventProcessor::instance()->setContextProperty("GroupWallet", QVariant::fromValue(QGroupWallets::instance()));
    QEventProcessor::instance()->setContextProperty("SharedWallet", QVariant::fromValue(QSharedWallets::instance()));
    QEventProcessor::instance()->setContextProperty("ProfileSetting", QVariant::fromValue(ProfileSetting::instance()));
    QEventProcessor::instance()->setContextProperty("ServiceSetting", QVariant::fromValue(ServiceSetting::instance()));
    QEventProcessor::instance()->setContextProperty("OnBoarding", QVariant::fromValue(OnBoardingModel::instance()));
    QEventProcessor::instance()->setContextProperty("PDFPrinter", QVariant::fromValue(QPDFPrinter::instance()));
    QEventProcessor::instance()->setContextProperty("SignerManagement", QVariant::fromValue(QSignerManagement::instance()));
    QEventProcessor::instance()->completed();
    QEventProcessor::instance()->sendEvent(E::EVT_STARTING_APPLICATION_ONLINEMODE);
    //    QEventProcessor::instance()->sendEvent(E::EVT_STARTING_APPLICATION_LOCALMODE);
    QObject::connect(Draco::instance(), &Draco::startCheckForUpdate, Draco::instance(),
        [](int result, const QString& title, const QString& message, const QString& doItLaterCTALbl)->void {
            QObject* obj = QEventProcessor::instance()->getQuickWindow()->rootObject();
            if(obj){
                if (result == 2) // Forced update
                {
                    QMetaObject::invokeMethod(obj, "funcUpdateRequired", Q_ARG(QVariant, title), Q_ARG(QVariant, message), Q_ARG(QVariant, doItLaterCTALbl));
                }
                else if (result == 1) { // Recommended update
                    static bool sendOneTime = false;
                    if (sendOneTime == false) {
                        sendOneTime = true;
                        QMetaObject::invokeMethod(obj, "funcUpdateAvailable", Q_ARG(QVariant, title), Q_ARG(QVariant, message), Q_ARG(QVariant, doItLaterCTALbl));
                    }
                }
            }
        }
    , Qt::QueuedConnection);
    QObject::connect(QEventProcessor::instance()->getQuickWindow(), &QQuickView::windowStateChanged, [=](int windowState) {
        QtConcurrent::run([windowState]() {
            static int state = -1;
            if (state != windowState) {
                state = windowState;
                if (windowState == Qt::WindowNoState) {
                    Draco::instance()->checkForUpdate();
                }
            }
        });
    });
    QEventProcessor::instance()->show();
    return app.exec();
}
