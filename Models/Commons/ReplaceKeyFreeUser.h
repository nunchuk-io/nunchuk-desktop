#ifndef REPLACEKEYFREEUSER_H
#define REPLACEKEYFREEUSER_H
#include <QObject>
#include <QJsonArray>
#include <nunchuk.h>
#include "TypeDefine.h"
#include "Models/DeviceModel.h"

class ReplaceKeyFreeUser
{
public:
    void MixMasterSignerAndSingleSignerAll();
    QVariantList signerExistList() const;
    void setSignerExistList(QJsonArray signerExistList);
    QString replaceFlow() const;
    void setReplaceFlow(const QString& replaceFlow);
    void SelectKeyToReplace(const QString& xfp, const int index);
    void CreateANewWallet();
    void walletCreateDone();
    bool replaceFree() const;
    void updateNewKeyReplace(const QString &xfp);
    void refreshScanDevices();

    DeviceListModel *deviceList() const ;
    void setDeviceList(const QDeviceListModelPtr &d);

    QString deviceType() const;
    void setDeviceType(const QString &deviceType);

    QString deviceType(const QString &xfp);
    bool ImportColdcardViaFile(const QString &fileName, int new_index);

    bool tranReplace() const;
    void setTranReplace(bool newTranReplace);

public slots:
    virtual int  reuseKeyGetCurrentIndex(const QString &xfp);
    virtual QString bip32path(const QString &xfp, int index);
    virtual bool updateKeyReplace(const QString &xfp, const int index);
signals:
    virtual void signerExistListChanged() = 0;
    virtual void replaceFlowChanged() = 0;
    virtual void deviceListChanged() = 0;
    virtual void deviceTypeChanged() = 0;
    virtual void tranReplaceChanged() = 0;
private:
    QJsonArray m_signerExistList {};
    QString m_replaceFlow {"replace-key-info"};
    struct Signer_t {
        nunchuk::SingleSigner signer;
        int index;
    };
    Signer_t m_curReplaceKey;
    bool m_replaceFree {false};
    QDeviceListModelPtr deviceList_;
    QString m_deviceType;
    static bool m_tranReplace;
};

#endif // REPLACEKEYFREEUSER_H
