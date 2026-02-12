#include "SyncWalletFromRemoteUseCase.h"
#include "core/bridge/ExternalBridges.h"
#include "core/restapi/RestApi.h"
#include "core/utils/Utils.h"
#include "core/ui/UiServices.inc"
#include "features/wallets/usecases/FetchWalletListUseCase.h"

namespace features::wallets::usecases {
using namespace core::usecase;

Result<SyncWalletFromRemoteResult> SyncWalletFromRemoteUseCase::execute(const SyncWalletFromRemoteInput &_input) {
    FetchWalletListUseCase fetchWalletListUC;
    FetchWalletListInput input;
    auto result = fetchWalletListUC.execute(input);
    if (result.isSuccess()) {
        QJsonArray walletList = result.value().walletList;
        DBG_INFO << walletList.size();
        WalletsMng->StoreWalletsInfo(walletList);
        for (const QJsonValue &walletValue : walletList) {
            QJsonObject walletData = walletValue.toObject();
            QString status = walletData.value("status").toString();
            if (status == "ACTIVE" || status == "LOCKED" || status == "REPLACED") {
                syncWalletFromRemote(walletData);
            } else if(status == "DELETED") {   
                QString wallet_id = walletData.value("local_id").toString();             
                if (bridge::nunchukHasWallet(wallet_id)) {
                    QWarningMessage msg;
                    bridge::nunchukDeleteWallet(wallet_id, msg);
                }
            }
        }
        
        WalletsMng->SyncDeleteWallets(0);
        WalletsMng->UpdateSyncWalletFlows();
    }
    SyncWalletFromRemoteResult ret;
    return Result<SyncWalletFromRemoteResult>::success(ret);
}

void SyncWalletFromRemoteUseCase::syncWalletFromRemote(const QJsonObject &walletData) {
    walletCached(walletData);
    DBG_INFO << "walletData";
    QString wallet_id = walletData.value("local_id").toString();
    if (!bridge::nunchukHasWallet(wallet_id)) {
        QWarningMessage msg;
        QString bsms = walletData.value("bsms").toString();
        nunchuk::Wallet wallet_result = qUtils::ParseWalletDescriptor(bsms, msg);
        if ((int)EWARNING::WarningType::NONE_MSG == msg.type()) {
            QString wallet_name = walletData.value("name").toString();
            QString wallet_description = walletData.value("description").toString();
            wallet_result.set_name(wallet_name.toStdString());
            wallet_result.set_description(wallet_description.toStdString());
            auto newWalet = bridge::wallet::CreateWallet(wallet_result, true, "");
            syncSignerFromRemote(walletData.value("signers").toArray(), newWalet);
        }
    } else {
        QWarningMessage msg;
        auto getWallet = bridge::nunchukGetOriginWallet(wallet_id, msg);
        getWallet.set_name(walletData.value("name").toString().toStdString());
        getWallet.set_description(walletData.value("description").toString().toStdString());
        bridge::UpdateWallet(getWallet, msg);
        syncSignerFromRemote(walletData.value("signers").toArray(), getWallet);
    }    
}

void SyncWalletFromRemoteUseCase::syncSignerFromRemote(const QJsonArray &signers, std::optional<nunchuk::Wallet> local_wallet) {
    for (QJsonValue signer_json : signers) {
        QJsonObject signer_obj = signer_json.toObject();
        syncSignerFromRemote(signer_obj, local_wallet);
    }
}


void SyncWalletFromRemoteUseCase::syncSignerFromRemote(const QJsonObject &signerData, std::optional<nunchuk::Wallet> local_wallet) {
    DBG_INFO << "signerData" << local_wallet.has_value();
    if (!local_wallet.has_value()) {
        return;
    }
    std::vector<nunchuk::SingleSigner> local_signers = local_wallet->get_signers();
    
    QJsonObject tapsigner = signerData.value("tapsigner").toObject();
    QString signer_name = signerData.value("name").toString();
    QString signer_xfp = signerData.value("xfp").toString();
    bool is_visible = signerData.value("is_visible").toBool();
    if (!tapsigner.isEmpty()) {
        QString card_id = tapsigner.value("card_id").toString();
        QString version = tapsigner.value("version").toString();
        int birth_height = tapsigner.value("birth_height").toInt();
        bool is_testnet = tapsigner.value("is_testnet").toBool();
        bridge::AddTapsigner(card_id, signer_xfp, signer_name, version, birth_height, is_testnet);
    } else {
        QString signer_xpub = signerData.value("xpub").toString();
        QString signer_pubkey = signerData.value("pubkey").toString();
        QString signer_derivation_path = signerData.value("derivation_path").toString();
        QString signer_type = signerData.value("type").toString();
        QJsonArray wtags = signerData.value("tags").toArray();
        std::vector<nunchuk::SignerTag> tags; // get tags from api signer.tags
        for (QJsonValue tag : wtags) {
            QString js_tag = tag.toString();
            tags.push_back(SignerTagFromStr(js_tag.toStdString()));
        }
        nunchuk::SingleSigner signer(signer_name.toStdString(), signer_xpub.toStdString(), signer_pubkey.toStdString(), signer_derivation_path.toStdString(),
                                     {}, signer_xfp.toStdString(), std::time(0));
        bool has_signer = bridge::nunchukHasSinger(signer);
        if (!has_signer) {
            nunchuk::SignerType type = nunchuk::SignerType::AIRGAP;
            try {
                type = SignerTypeFromStr(signer_type.toStdString());
            }
            catch (const nunchuk::BaseException &ex) {
                DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
            }
            catch (std::exception &e) {
                DBG_INFO << "THROW EXCEPTION " << e.what();
            }
            QWarningMessage msg;
            DBG_INFO << "Creating signer locally: " << signer_xfp;
            bridge::nunchukCreateOriginSigner(signer_name, signer_xpub, signer_pubkey, signer_derivation_path, signer_xfp, type, tags, false, msg);
        } else {
            DBG_INFO << "Signer already exists locally: " << signer_xfp;
            std::vector<nunchuk::SingleSigner>::iterator local_signer =
                std::find_if(local_signers.begin(), local_signers.end(),
                             [signer_xfp](const nunchuk::SingleSigner &local) { return local.get_master_fingerprint() == signer_xfp.toStdString(); });
            if (local_signer != local_signers.end()) {
                if (local_signer->has_master_signer()) {
                    QWarningMessage msg;
                    nunchuk::MasterSigner master_signer = bridge::nunchukGetOriginMasterSigner(signer_xfp, msg);
                    master_signer.set_name(signer_name.toStdString());
                    master_signer.set_tags(tags);

                    master_signer.set_visible(master_signer.is_visible() || is_visible);
                    msg.resetWarningMessage();
                    bridge::UpdateMasterSigner(master_signer, msg);
                } else {
                    DBG_INFO << "Local signer missing master signer, updating tags: " << signer_xfp;
                    nunchuk::SingleSigner remote_signer = *local_signer;
                    remote_signer.set_name(signer_name.toStdString());
                    remote_signer.set_tags(tags);
                    remote_signer.set_visible(remote_signer.is_visible() || is_visible);
                    QWarningMessage msg;
                    bridge::UpdateRemoteSigner(remote_signer, msg);
                }
            }
        }
    }
}

void SyncWalletFromRemoteUseCase::walletCached(const QJsonObject &walletData) {
    QString wallet_id = walletData.value("local_id").toString();
    QWalletCached cachedData;
    cachedData.groupId = walletData.value("group_id").toString();
    cachedData.slug = walletData.value("slug").toString();
    cachedData.myRole = walletData.value("role").toString();
    cachedData.status = walletData.value("status").toString();
    cachedData.backedup = true;
    cachedData.hideFiatCurrency = walletData.value("hide_fiat_currency").toBool();
    cachedData.registered = true;
    cachedData.isClaimed = walletData.value("is_claimed").toBool();
    AppSetting::instance()->setWalletCached(wallet_id, cachedData);
}

}