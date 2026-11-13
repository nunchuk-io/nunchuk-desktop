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
#include <QGuiApplication>
#include <QMessageBox>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QScreen>
#include <QDir>
#include <QThreadPool>
#include "QEventProcessor.h"
#include "Servers/QCaptchaVerification.h"
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
#include "app/AppRegister.h"

#include <algorithm>
#include <clocale>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <vector>

#ifdef ENABLE_THREAD_MONITOR
#include "QPingThread.h"
#endif

#ifdef ENABLE_BACKTRACE
#include <execinfo.h>
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

static void printBacktrace(int skip = 1)
{
#ifdef ENABLE_BACKTRACE
    void* frames[64];
    int n = backtrace(frames, 64);
    if (n <= 0) {
        fprintf(stderr, "=== backtrace unavailable ===\n");
        fflush(stderr);
        return;
    }

    char** syms = backtrace_symbols(frames, n);
    if (!syms) {
        fprintf(stderr, "backtrace_symbols failed\n");
        fflush(stderr);
        return;
    }

    fprintf(stderr, "=== backtrace (%d frames) ===\n", n);
    for (int i = skip; i < n; ++i) {
        fprintf(stderr, "#%-2d %s\n", i - skip, syms[i]);
    }
    fprintf(stderr, "=============================\n");
    fflush(stderr);

    free(syms);
#endif
}

class OurApplication : public QApplication
{
public:
    using QApplication::QApplication;

    bool notify(QObject* receiver, QEvent* event) override
    {
        const void* receiverPtr = receiver;
        const void* eventPtr = event;

        const char* receiverClass = "null";
        int eventType = -1;

        if (receiver) {
            receiverClass = receiver->metaObject()->className();
        }
        if (event) {
            eventType = int(event->type());
        }

        try {
            return QApplication::notify(receiver, event);
        } catch (const std::exception& e) {
            fprintf(stderr, "Exception escaped Qt event loop\n");
            fprintf(stderr, "receiver=%p class=%s objectName=%s event=%p eventType=%d\n",
                    receiverPtr,
                    receiverClass,
                    receiver && !receiver->objectName().isEmpty()
                        ? receiver->objectName().toUtf8().constData()
                        : "<empty>",
                    eventPtr,
                    eventType);
            fflush(stderr);
            printBacktrace();
        } catch (...) {
            fprintf(stderr, "Unknown exception escaped Qt event loop\n");
            fprintf(stderr, "receiver=%p class=%s event=%p eventType=%d\n",
                    receiverPtr, receiverClass, eventPtr, eventType);
            fflush(stderr);
            printBacktrace();
        }

        return false;
    }
};

namespace {
constexpr double kAutoScaleStep = 0.05;
constexpr double kAutoScaleMin  = 0.5;

bool hasUserScaleOverride()
{
    return qEnvironmentVariableIsSet("QT_SCALE_FACTOR")
        || qEnvironmentVariableIsSet("QT_SCREEN_SCALE_FACTORS");
}
}

static double calculateScaleFactor(int argc, char* argv[])
{
#if defined(Q_OS_LINUX) || defined(Q_OS_WIN)
    if (hasUserScaleOverride()) {
        DBG_INFO << "[SCALE] user override detected, auto-scale skipped."
                 << "QT_SCALE_FACTOR:" << qEnvironmentVariable("QT_SCALE_FACTOR")
                 << "QT_SCREEN_SCALE_FACTORS:" << qEnvironmentVariable("QT_SCREEN_SCALE_FACTORS");
        return 1.0;
    }

    std::vector<char*> probeArgv(argv, argv + argc);
    probeArgv.push_back(nullptr);
    int probeArgc = argc;

    double scale = 1.0;
    {
        QGuiApplication probe(probeArgc, probeArgv.data());
        QScreen* screen = QGuiApplication::primaryScreen();
        if (!screen) {
            DBG_WARN << "[SCALE] no primary screen, auto-scale skipped";
            return 1.0;
        }

        const QRect  avail   = screen->availableGeometry();
        const double usableW = avail.width()  - QAPP_GAP_HEIGHT;
        const double usableH = avail.height() - QAPP_GAP_HEIGHT;
        const double fit     = std::min(usableW / QAPP_WIDTH_EXPECTED,
                                        usableH / QAPP_HEIGHT_EXPECTED);

        if (fit < 1.0) {
            scale = std::floor(fit / kAutoScaleStep + 1e-9) * kAutoScaleStep;
            if (scale < kAutoScaleMin) {
                DBG_WARN << "[SCALE] screen too small, clamping" << scale << "->" << kAutoScaleMin;
                scale = kAutoScaleMin;
            }
        }

        DBG_INFO << "[SCALE] screen:" << screen->name()
                 << "available:" << avail.x() << avail.y() << avail.width() << avail.height()
                 << "dpr:" << screen->devicePixelRatio()
                 << "logicalDpi:" << screen->logicalDotsPerInch()
                 << "fit:" << fit
                 << "scale:" << scale;
    }

    if (scale < 1.0) {
        qputenv("QT_SCALE_FACTOR", QByteArray::number(scale, 'f', 2));
    }
    return scale;
#else
    Q_UNUSED(argc);
    Q_UNUSED(argv);
    return 1.0;
#endif
}

