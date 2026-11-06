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
    Q_PROPERTY(bool planSetupFlow                   READ planSetupFlow                  WRITE setPlanSetupFlow          NOTIFY planSetupFlowChanged)

public:
    Q_INVOKABLE void setPendingInheritanceEdit(int action) { m_pendingInheritanceEdit = action; }
    int takePendingInheritanceEdit() { int ret = m_pendingInheritanceEdit; m_pendingInheritanceEdit = -1; return ret; }
    bool planSetupFlow() const { return m_planSetupFlow; }
    void setPlanSetupFlow(bool value) { if (m_planSetupFlow == value) return; m_planSetupFlow = value; emit planSetupFlowChanged(); }
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
    void CreateAssignAvailableSigners(const QString &wallet_type);

    bool existHardware(const QString& tag);
    void makeHardwareSingerList(const QString& wallet_type, const QString& tag);

    // Mirrors QML's _CLAIM_AN_INHERITANCE (Qml/Screens/OnlineMode/SCR_SERVICE_SETTINGS.qml) - keep in
    // sync. Used in setOptionIndex() to detect entry into the claim feature regardless of which
    // sidebar/CTA variant triggered it (they all funnel through this one setter).
    static constexpr int OPTION_CLAIM_AN_INHERITANCE = 5;

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
    void requestSignerInitialClaimSetup(const QString &tag);
signals:
    void planSetupFlowChanged();
    void verifyPasswordTokenAlert(const QString& errormsg);
    void optionIndexChanged();
    void walletInfoChanged();
private:
    int m_optionIndex = 0;
    int m_pendingInheritanceEdit = -1;
    bool m_planSetupFlow = false;
    QWalletPtr          walletInfo_;
};

#endif // SERVICESETTING_H
