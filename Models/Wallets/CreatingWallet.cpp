#include "CreatingWallet.h"
#include "ViewsEnums.h"
#include "bridgeifaces.h"
#include "localization/STR_CPP.h"
#include "QThreadForwarder.h"

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
                signersList->addSingleSigner(ret);
            } else {
                QSingleSignerPtr ret = QSingleSignerPtr(new QSingleSigner(signer));
                if (needTopUps.contains(ret->fingerPrint())) {
                    ret->setNeedTopUpXpub(true);
                }
                ret->setName(QString("Key #%1").arg(i + 1));
                signersList->addSingleSigner(ret);
            }
        }
        signersList->requestSort();
        DBG_INFO << "needTopUpXpubs:" << signersList->needTopUpXpubs();
        emit signersList->needTopUpXpubsChanged();
    }
}

void CreatingWallet::CreateAssignAvailableSigners(const std::set<nunchuk::SignerType>& types, const std::set<nunchuk::SignerTag>& tags)
{
    QSingleSignerListModelPtr available_signers = CreateSupportedSigners(types, tags);
    if (!available_signers) {
        DBG_WARN << "CreateSupportedSigners returned null";
        return;
    }
    m_assignAvailableSigners = available_signers;
    emit assignAvailableSignersChanged();
}

QSingleSignerListModelPtr CreatingWallet::CreateSupportedSigners(const std::set<nunchuk::SignerType>& types, const std::set<nunchuk::SignerTag>& tags) {
    QSingleSignerListModelPtr supportedSigners(new SingleSignerListModel());
    std::set<nunchuk::SignerType> signer_types = types;
    if (types.empty()) {
        signer_types.insert(nunchuk::SignerType::HARDWARE);
        signer_types.insert(nunchuk::SignerType::AIRGAP);
        signer_types.insert(nunchuk::SignerType::SOFTWARE);
        signer_types.insert(nunchuk::SignerType::FOREIGN_SOFTWARE);
        signer_types.insert(nunchuk::SignerType::NFC);
        signer_types.insert(nunchuk::SignerType::COLDCARD_NFC);
        signer_types.insert(nunchuk::SignerType::SERVER);
        signer_types.insert(nunchuk::SignerType::PORTAL_NFC);
    }

    auto masterList = AppModel::instance()->masterSignerList();
    auto singleList = AppModel::instance()->remoteSignerList();
    if (masterList && singleList) {
        for (auto master : masterList->fullList()) {
            auto type = master->originMasterSigner().get_type();
            auto tag = master->signerTag();
            if (signer_types.contains(type) && (tags.empty() || tags.contains(tag))) {
                supportedSigners->addSingleSigner(master->cloneSingleSigner());
            }
        }
        for (auto single : singleList->fullList()) {
            auto type = single->originSingleSigner().get_type();
            auto tag = single->signerTag();
            if (signer_types.contains(type) && (tags.empty() || tags.contains(tag))) {
                supportedSigners->addSingleSigner(single);
            }
        }
        supportedSigners->signers();
    }
    DBG_INFO << "supportedSigners size:" << supportedSigners->signerCount();
    return supportedSigners;
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

        QSingleSignerPtr signer = nullptr;
        QWarningMessage warning_msg;
        const auto signer_type = static_cast<nunchuk::SignerType>(p->signerType());
        if (signer_type == nunchuk::SignerType::NFC) {
            signer = bridge::nunchukGetDefaultSignerFromMasterSigner(p->masterSignerId(), wallet_type, address_type, warning_msg);
        } else if (signer_type == nunchuk::SignerType::SOFTWARE || signer_type == nunchuk::SignerType::HARDWARE) {
            signer = bridge::nunchukGetUnusedSignerFromMasterSigner(p->masterSignerId(), wallet_type, address_type, warning_msg);
        }
        if (!signer) {
            signer = p;
        }
        if (signer) {
            signer_list.push_back(signer->originSingleSigner());
            if (warning_msg.type() == static_cast<int>(EWARNING::WarningType::NONE_MSG)) continue;
            needTopUps.append(p->fingerPrint());
            const int index = available_signers->getIndexByFingerPrint(p->fingerPrint());
            const QModelIndex model_index = available_signers->index(index, 0);
            available_signers->setData(model_index, QVariant(true), SingleSignerListModel::single_signer_need_Topup_Xpub_Role);
            AppModel::instance()->showToast(warning_msg.code(), warning_msg.what(), static_cast<EWARNING::WarningType>(warning_msg.type()));
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

void CreatingWallet::startGetXpubs(int indexSinger)
{
    DBG_INFO << "indexSinger: " << indexSinger;
    if (isBusy()) return;
    qApp->setOverrideCursor(QCursor(Qt::WaitCursor));
    QPointer<CreatingWallet> safeThis(this);
    runInThread([safeThis, indexSinger]() ->bool{
        SAFE_QPOINTER_CHECK(ptrLamda, safeThis)
        QWarningMessage warningmsg;
        if (auto signersList = ptrLamda->singleSignersAssigned()) {
            QSingleSignerPtr p = signersList->getSingleSignerByIndex(indexSinger);
            if(p){
                if (!p->originSingleSigner().has_master_signer()) {
                    int deviceIndex = AppModel::instance()->deviceList()->getDeviceIndexByXfp(p->fingerPrint());
                    if (deviceIndex < 0) {
                        if(AppModel::instance()->scanDevicesSync()) {
                            deviceIndex = AppModel::instance()->deviceList()->getDeviceIndexByXfp(p->fingerPrint());
                        }
                    }
                    QWarningMessage msg;
                    QMasterSignerPtr ret = bridge::nunchukCreateMasterSigner(p->name(),
                                                             deviceIndex,
                                                             msg);
                }
                auto masterSignerId = p->masterSignerId().isEmpty() ? p->fingerPrint() : p->masterSignerId();
                bridge::nunchukCacheMasterSignerXPub(masterSignerId, warningmsg);
                if ((int)EWARNING::WarningType::NONE_MSG != warningmsg.type()) {
                    AppModel::instance()->showToast(warningmsg.code(), warningmsg.what(), (EWARNING::WarningType)warningmsg.type());
                }
            }
        }
        return (int)EWARNING::WarningType::NONE_MSG == warningmsg.type();
    },[safeThis, indexSinger](bool ret) {
        qApp->restoreOverrideCursor();
        SAFE_QPOINTER_CHECK_RETURN_VOID(ptrLamda, safeThis)
        if (ret) {
            if (auto signersList = ptrLamda->singleSignersAssigned()) {
                QSingleSignerPtr p = signersList->getSingleSignerByIndex(indexSinger);
                if(p){
                    auto signer_type = static_cast<nunchuk::SignerType>(p->signerType());
                    if(nunchuk::SignerType::SOFTWARE == signer_type || nunchuk::SignerType::HARDWARE == signer_type || nunchuk::SignerType::NFC == signer_type)
                    {
                        const auto  wallet_type =  static_cast<ENUNCHUCK::WalletType>(ptrLamda->walletType());
                        const auto  address_type = static_cast<ENUNCHUCK::AddressType>(ptrLamda->walletAddressType());
                        QWarningMessage warningmsg;
                        QSingleSignerPtr signer{nullptr};
                        auto masterSignerId = p->masterSignerId().isEmpty() ? p->fingerPrint() : p->masterSignerId();
                        if (signer_type == nunchuk::SignerType::NFC) {
                            signer = bridge::nunchukGetDefaultSignerFromMasterSigner(masterSignerId, wallet_type, address_type, warningmsg);
                        } else if (signer_type == nunchuk::SignerType::SOFTWARE || signer_type == nunchuk::SignerType::HARDWARE) {
                            signer = bridge::nunchukGetUnusedSignerFromMasterSigner (masterSignerId, wallet_type, address_type,  warningmsg);
                        }
                        
                        if(signer && warningmsg.type() == (int)EWARNING::WarningType::NONE_MSG){
                            signersList->replaceSingleSigner(indexSinger, signer);
                            AppModel::instance()->showToast(0, STR_CPP_097, EWARNING::WarningType::SUCCESS_MSG);
                        }
                        else{
                            p->setNeedTopUpXpub(true);
                            AppModel::instance()->showToast(warningmsg.code(), warningmsg.what(), (EWARNING::WarningType)warningmsg.type());
                            AppModel::instance()->startScanDevices(E::STATE_ID_SCR_ADD_HARDWARE_SIGNER);
                        }
                    }
                }
                signersList->requestSort();
                bool needTopUp = signersList->needTopUpXpubs();
                ptrLamda->setCapableCreate(!needTopUp);
                DBG_INFO << "needTopUpXpubs:" << signersList->needTopUpXpubs();
                emit signersList->needTopUpXpubsChanged();
            }
        }
        emit AppModel::instance()->finishGenerateSigner();
    });
}

// void CreatingWallet::editBIP32Path(const QString &master_id, const QString &path) {
//     DBG_INFO << "master_id: " << master_id << "path: " << path;
//     if (!qUtils::QIsValidDerivationPath(path)) {
//         emit editBIP32PathSuccess(-1); // Invalid format
//         return;
//     }
//     QPointer<CreatingWallet> safeThis(this);
//     struct DataStruct {
//         nunchuk::SingleSigner signer{};
//         int errorType{0};
//     };
//     runInConcurrent(
//         [safeThis, master_id, path]() -> DataStruct {
//             SAFE_QPOINTER_CHECK(ptrLamda, safeThis)
//             QWarningMessage msg;
//             nunchuk::SingleSigner signer = bridge::GetSignerFromMasterSigner(master_id, path, msg);
//             DataStruct data;
//             if ((int)EWARNING::WarningType::NONE_MSG == msg.type()) {
//                 msg.resetWarningMessage();
//                 data.signer = signer;
//                 data.errorType = 1; // Success
//             } else {
//                 data.errorType = -2;
//             }
//             return data;
//         },
//         [safeThis](DataStruct data) {
//             SAFE_QPOINTER_CHECK_RETURN_VOID(ptrLamda, safeThis)
//             if (data.errorType == 1) {
//                 auto s = QSingleSignerPtr(new QSingleSigner(data.signer));
//                 const auto available_signers = safeThis->assignAvailableSigners();
//                 if (available_signers) {
//                     auto index = available_signers->getIndexByFingerPrint(s->fingerPrint());
//                     available_signers->replaceSingleSigner(index, s);
//                 }   
//             }
//             emit ptrLamda->editBIP32PathSuccess(data.errorType); // -2 Not Found Key
//         });
//     return;
// }