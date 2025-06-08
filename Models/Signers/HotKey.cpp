#include "HotKey.h"
#include "Chats/ClientController.h"
#include "Models/AppModel.h"
#include "Models/MasterSignerModel.h"
#include "Servers/Draco.h"
#include "ViewsEnums.h"
#include "bridgeifaces.h"
#include "localization/STR_CPP.h"

void HotKey::createHotKey() {
    auto listSS = AppModel::instance()->masterSignerListPtr()->fullList();
    QList<int> listId;
    for (auto signer : listSS) {
        if (signer->signerType() == (int)nunchuk::SignerType::SOFTWARE) {
            if (signer->name().startsWith("My key")) {
                if (qUtils::strCompare(signer->name(), "My key")) {
                    listId.append(0);
                } else {
                    QStringList listName = signer->name().split("My key");
                    QString lastName = listName.at(1);
                    lastName.replace("#", "");
                    bool isOK = false;
                    int id = lastName.toInt(&isOK);
                    DBG_INFO << listName << lastName << id << isOK;
                    if (isOK) {
                        listId.append(id);
                    }
                }
            }
        }
    }
    std::sort(listId.begin(), listId.end());
    QString nameKey = listId.isEmpty() ? "My key" : "My key #" + QString::fromStdString(std::to_string(listId.last() + 1));
    QString mnemonic = qUtils::GenerateMnemonic();
    QWarningMessage msg;
    DBG_INFO << nameKey << listId;
    auto ss = bridge::nunchukCreateSoftwareSigner(nameKey, mnemonic, "", false, false, msg);
    if ((int)EWARNING::WarningType::NONE_MSG == msg.type()) {
        ss->setNeedBackup(true);
        bridge::nunchukUpdateMasterSigner(ss);
        emit AppModel::instance()->startReloadMasterSigners();
        AppModel::instance()->setMasterSignerInfo(ss);
        finishCreateSoftwareSigner();
    }
}

void HotKey::requestBackupHotKey(const QString &masterSignerId) {
    QString mnemonic = bridge::GetHotKeyMnemonic(masterSignerId);
    AppModel::instance()->setMnemonic(mnemonic);
    setScreenFlow("backup-hot-key");
}

void HotKey::finishBackupHotKey() {
    emit AppModel::instance() -> startReloadMasterSigners();
    AppModel::instance()->setMnemonic("");
    clearState();
    if (auto s = AppModel::instance()->masterSignerInfoPtr()) {
        QWarningMessage msg;
        s->setNeedBackup(false);
        bridge::UpdateMasterSigner(s->originMasterSigner(), msg);
        AppModel::instance()->masterSignerList()->reloadOriginMasterSignerById(s->id());
        emit AppModel::instance()->startReloadMasterSigners();
        auto newSigner = AppModel::instance()->masterSignerList()->getMasterSignerById(s->id());
        if (newSigner) {
            AppModel::instance()->setMasterSignerInfo(newSigner);
        }
    }
    QString msg = QString("You have successfully backed up this key");
    AppModel::instance()->showToast(0, msg, EWARNING::WarningType::SUCCESS_MSG);
}