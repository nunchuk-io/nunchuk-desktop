#include <QQmlEngine>
#include <QJsonDocument>
#include "bridgeifaces.h"
#include "AppModel.h"
#include "QUserWallets.h"
#include "localization/STR_CPP.h"
#include "ServiceSetting.h"
#include "nunchuckiface.h"
#include "ViewsEnums.h"

QUserWallets::QUserWallets()
    : QAssistedDraftWallets(USER_WALLET)
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

QUserWallets::~QUserWallets()
{

}

QUserWallets *QUserWallets::instance()
{
    static QUserWallets mInstance;
    return &mInstance;
}
