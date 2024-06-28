#ifndef SERVICESETTING_H
#define SERVICESETTING_H

#include <QObject>
#include <QVariant>
#include "QCommonDefines.h"
#include <QJsonObject>
#include "TypeDefine.h"
#include "WalletModel.h"

class ServiceSetting : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int optionIndex                      READ optionIndex                    WRITE setOptionIndex            NOTIFY optionIndexChanged)
    Q_PROPERTY(int claimInheritanceFlow             READ claimInheritanceFlow           WRITE setClaimInheritanceFlow   NOTIFY claimInheritanceFlowChanged)
    Q_PROPERTY(QVariant servicesTag                 READ servicesTag                                                    CONSTANT)
    Q_PROPERTY(Wallet* walletInfo                   READ walletInfo                                                     NOTIFY walletInfoChanged)

public:
    Q_ENUMS(CIWithDraw)
    enum class CIWithDraw {
        WITHDRAW_TO_NUNCHUK_WALLET,
        WITHDRAW_TO_ADDRESS
    };
    Q_ENUMS(InheritanceEdit)
    enum class InheritanceEdit {
        IE_NONE,
        IE_ACTIVATION_DATE,
        IE_LEAVE_MESSAGE,
        IE_BUFFER_PERIOD,
        IE_NOTIFICATION
    };
    explicit ServiceSetting(QObject *parent = nullptr);
    static ServiceSetting *instance();
    ServiceSetting(ServiceSetting &other) = delete;
    ServiceSetting(ServiceSetting const &other) = delete;
    void operator=(const ServiceSetting &other) = delete;

    int claimInheritanceFlow() const;
    void setClaimInheritanceFlow(int flow);

    void checkInheritanceDownloadBackup();

    Q_INVOKABLE bool existHardware(const QString& tag);

    int optionIndex() const;
    void setOptionIndex(int index);

    QVariant servicesTag() const;
    QWalletServicesTagPtr servicesTagPtr() const;

    Wallet *walletInfo() const;
    QWalletPtr walletInfoPtr() const;
    void setWalletInfo(const QWalletPtr &d);
public slots:
    void clearWalletInfo();
signals:
    void claimInheritanceFlowChanged();
    void optionIndexChanged();
    void walletInfoChanged();
private:
    int m_claimInheritanceFlow {0};
    int m_optionIndex;
    QWalletPtr          walletInfo_;
};

#endif // SERVICESETTING_H
