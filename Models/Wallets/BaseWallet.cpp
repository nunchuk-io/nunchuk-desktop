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
#include "BaseWallet.h"
#include "qUtils.h"
#include "AppSetting.h"
#include "ViewsEnums.h"
#include "AppModel.h"
#include "bridgeifaces.h"
#include <QQmlEngine>

int BaseWallet::m_flow = 0;
BaseWallet::BaseWallet(const nunchuk::Wallet &w) :
    m_signers(QSingleSignerListModelPtr(new SingleSignerListModel())),
    m_transactionHistory(QTransactionListModelPtr(new TransactionListModel())),
    m_creationMode((int)CreationMode::CREATE_NEW_WALLET)
{
    setNunchukWallet(w);
}

void BaseWallet::convert(const nunchuk::Wallet w)
{
    setNunchukWallet(w);
}

QString BaseWallet::walletId() const {
    std::string id = "";
    try {
        id = nunchukWallet().get_id();
    }
    catch (const nunchuk::BaseException &ex) {
        // DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
    }
    return id.empty() ? m_id: QString::fromStdString(id);
}

void BaseWallet::setWalletId(const QString &data)
{
    if(data != m_id){
        m_id = data;
        emit walletChanged();
    }
}

int BaseWallet::walletM() {
    if(m_walletM == 0){
        m_walletM = qMax(0, nunchukWallet().get_m());
    }
    return m_walletM;
}

void BaseWallet::setWalletM(const int data) {
    DBG_INFO << "[Mini]" << data << walletType();
    int value = qMax(0, data);
    if(value != m_walletM){
        m_walletM = value;
        m_nunchukWallet.set_m(value);
        emit walletChanged();
    }
}

int BaseWallet::walletN() {
    if(m_walletN == 0){
        m_walletN = qMax(0, nunchukWallet().get_n());
    }
    return m_walletN;
}

void BaseWallet::setWalletN(const int data) {
    DBG_INFO << "[Mini]" << data << walletType();
    int value = qMax(0, data);
    if(value != m_walletN){
        m_walletN = value;
        m_nunchukWallet.set_n(value);
        emit walletChanged();
    }
}

QString BaseWallet::walletNameDisplay()
{
    QRegularExpression re("\\p{So}");
    QString originname = walletName();
    originname.remove(re);
    return originname;
}

QString BaseWallet::walletName()
{
    m_walletName = QString::fromStdString(m_nunchukWallet.get_name());
    return m_walletName;
}

void BaseWallet::setWalletName(const QString &data) {
    if(!qUtils::strCompare(data, m_walletName)){
        m_walletName = data;
        m_nunchukWallet.set_name(data.toStdString());
        emit walletChanged();
    }
}

int BaseWallet::walletAddressType() {
    if(m_walletAddressType == (int)nunchuk::AddressType::ANY){
        int cdata = (int)nunchukWallet().get_address_type();
        std::set<int> valid_numbers = {(int)nunchuk::AddressType::LEGACY, (int)nunchuk::AddressType::NESTED_SEGWIT, (int)nunchuk::AddressType::NATIVE_SEGWIT, (int)nunchuk::AddressType::TAPROOT};
        if (valid_numbers.find(cdata) != valid_numbers.end()) {
            m_walletAddressType = cdata;
        }
    }
    return m_walletAddressType;
}

void BaseWallet::setWalletAddressType(const int data) {
    if(data != m_walletAddressType){
        m_walletAddressType = data;
        m_nunchukWallet.set_address_type((nunchuk::AddressType)data);
        emit walletChanged();
    }
}

int BaseWallet::walletType()
{
    if(m_walletType == -1){
        int cdata = (int)nunchukWallet().get_wallet_type();
        std::set<int> valid_numbers = {(int)nunchuk::WalletType::SINGLE_SIG, (int)nunchuk::WalletType::MULTI_SIG, (int)nunchuk::WalletType::ESCROW, (int)nunchuk::WalletType::MINISCRIPT};
        if (valid_numbers.find(cdata) != valid_numbers.end()) {
            m_walletType = cdata;
        }
    }
    return m_walletType;
}

void BaseWallet::setWalletType(const int data)
{
    DBG_INFO << "[Mini] setWalletType" << data;
    if(data != m_walletType){
        m_walletType = data;
        try {
            m_nunchukWallet.set_wallet_type((nunchuk::WalletType)data);
        }
        catch (const nunchuk::BaseException &ex) {
            DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        }
        catch (std::exception &e) {
            DBG_INFO << "THROW EXCEPTION" << e.what();
        }
        emit walletChanged();
    }
}

