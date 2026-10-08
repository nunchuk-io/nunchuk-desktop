/**************************************************************************
 * This file is part of the Nunchuk software (https://nunchuk.io/)        *
 * Copyright (C) 2020-2022 Enigmo								          *
 * Copyright (C) 2022 Nunchuk								              *
 *                                                                        *
 * This program is free software; you can redistribute it and/or          *
 * modify it under the terms of the GNU General Public License            *
 * as published by the Free Software Foundation; either version 3         *
 * of the License, or (at your option) any later version.                 *
 *                                                                        *
 * This program is distributed in the hope that it will be useful,        *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 * GNU General Public License for more details.                           *
 *                                                                        *
 * You should have received a copy of the GNU General Public License      *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.  *
 *                                                                        *
 **************************************************************************/

#ifndef QASSISTEDDRAFTWALLETS_H
#define QASSISTEDDRAFTWALLETS_H

#include "QCommonDefines.h"
#include "QSwitchAPI.h"
#include "TypeDefine.h"
#include "bridgeifaces.h"
#include <QJsonArray>
#include <QObject>

using Key = ENUNCHUCK::AddHardware;
struct StructAddHardware {
    QString mTag{};
    QString mType{};
    QString mTextBtn{};
    QString mTitle{};
    int mHeight{};
    QString mGroupId{};
    QString mRequestId{};
    int mKeyIndex{-1};
    QStringList mTags{};
    QJsonObject mConfig{};
};
class QAssistedDraftWallets : public QSwitchAPI {
    Q_OBJECT
    Q_PROPERTY(QVariantList hardwareReq READ hardwareReq WRITE setHardwareReq NOTIFY hardwareReqChanged)
    Q_PROPERTY(DeviceListModel *refreshDeviceList READ refreshDeviceList NOTIFY deviceListChanged)
    Q_PROPERTY(QVariantList signerExistList READ signerExistList NOTIFY signerExistListChanged)
  public:
    QAssistedDraftWallets(int mode);
    virtual ~QAssistedDraftWallets();

    QString typeReq() const;

    // For draft-wallets
    void GetListAllRequestAddKey(const QJsonArray &groups = {});
    void reset();

    DeviceListModel *refreshDeviceList() const;

    QVariantList hardwareReq() const;
    void setHardwareReq(const QVariantList &hardwareReq);

    virtual void updateRequestKey();

    void createAssignAvailableSigners(const QString &wallet_type);
    QWalletServicesTagPtr servicesTagPtr() const;
    void MixMasterSignerAndSingleSigner(const QString &tag);
    void MixMasterSignerAndSingleSignerMiniscript(const QString &tag);

    virtual void finishScanDevices();
    QVariantList signerExistList() const;
    void setSignerExistList(QVariantList signerExistList);

    enum class ImportColdcard_t : int {
        eNone,
        eOK,
        eError_Keep_Screen,
        eError_Back,
    };
    ImportColdcard_t ImportColdcardViaFile(const QString &fileName, int new_index = -1);

    QJsonObject progressAddOrReplacementInfo(nunchuk::SingleSigner hardware);
    bool requestKeyReplacement(nunchuk::SingleSigner hardware);
    StructAddHardware request() const;

    ENUNCHUCK::WalletType walletType() const;

    bool AddOrUpdateToDraftWallet(nunchuk::SingleSigner hardware);
    QMap<Key, StructAddHardware> addRequest(const QJsonArray &requests, const QString &group_id = "");
    void makeListRequests();
    bool checkAndGetSingleSigner(const QString &xfp, int index, nunchuk::SingleSigner &outSigner);
    bool requestAddOrReplacementToServer(nunchuk::SingleSigner signer);
    bool canReplaceKey();
    void resultAddOrUpdateAKeyToDraftWallet(const QString& result, bool resetFlow = false);
  signals:
    void addHardwareAlert();
    void hardwareReqChanged();
    void signerExistListChanged();
    void deviceListChanged();
    void verifySingleSignerResult(int result);
  public slots:
    void cancelRequestKey(const QString &request_id, const QString &group_id = "");
    void addHardwareFromConfig(int hardwareType, const QString &group_id, int key_index = -1, bool is_inheritance = false);
    void addHardwareFromBanner(const QString &request_id);
    int reuseKeyGetCurrentIndex(const QString &xfp);
    bool requestAddOrReplacementWithIndex(const QString &xfp, int index);
    void requestAddOrReplacementWithIndexAsync(const QString &xfp, int index);
    QString bip32path(const QString &xfp, int index);
    QString reuseKeyXfp(const QString &fileName);
    void requestVerifySingleSignerViaConnectDevice(const int index, const QString &verifyType);
    void requestVerifySingleSignerViaQR(const QStringList &qr_data, const QString &verifyType);
    void requestVerifySingleSignerViaFile(const QString &fileName, const QString &verifyType);
    bool requestVerifySingleSigner(const QString &verifyType);
    bool addVerifySingleSigner(const QString &verifyType);
    bool replacementVerifySingleSigner(const QString &verifyType);
    bool requestRemoveSingleSigner(const QString &xfp);
    void newAccountIndexCached(const QString &xfp, int index);
    bool requestQRAddOrReplacementWithIndexAsync(const QStringList &qr_data, int index);
    bool requestImportFileAddOrReplacementWithIndexAsync(const QString &fileName, int index);
    bool requestAddOrReplacementBothIndicesIfPossible(const QString &xfp);
    bool requestAddOrReplacementBothIndicesIfPossibleAsync(const QString &xfp);
  private:
    QMap<Key, StructAddHardware> m_requests{};
    StructAddHardware m_request{};
    QVariantList m_hardwareReq{};
    QDeviceListModelPtr mRefreshDeviceList;
    QVariantList m_signerExistList;
    QString m_selectFingerPrint;
    QSingleSignerListModelPtr m_availableSignerListPtr;
};

#endif // QASSISTEDDRAFTWALLETS_H
