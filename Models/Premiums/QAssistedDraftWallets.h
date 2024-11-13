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

#include <QObject>
#include <QJsonArray>
#include "QCommonDefines.h"
#include "bridgeifaces.h"
#include "QSwitchAPI.h"
#include "TypeDefine.h"

using Key = ENUNCHUCK::AddHardware;
struct StructAddHardware
{
    QString mTag {};
    QString mType {};
    QString mTextBtn {};
    QString mTitle {};
    int mHeight {};
    QString mWalletId {};
    QString mGroupId {};
    QString mRequestId {};
    int     mKeyIndex {-1};
    QStringList mTags {};
};
class QAssistedDraftWallets : public QSwitchAPI
{
    Q_OBJECT
    Q_PROPERTY(QVariantList hardwareReq             READ hardwareReq        WRITE setHardwareReq    NOTIFY hardwareReqChanged)
    Q_PROPERTY(DeviceListModel* refreshDeviceList   READ refreshDeviceList                          NOTIFY deviceListChanged)
    Q_PROPERTY(QVariantList signerExistList         READ signerExistList                            NOTIFY signerExistListChanged)
public:
    QAssistedDraftWallets(int mode);
    virtual ~QAssistedDraftWallets();

    QString typeReq() const;

    // For draft-wallets
    void GetListAllRequestAddKey(const QJsonArray& groups = {});
    virtual bool AddOrUpdateAKeyToDraftWallet();
    virtual bool AddOrUpdateReuseKeyToDraftWallet(nunchuk::SingleSigner keyresued);
    void reset();

    DeviceListModel* refreshDeviceList() const;

    QVariantList hardwareReq() const;
    void setHardwareReq(const QVariantList &hardwareReq);

    virtual void updateRequestKey();

    QWalletServicesTagPtr servicesTagPtr() const;
    void MixMasterSignerAndSingleSigner(const QString &tag);

    virtual void finishScanDevices();
    QVariantList signerExistList() const;
    void setSignerExistList(QJsonArray signerExistList);

    void setSelectFingerPrint(const QString &selectFingerPrint);
    enum class ImportColdcard_t : int {
        eNone,
        eOK,
        eError_Keep_Screen,
        eError_Back,
    };
    ImportColdcard_t ImportColdcardViaFile(const QString &fileName, int new_index = -1);
    virtual bool requestKeyReplacement(QSingleSignerPtr signer);
private:
    bool RequestAddOrUpdateAKeyToDraftWallet(StructAddHardware hardware);
    bool RequestAddOrUpdateReuseKeyToDraftWallet(StructAddHardware hardware, nunchuk::SingleSigner keyresued);
    void addRequest(const QJsonArray& requests, const QString& group_id = "");
    void makeListRequests();
    bool getSignerFromMasterSigner(const QString &xfp, const int index);
    bool getSigner(const QString &xfp, const int index);
    bool requestAddReuseKey(nunchuk::SingleSigner key, int index);
    bool canReplaceKey();
signals:
    void addHardwareAlert();
    void hardwareReqChanged();
    void signerExistListChanged();
    void reuseKeyGetSignerResult(int result);
    void deviceListChanged();

public slots:
    void cancelRequestKey(const QString& request_id, const QString& group_id = "");
    void addHardwareFromConfig(int hardwareType, const QString& group_id, int key_index = -1);
    void addHardwareFromBanner(const QString& request_id);
    int  reuseKeyGetCurrentIndex(const QString &xfp);
    void reuseKeyFromMasterSigner(const QString &xfp, const int index);
    void reuseKeyGetSigner(const QString &xfp, const int index);
    QString bip32path(const QString &xfp, int index);
    QString reuseKeyXfp(const QString &fileName);
private:
    QMap<Key, StructAddHardware> m_requests {};
    StructAddHardware m_request {};
    QVariantList m_hardwareReq {};
    QWalletServicesTagPtr m_servicesTag;
    QDeviceListModelPtr         mRefreshDeviceList;
    QJsonArray m_signerExistList;
    QString m_selectFingerPrint;
};

#endif // QASSISTEDDRAFTWALLETS_H
