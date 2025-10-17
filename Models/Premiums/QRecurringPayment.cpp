#include "QRecurringPayment.h"
#include "ViewsEnums.h"
#include "Servers/Byzantine.h"
#include "bridgeifaces.h"
#include "qUtils.h"
#include "WalletModel.h"
#include "Premiums/QGroupDashboard.h"
#include "Premiums/QGroupWalletDummyTx.h"

QRecurringPayment::QRecurringPayment(WalletId wallet_id)
    : QBasePremium(wallet_id)
{
    m_payment = DefaultPayment();
    qmlRegisterType<PaymentEnum>("NUNCHUCKTYPE", 1, 0, "PaymentType");
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

int QRecurringPayment::flow() const
{
    if (m_flows.isEmpty()) return 0;
    return m_flows.top();
}

void QRecurringPayment::addFlow(int flow)
{
    m_flows.push(flow);
    emit flowChanged();
}

void QRecurringPayment::replaceFlow(int flow)
{
    if (m_flows.isEmpty())
    {
        addFlow(flow);
    }
    else {
        m_flows.pop();
        addFlow(flow);
    }
}

void QRecurringPayment::backFlow()
{
    if (!m_flows.isEmpty()) {
        m_flows.pop();
        emit flowChanged();
    }
}

void QRecurringPayment::clearFlow()
{
    m_flows.clear();
    emit flowChanged();
}

QString QRecurringPayment::sortOption() const
{
    return m_sortOption;
}

QVariantList QRecurringPayment::paymentList() const
{
    return m_payment_list.toVariantList();
}

void QRecurringPayment::setPaymentList(const QJsonArray &list)
{
    if (m_payment_list == list)
        return;

    m_payment_list = list;
    emit paymentListChanged();
}

QVariant QRecurringPayment::paymentCurrent() const
{
    return QVariant::fromValue(m_payment);
}

QJsonObject QRecurringPayment::paymentCurrentJs() const
{
    return m_payment;
}

void QRecurringPayment::setPaymentCurrent(const QJsonObject &payment)
{
    if (m_payment == payment)
        return;

    m_payment = payment;
    emit paymentCurrentChanged();
}

void QRecurringPayment::setPaymentCurrent(const QString &recurring_payment_id)
{
    for (auto v : m_payment_list) {
        QJsonObject payment = v.toObject();
        if(qUtils::strCompare(payment["id"].toString(), recurring_payment_id)){
            setPaymentCurrent(payment);
            return;
        }
    }
}

void QRecurringPayment::CreatePaymentFromAlert()
{
    if (auto dummytx = groupDummyTxPtr()) {
        QJsonObject paymentJs = dummytx->tx()["payload"].toObject()["data"].toObject();
        setPaymentCurrent(ConvertToDisplayQml(paymentJs));
    }
}

void QRecurringPayment::CancelPaymentFromAlert()
{
    if (auto dummytx = groupDummyTxPtr()) {
        QString recurring_payment_id = dummytx->tx()["payload"].toObject()["recurring_payment_id"].toString();
        GetOneRecurringPayment(recurring_payment_id);
    }
}

bool QRecurringPayment::CancelPaymentCancellationPending()
{
    if (groupDummyTxPtr().isNull()) return false;
    return groupDummyTxPtr()->CancelDummyTransaction();
}

bool QRecurringPayment::HasExistingPayments()
{
    if (auto dash = dashBoardPtr()) {
        QJsonObject output;
        QString errormsg;
        bool ret = Byzantine::instance()->GetListRecurringPayment(dash->groupId(), dash->wallet_id(), output, errormsg);
        if(ret){
            DBG_INFO << output;
            QJsonArray payments = output["recurring_payments"].toArray();
            int payment_count = payments.count();
            if(payment_count > 0){
                QJsonArray list;
                for (auto js : payments) {
                    list.append(ConvertToDisplayQml(js.toObject()));
                }
                setPaymentList(list);
                setSortOption("newest");
                return true;
            }
        }
        return false;
    }
    return false;
}

bool QRecurringPayment::CreateRecurringPayment()
{
    if (auto dash = dashBoardPtr()) {
        QJsonObject output;
        QString errormsg;
        QJsonObject request_body;
        request_body["nonce"] = Byzantine::instance()->nonce();
        request_body["body"] = ConvertToSend(m_payment);
        bool ret = Byzantine::instance()->CreateRecurringPayment(dash->groupId(), dash->wallet_id(), {}, request_body, true, output, errormsg);
        if(ret){
            QJsonObject payment = output["recurring_payment"].toObject();
            setPaymentCurrent(ConvertToDisplayQml(payment));
            QJsonObject dummy_transaction = output["dummy_transaction"].toObject();
            QString dummy_transaction_id = dummy_transaction["id"].toString();
            dummy_transaction["dummy_transaction_id"] = dummy_transaction_id;
            QJsonObject alert;
            alert["payload"] = dummy_transaction;
            dash->setAlertId(alert);
        }
        return ret;
    }
    return false;
}

bool QRecurringPayment::GetOneRecurringPayment(const QString &recurring_payment_id)
{
    if (auto dash = dashBoardPtr()) {
        QJsonObject output;
        QString errormsg;
        bool ret = Byzantine::instance()->GetOneRecurringPayment(dash->groupId(), dash->wallet_id(), recurring_payment_id, output, errormsg);
        if(ret){
            QJsonObject payment = output["recurring_payment"].toObject();
            setPaymentCurrent(ConvertToDisplayQml(payment));
        }
        return ret;
    }
    return false;
}

bool QRecurringPayment::CancelRecurringPayment()
{
    if (auto dash = dashBoardPtr()) {
        QString recurring_payment_id = m_payment["id"].toString();
        QJsonObject output;
        QString errormsg;
        QJsonObject request_body;
        request_body["nonce"] = Byzantine::instance()->nonce();
        request_body["body"] = ConvertToSend(m_payment);
        bool ret = Byzantine::instance()->CancelRecurringPayment(dash->groupId(), dash->wallet_id(), recurring_payment_id, {}, request_body, true, output, errormsg);
        if(ret){
            DBG_INFO << output;
            QJsonObject dummy_transaction = output["dummy_transaction"].toObject();
            QString dummy_transaction_id = dummy_transaction["id"].toString();
            dummy_transaction["dummy_transaction_id"] = dummy_transaction_id;
            QJsonObject alert;
            alert["payload"] = dummy_transaction;
            dash->setAlertId(alert);
        }
        return ret;
    }
    return false;
}

void QRecurringPayment::EditPayment(QJsonObject change)
{
#define CHANGE(KEY) \
    if (change.contains(KEY)) { \
        payment[KEY] = change[KEY]; \
    }

    QJsonObject payment = m_payment;
    CHANGE("name")
    CHANGE("payment_type")
    CHANGE("payment_payload")
    CHANGE("destination_type")
    CHANGE("destination_payload")
    CHANGE("frequency")
    CHANGE("start_date_millis")
    CHANGE("end_date_millis")
    CHANGE("allow_cosigning")
    CHANGE("transaction_note")
    CHANGE("created_time_millis")
    CHANGE("start_date")
    CHANGE("end_date")
    CHANGE("fee_rate")

    DBG_INFO << payment;
    setPaymentCurrent(payment);
}

QJsonObject QRecurringPayment::DefaultPayment() const
{
    QJsonObject json;
    json["name"] = "";
    json["payment_type"] = "FIXED_AMOUNT";
    QJsonObject payment_payload;
    payment_payload["currency"] = "USD";
    payment_payload["value"] = "";
    payment_payload["valuePercent"] = 0;
    payment_payload["calculation_method"] = "";
    json["payment_payload"] = payment_payload;
    json["destination_type"] = "DESTINATION_WALLET";
    QJsonObject destination_payload;
    destination_payload["bsms"] = "";
    destination_payload["first_address"] = "";
    json["destination_payload"] = destination_payload;
    json["frequency"] = "DAILY";
    json["start_date_millis"] = 0;
    json["end_date_millis"] = 0;
    json["allow_cosigning"] = false;
    json["transaction_note"] = "";
    json["created_time_millis"] = 0;
    return json;
}

bool QRecurringPayment::importQRWallet(const QStringList qrtags)
{
    QStringList in = qrtags;
    in.removeDuplicates();
    if(in.isEmpty()){
        return false;
    }
    QWarningMessage importmsg;
    nunchuk::Wallet wallet = qUtils::ParseKeystoneWallet((nunchuk::Chain)AppSetting::instance()->primaryServer(), in, importmsg);
    if (importmsg.type() == (int)EWARNING::WarningType::NONE_MSG) {
        importmsg.resetWarningMessage();
        std::string walletid = "";
        try {
            walletid = wallet.get_id();
        }
        catch (const nunchuk::BaseException &ex) {
            DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        }
        catch (std::exception &e) {
            DBG_INFO << "THROW EXCEPTION" << e.what();
        }
        if (qUtils::strCompare(QString::fromStdString(walletid), wallet_id())) {
            QString message = QString("Destination cannot be the same wallet.");
            AppModel::instance()->showToast(0, message, EWARNING::WarningType::ERROR_MSG);
            return false;
        }
        return ConvertWalletToBsmsAndAddress(wallet);
    } else {
        QString message = QString("Invalid QR");
        AppModel::instance()->showToast(0, message, EWARNING::WarningType::ERROR_MSG);
    }
    return false;
}

bool QRecurringPayment::isValidDate(const QString& start_date, const QString& end_date, bool noEndDate, const QString& frequ)
{
    QDateTime cur_date = QDateTime(QDate::currentDate());
    long int currentDate = cur_date.toTime_t();
    long int start = qUtils::GetTimeSecond(start_date);
    long int end = qUtils::GetTimeSecond(end_date);
    bool ret {false};
    QString message {};
    if (noEndDate) {
        message = QString("Start date must be equal to or later than the current date.");
        ret = start >= currentDate;
    }
    else {
        message = QString("The end date should be after the start date.");
        ret = (end > start) and (start >= currentDate);
    }
    if (!ret) {
        AppModel::instance()->showToast(0, message, EWARNING::WarningType::ERROR_MSG);
    }
    return ret;
}

bool QRecurringPayment::isValidDate(const QString &date)
{
    long int start = qUtils::GetTimeSecond(date);
    auto _date = QDateTime::fromTime_t(start);
    auto day = _date.date().day();
    bool waring = day == 29 || day == 30 || day == 31;
    return waring;
}

void QRecurringPayment::setSortOption(const QString& option)
{
    if (m_sortOption == option)
        return;

    m_sortOption = option;
    emit sortOptionChanged();
    if (option == "oldest") {
        SortStartDateNewestToOldest();
    } else if (option == "newest") {
        SortStartDateOldestToNewest();
    } else if (option == "A-Z") {
        SortPaymentNameAToZ();
    } else if (option == "Z-A") {
        SortPaymentNameZToA();
    }
}

QStringList QRecurringPayment::walletIdList() const
{
    QStringList list;
    for (auto w : AppModel::instance()->walletListPtr()->fullList()) {
        if (w) {
            list.append(w->walletId());
        }
    }
    return list;
}

void QRecurringPayment::SortStartDateOldestToNewest()
{
    QVariantList payments = m_payment_list.toVariantList();
    qSort(payments.begin(), payments.end(), [](const QVariant& a, const QVariant& b)->bool{
        long int a_start_date_millis = static_cast<long int>(a.toMap()["start_date_millis"].toDouble()/1000);
        long int b_start_date_millis = static_cast<long int>(b.toMap()["start_date_millis"].toDouble()/1000);
        return a_start_date_millis < b_start_date_millis;
    });
    setPaymentList(QJsonArray::fromVariantList(payments));
}

void QRecurringPayment::SortStartDateNewestToOldest()
{
    QVariantList payments = m_payment_list.toVariantList();
    qSort(payments.begin(), payments.end(), [](const QVariant& a, const QVariant& b)->bool{
        long int a_start_date_millis = static_cast<long int>(a.toMap()["start_date_millis"].toDouble()/1000);
        long int b_start_date_millis = static_cast<long int>(b.toMap()["start_date_millis"].toDouble()/1000);
        return a_start_date_millis > b_start_date_millis;
    });
    setPaymentList(QJsonArray::fromVariantList(payments));
}

void QRecurringPayment::SortPaymentNameAToZ()
{
    QVariantList payments = m_payment_list.toVariantList();
    qSort(payments.begin(), payments.end(), [](const QVariant& a, const QVariant& b)->bool{
        QString aName = a.toMap()["name"].toString();
        QString bName = b.toMap()["name"].toString();
        return QString::compare(aName, bName) < 0;
    });
    setPaymentList(QJsonArray::fromVariantList(payments));
}

void QRecurringPayment::SortPaymentNameZToA()
{
    QVariantList payments = m_payment_list.toVariantList();
    qSort(payments.begin(), payments.end(), [](const QVariant& a, const QVariant& b)->bool{
        QString aName = a.toMap()["name"].toString();
        QString bName = b.toMap()["name"].toString();
        return QString::compare(aName, bName) > 0;
    });
    setPaymentList(QJsonArray::fromVariantList(payments));
}

bool QRecurringPayment::ImportBSMSWallet(const QString& filepath)
{
    return ImportFileWallet(filepath, true);
}

bool QRecurringPayment::ImportColdcardNFCWallet(const QString &filepath)
{
    return ImportFileWallet(filepath, false);
}

bool QRecurringPayment::ImportAddressesWallet(const QStringList addresses)
{
    QStringList list = addresses;
    list.removeAll("");
    QStringList listValid;
    QWarningMessage msg;
    for (QString addr : list)
    {
        bool isValid = qUtils::IsValidAddress(addr);
        bool isMyAddress = bridge::IsMyAddress(wallet_id(), addr, msg);
        if (!isValid) {
            QString message = QString("Invalid address: %1").arg(addr);
            AppModel::instance()->showToast(0, message, EWARNING::WarningType::ERROR_MSG);
        }
        else if (isMyAddress) {
            QString message = QString("Destination cannot be the same wallet.");
            AppModel::instance()->showToast(0, message, EWARNING::WarningType::ERROR_MSG);
        }
        else {
            listValid.append(addr);
        }
    }
    if (listValid.size() == list.size()) {
        QJsonObject destination_payload;
        destination_payload["addresses"] = QJsonArray::fromStringList(listValid);
        destination_payload["current_index"] = {};
        EditPayment({{"destination_payload", destination_payload}});
        return true;
    }
    return false;
}

bool QRecurringPayment::ImportWallet(WalletId w_id)
{
    if (qUtils::strCompare(w_id, wallet_id())) {
        QString message = QString("Destination cannot be the same wallet.");
        AppModel::instance()->showToast(0, message, EWARNING::WarningType::ERROR_MSG);
        return false;
    }
    if (auto w = walletInfoPtr()) {
        nunchuk::Wallet wallet = w->nunchukWallet();
        return ConvertWalletToBsmsAndAddress(wallet);
    }
    return false;
}

bool QRecurringPayment::hasServerKey()
{
    QWarningMessage msgWallet;
    if (!m_has_server_key) {
        auto js = WalletsMng->walletInfo(wallet_id());
        QJsonObject server_key = js["server_key"].toObject();
        if (!server_key.isEmpty()) {
            m_has_server_key = true;
        }
        else {
            nunchuk::Wallet wallet = bridge::nunchukGetOriginWallet(wallet_id(), msgWallet);
            for (auto key : wallet.get_signers()) {
                if (key.get_type() == nunchuk::SignerType::SERVER) {
                    m_has_server_key = true;
                }
            }
        }
    }
    return m_has_server_key;
}

bool QRecurringPayment::ImportFileWallet(const QString &filepath, bool bsms)
{
    QString fileContent = qUtils::ImportDataViaFile(filepath);
    QWarningMessage parsewarningmsg;
    nunchuk::Wallet wallet = qUtils::ParseWalletDescriptor(fileContent, parsewarningmsg);
    if((int)EWARNING::WarningType::NONE_MSG == parsewarningmsg.type()){
        parsewarningmsg.resetWarningMessage();
        std::string walletid = "";
        try {
            walletid = wallet.get_id();
        }
        catch (const nunchuk::BaseException &ex) {
            DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        }
        catch (std::exception &e) {
            DBG_INFO << "THROW EXCEPTION" << e.what();
        }
        if (qUtils::strCompare(QString::fromStdString(walletid), wallet_id())) {
            QString message = QString("Destination cannot be the same wallet.");
            AppModel::instance()->showToast(0, message, EWARNING::WarningType::ERROR_MSG);
            return false;
        }
        return ConvertWalletToBsmsAndAddress(wallet);
    }
    else {
        QString message = QString("Invalid wallet configuration");
        AppModel::instance()->showToast(0, message, EWARNING::WarningType::ERROR_MSG);
    }
    return false;
}

bool QRecurringPayment::ConvertWalletToBsmsAndAddress(const nunchuk::Wallet &wallet)
{
    QWarningMessage parsewarningmsg;
    nunchuk::WalletType type = wallet.get_wallet_type();
    QString bsms = bridge::nunchukGetWalletExportData(wallet, (nunchuk::WalletType::MINISCRIPT == type) ? nunchuk::ExportFormat::DESCRIPTOR_EXTERNAL_ALL : nunchuk::ExportFormat::BSMS);
    QStringList addressList = qUtils::DeriveAddresses(wallet, 0, 0, parsewarningmsg);
    if((int)EWARNING::WarningType::NONE_MSG == parsewarningmsg.type()){
        QJsonObject destination_payload;
        destination_payload["bsms"] = bsms;
        destination_payload["first_address"] = addressList.size() > 0 ? addressList.first() : 0;
        EditPayment({{"destination_payload", destination_payload}});
        return true;
    }
    return false;
}

QJsonObject QRecurringPayment::ConvertToDisplayQml(QJsonObject data)
{
    long int start_date_millis = static_cast<long int>(data.value("start_date_millis").toDouble()/1000);
    if (start_date_millis > 0) {
        data["start_date"] = QDateTime::fromTime_t(start_date_millis).date().toString("MM/dd/yyyy");
    } else {
        data["start_date"] = "";
    }
    long int end_date_millis = static_cast<long int>(data.value("end_date_millis").toDouble()/1000);
    if (end_date_millis > 0) {
        data["end_date"] = QDateTime::fromTime_t(end_date_millis).date().toString("MM/dd/yyyy");
    } else {
        data["end_date"] = "";
    }
    QString payment_type = data["payment_type"].toString();
    if (payment_type == "PERCENTAGE") {
        QJsonObject payment_payload = data["payment_payload"].toObject();
        payment_payload["valuePercent"] = payment_payload["value"].toDouble() * 100;
        data["payment_payload"] = payment_payload;
    }
    DBG_INFO << data;
    return data;
}

QJsonObject QRecurringPayment::ConvertToSend(QJsonObject data)
{
    QString start_date = data["start_date"].toString();
    long int start = qUtils::GetTimeSecond(start_date);
    if (start > 0) {
        data["start_date_millis"] = (double)start*1000;
    }
    else {
        data["start_date_millis"] = {};
    }

    QString end_date = data["end_date"].toString();
    long int end = qUtils::GetTimeSecond(end_date);
    if (end > 0) {
        data["end_date_millis"] = (double)end*1000;
    }
    else {
        data["end_date_millis"] = {};
    }
    QString payment_type = data["payment_type"].toString();
    if (payment_type == "PERCENTAGE") {
        QJsonObject payment_payload = data["payment_payload"].toObject();
        payment_payload["value"] = payment_payload["valuePercent"].toDouble() / 100;
        payment_payload.remove("valuePercent");
        payment_payload.remove("currency");
        data["payment_payload"] = payment_payload;
    }
    else if (payment_type == "FIXED_AMOUNT") {
        QJsonObject payment_payload = data["payment_payload"].toObject();
        payment_payload.remove("valuePercent");
        payment_payload.remove("calculation_method");
        data["payment_payload"] = payment_payload;
    }
    data.remove("start_date");
    data.remove("end_date");
    data.remove("created_time_millis");
    return data;
}
