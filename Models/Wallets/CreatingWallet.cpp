#include "CreatingWallet.h"
#include "ViewsEnums.h"
#include "bridgeifaces.h"
#include "localization/STR_CPP.h"
#include "QThreadForwarder.h"
#include "Signers/QSignerManagement.h"

int CreatingWallet::m_walletOptionType = 0;
CreatingWallet::CreatingWallet(const nunchuk::Wallet &w) :
    MiniscriptWallet{w},
    m_assignAvailableSigners(QSingleSignerListModelPtr(new SingleSignerListModel()))
{}


int CreatingWallet::walletOptType() const
{
    return m_walletOptionType;
}

void CreatingWallet::setWalletOptType(int optType)
{
    m_walletOptionType = optType;
    emit walletOptTypeChanged();
}

bool CreatingWallet::needBackup() const
{
    if (isReplaced() || isAssistedWallet()) {
        return false; // Replaced or assisted wallets do not require backup
    }
    QWalletCached<QString, QString, QString, QString, bool, bool, bool> data;
    bool ret = AppSetting::instance()->getwalletCached(walletId(), data);
    if(ret){
        return !data.backedup;
    }
    return false;
}

void CreatingWallet::setNeedBackup(const bool data)
{
    if (isReplaced() || isAssistedWallet()) {
        return; // Replaced or assisted wallets do not require backup
    }
    QWalletCached<QString, QString, QString, QString, bool, bool, bool> cache;
    cache.groupId   = groupId();
    cache.slug      = slug();
    cache.myRole    = myRole();
    cache.status    = status();
    cache.backedup  = !data;
    cache.hideFiatCurrency = isByzantineGuardian();
    AppSetting::instance()->setWalletCached(walletId(), cache);
    emit needBackupChanged();
}

bool CreatingWallet::needRegistered() const
{
    if (isReplaced() || isAssistedWallet()) {
        return false; // Replaced or assisted wallets do not require registration
    }
    QWalletCached<QString, QString, QString, QString, bool, bool, bool> data;
    bool ret = AppSetting::instance()->getwalletCached(walletId(), data);
    if(ret){
        return !data.registered;
    }
    return false;
}

void CreatingWallet::setNeedRegistered(const bool data)
{
    if (isReplaced() || isAssistedWallet()) {
        return; // Replaced or assisted wallets do not require registration
    }
    QWalletCached<QString, QString, QString, QString, bool, bool, bool> cache;
    cache.groupId   = groupId();
    cache.slug      = slug();
    cache.myRole    = myRole();
    cache.status    = status();
    cache.registered = !data;
    cache.hideFiatCurrency = isByzantineGuardian();
    AppSetting::instance()->setWalletCached(walletId(), cache);
    emit needRegisteredChanged();
}

void CreatingWallet::CreateSignerListReviewWallet(const std::vector<nunchuk::SingleSigner> &signers, const QList<QString>& needTopUps)
{
    if (auto signersList = singleSignersAssigned()) {
        // Create QMap for finger print and Valuekey
        // -- 8771 - begin
        QMap<QString, bool> existingValuekeyMap;
        for (int var = 0; var < signersList->rowCount(); var++) {
            existingValuekeyMap[signersList->getSingleSignerByIndex(var)->fingerPrint()] = signersList->getSingleSignerByIndex(var)->valueKey();
        }
        // -- 8771 - end
        signersList->cleardata();
        for (size_t i = 0; i < signers.size(); i++) {
            nunchuk::SingleSigner signer = signers.at(i);
            QWarningMessage msg;
            nunchuk::SingleSigner localSigner = bridge::nunchukGetOriginSingleSigner(signer, msg);
            if (localSigner.is_visible() && bridge::nunchukHasSinger(localSigner) && (int)EWARNING::WarningType::NONE_MSG == msg.type()) {
                QSingleSignerPtr ret = QSingleSignerPtr(new QSingleSigner(localSigner));
                if (needTopUps.contains(ret->fingerPrint())) {
                    ret->setNeedTopUpXpub(true);
                }
                ret->setValueKey(existingValuekeyMap.value(ret->fingerPrint(), false));
                signersList->addSingleSigner(ret);
            }
            else {
                QSingleSignerPtr ret = QSingleSignerPtr(new QSingleSigner(signer));
                if (needTopUps.contains(ret->fingerPrint())) {
                    ret->setNeedTopUpXpub(true);
                }
                ret->setName(QString("Key #%1").arg(i + 1));
                ret->setValueKey(existingValuekeyMap.value(ret->fingerPrint(), false));
                signersList->addSingleSigner(ret);
            }
        }
        signersList->requestSort();
        DBG_INFO << "needTopUpXpubs:" << signersList->needTopUpXpubs();
        emit signersList->needTopUpXpubsChanged();
    }
}

