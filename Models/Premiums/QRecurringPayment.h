#ifndef QRECURRINGPAYMENT_H
#define QRECURRINGPAYMENT_H

#include <QJsonObject>
#include <QJsonArray>
#include <QStack>
#include "TypeDefine.h"
#include "Premiums/QBasePremium.h"
#include <nunchuk.h>

class PaymentEnum : public QObject
{
    Q_OBJECT
public:
    Q_ENUMS(Enum_t)
    enum class Enum_t : int {
        EMPTY_STATE,
        ENTER_NAME_PAYMENT,
        ENTER_FIXED_AMOUNT,
        SELECT_DESTINATION_PAYMENT,
        SELECT_IMPORT_FORMAT,
        ENTER_PERCENTAGE,
        SELECT_PERCENTAGE_CALCULATED,
        ENTER_WHITELISTED_ADDRESS,
        IMPORT_FIRST_ADDRESS_OF_WALLET,
        ENTER_FREQUENCY_OF_PAYMENT,
        SELECT_PLATFORM_KEY_CO_SIGN,
        ENTER_TRANSACTION_NOTE,
        RECURRING_PAYMENTS_REVIEW,
        VIEWING_RECURRING_PAYMENTS,
        RECURRING_PAYMENTS_POPULATED_SATE,
        SELECT_FEE_RATE,
        VIEWING_RECURRING_PAYMENTS_ALERT,
    };
};
#define Flow_Payment public
#define Property_Payment public
#define Method_Payment public
class QRecurringPayment : public QBasePremium
{
    Q_OBJECT
    Q_PROPERTY(int flow                 READ flow               NOTIFY flowChanged)
    Q_PROPERTY(QVariant payment         READ paymentCurrent     NOTIFY paymentCurrentChanged)
    Q_PROPERTY(QVariantList paymentList READ paymentList        NOTIFY paymentListChanged)
    Q_PROPERTY(QString sortOption       READ sortOption         NOTIFY sortOptionChanged)
    Q_PROPERTY(int scan_percent         READ scan_percent       NOTIFY scan_percentChanged)
public:
    explicit QRecurringPayment(WalletId wallet_id);
Flow_Payment:
    int flow() const;
    void addFlow(int flow);
    void replaceFlow(int flow);
    void backFlow();
    void clearFlow();

Property_Payment:
    QString sortOption() const;
    QVariantList paymentList() const;
    void setPaymentList(const QJsonArray &list);

    QVariant paymentCurrent() const;
    QJsonObject paymentCurrentJs() const;
    void setPaymentCurrent(const QJsonObject &payment);
    void setPaymentCurrent(const QString &recurring_payment_id);
    void CreatePaymentFromAlert();
    void CancelPaymentFromAlert();
    bool CancelPaymentCancellationPending();
Method_Payment:
    bool HasExistingPayments();
    bool CreateRecurringPayment();
    bool GetOneRecurringPayment(const QString &recurring_payment_id);
    bool CancelRecurringPayment();
    void EditPayment(QJsonObject change);
    QJsonObject DefaultPayment() const;
    bool ImportBSMSWallet(const QString& filepath);
    bool ImportColdcardNFCWallet(const QString& filepath);
    bool ImportAddressesWallet(const QStringList addresses);
    bool ImportWallet(WalletId wallet_id);

    int scan_percent() const;
public slots:
    bool hasServerKey();
    void setScan_percent(int scan_percent);
    bool parseQRSuccess(const QStringList& qrtags);
    bool importQRWallet(const QStringList qrtags);
    bool isValidDate(const QString& start_date, const QString& end_date, bool noEndDate, const QString& frequ);
    bool isValidDate(const QString& date);
    void setSortOption(const QString& option);
    QStringList walletIdList() const;
private:
    void SortStartDateOldestToNewest();
    void SortStartDateNewestToOldest();
    void SortPaymentNameAToZ();
    void SortPaymentNameZToA();
    bool ImportFileWallet(const QString& filepath, bool bsms);
    bool ConvertWalletToBsmsAndAddress(const nunchuk::Wallet& wallet);
    QJsonObject ConvertToDisplayQml(QJsonObject data);
    QJsonObject ConvertToSend(QJsonObject data);
signals:
    void flowChanged();
    void paymentListChanged();
    void paymentCurrentChanged();
    void sortOptionChanged();
    void scan_percentChanged();
private:
    QStack<int> m_flows;
    bool m_has_server_key {false};
    QJsonArray m_payment_list {};
    QJsonObject m_payment {};
    QString m_sortOption {"newest"};
    int     m_scan_percent {0};
};

#endif // QRECURRINGPAYMENT_H
