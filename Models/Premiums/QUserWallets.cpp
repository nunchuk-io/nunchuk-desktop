#include <QQmlEngine>
#include <QJsonDocument>
#include "bridgeifaces.h"
#include "AppModel.h"
#include "QUserWallets.h"
#include "localization/STR_CPP.h"
#include "ServiceSetting.h"
#include "nunchuckiface.h"
#include "ViewsEnums.h"
#include "ifaces/Servers/Draco.h"

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

void QUserWallets::GetDraftWallet()
{
    QJsonObject output;
    QString error_msg = "";
    bool ret = Draco::instance()->DraftWalletGetCurrent(output, error_msg);
    if (ret) {
        QJsonObject draft_wallet = output["draft_wallet"].toObject();
        if (draft_wallet.isEmpty()) {
            mDashboard.clear();
        }
        else {
            QJsonObject info;
            info["status"] = "PENDING_WALLET";
            info["id"] = draft_wallet["id"];
            QJsonObject user;
            DracoUser   me   = ClientController::instance()->getMe();
            user["id"] = me.id;
            user["name"] = me.name;
            user["email"] = me.email;
            user["avatar"] = me.avatar;
            user["chat_id"] = me.chat_id;
            QJsonObject member;
            member["user"] = user;
            member["status"] = "ACTIVE";
            member["role"] = "MASTER";
            QJsonArray members;
            members.append(member);
            info["members"] = members;
            QGroupDashboardPtr dashboard = QGroupDashboardPtr(new QGroupDashboard(""));
            dashboard->setGroupInfo(info);
            DBG_INFO << info;
            mDashboard = dashboard;
            QJsonArray signers = draft_wallet["signers"].toArray();
            for (auto js : signers) {
                QJsonObject signer = js.toObject();
                WalletsMng->UpdateSigner(signer);
            }
            emit WalletsMng->signalUpdateSigner();
        }
    }
}

QGroupDashboardPtr QUserWallets::dashboardInfoPtr()
{
    return mDashboard;
}

bool QUserWallets::hasDraftWallet(const QString& group_id)
{
    if (!mDashboard.isNull() && mDashboard->groupId() == group_id) {
        return true;
    }
    return false;
}