void CreatingWallet::CreateAssignAvailableSigners() {
    QPointer<CreatingWallet> safeThis(this);
    bridge::CreateAssignAvailableSigners(
        static_cast<nunchuk::AddressType>(walletAddressType()),
        static_cast<nunchuk::WalletType>(walletType()),
        [safeThis](const QSingleSignerListModelPtr& available_signers) {
            SAFE_QPOINTER_CHECK_RETURN_VOID(ptrLamda, safeThis)
            ptrLamda->m_assignAvailableSigners = available_signers;
            emit ptrLamda->assignAvailableSignersChanged();
        }
    );
}

SingleSignerListModel *CreatingWallet::assignAvailableSigners()
{
    return m_assignAvailableSigners.data();
}

bool CreatingWallet::confirmAndAssignKeysToWallet()
{
    std::vector<nunchuk::SingleSigner> signer_list;
    const auto wallet_n = walletN();
    const auto wallet_type = wallet_n > 1 ? ENUNCHUCK::WalletType::MULTI_SIG : ENUNCHUCK::WalletType::SINGLE_SIG;
    const auto address_type = static_cast<ENUNCHUCK::AddressType>(walletAddressType());
    setWalletType(static_cast<int>(wallet_type));
    const auto available_signers = assignAvailableSigners();
    if (!available_signers) {
        DBG_WARN << "assignAvailableSigners() returned null";
        return false;
    }

    const auto full_list = available_signers->fullList();
    QList<QString> needTopUps;
    for (const QSingleSignerPtr& p : full_list) {
        if (!p || !p->checked()) {
            continue;
        }
        QSingleSignerPtr signer = p;
        QWarningMessage msg;
        QString master_id = p->masterSignerId().isEmpty() ? p->fingerPrint() : p->masterSignerId();
        QString new_path = p->derivationPath();
        // if (bridge::IsSignerUsedInWallets(master_id, p->originSingleSigner())) { // TBD need improve
            auto signer_tmp = bridge::nunchukGetAvailableSignerFromSingleSigner(p, wallet_type, address_type, msg);
            if (msg.type() == static_cast<int>(EWARNING::WarningType::NONE_MSG) && signer_tmp) {
                signer = signer_tmp;
            }
        // }
        if (signer) {
            signer_list.push_back(signer->originSingleSigner());
            if (msg.type() == static_cast<int>(EWARNING::WarningType::NONE_MSG)) continue;
            needTopUps.append(p->fingerPrint());
            const int index = available_signers->getIndexByFingerPrint(p->fingerPrint());
            const QModelIndex model_index = available_signers->index(index, 0);
            available_signers->setData(model_index, QVariant(true), SingleSignerListModel::single_signer_need_Topup_Xpub_Role);
            AppModel::instance()->showToast(msg.code(), msg.what(), static_cast<EWARNING::WarningType>(msg.type()));
        }
    }
    if (wallet_n == static_cast<int>(signer_list.size())) {
        CreateSignerListReviewWallet(signer_list, needTopUps);
        return true;
    } else {
        DBG_INFO << "Signer list size mismatch: expected" << wallet_n << "but got" << signer_list.size();
        return false;
    }
}

