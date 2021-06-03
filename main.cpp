#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QScreen>
#include <QDir>
#include "QQuickViewer.h"
#include "Views/Views.h"
#include "Models/AppModel.h"
#include "Models/AppSetting.h"
#include "Models/QWarningMessage.h"
#include "ifaces/QRCodeItem.h"
#include "bridgeifaces.h"

#ifdef ENABLE_DECODER_QR_CODE
#include "contrib/qzxing/src/QZXing.h"
#endif

QStringList latoFonts =  {
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

QStringList montserratFonts =  {
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

int main(int argc, char *argv[])
{
    Q_UNUSED(argc);
    Q_UNUSED(argv);

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    static char qt_arg[] = "";
    static char* qt_argv = qt_arg;
    static int argc_own = 1;

    QGuiApplication app(argc_own, &qt_argv);
    AppSetting::instance();
    AppModel::instance();

    app.setWindowIcon(QIcon(":/Images/Images/Logo_Palcehoder.png"));
    app.setOrganizationName("nunchuk");
    app.setOrganizationDomain("nunchuk.io");
    app.setApplicationName("NunchukClient");
    app.setApplicationVersion("1.0");
    app.setApplicationDisplayName(QString("%1 %2").arg("Nunchuk").arg(app.applicationVersion()));

    DBG_INFO << "Execution Path: " << qApp->applicationDirPath();
    AppSetting::instance()->setExecutePath(qApp->applicationDirPath());

    auto path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    if (path.isEmpty()){
        DBG_INFO << "Cannot determine settings storage location";
    }
    else{
        QDir d{path};
        if (d.mkpath(d.absolutePath()) && QDir::setCurrent(d.absolutePath())) {
            AppSetting::instance()->setStoragePath(QDir::currentPath());
            DBG_INFO << "AppDataLocation in" << QDir::currentPath();
        }
    }

#ifdef ENABLE_DECODER_QR_CODE
    QZXing::registerQMLTypes();
#endif

    QQuickViewer::registerStates(STATE_ALL, ALEN(STATE_ALL));
    qmlRegisterType<E>("HMIEVENTS", 1, 0, "EVT");
    qmlRegisterType<QRCodeItem>("QRCodeItem", 1, 0, "QRCodeItem");
    qmlRegisterType<ENUNCHUCK>("NUNCHUCKTYPE", 1, 0, "NUNCHUCKTYPE");
    qmlRegisterType<EWARNING>("EWARNING", 1, 0, "EWARNING");
    QQuickViewer::instance()->initialized();
    QQuickViewer::instance()->initFonts(latoFonts);
    QQuickViewer::instance()->initFonts(montserratFonts);
    QQuickViewer::instance()->completed();
    QQuickViewer::instance()->registerContextProperty("MAINNET_SERVER", MAINNET_SERVER);
    QQuickViewer::instance()->registerContextProperty("TESTNET_SERVER", TESTNET_SERVER);
    QQuickViewer::instance()->registerContextProperty("BLOCKSTREAM_TESTNET", BLOCKSTREAM_TESTNET);
    QQuickViewer::instance()->registerContextProperty("BLOCKSTREAM_MAINNET", BLOCKSTREAM_MAINNET);
    QQuickViewer::instance()->registerContextProperty("AppModel", QVariant::fromValue(AppModel::instance()));
    QQuickViewer::instance()->registerContextProperty("AppSetting", QVariant::fromValue(AppSetting::instance()));
    QQuickViewer::instance()->sendEvent(E::EVT_STARTING_APPLICATION_LOCALMODE);
    QQuickViewer::instance()->show();
    return app.exec();
}
