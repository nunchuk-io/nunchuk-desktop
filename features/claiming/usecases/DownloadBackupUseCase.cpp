#include "DownloadBackupUseCase.h"
#include "core/bridge/ExternalBridges.h"
#include "core/common/resources/AppStrings.h"
#include "core/restapi/RestApi.h"
#include "core/utils/Utils.h"

namespace features::claiming::usecases {
using namespace core::usecase;

Result<DownloadBackupResult> DownloadBackupUseCase::execute(const DownloadBackupInput &input) {
    QStringList backup_keys;
    backup_keys.clear();
    if (!input.backupPwd.isEmpty()) {
        backup_keys.push_back(input.backupPwd);
    }
    if (!input.backupPwdTwo.isEmpty()) {
        backup_keys.push_back(input.backupPwdTwo);
    }
    DBG_INFO << "magic:" << input.magic << " backup_keys size:" << backup_keys.size();
    QJsonObject output;
    bool ret = Draco::instance()->inheritanceDownloadBackup(input.magic, backup_keys, output);
    DBG_INFO << "output:" << output;
    if (ret) {
        DownloadBackupResult backupResult;
        QJsonArray keys = output.value("keys").toArray();
        std::vector<nunchuk::SingleSigner> signers;
        QStringList used_key_names;
        used_key_names.clear();
        used_key_names.append(input.keyName);
        used_key_names.append(input.keyNameTwo);
        for (QJsonValue js : keys) {
            QJsonObject key = js.toObject();
            // Try with all backup_keys (Backup Password)
            for (int i = 0; i < backup_keys.size(); i++) {
                auto signer = downloadKey(key, backup_keys[i], used_key_names[i]);
                if (signer.has_value()) {
                    signers.push_back(*signer);
                }
            }
        }
        if (backup_keys.size() != (int)signers.size()) {
            // ERROR: Backup Password is incorrect
            // Remove master_singer
            for (auto s : signers) {
                bridge::nunchukDeleteMasterSigner(QString::fromStdString(s.get_master_fingerprint()));
            }
            return Result<DownloadBackupResult>::failure(Strings.STR_QML_2106(), DRACO_CODE::INHERITANCE_801);
        }
        backupResult.signers = std::move(signers);
        // Populate backupResult with the necessary data from result
        return Result<DownloadBackupResult>::success(backupResult);
    } else {
        int response_code = output.value("code").toInt();
        QString error_msg = output.value("message").toString();
        return Result<DownloadBackupResult>::failure(QString("Error %1: %2").arg(response_code).arg(error_msg), response_code);
    }
}

std::optional<nunchuk::SingleSigner> DownloadBackupUseCase::downloadKey(const QJsonObject &key, const QString &backup_key, const QString &key_name) {
    QWarningMessage msg;
    QString base64 = key.value("key_backup_base64").toString();
    QByteArray ba;
    ba.append(base64);
    QByteArray base64bin = QByteArray::fromBase64(ba);
    std::vector<unsigned char> base64vec(base64bin.begin(), base64bin.end());
    QString derivation_path = key.value("derivation_path").toString();
    auto master_signer_ptr = bridge::ImportBackupKey(base64vec, backup_key, key_name, false, msg);
    std::vector<nunchuk::SingleSigner> single_signers;
    if (msg.isSuccess()) {
        msg.resetWarningMessage();
        auto master_signer = master_signer_ptr->originMasterSigner();
        // update tag
        master_signer.set_tags({nunchuk::SignerTag::INHERITANCE});
        bridge::UpdateMasterSigner(master_signer, msg);
        single_signers.push_back(bridge::GetSignerFromMasterSigner(QString::fromStdString(master_signer.get_id()), derivation_path, msg));
    }
    if (msg.isSuccess()) {
        return single_signers.back();
    } else {
        return std::nullopt;
    }
}

} // namespace features::claiming::usecases