void CreatingWallet::clearPassphrase(const QString &masterFingerPrint)
{
    const auto available_signers = assignAvailableSigners();
    if (!available_signers) {
        DBG_WARN << "assignAvailableSigners() returned null";
        return;
    }
    auto signer = available_signers->getSingleSignerByFingerPrint(masterFingerPrint);
    if(signer->signerType() == (int)ENUNCHUCK::SignerType::SOFTWARE){
        QWarningMessage warningmsg;
        bridge::nunchukClearSignerPassphrase(signer->masterSignerId(), warningmsg);
    }
}
static int m_indexXpub = -1;
void CreatingWallet::startGetXpubs(int indexXpub)
{
    DBG_INFO << "indexXpub: " << m_indexXpub;
    m_indexXpub = indexXpub;
    auto topUpXpubFunc = []() -> bool {
        QWarningMessage msg;
        if (auto w = AppModel::instance()->newWalletInfoPtr()) {
            auto signersList = w->singleSignersAssigned();
            if (signersList) {
                const auto address_type = static_cast<ENUNCHUCK::AddressType>(w->walletAddressType());
                const auto wallet_type = static_cast<ENUNCHUCK::WalletType>(w->walletType());
                QSingleSignerPtr p = signersList->getSingleSignerByIndex(m_indexXpub);
                if(p){
                    QSingleSignerPtr signer = bridge::nunchukGetAvailableSignerFromSingleSigner(p, wallet_type, address_type, msg);
                    
                    if(signer && msg.type() == (int)EWARNING::WarningType::NONE_MSG){
                        signersList->replaceSingleSigner(m_indexXpub, signer);
                        AppModel::instance()->showToast(0, STR_CPP_097, EWARNING::WarningType::SUCCESS_MSG);
                        emit signersList->needTopUpXpubsChanged();
                        DBG_INFO << "Get xpub success: " << signer->xpub();
                        return true;
                    }
                }
            }
        }
        return false;
    };

    QSignerManagement::instance()->registerCreateTopUpXpub(topUpXpubFunc);
    if(!topUpXpubFunc()) {
        QMap<QString,QVariant> data = singleSignersAssigned()->get(m_indexXpub).toMap();
        data["isTopUpXpub"] = true;
        DBG_INFO << "startScanDevices for TopUpXpub: " << data;
        AppModel::instance()->startScanDevices(QVariant::fromValue(data));
    }   
}

bool CreatingWallet::editBIP32Path(const QVariant &singleData, const QVariant &customData, const QString &path) {
    if (walletType() == (int)nunchuk::WalletType::MINISCRIPT) {
        return MiniscriptWallet::editBIP32Path(singleData, customData, path);
    } else {
        auto maps = singleData.toMap();
        auto customMaps = customData.toMap();
        QString master_id = maps.value("singleSigner_masterSignerId").toString();
        bool isChecked = maps.value("single_signer_checked").toBool();
        DBG_INFO << "master_id: " << master_id << "path: " << path;
        if (!qUtils::QIsValidDerivationPath(path)) {
            emit editBIP32PathSuccess(-1); // Invalid format
            return false;
        }
        QPointer<CreatingWallet> safeThis(this);
        struct DataStruct {
            nunchuk::SingleSigner signer{};
            int errorType{0};
        };
        runInConcurrent(
            [safeThis, master_id, path]() -> DataStruct {
                SAFE_QPOINTER_CHECK(ptrLamda, safeThis)
                QWarningMessage msg;
                nunchuk::SingleSigner signer = bridge::GetSignerFromMasterSigner(master_id, path, msg);
                DataStruct data;
                if ((int)EWARNING::WarningType::NONE_MSG == msg.type()) {
                    msg.resetWarningMessage();
                    data.signer = signer;
                    data.errorType = 1; // Success
                } else {
                    data.errorType = -2;
                }
                return data;
            },
            [safeThis, isChecked](DataStruct data) {
                SAFE_QPOINTER_CHECK_RETURN_VOID(ptrLamda, safeThis)
                if (data.errorType == 1) {
                    auto s = QSingleSignerPtr(new QSingleSigner(data.signer));
                    const auto available_signers = safeThis->assignAvailableSigners();
                    if (available_signers) {
                        auto index = available_signers->getIndexByFingerPrint(s->fingerPrint());
                        s->setChecked(isChecked);
                        available_signers->replaceSingleSigner(index, s);
                    }
                }
                emit ptrLamda->editBIP32PathSuccess(data.errorType); // -2 Not Found Key
            });
    }
    return true;
}