int main(int argc, char* argv[])
{
#if defined(Q_OS_WIN) && defined(_MSC_VER)
    // QString::toStdString() returns UTF-8 in Qt 6. Configure the MSVC CRT
    // conversion used by std::filesystem before any worker threads or path
    // conversions are created. LC_CTYPE is sufficient and avoids changing
    // numeric/date formatting for the rest of the process.
    const bool isUtf8FilesystemLocaleConfigured =
        std::setlocale(LC_CTYPE, ".UTF8") != nullptr;
#endif
#ifdef ENABLE_BACKTRACE
    signal(SIGSEGV, signalHandler);  // Segmentation fault
    signal(SIGABRT, signalHandler);  // Abort signal
#endif

#ifdef __linux__
    setenv("QTWEBENGINE_DISABLE_SANDBOX", "1", 1);
#endif
    qputenv("QTWEBENGINE_CHROMIUM_FLAGS",
            "--disable-web-bluetooth "
            "--disable-features=WebBluetooth,WebBluetoothNewPermissionsBackend");

    // Remote debugging is opt-in. Set QTWEBENGINE_REMOTE_DEBUGGING externally
    // when Chromium DevTools or WebEngineDriver access is needed.

    // Application identity first (static setters, no app instance needed):
    // every log from here on — including inside calculateScaleFactor() — makes
    // the lazily-created log file resolve to the app-specific data directory.
    QCoreApplication::setOrganizationName("nunchuk");
    QCoreApplication::setOrganizationDomain("nunchuk.io");
    QCoreApplication::setApplicationName("NunchukClient");
    QCoreApplication::setApplicationVersion("2.9.4");

    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(
        Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);

    const double scale_factor = calculateScaleFactor(argc, argv);
    Q_UNUSED(scale_factor);
    OurApplication appNunchuk(argc, argv);

    appNunchuk.setWindowIcon(QIcon(":/Images/Images/logo-app.svg"));
    appNunchuk.setApplicationDisplayName(QString("%1 %2").arg("Nunchuk").arg(appNunchuk.applicationVersion()));

    // One startup record is enough to distinguish a stale/running app bundle
    // from the executable that was just built. Never include user/session data.
    DBG_INFO << "[APP_RUNTIME_TRACE]"
             << "pid:" << QCoreApplication::applicationPid()
             << "executable:" << QCoreApplication::applicationFilePath()
             << "appVersion:" << QCoreApplication::applicationVersion()
             << "qtRuntime:" << qVersion();

#if defined(Q_OS_WIN) && defined(_MSC_VER)
    if (isUtf8FilesystemLocaleConfigured) {
        DBG_INFO << "Windows filesystem locale configured for UTF-8";
    } else {
        DBG_ERROR << "Failed to configure Windows filesystem locale for UTF-8";
        QMessageBox::critical(
            nullptr, QStringLiteral("Nunchuk"),
            QStringLiteral(
                "Unable to initialize UTF-8 filesystem support. Nunchuk "
                "cannot safely open its data directory. Please update "
                "Windows and try again."));
        return EXIT_FAILURE;
    }
#endif

    // Logged AFTER the application identity is set so the (lazily created) log
    // file resolves to the app-specific directory; QSslSocket::supportsSsl()
    // also force-loads OpenSSL, so keep it off the pre-QApplication path.
    DBG_INFO << "Build version:" << QSslSocket::sslLibraryBuildVersionString();
    DBG_INFO << "Runtime version:" << QSslSocket::sslLibraryVersionString();
    DBG_INFO << "Support SSL:" << QSslSocket::supportsSsl();

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
    // loguru::add_file("logs/nunchuk.log", loguru::Append, loguru::Verbosity_MAX);
    // loguru::g_stderr_verbosity = loguru::Verbosity_INFO;
   loguru::g_stderr_verbosity = loguru::Verbosity_OFF;
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

    // Qt6: the macOS native style refuses background/contentItem customization
    // on TextField, TextArea, ScrollBar, etc. — emitting runtime warnings for every
    // instance and silently ignoring the custom visual.  "Basic" is the non-native
    // cross-platform style; it honours all QML background/contentItem overrides,
    // which is what the entire Nunchuk QML UI relies on.
    // Must be set before the QML engine is created (QEventProcessor::initialized()).
    QQuickStyle::setStyle("Basic");

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
    QScreen* primaryScr = OurApplication::primaryScreen();
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
    QEventProcessor::instance()->setContextProperty("qapplicationVersion", appNunchuk.applicationVersion());
    QEventProcessor::instance()->setContextProperty("UserWallet", QVariant::fromValue(QUserWallets::instance()));
    QEventProcessor::instance()->setContextProperty("GroupWallet", QVariant::fromValue(QGroupWallets::instance()));
    QEventProcessor::instance()->setContextProperty("SharedWallet", QVariant::fromValue(QSharedWallets::instance()));
    QEventProcessor::instance()->setContextProperty("ProfileSetting", QVariant::fromValue(ProfileSetting::instance()));
    QEventProcessor::instance()->setContextProperty("ServiceSetting", QVariant::fromValue(ServiceSetting::instance()));
    QEventProcessor::instance()->setContextProperty("OnBoarding", QVariant::fromValue(OnBoardingModel::instance()));
    QEventProcessor::instance()->setContextProperty("PDFPrinter", QVariant::fromValue(QPDFPrinter::instance()));
    QEventProcessor::instance()->setContextProperty("SignerManagement", QVariant::fromValue(QSignerManagement::instance()));
    app::registerContextProperty();
    app::registerViewModels();
    QEventProcessor::instance()->completed();
    QEventProcessor::instance()->sendEvent(E::EVT_STARTING_APPLICATION_ONLINEMODE);
    //    QEventProcessor::instance()->sendEvent(E::EVT_STARTING_APPLICATION_LOCALMODE);
    QObject::connect(Draco::instance(), &Draco::startCheckForUpdate, Draco::instance(),
        [](int result, const QString& title, const QString& message,
           const QString& doItLaterCTALbl, const QString& downloadUrl,
           const QString& primaryCTALbl)->void {
            QObject* obj = QEventProcessor::instance()->getQuickWindow()->rootObject();
            if(obj){
                if (result == 2) // Forced update
                {
                    QMetaObject::invokeMethod(obj, "funcUpdateRequired",
                                              Q_ARG(QVariant, title),
                                              Q_ARG(QVariant, message),
                                              Q_ARG(QVariant, doItLaterCTALbl),
                                              Q_ARG(QVariant, downloadUrl),
                                              Q_ARG(QVariant, primaryCTALbl));
                }
                else if (result == 1) { // Recommended update
                    static bool sendOneTime = false;
                    if (sendOneTime == false) {
                        sendOneTime = true;
                        QMetaObject::invokeMethod(obj, "funcUpdateAvailable",
                                                  Q_ARG(QVariant, title),
                                                  Q_ARG(QVariant, message),
                                                  Q_ARG(QVariant, doItLaterCTALbl),
                                                  Q_ARG(QVariant, downloadUrl),
                                                  Q_ARG(QVariant, primaryCTALbl));
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


    const int execResult = appNunchuk.exec();
    DBG_INFO << "[SHUTDOWN] app.exec RETURNED"
             << "result:" << execResult
             << "activeThreadCount:" << QThreadPool::globalInstance()->activeThreadCount();

    // QEventProcessor is a function-local-static singleton. Tear down the
    // QQuickView and cached QQmlComponents before QApplication is destroyed.
    DBG_INFO << "[SHUTDOWN] QEventProcessor::shutdown BEGIN"
             << "activeThreadCount:" << QThreadPool::globalInstance()->activeThreadCount();
    QEventProcessor::instance()->shutdown();
    DBG_INFO << "[SHUTDOWN] QEventProcessor::shutdown END"
             << "activeThreadCount:" << QThreadPool::globalInstance()->activeThreadCount();

    return execResult;
}