bool BaseWallet::walletEscrow() const {
    return ((nunchuk::WalletType::ESCROW == nunchukWallet().get_wallet_type()) ? true : false);
}

bool BaseWallet::enableValuekeyset()
{
    int wallet_Type = walletType();
    int walletAddress_Type = walletAddressType();
    int walletTemplate_Type = walletTemplate();
    if(((int)nunchuk::AddressType::TAPROOT == walletAddress_Type)
        && ((int)nunchuk::WalletType::MULTI_SIG == wallet_Type)
        && ((int)nunchuk::WalletTemplate::DEFAULT == walletTemplate_Type))
    {
        m_enableValuekeyset = true;
    }
    else {
        m_enableValuekeyset = false;
    }
    return m_enableValuekeyset;
}

void BaseWallet::setEnableValuekeyset(bool data)
{
    if (m_enableValuekeyset != data){
        m_enableValuekeyset = data;
        emit walletChanged();
    }
    int template_type = (int)nunchuk::WalletTemplate::DISABLE_KEY_PATH;
    if(m_enableValuekeyset){
        template_type = (int)nunchuk::WalletTemplate::DEFAULT;
    }
    setWalletTemplate(template_type);
}

int BaseWallet::walletTemplate()
{
    if(!walletId().isEmpty()){
        int cdata = (int)nunchukWallet().get_wallet_template();
        std::set<int> valid_numbers = {(int)nunchuk::WalletTemplate::DEFAULT, (int)nunchuk::WalletTemplate::DISABLE_KEY_PATH};
        if (valid_numbers.find(cdata) != valid_numbers.end()) {
            m_walletTemplate = cdata;
        }
    }
    return m_walletTemplate;
}

void BaseWallet::setWalletTemplate(const int data)
{
    if(data != m_walletTemplate){
        m_walletTemplate = data;
        m_nunchukWallet.set_wallet_template((nunchuk::WalletTemplate)data);
        emit walletChanged();
    }
}

qint64 BaseWallet::balanceSats() const
{
    //2,100,000,000,000,000 Max
    return nunchukWallet().get_unconfirmed_balance();
}

QString BaseWallet::balanceBTC() const
{
    return qUtils::QValueFromAmount(balanceSats());
}

QString BaseWallet::balanceDisplay() const {
    if((int)AppSetting::Unit::SATOSHI == AppSetting::instance()->unit()){
        QLocale locale(QLocale::English);
        return locale.toString(balanceSats());
    }
    else{
        return balanceBTC();
    }
}

QString BaseWallet::balanceCurrency() const
{
    return qUtils::currencyLocale(balanceSats());
}

void BaseWallet::setWalletBalance(const qint64 data) {
    if(data != balanceSats()){
        m_nunchukWallet.set_balance(data);
        emit walletChanged();
    }
}

QString BaseWallet::walletCreateDateDisp() const {
    if(0 == walletCreateDate().toTime_t()){
        return "--/--/----"; // There is no time
    }
    else {
        //    return createDate_.toOffsetFromUtc(QDateTime::currentDateTime().offsetFromUtc()).toString(Qt::ISODate);
        return walletCreateDate().toString("MM/dd/yyyy hh:mm AP");
    }
}

QDateTime BaseWallet::walletCreateDate() const
{
    time_t date = nunchukWallet().get_create_date();
    return QDateTime::fromTime_t(date);
}

int BaseWallet::walletGapLimit() const
{
    return (nunchukWallet().get_gap_limit());
}

void BaseWallet::setWalletGapLimit(const int data)
{
    if(data != walletGapLimit()){
        m_nunchukWallet.set_gap_limit(data);
        emit walletChanged();
    }
}

void BaseWallet::setSigners(const QSingleSignerListModelPtr &d){
    m_signers = d;
    emit walletChanged();
    emit walletSignersChanged();
}

QString BaseWallet::descriptior() const
{
    return m_descriptior;
}

void BaseWallet::setDescriptior(const QString &descriptior)
{
    m_descriptior = descriptior;
}

SingleSignerListModel* BaseWallet::singleSignersAssigned() {
    if(!m_signers){
        m_signers = QSingleSignerListModelPtr(new SingleSignerListModel());
    }
    if(m_signers.data()->signerCount() == 0){
        m_signers->cleardata();
        int m = walletM();
        std::vector<nunchuk::SingleSigner> signers = nunchukWallet().get_signers();
        for (int i = 0; i < signers.size(); i++) {
            nunchuk::SingleSigner signer = signers.at(i);
            QSingleSignerPtr ret = QSingleSignerPtr(new QSingleSigner(signer));
            if(enableValuekeyset()){
                if(i < m){
                    ret.data()->setValueKey(true);
                }
            }
            m_signers->addSingleSigner(ret);
        }
    }
    if(m_signers.data()->signerCount() > 1){
        m_signers->requestSort();
    }
    return m_signers.data();
}

