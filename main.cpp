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
#include <QGuiApplication>
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
#include "contrib/libnunchuk/src/utils/loguru.hpp"
#include "RegisterTypes/DashRectangle.h"
#include "QPingThread.h"
#include "QRScanner/QBarcodeFilter.h"
#include "QPDFPrinter.h"

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

inline double calculateScaleFactor()
{
    static char  temp_arg[] = "";
    static char* temp_argv = temp_arg;
    static int   temp_argc = 1;
    // assumes that the default desktop resolution is 1080 (scale of 1)
    double scalePref = 1.0;

    QGuiApplication* temp = new QGuiApplication(temp_argc, &temp_argv);
    QScreen* primaryScr = QGuiApplication::primaryScreen();
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
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    double scale_factor = calculateScaleFactor();
    static char  qt_arg[] = "";
    static char* qt_argv = qt_arg;
    static int   argc_own = 1;

    QGuiApplication app(argc_own, &qt_argv);
    app.setWindowIcon(QIcon(":/Images/Images/logo-app.svg"));
    app.setOrganizationName("nunchuk");
    app.setOrganizationDomain("nunchuk.io");
    app.setApplicationName("NunchukClient");
    app.setApplicationVersion("1.9.35");
    app.setApplicationDisplayName(QString("%1 %2").arg("Nunchuk").arg(app.applicationVersion()));
    AppModel::instance();
    Draco::instance();
    QWalletManagement::instance();

#ifndef RELEASE_MODE
//    QPingThread objTracking;
//    objTracking.startPing();
#endif
    DBG_INFO << "Execution Path: " << qApp->applicationDirPath();
    loguru::g_stderr_verbosity = loguru::Verbosity_OFF;

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
    QEventProcessor::instance()->addImageProvider("nunchuk", CLIENT_INSTANCE->imageprovider());
    QEventProcessor::instance()->initialized();
    QEventProcessor::instance()->initFonts(latoFonts);
    QEventProcessor::instance()->initFonts(montserratFonts);

    // Handle window size
#if defined(Q_OS_LINUX) || defined (Q_OS_WIN)
    QEventProcessor::instance()->registerCtxProperty("QAPP_DEVICE_WIDTH", QAPP_WIDTH_EXPECTED);
    QEventProcessor::instance()->registerCtxProperty("QAPP_DEVICE_HEIGHT", QAPP_HEIGHT_EXPECTED);
    QEventProcessor::instance()->registerCtxProperty("QAPP_DEVICE_HEIGHT_RATIO", fmin(1.0, (double)QAPP_HEIGHT_EXPECTED/(double)QAPP_HEIGHT_EXPECTED));
    QEventProcessor::instance()->setViewerSize(QAPP_WIDTH_EXPECTED, QAPP_HEIGHT_EXPECTED);
#else
    DBG_INFO << scale_factor;
    QScreen* primaryScr = QGuiApplication::primaryScreen();
    if (primaryScr) {
        QRect rect = primaryScr->availableGeometry();
        int screenHeight = rect.height();
        if(screenHeight < QAPP_HEIGHT_EXPECTED){
            QEventProcessor::instance()->registerCtxProperty("QAPP_DEVICE_WIDTH", QAPP_WIDTH_MIN);
            QEventProcessor::instance()->registerCtxProperty("QAPP_DEVICE_HEIGHT", QAPP_HEIGHT_MIN);
            QEventProcessor::instance()->registerCtxProperty("QAPP_DEVICE_HEIGHT_RATIO", fmin(1.0, (double)QAPP_HEIGHT_MIN/(double)QAPP_HEIGHT_EXPECTED));
            QEventProcessor::instance()->setViewerSize(QAPP_WIDTH_MIN, QAPP_HEIGHT_MIN);
        }
        else {
            QEventProcessor::instance()->registerCtxProperty("QAPP_DEVICE_WIDTH", QAPP_WIDTH_EXPECTED);
            QEventProcessor::instance()->registerCtxProperty("QAPP_DEVICE_HEIGHT", QAPP_HEIGHT_EXPECTED);
            QEventProcessor::instance()->registerCtxProperty("QAPP_DEVICE_HEIGHT_RATIO", fmin(1.0, (double)QAPP_HEIGHT_EXPECTED/(double)QAPP_HEIGHT_EXPECTED));
            QEventProcessor::instance()->setViewerSize(QAPP_WIDTH_EXPECTED, QAPP_HEIGHT_EXPECTED);
        }
    }
#endif

    QEventProcessor::instance()->registerCtxProperty("MAINNET_SERVER", MAINNET_SERVER);
    QEventProcessor::instance()->registerCtxProperty("TESTNET_SERVER", TESTNET_SERVER);
#ifdef SIGNET_SUPPORT
    QEventProcessor::instance()->registerCtxProperty("SIGNET_SERVER", SIGNET_SERVER);
#else
    QEventProcessor::instance()->registerCtxProperty("SIGNET_SERVER", "");
#endif
    QEventProcessor::instance()->registerCtxProperty("EXPLORER_TESTNET", EXPLORER_TESTNET);
    QEventProcessor::instance()->registerCtxProperty("EXPLORER_MAINNET", EXPLORER_MAINNET);
    QEventProcessor::instance()->registerCtxProperty("EXPLORER_SIGNNET", EXPLORER_SIGNNET);
    QEventProcessor::instance()->registerCtxProperty("MAX_UNUSED_ADDR", MAX_UNUSED_ADDR);
    QEventProcessor::instance()->registerCtxProperty("AppModel", QVariant::fromValue(AppModel::instance()));
    QEventProcessor::instance()->registerCtxProperty("AppSetting", QVariant::fromValue(AppSetting::instance()));
    QEventProcessor::instance()->registerCtxProperty("Draco", QVariant::fromValue(Draco::instance()));
    QEventProcessor::instance()->registerCtxProperty("ClientController", QVariant::fromValue(CLIENT_INSTANCE));
    QEventProcessor::instance()->registerCtxProperty("qapplicationVersion", app.applicationVersion());
    QEventProcessor::instance()->registerCtxProperty("UserWallet", QVariant::fromValue(QUserWallets::instance()));
    QEventProcessor::instance()->registerCtxProperty("GroupWallet", QVariant::fromValue(QGroupWallets::instance()));
    QEventProcessor::instance()->registerCtxProperty("ProfileSetting", QVariant::fromValue(ProfileSetting::instance()));
    QEventProcessor::instance()->registerCtxProperty("ServiceSetting", QVariant::fromValue(ServiceSetting::instance()));
    QEventProcessor::instance()->registerCtxProperty("OnBoarding", QVariant::fromValue(OnBoardingModel::instance()));
    QEventProcessor::instance()->registerCtxProperty("PDFPrinter", QVariant::fromValue(QPDFPrinter::instance()));
    QEventProcessor::instance()->completed();
    QEventProcessor::instance()->sendEvent(E::EVT_STARTING_APPLICATION_ONLINEMODE);
    //    QEventProcessor::instance()->sendEvent(E::EVT_STARTING_APPLICATION_LOCALMODE);
    QObject::connect(Draco::instance(), &Draco::startCheckForUpdate, Draco::instance(),
        [](int result, const QString& title, const QString& message, const QString& doItLaterCTALbl)->void {
            QObject* obj = QEventProcessor::instance()->getQuickWindow()->rootObject();
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
