#include "WalletKeys.h"
#include "Servers/Draco.h"

bool WalletKeys::GetHistorySignerList()
{
    QJsonObject output;
    QString errormsg = "";
    bool ret {false};
    ret = Draco::instance()->GetHistorySignerList(fingerPrint(), output, errormsg);
    DBG_INFO << ret << output;
    if(ret){
        QJsonArray histories = output["history"].toArray();
        QJsonArray v_histories;
        for (auto js : histories) {
            QJsonObject history = js.toObject();
            long int created_time_millis = static_cast<long int>(history.value("created_time_millis").toDouble()/1000);
            QDateTime date_time = QDateTime::fromTime_t(created_time_millis);
            history["created_time_millis"] = QString("%1")
                                                 .arg(date_time.date().toString("MMM dd,yyyy"));
            v_histories.append(history);
        }
        setHealthCheckHistory(v_histories);
    }
    return ret;
}

bool WalletKeys::SyncSignerToServer(const nunchuk::SingleSigner &signer)
{
    QJsonObject data;
    data["name"] = QString::fromStdString(signer.get_name());
    data["type"] = QString::fromStdString(SignerTypeToStr(signer.get_type()));
    QJsonArray tags;
    for (auto tag : signer.get_tags()) {
        tags.append(QString::fromStdString(SignerTagToStr(tag)));
    }
    data["tags"] = tags;
    if (signer.get_type() == nunchuk::SignerType::NFC) {
        QWarningMessage msgGetTap;
        nunchuk::TapsignerStatus tap = nunchukiface::instance()->GetTapsignerStatusFromMasterSigner(signer.get_master_fingerprint(), msgGetTap);

        QJsonObject tapsigner;
        tapsigner["card_id"] = QString::fromStdString(tap.get_card_ident());
        tapsigner["version"] = QString::fromStdString(tap.get_version());
        tapsigner["birth_height"] = tap.get_birth_height();
        tapsigner["is_testnet"] = tap.is_testnet();
        data["tapsigner"] = tapsigner;
    } else {
        data["tapsigner"] = {};
    }
    DBG_INFO << QString::fromStdString(signer.get_master_fingerprint()) << tags;
    bool ret = Draco::instance()->assistedKeyUpdateName(QString::fromStdString(signer.get_master_fingerprint()), "", data);
    return ret;
}

QVariantList WalletKeys::healthCheckHistory() const
{
    return m_healthCheckHistory.toVariantList();
}

void WalletKeys::setHealthCheckHistory(QJsonArray list)
{
    if (m_healthCheckHistory == list)
        return;
    m_healthCheckHistory = list;
    healthCheckHistoryChanged();
}

#include "QOutlog.h"