QSingleSignerListModelPtr BaseWallet::signersPtr() const
{
    return m_signers;
}


QString BaseWallet::walletDescription()
{
    QString data_description = QString::fromStdString(nunchukWallet().get_description());
    if(data_description != ""){
        m_walletDescription = data_description;
    }
    return m_walletDescription;
}

void BaseWallet::setWalletDescription(const QString &data)
{
    if(!qUtils::strCompare(data, walletDescription())){
        m_walletDescription = data;
        m_nunchukWallet.set_description(data.toStdString());
        emit walletChanged();
    }
}

QString BaseWallet::address() const {
    return m_address;
}

void BaseWallet::setAddress(const QString &d){
    if(d != m_address){
        m_address = d;
        emit addressChanged();
    }
}

QStringList BaseWallet::usedAddressList() const {
    return m_usedAddressList;
}

void BaseWallet::setUsedAddressList(const QStringList &d){
    if(d != m_usedAddressList){
        m_usedAddressList = d;
        emit usedAddressChanged();
    }
}

QStringList BaseWallet::unUsedAddressList() const {
    return m_unUsedAddressList;
}

void BaseWallet::setunUsedAddressList(const QStringList &d){
    m_unUsedAddressList = d;
    emit unUsedAddressChanged();
    if(m_unUsedAddressList.isEmpty()){
        setAddress("There is no avaialable address");
    }
    else{
        setAddress(m_unUsedAddressList.first());
    }
}

bool BaseWallet::capableCreate() const
{
    return m_capableCreate;
}

void BaseWallet::setCapableCreate(bool capableCreate)
{
    if(m_capableCreate != capableCreate){
        m_capableCreate = capableCreate;
        emit capableCreateChanged();
    }
}

QStringList BaseWallet::usedChangeAddressList() const
{
    return m_usedChangeAddressList;
}

void BaseWallet::setUsedChangeAddressList(const QStringList &usedChangeAddressList)
{
    if(m_usedChangeAddressList != usedChangeAddressList){
        m_usedChangeAddressList = usedChangeAddressList;
        emit usedChangeAddressChanged();
    }
}

QStringList BaseWallet::unUsedChangeddAddressList() const
{
    return m_unUsedChangedAddressList;
}

void BaseWallet::setUnUsedChangeddAddressList(const QStringList &unUseChangeddAddressList)
{
    if(m_unUsedChangedAddressList != unUseChangeddAddressList){
        m_unUsedChangedAddressList = unUseChangeddAddressList;
        emit unUsedChangeAddressChanged();
    }
}

TransactionListModel *BaseWallet::transactionHistory() const{
    return m_transactionHistory.data();
}

QTransactionPtr BaseWallet::getTransactionByIndex(const int index) const
{
    if(m_transactionHistory.data()){
         return m_transactionHistory.data()->getTransactionByIndex(index);
    }
    return NULL;
}

QTransactionPtr BaseWallet::getTransactionByTxid(const QString &txid) const
{
    if(m_transactionHistory.data()){
         return m_transactionHistory.data()->getTransactionByTxid(txid);
    }
    return NULL;
}

void BaseWallet::setTransactionHistory(const QTransactionListModelPtr &d) {
    m_transactionHistory = d;
    if(m_transactionHistory){
        m_transactionHistory->requestSort(TransactionListModel::TransactionRoles::transaction_blocktime_role, Qt::DescendingOrder);
    }
    emit transactionHistoryChanged();
}

QString BaseWallet::unUseAddress() const
{
    QString wallet_id = walletId();
    QString address;
    QStringList addrs = bridge::nunchukGetUnusedAddresses(wallet_id, false);
    if (addrs.size() > 0) {
        address = addrs.first();
    }
    else {
        address = bridge::nunchukGenNewAddresses(wallet_id,false);
    }
    return address;
}

bool BaseWallet::isDeleting() const
{
    return m_isDeleting;
}

void BaseWallet::setIsDeleting(const bool val)
{
    if (m_isDeleting == val)
        return;
    m_isDeleting = val;
    emit isDeletingChanged();
}

int BaseWallet::limitKeySet() const
{
    return m_limitKeySet;
}

int BaseWallet::getCreationMode() const
{
    return m_creationMode;
}

void BaseWallet::setCreationMode(int creationMode)
{
    m_creationMode = creationMode;
}

