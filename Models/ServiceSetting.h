#ifndef SERVICESETTING_H
#define SERVICESETTING_H

#include <QObject>
#include <QVariant>
#include "QCommonDefines.h"
#include <QJsonObject>
#include "TypeDefine.h"
#include "WalletModel.h"
#include "Commons/QStateFlow.h"

class ServiceSetting : public QStateFlow
{
    Q_OBJECT
    Q_PROPERTY(int optionIndex                      READ optionIndex                    WRITE setOptionIndex            NOTIFY optionIndexChanged)
    Q_PROPERTY(QVariant servicesTag                 READ servicesTag                                                    NOTIFY walletInfoChanged)
    Q_PROPERTY(Wallet* walletInfo                   READ walletInfo                                                     NOTIFY walletInfoChanged)

public:
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

    bool existHardware(const QString& tag);

    int optionIndex() const;
    void setOptionIndex(int index);

    QVariant servicesTag() const;
    QWalletServicesTagPtr servicesTagPtr() const;

    Wallet *walletInfo() const;
    QWalletPtr walletInfoPtr() const;
    void setWalletInfo(const QWalletPtr &d);
public slots:
    void requestStartAddHardwareKey(const QString &tag);
    void clearWalletInfo();
    void handleClaimInheritance(const QVariant &msg);
signals:
    void verifyPasswordTokenAlert(const QString& errormsg);
    void optionIndexChanged();
    void walletInfoChanged();
private:
    int m_optionIndex;
    QWalletPtr          walletInfo_;
};

#endif // SERVICESETTING_H
