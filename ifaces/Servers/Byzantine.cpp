#include "Byzantine.h"
#include "Draco.h"

using namespace Command;
Byzantine::Byzantine()
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

Byzantine::~Byzantine()
{

}

Byzantine *Byzantine::instance()
{
    static Byzantine mInstance;
    return &mInstance;
}

bool Byzantine::GetListGroupWallets(QJsonObject &output, QString &errormsg)
{
    QJsonObject data;
    data["offset"] = "0";
    data["limit"] = "100";
    data["statuses"] = "ACTIVE,DELETED";
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Group::CMD_IDX::GROUP_WALLET_LIST_WALLETS];
    QJsonObject jsonObj = getSync(cmd, data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj.value("data").toObject();
            return true;
        }
        else{
            errormsg = response_msg;
            DBG_INFO << response_code << response_msg;
#if 0 //NO NEED
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
#endif
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Byzantine::GetAllGroupWallets(QJsonObject &output, QString &errormsg)
{
    DBG_INFO;
    QJsonObject data;
    int     reply_code = -1;
    QString reply_msg  = "";

    QJsonObject jsonObj = getSync(commands[Group::CMD_IDX::GROUP_WALLETS_LIST], data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else{
            errormsg = response_msg;
            DBG_INFO << response_code << response_msg;
#if 0 //NO NEED
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
#endif
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Byzantine::GetOneGroupWallets(const QString group_id, QJsonObject &output, QString &errormsg)
{
    if (group_id.isEmpty()) return false;
    QJsonObject data;
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Group::CMD_IDX::GROUP_WALLETS_ONE];
    cmd.replace("{group_id}", group_id);
    QJsonObject jsonObj = getSync(cmd, data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else{
            errormsg = response_msg;
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Byzantine::AcceptGroupWallet(const QString group_id, QJsonObject &output, QString &errormsg)
{
    if (group_id.isEmpty()) return false;
    QJsonObject data;
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Group::CMD_IDX::GROUP_WALLETS_ACCEPT];
    cmd.replace("{group_id}", group_id);

    QJsonObject jsonObj = putSync(cmd, data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else{
            errormsg = response_msg;
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Byzantine::DenyGroupWallet(const QString group_id, QJsonObject &output, QString &errormsg)
{
    if (group_id.isEmpty()) return false;
    QJsonObject data;
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Group::CMD_IDX::GROUP_WALLETS_DENY];
    cmd.replace("{group_id}", group_id);

    QJsonObject jsonObj = putSync(cmd, data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else{
            errormsg = response_msg;
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Byzantine::ResetGroupWallet(const QString group_id, QJsonObject &output, QString &errormsg)
{
    if (group_id.isEmpty()) return false;
    QJsonObject data;
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Group::CMD_IDX::GROUP_DRAFT_WALLETS_RESET_CURRENT];
    cmd.replace("{group_id}", group_id);

    QJsonObject jsonObj = deleteSync(cmd, data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else{
            errormsg = response_msg;
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Byzantine::DeleteGroupWallet(const QString &wallet_id, const QString &group_id, const QStringList &signatures, const QString &passwordToken, const QString &secQuesToken, QJsonObject &output, QString &errormsg)
{
    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject data;
    data["nonce"] = nonce();
    data["body"] = {};
    QMap<QString, QString> params;
    for (int i = 0; i < signatures.count(); i++) {
        params[QString("AuthorizationX-%1").arg(i+1)] = signatures.at(i);
    }
    params["Verify-token"] = passwordToken;
    if (!secQuesToken.isEmpty()) {
        params["Security-Question-token"] = secQuesToken;
    }
    QString cmd = commands[Group::CMD_IDX::GROUP_WALLET_REMOVE_WALLET];
    cmd.replace("{wallet_id_or_local_id}",wallet_id);
    cmd.replace("{group_id}",group_id);
    QJsonObject jsonObj = deleteSync(cmd, {}, params, data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else {
            errormsg = response_msg;
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Byzantine::DeleteGroupWalletRequiredSignatures(const QString &wallet_id, const QString &group_id, QJsonObject &output, QString &errormsg)
{
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Group::CMD_IDX::GROUP_WALLET_REMOVE_WALLET_REQUIRED_SIGNATURES];
    cmd.replace("{wallet_id_or_local_id}",wallet_id);
    cmd.replace("{group_id}",group_id);
    QJsonObject jsonObj = postSync(cmd, {}, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else{
            errormsg = response_msg;
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Byzantine::GetGroupAlerts(const QString group_id, QJsonObject &output, QString &errormsg)
{
    if (group_id.isEmpty()) return false;
    QJsonObject data;
    QJsonObject criteria;
    criteria["offset"] = 0; // Adjust offset
    criteria["limit"] = 0;  // Adjust limit
    data["criteria"] = criteria;

    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Group::CMD_IDX::GROUP_WALLET_ALERTS];
    cmd.replace("{group_id}", group_id);
    QJsonObject jsonObj = getSync(cmd, data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else{
            errormsg = response_msg;
            DBG_INFO << response_code << response_msg;
#if 0 //NO NEED
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
#endif
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Byzantine::GetGroupAlertsCount(const QString group_id, QJsonObject &output, QString &errormsg)
{
    if (group_id.isEmpty()) return false;
    QJsonObject data;
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Group::CMD_IDX::GROUP_WALLET_ALERTS_COUNT];
    cmd.replace("{group_id}", group_id);
    QJsonObject jsonObj = getSync(cmd, data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else{
            errormsg = response_msg;
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Byzantine::DismissGroupAlert(const QString group_id, const QString alert_id, QJsonObject &output, QString &errormsg)
{
    if (group_id.isEmpty()) return false;
    QJsonObject data;
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Group::CMD_IDX::GROUP_WALLET_ALERTS_DISMISS];
    cmd.replace("{group_id}", group_id);
    cmd.replace("{alert_id}", alert_id);

    QJsonObject jsonObj = putSync(cmd, data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else{
            errormsg = response_msg;
#if 0 //NO NEED
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
#endif
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Byzantine::MarkGroupAlertAsRead(const QString group_id, const QString alert_id, QJsonObject &output, QString &errormsg)
{
    if (group_id.isEmpty()) return false;
    QJsonObject data;
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Group::CMD_IDX::GROUP_WALLET_ALERTS_READ];
    cmd.replace("{group_id}", group_id);
    cmd.replace("{alert_id}", alert_id);

    QJsonObject jsonObj = putSync(cmd, data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else{
            errormsg = response_msg;
#if 0 //NO NEED
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
#endif
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Byzantine::GetAllPermissions(QJsonObject &output, QString &errormsg)
{
    QJsonObject data;
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Group::CMD_IDX::GROUP_WALLETS_ALL_PERMISSION];
    QJsonObject jsonObj = getSync(cmd, data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else{
            errormsg = response_msg;
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Byzantine::GetOnePermissions(const QString slug, QJsonObject &output, QString &errormsg)
{
    QJsonObject data;
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Group::CMD_IDX::GROUP_WALLETS_ONE_PERMISSION];
    cmd.replace("{slug}", slug);
    QJsonObject jsonObj = getSync(cmd, data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else{
            errormsg = response_msg;
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Byzantine::GetDefaultPermissions(QJsonObject &output, QString &errormsg)
{
    QJsonObject data;
    data["n"] = 4; //Default
    data["m"] = 2; //Default
    data["required_server_key"] = true; //Default
    data["allow_inheritance"] = true; //Default
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Group::CMD_IDX::GROUP_WALLETS_ONE_PERMISSION];
    QJsonObject jsonObj = getSync(cmd, data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else{
            errormsg = response_msg;
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Byzantine::DraftWalletAddKey(const QString group_id, const QString request_id, const QJsonObject data, bool &isDuplicateKey, QString &errormsg)
{
    if (group_id.isEmpty()) return false;
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Group::CMD_IDX::GROUP_DRAFT_WALLETS_ADD_KEY];
    cmd.replace("{group_id}", group_id);
    QMap<QString, QString> paramsQuery;
    paramsQuery["request_id"] = request_id;
    QJsonObject jsonObj = postSync(cmd, paramsQuery, {}, data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            return true;
        }
        else{
            errormsg = response_msg;
            QString message = errorObj["message"].toString();
            if (message.contains("Duplicate")) {
                isDuplicateKey = true;
            }
            DBG_INFO << errorObj << isDuplicateKey;
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Byzantine::GetCurrentGroupWallet(const QString group_id, QJsonObject &output, QString &errormsg)
{
    if (group_id.isEmpty()) return false;
    QJsonObject data;
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Group::CMD_IDX::GROUP_WALLETS_GET_CURRENT];
    cmd.replace("{group_id}", group_id);
    QJsonObject jsonObj = getSync(cmd, data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else{
            errormsg = response_msg;
            DBG_INFO << response_code << response_msg;
#if 0 //NO NEED
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
#endif
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Byzantine::GetCurrentGroupDraftWallet(const QString group_id, QJsonObject &output, QString &errormsg)
{
    if (group_id.isEmpty()) return false;
    QJsonObject data;
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Group::CMD_IDX::GROUP_DRAFT_WALLETS_GET_CURRENT];
    cmd.replace("{group_id}", group_id);
    QJsonObject jsonObj = getSync(cmd, data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            DBG_INFO << output;
            return true;
        }
        else{
            errormsg = response_msg;
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Byzantine::GetAllListRequestAddKey(const QString &group_id, QJsonObject &output, QString &errormsg)
{
    if (group_id.isEmpty()) return false;
    QJsonObject data;
    data["statuses"] = "PENDING";
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Group::CMD_IDX::GROUP_DRAFT_WALLETS_REQUEST_ADD_KEY];
    cmd.replace("{group_id}", group_id);
    QJsonObject jsonObj = getSync(cmd, data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else {
            errormsg = response_msg;
            DBG_INFO << response_code << response_msg;
#if 0 //NO NEED
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
#endif
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Byzantine::DeleteRequestAddKey(const QString &group_id, const QString &request_id)
{
    if (group_id.isEmpty()) return false;
    QJsonObject data;
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Group::CMD_IDX::GROUP_DRAFT_WALLETS_DELETE_REQUEST_ADD_KEY];
    cmd.replace("{group_id}", group_id);
    cmd.replace("{request_id}", request_id);
    QJsonObject jsonObj = deleteSync(cmd, data, reply_code, reply_msg);
    if (reply_code == DRACO_CODE::SUCCESSFULL) {
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if (response_code == DRACO_CODE::RESPONSE_OK) {
            return true;
        }
        else{
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
            return false;
        }
    }
    return false;
}

bool Byzantine::UpdateWallet(const QString &group_id, const QString &wallet_id, const QString &name, const QString &description, QJsonObject &output, QString &errormsg)
{
    if (group_id.isEmpty()) return false;
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Group::CMD_IDX::GROUP_WALLET_UPDATE_WALLET];
    cmd.replace("{group_id}", group_id);
    cmd.replace("{wallet_id_or_local_id}", wallet_id);

    QJsonObject data;
    if(name != ""){
        data["name"] = name;
    }
    if(description != ""){
        data["description"] = description;
    }

    QJsonObject jsonObj = putSync(cmd, data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else{
            errormsg = response_msg;
            DBG_INFO << response_code << response_msg;
#if 0 //NO NEED
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
#endif
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Byzantine::GetWalletAlias(const QString &group_id, const QString &wallet_id, QJsonObject &output, QString &errormsg)
{
    if (group_id.isEmpty()) return false;
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Group::CMD_IDX::GROUP_WALLET_GET_ALIAS];
    cmd.replace("{group_id}", group_id);
    cmd.replace("{wallet_id_or_local_id}", wallet_id);

    QJsonObject data;
    QJsonObject jsonObj = getSync(cmd, data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else{
            errormsg = response_msg;
            DBG_INFO << response_code << response_msg;
#if 0 //NO NEED
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
#endif
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Byzantine::UpdateWalletAlias(const QString &group_id, const QString &wallet_id, const QString &alias, QJsonObject &output, QString &errormsg)
{
    if (group_id.isEmpty()) return false;
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Group::CMD_IDX::GROUP_WALLET_UPDATE_ALIAS];
    cmd.replace("{group_id}", group_id);
    cmd.replace("{wallet_id_or_local_id}", wallet_id);

    QJsonObject data;
    data["alias"] = alias;
    QJsonObject jsonObj = putSync(cmd, data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else{
            errormsg = response_msg;
            DBG_INFO << response_code << response_msg;
#if 0 //NO NEED
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
#endif
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Byzantine::DeleteWalletAlias(const QString &group_id, const QString &wallet_id, QJsonObject &output, QString &errormsg)
{
    if (group_id.isEmpty()) return false;
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Group::CMD_IDX::GROUP_WALLET_DELETE_ALIAS];
    cmd.replace("{group_id}", group_id);
    cmd.replace("{wallet_id_or_local_id}", wallet_id);

    QJsonObject data;
    QJsonObject jsonObj = deleteSync(cmd, data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else{
            errormsg = response_msg;
            DBG_INFO << response_code << response_msg;
#if 0 //NO NEED
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
#endif
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Byzantine::UpdateWalletPrimaryOwner(const QString &group_id, const QString &wallet_id, const QString &primary_membership_id, QJsonObject &output, QString &errormsg)
{
    if (group_id.isEmpty()) return false;
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Group::CMD_IDX::GROUP_WALLET_UPDATE_PRIMARY_OWNER];
    cmd.replace("{group_id}", group_id);
    cmd.replace("{wallet_id_or_local_id}", wallet_id);

    QJsonObject data;
    data["primary_membership_id"] = primary_membership_id;
    QJsonObject jsonObj = putSync(cmd, data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else{
            errormsg = response_msg;
            DBG_INFO << response_code << response_msg;
#if 0 //NO NEED
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
#endif
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Byzantine::GetCurrentGroupChat(const QString &group_id, QJsonObject &output, QString &errormsg)
{
    if (group_id.isEmpty()) return false;
    return true;
}

bool Byzantine::UpdateCurrentGroupChat(const QString &group_id, QJsonObject &output, QString &errormsg)
{
    if (group_id.isEmpty()) return false;
    return true;
}

bool Byzantine::DeleteCurrentGroupChat(const QString &group_id, QJsonObject &output, QString &errormsg)
{
    if (group_id.isEmpty()) return false;
    return true;
}

bool Byzantine::CreateCurrentGroupChat(const QString &group_id, QJsonObject &output, QString &errormsg)
{
    if (group_id.isEmpty()) return false;
    return true;

}

bool Byzantine::GetCurrentGroupChatHistory(const QString &group_id, QJsonObject &output, QString &errormsg)
{
    if (group_id.isEmpty()) return false;
    return true;
}

QString Byzantine::nonce()
{
    return Draco::instance()->randomNonce();
}

bool Byzantine::RequestHealthCheckForKey(const QString &group_id, const QString &wallet_id, const QString &xfp, QJsonObject &output, QString &errormsg)
{
    if (group_id.isEmpty()) return false;
    QJsonObject data;
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Group::CMD_IDX::GROUP_WALLETS_REQUEST_HEALTHCHECK];
    cmd.replace("{group_id}", group_id);
    cmd.replace("{wallet_id_or_local_id}", wallet_id);
    cmd.replace("{xfp}", xfp);

    QJsonObject jsonObj = postSync(cmd, data, reply_code, reply_msg);
    if (reply_code == DRACO_CODE::SUCCESSFULL) {
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else {
            errormsg = response_msg;
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Byzantine::HealthCheckForKey(const QString &group_id, const QString &wallet_id, const QString &xfp, QJsonObject &output, QString &errormsg)
{
    if (group_id.isEmpty()) return false;
    QJsonObject data;
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Group::CMD_IDX::GROUP_WALLETS_HEALTHCHECK];
    cmd.replace("{group_id}", group_id);
    cmd.replace("{wallet_id_or_local_id}", wallet_id);
    cmd.replace("{xfp}", xfp);

    data["nonce"] = nonce();
    data["body"] = QJsonObject();

    QJsonObject jsonObj = postSync(cmd, data, reply_code, reply_msg);
    if (reply_code == DRACO_CODE::SUCCESSFULL) {
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else {
            errormsg = response_msg;
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Byzantine::GetWalletHealthStatus(const QString &group_id, const QString &wallet_id, QJsonObject &output, QString &errormsg)
{
    if (group_id.isEmpty()) return false;
    QJsonObject data;
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Group::CMD_IDX::GROUP_WALLETS_GET_HEALTH];
    cmd.replace("{group_id}", group_id);
    cmd.replace("{wallet_id_or_local_id}", wallet_id);

    QJsonObject jsonObj = getSync(cmd, data, reply_code, reply_msg);
    if (reply_code == DRACO_CODE::SUCCESSFULL) {
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else {
            errormsg = response_msg;
            DBG_INFO << response_code << response_msg;
#if 0 //NO NEED
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
#endif
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Byzantine::GetDummyTransaction(const QString &group_id, const QString &wallet_id, const QString &txid, QJsonObject &output, QString &errormsg)
{
    if (group_id.isEmpty()) return false;
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Group::CMD_IDX::GROUP_WALLETS_GET_DUMMY_TX];
    cmd.replace("{group_id}", group_id);
    cmd.replace("{wallet_id_or_local_id}", wallet_id);
    cmd.replace("{dummy_transaction_id}", txid);

    QJsonObject data;
    QJsonObject jsonObj = getSync(cmd, data, reply_code, reply_msg);
    if (reply_code == DRACO_CODE::SUCCESSFULL) {
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else {
            errormsg = response_msg;
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Byzantine::UpdateDummyTransaction(const QString &group_id, const QString &wallet_id, const QStringList &authorizations, const QString &txid, QJsonObject &output, QString &errormsg)
{
    if (group_id.isEmpty()) return false;
    QMap<QString, QString> params;
    for (int i = 0; i < authorizations.count(); i++) {
        params[QString("AuthorizationX-%1").arg(i+1)] = authorizations.at(i);
    }
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Group::CMD_IDX::GROUP_WALLETS_UPDATE_DUMMY_TX];
    cmd.replace("{group_id}", group_id);
    cmd.replace("{wallet_id_or_local_id}", wallet_id);
    cmd.replace("{dummy_transaction_id}", txid);

    QJsonObject data;
    QJsonObject jsonObj = putSync(cmd, {}, params, data, reply_code, reply_msg);
    if (reply_code == DRACO_CODE::SUCCESSFULL) {
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else {
            errormsg = response_msg;
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Byzantine::CancelDummyTransaction(const QString &group_id, const QString &wallet_id, const QString &txid, QJsonObject &output, QString &errormsg)
{
    if (group_id.isEmpty()) return false;
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Group::CMD_IDX::GROUP_WALLETS_GET_DUMMY_TX];
    cmd.replace("{group_id}", group_id);
    cmd.replace("{wallet_id_or_local_id}", wallet_id);
    cmd.replace("{dummy_transaction_id}", txid);

    QJsonObject data;
    QJsonObject jsonObj = deleteSync(cmd, data, reply_code, reply_msg);
    if (reply_code == DRACO_CODE::SUCCESSFULL) {
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else {
            errormsg = response_msg;
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Byzantine::FinalizeDummyTransaction(const QString &group_id, const QString &wallet_id, const QString &txid, QJsonObject &output, QString &errormsg)
{
    if (group_id.isEmpty()) return false;
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Group::CMD_IDX::GROUP_WALLETS_FINALIZE_DUMMY_TX];
    cmd.replace("{group_id}", group_id);
    cmd.replace("{wallet_id_or_local_id}", wallet_id);
    cmd.replace("{dummy_transaction_id}", txid);

    QJsonObject data;
    QJsonObject jsonObj = putSync(cmd, data, reply_code, reply_msg);
    if (reply_code == DRACO_CODE::SUCCESSFULL) {
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else {
            errormsg = response_msg;
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Byzantine::GetAllTransaction(const QString &group_id, const QString &wallet_id, QJsonObject &output, QString &errormsg)
{
    if (group_id.isEmpty()) return false;
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Group::CMD_IDX::GROUP_WALLETS_GET_ALL_TX];
    cmd.replace("{group_id}", group_id);
    cmd.replace("{wallet_id_or_local_id}", wallet_id);

    QJsonObject data;
    data["offset"] = "0";
    data["limit"] = "100";
    data["statuses"] = "PENDING_SIGNATURES,READY_TO_BROADCAST";
    data["types"] = "STANDARD,SCHEDULED,CLAIMING,ROLLOVER,RECURRING";

    QJsonObject jsonObj = getSync(cmd, data, reply_code, reply_msg);
    if (reply_code == DRACO_CODE::SUCCESSFULL) {
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else {
            errormsg = response_msg;
            DBG_INFO << response_code << response_msg;
#if 0 //NO NEED
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
#endif
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Byzantine::GetAllCancelledTransaction(const QString &group_id, const QString &wallet_id, const int offset, const int limit, QJsonObject &output, QString &errormsg)
{
    if (group_id.isEmpty()) return false;
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Group::CMD_IDX::GROUP_WALLETS_GET_ALL_TX];
    cmd.replace("{group_id}", group_id);
    cmd.replace("{wallet_id_or_local_id}", wallet_id);

    QJsonObject data;
    data["offset"] = QString("%1").arg(offset);
    data["limit"] = QString("%1").arg(limit);
    data["statuses"] = "CANCELED";
    data["types"] = "STANDARD,SCHEDULED,CLAIMING,ROLLOVER";

    QJsonObject jsonObj = getSync(cmd, data, reply_code, reply_msg);
    if (reply_code == DRACO_CODE::SUCCESSFULL) {
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else {
            errormsg = response_msg;
            DBG_INFO << response_code << response_msg;
#if 0 //NO NEED
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
#endif
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Byzantine::GetOneTransaction(const QString &group_id, const QString &wallet_id, const QString &txid, QJsonObject &output, QString &errormsg)
{
    if (group_id.isEmpty()) return false;
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Group::CMD_IDX::GROUP_WALLETS_GET_ONE_TX];
    cmd.replace("{group_id}", group_id);
    cmd.replace("{wallet_id_or_local_id}", wallet_id);
    cmd.replace("{transaction_id}", txid);

    QJsonObject jsonObj = getSync(cmd, QJsonObject(), reply_code, reply_msg);
    if (reply_code == DRACO_CODE::SUCCESSFULL) {
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else {
            errormsg = response_msg;
            DBG_INFO << response_code << response_msg;
#if 0 //NO NEED
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
#endif
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Byzantine::UpdateTransaction(const QString &group_id, const QString &wallet_id, const QString &txid, const QString &note, QJsonObject &output, QString &errormsg)
{
    if (group_id.isEmpty()) return false;
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Group::CMD_IDX::GROUP_WALLETS_UPDATE_TX];
    cmd.replace("{group_id}", group_id);
    cmd.replace("{wallet_id_or_local_id}", wallet_id);
    cmd.replace("{transaction_id}", txid);

    QJsonObject data;
    data["note"] = note;

    QJsonObject jsonObj = putSync(cmd, data, reply_code, reply_msg);
    if (reply_code == DRACO_CODE::SUCCESSFULL) {
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else {
            errormsg = response_msg;
            DBG_INFO << response_code << response_msg;
#if 0 // NO NEED
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
#endif
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Byzantine::CancelTransaction(const QString &group_id, const QString &wallet_id, const QString &txid, QJsonObject &output, QString &errormsg)
{
    if (group_id.isEmpty()) return false;
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Group::CMD_IDX::GROUP_WALLETS_CANCEL_TX];
    cmd.replace("{group_id}", group_id);
    cmd.replace("{wallet_id_or_local_id}", wallet_id);
    cmd.replace("{transaction_id}", txid);

    QJsonObject jsonObj = deleteSync(cmd, QJsonObject(), reply_code, reply_msg);
    if (reply_code == DRACO_CODE::SUCCESSFULL) {
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else {
            errormsg = response_msg;
            DBG_INFO << response_code << response_msg;
#if 0 //NO NEED
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
#endif
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Byzantine::SyncTransaction(const QString &group_id, const QString &wallet_id, const QString &txid, const QString &psbt, const QString &note, QJsonObject &output, QString &errormsg)
{
    if (group_id.isEmpty()) return false;
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Group::CMD_IDX::GROUP_WALLETS_SYNC_TX];
    cmd.replace("{group_id}", group_id);
    cmd.replace("{wallet_id_or_local_id}", wallet_id);
    cmd.replace("{transaction_id}", txid);

    QJsonObject data;
    data["psbt"] = psbt;
    data["note"] = note;

    QJsonObject jsonObj = postSync(cmd, data, reply_code, reply_msg);
    if (reply_code == DRACO_CODE::SUCCESSFULL) {
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else {
            errormsg = response_msg;
            DBG_INFO << response_code << response_msg;
#if 0 //NO NEED
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
#endif
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Byzantine::RbfTransaction(const QString &group_id, const QString &wallet_id, const QString &txid, const QString &psbt, QJsonObject &output, QString &errormsg)
{
    if (group_id.isEmpty()) return false;
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Group::CMD_IDX::GROUP_WALLETS_RBF_TX];
    cmd.replace("{group_id}", group_id);
    cmd.replace("{wallet_id_or_local_id}", wallet_id);
    cmd.replace("{transaction_id}", txid);

    QJsonObject data;
    data["psbt"] = psbt;

    QJsonObject jsonObj = putSync(cmd, data, reply_code, reply_msg);
    if (reply_code == DRACO_CODE::SUCCESSFULL) {
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else {
            errormsg = response_msg;
            DBG_INFO << response_code << response_msg;
#if 0 //NO NEED
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
#endif
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Byzantine::SignTransaction(const QString &group_id, const QString &wallet_id, const QString &txid, const QString &psbt, const QString &note, QJsonObject &output, QString &errormsg)
{
    if (group_id.isEmpty()) return false;
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Group::CMD_IDX::GROUP_WALLETS_SIGN_TX];
    cmd.replace("{group_id}", group_id);
    cmd.replace("{wallet_id_or_local_id}", wallet_id);
    cmd.replace("{transaction_id}", txid);

    QJsonObject data;
    data["psbt"] = psbt;
    data["note"] = note;

    QJsonObject jsonObj = postSync(cmd, data, reply_code, reply_msg);
    if (reply_code == DRACO_CODE::SUCCESSFULL) {
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else {
            errormsg = response_msg;
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Byzantine::CreateTransaction(const QString &group_id, const QString &wallet_id, const QString &psbt, const QString &note, QJsonObject &output, QString &errormsg)
{
    if (group_id.isEmpty()) return false;
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Group::CMD_IDX::GROUP_WALLETS_CREATE_TX];
    cmd.replace("{group_id}", group_id);
    cmd.replace("{wallet_id_or_local_id}", wallet_id);

    QJsonObject data;
    data["psbt"] = psbt;
    data["note"] = note;

    QJsonObject jsonObj = postSync(cmd, data, reply_code, reply_msg);
    if (reply_code == DRACO_CODE::SUCCESSFULL) {
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else {
            errormsg = response_msg;
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Byzantine::requestSignature(const QString &group_id, const QString &wallet_id, const QString &txid, const QString &membership_id, QJsonObject &output, QString &errormsg)
{
    if (group_id.isEmpty()) return false;
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Group::CMD_IDX::GROUP_WALLETS_REQUEST_SIGNATURE_TX];
    cmd.replace("{group_id}", group_id);
    cmd.replace("{wallet_id_or_local_id}", wallet_id);
    cmd.replace("{transaction_id}", txid);
    QJsonObject data;
    data["membership_id"] = membership_id;

    QJsonObject jsonObj = postSync(cmd, data, reply_code, reply_msg);
    if (reply_code == DRACO_CODE::SUCCESSFULL) {
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else {
            errormsg = response_msg;
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Byzantine::ServerKeysGet(const QString &group_id, const QString &key_id_or_xfp, const QString &derivation_path, QJsonObject &output, QString &errormsg)
{
    if (group_id.isEmpty()) return false;
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Group::CMD_IDX::SERVER_KEYS_GET];
    cmd.replace("{group_id}", group_id);
    cmd.replace("{key_id_or_xfp}", key_id_or_xfp);
    QJsonObject data;
    data["derivation_path"] = derivation_path;

    QJsonObject jsonObj = getSync(cmd, data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            QJsonObject data = jsonObj["data"].toObject();
            output = data["key"].toObject();
            return true;
        }
        else{
            errormsg = response_msg;
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Byzantine::ServerKeysUpdate(const QString &group_id,
                                 const QString &passwordToken,
                                 const QString &secQuesToken,
                                 const QString &key_id_or_xfp,
                                 const QString &derivation_path,
                                 const QStringList &signatures,
                                 const QJsonObject& request_body,
                                 bool isDraft,
                                 QJsonObject &output,
                                 QString &errormsg)
{
    if (group_id.isEmpty()) return false;
    int     reply_code = -1;
    QString reply_msg  = "";
    QMap<QString, QString> params;
    for (int i = 0; i < signatures.count(); i++) {
        params[QString("AuthorizationX-%1").arg(i+1)] = signatures.at(i);
    }
    params["Verify-token"] = passwordToken;
    if (!secQuesToken.isEmpty()) {
        params["Security-Question-token"] = secQuesToken;
    }
    QMap<QString, QString> paramsQuery;
    paramsQuery["derivation_path"] = derivation_path;
    paramsQuery["draft"] = isDraft ? "true" : "false";

    QString cmd = commands[Group::CMD_IDX::SERVER_KEYS_UPDATE];
    cmd.replace("{group_id}", group_id);
    cmd.replace("{key_id_or_xfp}", key_id_or_xfp);

    QJsonObject jsonObj = putSync(cmd, paramsQuery, params, request_body, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else{
            errormsg = response_msg;
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Byzantine::ServerKeysRequiredSignature(const QString &group_id,
                                            const QString &key_id_or_xfp,
                                            const QString &derivation_path,
                                            const QJsonObject &request_body,
                                            QJsonObject &output,
                                            QString &errormsg)
{
    if (group_id.isEmpty()) return false;
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Group::CMD_IDX::SERVER_KEYS_REQUIRED_SIGNATURES];
    cmd.replace("{key_id_or_xfp}", key_id_or_xfp);
    cmd.replace("{group_id}", group_id);
    QMap<QString, QString> paramsQuery;
    paramsQuery["derivation_path"] = derivation_path;

    QJsonObject jsonObj = postSync(cmd, paramsQuery, {}, request_body, reply_code, reply_msg);
    DBG_INFO << jsonObj;
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else{
            errormsg = response_msg;
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Byzantine::lockdownPeriods(QJsonArray &output, QString &errormsg)
{
    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = getSync(commands[Group::CMD_IDX::LOCKDOWN_GET_PERIOD], QJsonObject(), reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            QJsonObject dataObj = jsonObj["data"].toObject();
            QJsonArray periods = dataObj["periods"].toArray();
            output = periods;
            return true;
        }
        else{
            errormsg = response_msg;
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Byzantine::lockdownRequiredSignatures(const QString &group_id,
                                           const QString &period_id,
                                           const QString &wallet_id,
                                           QJsonObject &output,
                                           QString &errormsg)
{
    QJsonObject data;
    data["period_id"] = period_id;
    data["group_id"] = group_id;
    data["wallet"] = wallet_id;

    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = postSync(commands[Group::CMD_IDX::LOCKDOWN_REQUIRED_SIGNATURES], data, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else{
            errormsg = response_msg;
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Byzantine::lockdownByAnswerSecQues(const QString &group_id, const QString &passwordToken, const QString &secQuesToken, const QString &period_id, const QString &wallet_id, QString &until_time, QString &errormsg)
{
    QJsonObject body;
    body["period_id"] = period_id;
    body["group_id"] = group_id;
    body["wallet"] = wallet_id;
    QJsonObject data;
    data["nonce"] = Draco::instance()->randomNonce();
    data["body"] = body;

    QMap<QString, QString> params;
    params["Verify-token"] = passwordToken;
    params["Security-Question-token"] = secQuesToken;
    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = postSync(commands[Group::CMD_IDX::LOCKDOWN_SET_ANSWER_SECURITY_QUESTION], {}, params, data, reply_code, reply_msg);
    DBG_INFO << jsonObj;
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            QJsonObject dataObj = jsonObj["data"].toObject();
            QJsonObject resultObj = dataObj["period"].toObject();
            until_time = resultObj["display_name"].toString();
            return true;
        }
        else{
            errormsg = response_msg;
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Byzantine::lockdownByConfirmationCode(const QString &passwordToken, const QString &confirmToken, const QJsonObject &requestBody, QString &until_time, QString &errormsg)
{
    QMap<QString, QString> params;
    params["Verify-token"] = passwordToken;
    params["Confirmation-token"] = confirmToken;
    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = postSync(commands[Group::CMD_IDX::LOCKDOWN_SET_CONFIRMATION_CODE], {}, params, requestBody, reply_code, reply_msg);
    DBG_INFO << jsonObj;
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            QJsonObject dataObj = jsonObj["data"].toObject();
            QJsonObject resultObj = dataObj["period"].toObject();
            until_time = resultObj["display_name"].toString();
            return true;
        }
        else{
            errormsg = response_msg;
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Byzantine::lockdownBySignDummyTx(const QString &group_id, const QStringList &signatures, const QString &passwordToken, const QString &period_id, const QString &wallet_id, QString &until_time, QString &errormsg)
{
    QMap<QString, QString> params;
    for (int i = 0; i < signatures.count(); i++) {
        params[QString("AuthorizationX-%1").arg(i+1)] = signatures.at(i);
    }
    params["Verify-token"] = passwordToken;

    QJsonObject body;
    body["period_id"] = period_id;
    body["group_id"] = group_id;
    body["wallet"] = wallet_id;
    QJsonObject data;
    data["nonce"] = Draco::instance()->randomNonce();
    data["body"] = body;

    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = postSync(commands[Group::CMD_IDX::LOCKDOWN_SET_SIGN_DUMMY_TX], {}, params, data, reply_code, reply_msg);
    qInfo() << jsonObj;
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            QJsonObject dataObj = jsonObj["data"].toObject();
            QJsonObject resultObj = dataObj["period"].toObject();
            until_time = resultObj["display_name"].toString();
            qInfo() << resultObj;
            return true;
        }
        else{
            errormsg = response_msg;
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

QJsonObject Byzantine::assistedGetWalletConfig()
{
    int     reply_code = -1;
    QString reply_msg  = "";
    QJsonObject jsonObj = getSync(commands[Group::CMD_IDX::ASSISTED_WALLET_GET_CONFIG], {}, reply_code, reply_msg);
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            QJsonObject data = jsonObj.value("data").toObject();
            return data;
        }
        else{
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
            return {};
        }
        DBG_INFO << errorObj;
    }
    return {};
}


bool Byzantine::GetGroupChat(const QString &group_id, QJsonObject &output, QString &errormsg)
{
    if (!group_id.isEmpty()) {
        int     reply_code = -1;
        QString reply_msg  = "";
        QString cmd = commands[Group::CMD_IDX::GROUP_WALLETS_CHAT_GET];
        cmd.replace("{group_id}", group_id);
        QJsonObject data;
        QJsonObject jsonObj = getSync(cmd, data, reply_code, reply_msg);
        if (reply_code == DRACO_CODE::SUCCESSFULL) {
            QJsonObject errorObj = jsonObj["error"].toObject();
            int response_code = errorObj["code"].toInt();
            QString response_msg = errorObj["message"].toString();
            if(response_code == DRACO_CODE::RESPONSE_OK){
                output = jsonObj["data"].toObject();
                return true;
            }
            else {
                errormsg = response_msg;
                AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
                return false;
            }
        }
        else{
            errormsg = reply_msg;
        }
    }
    return false;
}

bool Byzantine::UpdateGroupChat(const QString &group_id, QJsonObject &output, QString &errormsg)
{
    if (!group_id.isEmpty()) {
        int     reply_code = -1;
        QString reply_msg  = "";
        QString cmd = commands[Group::CMD_IDX::GROUP_WALLETS_CHAT_UPDATE];
        cmd.replace("{group_id}", group_id);
        QJsonObject data;
        QJsonObject jsonObj = putSync(cmd, data, reply_code, reply_msg);
        if (reply_code == DRACO_CODE::SUCCESSFULL) {
            QJsonObject errorObj = jsonObj["error"].toObject();
            int response_code = errorObj["code"].toInt();
            QString response_msg = errorObj["message"].toString();
            if(response_code == DRACO_CODE::RESPONSE_OK){
                output = jsonObj["data"].toObject();
                return true;
            }
            else {
                errormsg = response_msg;
                AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
                return false;
            }
        }
        else{
            errormsg = reply_msg;
        }
    }
    return false;
}

bool Byzantine::DeleteGroupChat(const QString &group_id, QJsonObject &output, QString &errormsg)
{
    if (!group_id.isEmpty()) {
        int     reply_code = -1;
        QString reply_msg  = "";
        QString cmd = commands[Group::CMD_IDX::GROUP_WALLETS_CHAT_DELETE];
        cmd.replace("{group_id}", group_id);
        QJsonObject data;
        QJsonObject jsonObj = deleteSync(cmd, data, reply_code, reply_msg);
        if (reply_code == DRACO_CODE::SUCCESSFULL) {
            QJsonObject errorObj = jsonObj["error"].toObject();
            int response_code = errorObj["code"].toInt();
            QString response_msg = errorObj["message"].toString();
            if(response_code == DRACO_CODE::RESPONSE_OK){
                output = jsonObj["data"].toObject();
                return true;
            }
            else {
                errormsg = response_msg;
                AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
                return false;
            }
        }
        else{
            errormsg = reply_msg;
        }
    }
    return false;
}

bool Byzantine::CreateGroupChat(const QString &group_id, const QString &room_id, QJsonObject &output, QString &errormsg)
{
    DBG_ERROR << group_id << room_id;
    if (!group_id.isEmpty()) {
        int     reply_code = -1;
        QString reply_msg  = "";
        QString cmd = commands[Group::CMD_IDX::GROUP_WALLETS_CHAT_CREATE];
        cmd.replace("{group_id}", group_id);

        QJsonObject data;
        data["room_id"] = room_id;

        QJsonObject jsonObj = postSync(cmd, data, reply_code, reply_msg);
        DBG_INFO << jsonObj;
        if (reply_code == DRACO_CODE::SUCCESSFULL) {
            QJsonObject errorObj = jsonObj["error"].toObject();
            int response_code = errorObj["code"].toInt();
            QString response_msg = errorObj["message"].toString();
            if(response_code == DRACO_CODE::RESPONSE_OK){
                output = jsonObj["data"].toObject();
                return true;
            }
            else {
                errormsg = response_msg;
                AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
                return false;
            }
        }
    }
    return false;
}

bool Byzantine::GetAllGroupChat(QJsonObject &output, QString &errormsg)
{
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Group::CMD_IDX::GROUP_WALLETS_CHAT_GET_ALL];
    QJsonObject data;
    QJsonObject jsonObj = getSync(cmd, data, reply_code, reply_msg);
    if (reply_code == DRACO_CODE::SUCCESSFULL) {
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else {
            errormsg = response_msg;
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Byzantine::GetHistoryPeriodsGroupChat(QJsonObject &output, QString &errormsg)
{
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Group::CMD_IDX::GROUP_WALLETS_CHAT_GET_HISTORY_PERIODS];
    QJsonObject data;
    QJsonObject jsonObj = getSync(cmd, data, reply_code, reply_msg);
    if (reply_code == DRACO_CODE::SUCCESSFULL) {
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else {
            errormsg = response_msg;
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Byzantine::GetListRecurringPayment(const QString &group_id, const QString &wallet_id, QJsonObject &output, QString &errormsg)
{
    if (group_id.isEmpty()) return false;
    QJsonObject data;
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Group::CMD_IDX::GROUP_WALLET_RECURRING_PAYMENT_LIST];
    cmd.replace("{group_id}", group_id);
    cmd.replace("{wallet_id_or_local_id}", wallet_id);

    QJsonObject jsonObj = getSync(cmd, data, reply_code, reply_msg);
    if (reply_code == DRACO_CODE::SUCCESSFULL) {
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else {
            errormsg = response_msg;
            DBG_INFO << response_code << response_msg;
#if 0 //NO NEED
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
#endif
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Byzantine::CreateRecurringPayment(const QString &group_id,
                                       const QString &wallet_id,
                                       const QStringList &signatures,
                                       const QJsonObject &request_body,
                                       const bool isDraft,
                                       QJsonObject &output,
                                       QString &errormsg)
{
    if (group_id.isEmpty()) return false;
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Group::CMD_IDX::GROUP_WALLET_RECURRING_PAYMENT_CREATE];
    cmd.replace("{group_id}", group_id);
    cmd.replace("{wallet_id_or_local_id}", wallet_id);

    QMap<QString, QString> headers;
    for (int i = 0; i < signatures.count(); i++) {
        headers[QString("AuthorizationX-%1").arg(i+1)] = signatures.at(i);
    }
    QMap<QString, QString> queries;
    queries["draft"] = isDraft ? "true" : "false";
    DBG_INFO << request_body;
    QJsonObject jsonObj = postSync(cmd, queries, headers, request_body, reply_code, reply_msg);
    DBG_INFO << jsonObj;
    if(reply_code == DRACO_CODE::SUCCESSFULL){
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else{
            errormsg = response_msg;
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Byzantine::GetOneRecurringPayment(const QString &group_id, const QString &wallet_id, const QString &recurring_payment_id, QJsonObject &output, QString &errormsg)
{
    if (group_id.isEmpty()) return false;
    QJsonObject data;
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Group::CMD_IDX::GROUP_WALLET_RECURRING_PAYMENT_GET];
    cmd.replace("{group_id}", group_id);
    cmd.replace("{wallet_id_or_local_id}", wallet_id);
    cmd.replace("{recurring_payment_id}", recurring_payment_id);

    QJsonObject jsonObj = getSync(cmd, data, reply_code, reply_msg);
    if (reply_code == DRACO_CODE::SUCCESSFULL) {
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else {
            errormsg = response_msg;
            DBG_INFO << response_code << response_msg;
#if 0 //NO NEED
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
#endif
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Byzantine::CancelRecurringPayment(const QString &group_id,
                                       const QString &wallet_id,
                                       const QString &recurring_payment_id,
                                       const QStringList &signatures,
                                       const QJsonObject& request_body,
                                       const bool isDraft,
                                       QJsonObject& output,
                                       QString &errormsg)
{
    if (group_id.isEmpty()) return false;
    int     reply_code = -1;
    QString reply_msg  = "";
    QString cmd = commands[Group::CMD_IDX::GROUP_WALLET_RECURRING_PAYMENT_CANCEL];
    cmd.replace("{group_id}", group_id);
    cmd.replace("{wallet_id_or_local_id}", wallet_id);
    cmd.replace("{recurring_payment_id}", recurring_payment_id);

    QMap<QString, QString> headers;
    for (int i = 0; i < signatures.count(); i++) {
        headers[QString("AuthorizationX-%1").arg(i+1)] = signatures.at(i);
    }
    QMap<QString, QString> queries;
    queries["draft"] = isDraft ? "true" : "false";
    DBG_INFO << request_body;
    QJsonObject jsonObj = deleteSync(cmd, queries, headers, request_body, reply_code, reply_msg);
    if (reply_code == DRACO_CODE::SUCCESSFULL) {
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if(response_code == DRACO_CODE::RESPONSE_OK){
            output = jsonObj["data"].toObject();
            return true;
        }
        else {
            errormsg = response_msg;
            DBG_INFO << response_code << response_msg;
#if 0 //NO NEED
            AppModel::instance()->showToast(response_code, response_msg, EWARNING::WarningType::EXCEPTION_MSG);
#endif
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}