bool BaseWallet::getContainsHWSigner() const
{
    if(m_signers){
        return m_signers.data()->containsHardwareKey();
    }
    return false;
}

bool BaseWallet::containsColdcard()
{
    if(m_signers){
        return m_signers.data()->containsColdcard();
    }
    return false;
}

bool BaseWallet::isArchived() const
{
    return m_nunchukWallet.is_archived();
}

void BaseWallet::setArchived(bool archived)
{
    m_nunchukWallet.set_archived(archived);
    QWarningMessage msg;
    bridge::UpdateWallet(m_nunchukWallet, msg);
    if(msg.type() == (int)EWARNING::WarningType::NONE_MSG){
        QString msg_content = archived ? "Archived wallet" : "Unarchived wallet";
        AppModel::instance()->showToast(0, msg_content, EWARNING::WarningType::SUCCESS_MSG);
    }
    emit walletChanged();
}

nunchuk::Wallet BaseWallet::nunchukWallet() const
{
    return m_nunchukWallet;
}

void BaseWallet::setNunchukWallet(const nunchuk::Wallet &data)
{
    m_nunchukWallet = data;
    emit walletChanged();
}

int BaseWallet::flow() const
{
    return m_flow;
}

void BaseWallet::setFlow(int flow)
{
    m_flow = flow;
    emit flowChanged();
}

void BaseWallet::updateSignMessage(const QString &xfp, int wallet_type)
{
    QtConcurrent::run([=, this]() {
        ENUNCHUCK::AddressType type = (ENUNCHUCK::AddressType)wallet_type;
        QWarningMessage msg;
        QSingleSignerPtr single = AppModel::instance()->remoteSignerListPtr()->getSingleSignerByFingerPrint(xfp);
        if (single) {
            QString address = bridge::GetSignerAddress(single->originSingleSigner(), (nunchuk::AddressType)type);
            QString signature = bridge::SignMessage(single->originSingleSigner(), single->message());
            single->setAddress(address);
            single->setSignature(signature);
            DBG_INFO << address << signature;
            if (!signature.isEmpty()) {
                AppModel::instance()->showToast(0, "The message has been signed", EWARNING::WarningType::SUCCESS_MSG );
            }
        }
        if (single.isNull()) {
            QMasterSignerPtr master = AppModel::instance()->masterSignerListPtr()->getMasterSignerByXfp(xfp);
            single = bridge::nunchukGetDefaultSignerFromMasterSigner(master->id(),
                                                                     ENUNCHUCK::WalletType::MULTI_SIG,
                                                                     type ,
                                                                     msg);
            if (single) {
                QString address = bridge::GetSignerAddress(single->originSingleSigner(), (nunchuk::AddressType)type);
                if (master) {
                    QString signature = bridge::SignMessage(single->originSingleSigner(), master->message());
                    master->setAddress(address);
                    master->setSignature(signature);
                    DBG_INFO << address << signature;
                    if (!signature.isEmpty()) {
                        AppModel::instance()->showToast(0, "The message has been signed", EWARNING::WarningType::SUCCESS_MSG );
                    }
                }
            }
        }
        emit signMessageChanged();
    });
}

void BaseWallet::exportBitcoinSignedMessage(const QString &xfp, const QString &file_path, int wallet_type)
{
    QString path = qUtils::QGetFilePath(file_path);
    QString address_type = qUtils::qAddressTypeToStr((nunchuk::AddressType)wallet_type);
    QSingleSignerPtr single = AppModel::instance()->remoteSignerListPtr()->getSingleSignerByFingerPrint(xfp);
    QString signMessage;
    if (single) {
        signMessage = qUtils::ExportBitcoinSignedMessage(single->message(), address_type, single->signature());
    }
    if (single.isNull()) {
        QMasterSignerPtr master = AppModel::instance()->masterSignerListPtr()->getMasterSignerByXfp(xfp);
        if (master) {
            signMessage = qUtils::ExportBitcoinSignedMessage(master->message(), address_type, master->signature());
        }
    }
    QFile file(path);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream st(&file);
        st.setCodec("UTF-8");
        st << signMessage << Qt::endl;
        st.flush();
        file.close();
    }
}

QVariantList BaseWallet::signerExistList() const
{
    return m_signerExistList;
}

void BaseWallet::setSignerExistList(QVariantList signerExistList)
{
    if (m_signerExistList == signerExistList)
        return;

    m_signerExistList = signerExistList;
    emit signerExistListChanged();
}

DeviceListModel *BaseWallet::deviceList() const
{
    return deviceList_.data();
}

void BaseWallet::setDeviceList(const QDeviceListModelPtr &d)
{
    if (deviceList_.isNull()) {
        deviceList_ = d;
        emit deviceListChanged();
    } else {
        deviceList_->updateDeviceList(d);
        emit deviceListChanged();
    }
}

QString BaseWallet::deviceType() const
{
    return m_deviceType;
}

QString BaseWallet::deviceType(const QString &xfp)
{
    auto m = bridge::nunchukGetMasterSignerFingerprint(xfp);
    if (m) {
        return m->devicePtr()->type();
    } else {
        auto s = bridge::nunchukGetRemoteSigner(xfp);
        if (s) {
            if (!s->tag().isEmpty()) {
                QMap<QString, QString> tags;
                tags.insert("LEDGER",   "ledger");
                tags.insert("TREZOR",   "trezor");
                tags.insert("COLDCARD", "coldcard");
                tags.insert("BITBOX",   "bitbox02");
                return tags.value(s->tag());
            } else if(s->signerType() == (int)ENUNCHUCK::SignerType::COLDCARD_NFC) {
                return "coldcard";
            } else {
                return s->devicetype();
            }
        }
    }
    return "";
}

void BaseWallet::setDeviceType(const QString &deviceType)
{
    if (m_deviceType == deviceType)
        return;
    m_deviceType = deviceType;
    emit deviceTypeChanged();
}

void BaseWallet::UpdateWallet(const QString &name, const QString &description)
{
    QWarningMessage msg;
    m_nunchukWallet.set_name(name.toStdString());
    m_nunchukWallet.set_description(description.toStdString());
    bridge::UpdateWallet(m_nunchukWallet, msg);
    emit walletChanged();
}

QWalletPtr BaseWallet::clone() const
{
    QWalletPtr to = QWalletPtr(new Wallet());
    to.data()->convert(nunchukWallet());
    return to;
}

std::vector<nunchuk::SingleSigner> BaseWallet::localSigners()
{
    std::vector<nunchuk::SingleSigner> ret;
    if(m_signers){
        ret = m_signers.data()->localSigners();
    }
    return ret;
}

bool BaseWallet::isContainKey(const QString &xfp)
{
    std::vector<nunchuk::SingleSigner> signers = nunchukWallet().get_signers();
    for (int i = 0; i < signers.size(); i++) {
        nunchuk::SingleSigner signer = signers.at(i);
        QString xfp_from_signer = QString::fromStdString(signer.get_master_fingerprint());
        if (qUtils::strCompare(xfp_from_signer, xfp)) {
            return true;
        }
    }
    return false;
}

void BaseWallet::setValueKeyset(int index)
{
    DBG_INFO << index;
    if (!singleSignersAssigned()) return;
    auto signers = singleSignersAssigned();
    auto list = singleSignersAssigned()->fullList();
    int count = 0;
    for (int i = 0; i < list.size(); i++) {
        auto signer = list.at(i);
        if (signer->valueKey()) {
            count++;
        }
        if (i == index) {
            if (count < walletM()) {
                signer->setValueKey(!signer->valueKey());
            } else {
                signer->setValueKey(false);
            }
            signers->setUserChecked(signer->valueKey(), i);
        }
    }
    singleSignersAssigned()->requestSort();
    m_limitKeySet = singleSignersAssigned()->signerSelectedCount();
    DBG_INFO << m_limitKeySet;
    emit limitKeySetChanged();
}

bool BaseWallet::isValidAddress(const QString &address)
{
    return qUtils::IsValidAddress(address);
}

QString BaseWallet::addressPath(const QString &address)
{
    return bridge::nunchukGetAddressPath(walletId(), address);
}

QString BaseWallet::addressBalance(const QString &address)
{
    qint64 amount = bridge::nunchukGetAddressBalance(walletId(), address);
    if((int)AppSetting::Unit::SATOSHI == AppSetting::instance()->unit()){
        QLocale locale(QLocale::English);
        return locale.toString(amount);
    }
    else{
        return qUtils::QValueFromAmount(amount);
    }
}

bool BaseWallet::markAddressUsed(const QString &address)
{
    bool ret = bridge::nunchukMarkAddressUsed(walletId(), address);
    if(ret){
        QtConcurrent::run([=, this]() {
            QStringList used_addr = bridge::nunchukGetUsedAddresses(walletId(), false);
            QStringList unused_addr = bridge::nunchukGetUnusedAddresses(walletId(), false);
            this->setUsedAddressList(used_addr);
            this->setunUsedAddressList(unused_addr);
        });
    }
    return ret;
}
