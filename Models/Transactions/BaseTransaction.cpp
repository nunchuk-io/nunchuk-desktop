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
#include "BaseTransaction.h"
#include "AppSetting.h"
#include "QtGui/qclipboard.h"
#include "qUtils.h"
#include "QOutlog.h"
#include "bridgeifaces.h"
#include "Chats/matrixbrigde.h"
#include "AppModel.h"
#include <QQmlEngine>
#include <nunchukmatrix.h>
#include "Servers/Byzantine.h"
#include "Premiums/QGroupDashboard.h"
#include "Models/TransactionModel.h"

Destination::Destination(): address_(""), amount_(0){
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

Destination::Destination(const QString &address, const qint64 amount) : address_(address), amount_(amount){
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

Destination::~Destination() {}

QString Destination::amountDisplay() const
{
    if((int)AppSetting::Unit::SATOSHI == AppSetting::instance()->unit()){
        QLocale locale(QLocale::English);
        return locale.toString(amountSats());
    }
    else{
        return amountBTC();
    }
}

QString Destination::amountBTC() const
{
    return qUtils::QValueFromAmount(amount_);
}

QString Destination::amountCurrency() const
{
    return qUtils::currencyLocale(amountSats());
}

qint64 Destination::amountSats() const
{
    return amount_;
}

QString Destination::address() const {
    return address_;
}

void Destination::setAddress(const QString &value){
    if(value != address_){
        address_ = value;
        emit addressChanged();
    }
}

QString Destination::label()
{
    QStringList fav = AppSetting::instance()->favoriteAddresses();
    for (const QString &item : fav) {
        if (item.contains(address_, Qt::CaseInsensitive)) {
            return item.split("[split]")[0];
        }
    }
    return "";
}

void Destination::setAmount(const qint64 value){
    if(value != amount_){
        amount_ = value;
        emit amountChanged();
    }
}

DestinationListModel::DestinationListModel(){
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

DestinationListModel::~DestinationListModel(){m_data.clear();}

int DestinationListModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return m_data.count();
}

QVariant DestinationListModel::data(const QModelIndex &index, int role) const {
    switch (role) {
    case destination_label_role:
        return m_data[index.row()]->label();
    case destination_address_role:
        return m_data[index.row()]->address();
    case destination_amount_role:
        return m_data[index.row()]->amountDisplay();
    case destination_amount_btc_role:
        return m_data[index.row()]->amountBTC();
    case destination_amount_currency_role:
        return m_data[index.row()]->amountCurrency();
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> DestinationListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[destination_label_role] = "destination_label";
    roles[destination_address_role] = "destination_address";
    roles[destination_amount_role] = "destination_amount";
    roles[destination_amount_btc_role] = "destination_amount_btc";
    roles[destination_amount_currency_role] = "destination_amount_currency";
    return roles;
}

void DestinationListModel::addDestination(const QString &address, const qint64 amount) {
    qint64 am = 0;
    if (!contains(address, am)) {
        beginResetModel();
        m_data.append(QDestinationPtr(new Destination(address, amount)));
        endResetModel();
    }
}

QMap<QString, qint64> DestinationListModel::getOutputs() const
{
    QMap<QString, qint64> outputs;
    foreach (QDestinationPtr i , m_data ){
        outputs[i->address()] = i->amountSats();
    }
    return outputs;
}

bool DestinationListModel::contains(const QString &address, qint64 &amount)
{
    for (const auto &dest : std::as_const(m_data)) {
        auto ptr = dest.data();
        if (qUtils::strCompare(address, ptr->address())) {
            amount = ptr->amountSats();
            return true;
        }
    }
    return false;
}

qint64 DestinationListModel::getAmountByAddress(const QString &address)
{
    for (const auto &dest : std::as_const(m_data)) {
        const auto *ptr = dest.data();
        if (address == ptr->address()) {
            return ptr->amountSats();
        }
    }
    return 0;
}

void DestinationListModel::notifyUnitChanged()
{
    beginResetModel();
    foreach (QDestinationPtr it, m_data) {
        if(it.data()){
            it.data()->amountChanged();
        }
    }
    endResetModel();
}

QString DestinationListModel::reciever()
{
    if(m_data.count() > 2){
        return "Multiple addresses";
    }
    else{
        return (m_data.count() == 0 ? "unknown" : m_data.at(0).data()->address());
    }
}

void DestinationListModel::clearAll()
{
    m_data.clear();
}

BaseTransaction::BaseTransaction() :
    m_destinations(QDestinationListModelPtr(new DestinationListModel())),
    m_signers(QSingleSignerListModelPtr(new (SingleSignerListModel))),
    m_keysets(QSingleSignerListModelPtr(new (SingleSignerListModel))),
    m_change(QDestinationPtr(new Destination())),
    m_walletId(""),
    m_roomId(""),
    m_initEventId(""),
    m_createByMe(true),
    m_serverKeyMessage("")
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

BaseTransaction::BaseTransaction(const nunchuk::Transaction &tx):
    m_destinations(QDestinationListModelPtr(new DestinationListModel())),
    m_signers(QSingleSignerListModelPtr(new (SingleSignerListModel))),
    m_keysets(QSingleSignerListModelPtr(new (SingleSignerListModel))),
    m_change(QDestinationPtr(new Destination())),
    m_transaction(tx),
    m_walletId(""),
    m_roomId(""),
    m_initEventId(""),
    m_createByMe(true),
    m_serverKeyMessage("")
{
    createDestinationList();
}

BaseTransaction::~BaseTransaction()
{
    m_destinations.clear();
    m_keysets.clear();
    m_signers.clear();
    m_change.clear();
}

bool BaseTransaction::isReceiveTx() const
{
    return m_transaction.is_receive();
}

void BaseTransaction::setIsReceiveTx(bool receive)
{
    m_transaction.set_receive(receive);
    emit nunchukTransactionChanged();
}

QString BaseTransaction::txid() const {
    return QString::fromStdString(m_transaction.get_txid());
}

QString BaseTransaction::memo() const {
    return QString::fromStdString(m_transaction.get_memo());
}

void BaseTransaction::setMemo(const QString &memo)
{
    DBG_INFO << memo;
    if(!qUtils::strCompare(memo, QString::fromStdString(m_transaction.get_memo()))){
        m_transaction.set_memo(memo.toStdString());
        bridge::nunchukUpdateTransactionMemo(walletId(), txid(), memo);
        emit nunchukTransactionChanged();
    }
}

int BaseTransaction::status() const {
    return (int)m_transaction.get_status();
}

void BaseTransaction::setStatus(int status)
{
    m_transaction.set_status((nunchuk::TransactionStatus)status);
    emit nunchukTransactionChanged();
}

const std::vector<nunchuk::KeysetStatus> &BaseTransaction::keysetStatus() const
{
    return m_transaction.get_keyset_status();
}

int BaseTransaction::keysetsCount()
{
    return keysetStatus().size();
}

int BaseTransaction::m() const {
    return m_transaction.get_m();
}

int BaseTransaction::height() const
{
    return m_transaction.get_height();
}

void BaseTransaction::setHeight(const int value){
    m_transaction.set_height(value);
    emit nunchukTransactionChanged();
}

QString BaseTransaction::feeDisplay() const
{
    if((int)AppSetting::Unit::SATOSHI == AppSetting::instance()->unit()){
        QLocale locale(QLocale::English);
        return locale.toString(feeSats());
    }
    else{
        return feeBTC();
    }
}

QString BaseTransaction::feeBTC() const
{
    return qUtils::QValueFromAmount(feeSats());
}

QString BaseTransaction::feeCurrency() const
{
    return qUtils::currencyLocale(feeSats());
}

qint64 BaseTransaction::feeSats() const
{
    return (qint64)m_transaction.get_fee();
}

void BaseTransaction::setFee(const qint64 fee)
{
    m_transaction.set_fee(fee);
    emit nunchukTransactionChanged();
}

bool BaseTransaction::subtractFromFeeAmount() const
{
    return m_transaction.subtract_fee_from_amount();
}

QString BaseTransaction::feeRate() const
{
    return QString::number((double)feeRateSats()/1000, 'f', 2);
}

qint64 BaseTransaction::feeRateSats() const
{
    return m_transaction.get_fee_rate();
}

QString BaseTransaction::psbt() const
{
    return QString::fromStdString(m_transaction.get_psbt());
}

time_t BaseTransaction::scheduleTime(){
    return m_transaction.get_schedule_time();
}

bool BaseTransaction::isCosigning() const
{
    return m_is_cosigning;
}

void BaseTransaction::setIsCosigning(bool is_cosigning)
{
    m_is_cosigning = is_cosigning;
}

bool BaseTransaction::parseQRTransaction(const QStringList &qrtags)
{
    QStringList in = qrtags;
    in.removeDuplicates();
    if(in.isEmpty()){
        return false;
    }
    QString wallet_id = walletId();
    if(ImportQRTransaction(in)){
        AppModel::instance()->requestSyncWalletDb(wallet_id);
        AppModel::instance()->transactionInfoChanged();
        return true;
    }
    return false;
}

void BaseTransaction::copyTransactionID()
{
    qApp->clipboard()->setText(txid());
}

void BaseTransaction::requestSignatures(const QString &membership_id)
{
    QJsonObject data;
    QString errormsg = "";
    QString groupid = AppModel::instance()->walletInfo() ? AppModel::instance()->walletInfo()->groupId() : "";
    DBG_INFO << groupid << walletId() << txid() << membership_id;
    bool ret = Byzantine::instance()->requestSignature(groupid, walletId(), txid(), membership_id, data, errormsg);
    if(ret){
        AppModel::instance()->showToast(0, "Signature request sent", EWARNING::WarningType::SUCCESS_MSG);
    }
}

void BaseTransaction::scheduleBroadcast()
{

}

void BaseTransaction::cancelTransaction()
{

}

bool BaseTransaction::hasDraftCoinChange()
{
    createParentCoinTag();
    if (m_parentCoinsTag.isNull()) return false;
    return hasChange() && m_parentCoinsTag->count() > 0;
}

void BaseTransaction::createParentCoinTag()
{
    if (!m_parentCoinsTag) {
        m_parentCoinsTag = QCoinTagsModelPtr(new QCoinTagsModel());
    }
    m_parentCoinsTag->clear();
    QList<int> tags;
    auto inputs = bridge::nunchukGetOriginCoinsFromTxInputs(m_walletId, m_transaction.get_inputs());
    for (nunchuk::UnspentOutput it : inputs) {
        for (auto tag: it.get_tags()) {
            tags.append(tag);
        }
    }
    if (auto w = AppModel::instance()->walletList()->getWalletById(walletId())) {
        m_parentCoinsTag->setChecked(w->tagsInTxAssigned());
    }
    m_parentCoinsTag->refreshTags(tags, m_walletId, true);
    emit parentCoinsTagChanged();
}

bool BaseTransaction::ImportQRTransaction(const QStringList& qrtags)
{
    QStringList in = qrtags;
    in.removeDuplicates();
    QString wallet_id = walletId();
    QWarningMessage msg;
    nunchuk::Transaction trans_result = bridge::nunchukImportQRTransaction(wallet_id, in, msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        setNunchukTransaction(trans_result);
        if (roomId() != "") {
            QWarningMessage room_msg;
            nunchuk::RoomTransaction room_tx = matrixbrigde::GetOriginPendingTransaction(roomId(), txid(), room_msg);
            if((int)EWARNING::WarningType::NONE_MSG == room_msg.type()){
                setInitEventId(QString::fromStdString(room_tx.get_init_event_id()));
            }

            QWarningMessage sign_msg;
            auto signers = singleSignersAssigned();
            for(QSingleSignerPtr signer : signers->fullList()){
                if(signer->signerType() == (int)ENUNCHUCK::SignerType::AIRGAP
                        && signer->signerSigned()
                        && signers->containsFingerPrint(signer->masterFingerPrint())
                        && initEventId() != ""){
                    matrixbrigde::SignAirgapTransaction(initEventId(),
                                                        signer->masterFingerPrint(),
                                                        sign_msg);
                }
            }
        }
        return true;
    }
    else{
        return false;
    }
}

QString BaseTransaction::feeOtherKeysetDisplay() const
{
    if((int)AppSetting::Unit::SATOSHI == AppSetting::instance()->unit()){
        QLocale locale(QLocale::English);
        return locale.toString(feeOtherKeyset());
    }
    else{
        return feeOtherKeysetBTC();
    }
}

QString BaseTransaction::feeOtherKeysetBTC() const
{
    return qUtils::QValueFromAmount(feeOtherKeyset());
}

QString BaseTransaction::feeOtherKeysetCurrency() const
{
    return qUtils::currencyLocale(feeOtherKeyset());
}

qint64 BaseTransaction::feeOtherKeyset() const
{
    return m_fee_otherKeyset;
}

void BaseTransaction::setFeeOtherKeyset(qint64 data)
{
    if (m_fee_otherKeyset == data)
        return;
    m_fee_otherKeyset = data;
    emit feeOtherKeysetChanged();
}

bool BaseTransaction::useScriptPath()
{
    return m_useScriptPath;
}

void BaseTransaction::setUseScriptPath(bool data, bool cached)
{
    if (m_useScriptPath != data){
        m_useScriptPath = data;
        emit useScriptPathChanged();
    }
    if(cached){
        QString tx_id = txid();
        AppSetting::instance()->setTransactionUsescriptpath(tx_id, m_useScriptPath);
    }
    emit useScriptPathChanged();
}

QString BaseTransaction::txidReplacing() const
{
    return m_txidReplacing;
}

void BaseTransaction::setTxidReplacing(const QString &id)
{
    if (m_txidReplacing == id)
        return;
    m_txidReplacing = id;
    emit txidReplacingChanged();
}

bool BaseTransaction::isClaimTx() const {
    return m_isClaimTx;
}

void BaseTransaction::setIsClaimTx(bool is_claim_tx) {
    if (m_isClaimTx == is_claim_tx)
        return;
    m_isClaimTx = is_claim_tx;
}

QUTXOListModel* BaseTransaction::inputCoins()
{
    if(!m_inputCoins){
        m_inputCoins = QUTXOListModelPtr(new QUTXOListModel(m_walletId));
    }
    std::vector<nunchuk::UnspentOutput> inputs = bridge::nunchukGetOriginCoinsFromTxInputs(m_walletId, m_transaction.get_inputs());
    m_inputCoins->clearAll();
    for (nunchuk::UnspentOutput it : inputs) {
        m_inputCoins->addUTXO(it);
    }
    return m_inputCoins.data();
}

QUTXOListModel *BaseTransaction::manualCoins()
{
    return m_manualCoins.data();
}

void BaseTransaction::createFilteringCoinInCoinSelection()
{
    if (m_manualCoins.isNull()) {
        auto newCoins = bridge::nunchukGetOriginUnspentOutputs(m_walletId);
        m_manualCoins = QUTXOListModelPtr(new QUTXOListModel(m_walletId));
        for (nunchuk::UnspentOutput it : newCoins) {
            if (!it.is_locked()) {
                m_manualCoins->addUTXO(it);
            }
        }
    }
    for (auto old_utxo : m_inputCoins->fullList()) {
        for (auto new_utxo : m_manualCoins->fullList()) {
            if (old_utxo->txid() == new_utxo->txid()) {
                new_utxo->setSelected(true);
            }
        }
    }
    emit manualCoinsChanged();
}

QUTXOListModelPtr BaseTransaction::GetUtxoListSelected()
{
    QUTXOListModelPtr inputs = QUTXOListModelPtr(new QUTXOListModel(m_walletId));
    if(m_manualCoins){
        for (int i = 0; i < m_manualCoins->rowCount(); i++) {
            QUTXOPtr it = m_manualCoins->getUTXOByIndex(i);
            if(it.data() && it.data()->selected()){
                DBG_INFO << "UTXO Selected:" << it.data()->txid() << it.data()->getTags();
                inputs->addUTXO(it.data()->getUnspentOutput());
            }
        }
    }
    return inputs;
}

QString BaseTransaction::scriptPathFeeRate()
{
    QString wallet_Id = walletId();
    QWalletPtr wallet = QWalletPtr(NULL);
    if (AppModel::instance()->walletList()) {
        wallet = AppModel::instance()->walletList() ? AppModel::instance()->walletList()->getWalletById(wallet_Id) : QWalletPtr(NULL);
    }
    if(wallet){
        setRoomId(wallet->roomId());
        int walletAddressType = wallet->walletAddressType();
        int walletTemplateType = wallet->walletTemplate();
        int walletType        = wallet->walletType();
        if((walletAddressType == (int)nunchuk::AddressType::TAPROOT)
            && ((int)nunchuk::WalletType::MULTI_SIG == walletType)
            && (walletTemplateType == (int)nunchuk::WalletTemplate::DEFAULT))
        {
            int fee_rate  = bridge::GetScriptPathFeeRate(wallet_Id, nunchukTransaction());
            QString script_path_fee = QString::number((double)fee_rate/1000, 'f', 2);
            return script_path_fee;
        }
    }
    return "";
}

void BaseTransaction::setTxJson(const QJsonObject &txJs)
{
    m_txJson = txJs;
}

QJsonObject BaseTransaction::txJson() const
{
    return m_txJson;
}

void BaseTransaction::setSignatures(const QMap<QString, QString> &signatures)
{
    m_signatures = signatures;
}

QMap<QString, QString> BaseTransaction::signatures() const
{
    return m_signatures;
}

bool BaseTransaction::hasMoreBtn() const
{
    return m_hasMoreBtn;
}

void BaseTransaction::setHasMoreBtn(bool has)
{
    if (m_hasMoreBtn == has)
        return;

    m_hasMoreBtn = has;
    emit hasMoreBtnChanged();
}

QString BaseTransaction::dummyXfp() const
{
    return {};
}

bool BaseTransaction::isDummyTx() const
{
    return false;
}

QStringList BaseTransaction::hideSignBtns() const
{
    return {};
}

bool BaseTransaction::enableRequestSignature()
{
    if(AppModel::instance()->walletInfo()){
        QString groupid = AppModel::instance()->walletInfo()->groupId();
        if(groupid != "" && AppModel::instance()->walletInfo()->dashboard()){
            QString role = AppModel::instance()->walletInfo()->dashboard()->myRole();
            DBG_INFO << !qUtils::strCompare(role, "OBSERVER");
            return (!qUtils::strCompare(role, "OBSERVER"));
        }
    }
    return false;
}

bool BaseTransaction::enableScheduleBroadcast()
{
    return false;
}

bool BaseTransaction::enableCancelTransaction()
{
    if(AppModel::instance()->walletInfo() && AppModel::instance()->walletInfo()->isAssistedWallet()){
        QString role = AppModel::instance()->walletInfo()->dashboard() ? AppModel::instance()->walletInfo()->dashboard()->myRole() : "";
        if(role == ""){
            return false;
        }
        else{
            return !(qUtils::strCompare(role, "OBSERVER"));
        }
    }
    return true;
}

nunchuk::TxInput BaseTransaction::changeInfo() // rename
{
    return {m_transaction.get_txid(), static_cast<uint32_t>(m_transaction.get_change_index())};
}

QCoinTagsModel *BaseTransaction::changeCoinsTag()
{
    if (!m_changeCoinsTag) {
        m_changeCoinsTag = QCoinTagsModelPtr(new QCoinTagsModel());
    }
    // ==> Call API lib
    nunchuk::TxInput tx_input = changeInfo();
    std::vector<nunchuk::TxInput> tx_inputs = {tx_input};
    // Convert sang UTXT model
    auto inputs = bridge::nunchukGetOriginCoinsFromTxInputs(m_walletId, tx_inputs);
    QList<int> tags;
    for (nunchuk::UnspentOutput it : inputs) {
        if (it.get_txid() == tx_input.txid && it.get_vout() == tx_input.vout){
            for (auto tag: it.get_tags()) {
                tags.append(tag);
            }
        }
    }
    m_changeCoinsTag->clear();
    m_changeCoinsTag->refreshTags(tags, m_walletId, true);
    return m_changeCoinsTag.data();
}

QCoinTagsModel *BaseTransaction::parentCoinsTag()
{
    return m_parentCoinsTag.data();
}

bool BaseTransaction::hasChange() const {
    int index_change = m_transaction.get_change_index();
    if(index_change >= 0 && index_change < (int)m_transaction.get_outputs().size()) {
        return true;
    }
    else{
        return false;
    }
}

Destination *BaseTransaction::change() {
    if(!m_change){
        m_change = QDestinationPtr(new Destination());
    }
    int index_change = m_transaction.get_change_index();
    if(index_change >= 0 && index_change < (int)m_transaction.get_outputs().size()) {
        std::pair<std::string, int> change_ret = m_transaction.get_outputs().at(index_change);
        m_change.data()->setAddress(QString::fromStdString(change_ret.first));
        m_change.data()->setAmount(change_ret.second);
    }
    return m_change.data();
}

QString BaseTransaction::blocktimeDisplay() const
{
    if(0 >= m_transaction.get_blocktime()){
        return "--/--/----"; // There is no time
    }
    else{
        return QDateTime::fromTime_t(m_transaction.get_blocktime()).toString( "MM/dd/yyyy hh:mm AP");
    }
}

time_t BaseTransaction::blocktime() const
{
    return m_transaction.get_blocktime();
}

QString BaseTransaction::walletId() const
{
    return m_walletId;
}

void BaseTransaction::setWalletId(const QString &walletId)
{
    if(m_walletId != walletId){
        m_walletId = walletId;
        emit walletIdChanged();
    }
}

SingleSignerListModel *BaseTransaction::singleSignersAssigned() {
    QWalletPtr wallet = QWalletPtr(NULL);
    if (AppModel::instance()->walletList() && AppModel::instance()->isSignIn()) {
        wallet = AppModel::instance()->walletInfoPtr();
    } else {
        wallet = AppModel::instance()->walletList() ? AppModel::instance()->walletList()->getWalletById(walletId()) : QWalletPtr(NULL);
    }
    if (wallet) {
        setRoomId(wallet->roomId());
        int walletAddress_Type = wallet->walletAddressType();
        int wallet_Type = wallet->walletType();
        auto trans = dynamic_cast<Transaction*>(this);
        if (walletAddress_Type == (int)nunchuk::AddressType::TAPROOT && wallet_Type == (int)nunchuk::WalletType::MULTI_SIG) {
            return taprootKeysetSigners(wallet);
        }
        else if (wallet->keyPathActivated() && !trans->isScriptPath()) {
            return taprootMiniscriptSigners(wallet);
        }
        else {
            return normalWalletSigners(wallet);
        }
    }
    else {
        return (new SingleSignerListModel());
    }
}

SingleSignerListModel *BaseTransaction::taprootKeysetSigners(const QWalletPtr &wallet) {
    if (status() == (int)nunchuk::TransactionStatus::PENDING_SIGNATURES || status() == (int)nunchuk::TransactionStatus::READY_TO_BROADCAST) {
        std::vector<nunchuk::KeysetStatus> keyset_status = keysetStatus();
        if(wallet->singleSignersAssigned()){
            if(wallet->isSharedWallet() && wallet->singleSignersAssigned()->needSyncNunchukEmail()){
                wallet->syncCollabKeyname();
            }
            m_keysets = wallet->singleSignersAssigned()->cloneKeysets(keyset_status);
            if(m_keysets->needSyncNunchukEmail()){
                m_keysets = wallet->singleSignersAssigned()->cloneKeysets(keyset_status);
            }
        }
        if(m_keysets){
            if(keysetSelected() > 0) {
                m_keysets.data()->requestSortKeysetSelected();
            }
            else {
                m_keysets.data()->requestSortKeyset();
            }
        }
        if(m_keysets){
            emit m_keysets.data()->keyinfoChanged();
        }
        return m_keysets.data();
    }
    else {
        std::map<std::string, bool> final_signers = m_transaction.get_signers();
        if (wallet->singleSignersAssigned()) {
            if (wallet->isSharedWallet() && wallet->singleSignersAssigned()->needSyncNunchukEmail()) {
                wallet->syncCollabKeyname();
            }
            m_signers = wallet->singleSignersAssigned()->cloneFinalSigners(final_signers);
            if (m_signers->needSyncNunchukEmail()) {
                m_signers = wallet->singleSignersAssigned()->cloneFinalSigners(final_signers);
            }
        }
        return m_signers.data();
    }
}

SingleSignerListModel *BaseTransaction::taprootMiniscriptSigners(const QWalletPtr &wallet) {
    if (wallet->singleSignersKeyPath()) {
        std::map<std::string, bool> final_signers = m_transaction.get_signers();
        m_signers = wallet->singleSignersKeyPath()->cloneFinalSigners(final_signers);
    }
    return m_signers.data();
}

SingleSignerListModel *BaseTransaction::normalWalletSigners(const QWalletPtr &wallet) {
    if(wallet->singleSignersAssigned()){
        if(wallet->isSharedWallet() && wallet->singleSignersAssigned()->needSyncNunchukEmail()){
            wallet->syncCollabKeyname();
        }
        if(m_signers.data()->rowCount() == 0){
            m_signers = wallet->singleSignersAssigned()->clone();
        }
        else{
            if(m_signers->needSyncNunchukEmail()){
                m_signers = wallet->singleSignersAssigned()->clone();
            }
        }
    }
    if(m_signers){
        m_signers.data()->initSignatures();
        std::map<std::string, bool> signers = m_transaction.get_signers();
        for ( std::map<std::string, bool>::iterator it = signers.begin(); it != signers.end(); it++ ){
            m_signers.data()->updateSignatures(QString::fromStdString(it->first), it->second, "");
        }
        updateSignaturesForDummyTx();
        m_signers.data()->requestSort(true);
        return m_signers.data();
    }
    else {
        return (new SingleSignerListModel());
    }
}

SingleSignerListModel *BaseTransaction::allFinalSigners() {
    QWalletPtr wallet = QWalletPtr(NULL);
    if (AppModel::instance()->walletList() && AppModel::instance()->isSignIn()) {
        wallet = AppModel::instance()->walletInfoPtr();
    } else {
        wallet = AppModel::instance()->walletList() ? AppModel::instance()->walletList()->getWalletById(walletId()) : QWalletPtr(NULL);
    }
    if (wallet) {
        std::map<std::string, bool> final_signers = m_transaction.get_signers();
        if (wallet->singleSignersAssigned()) {
            m_signers = wallet->singleSignersAssigned()->cloneFinalSigners(final_signers);
        }
        updateSignaturesForDummyTx();
        return m_signers.data();
    } else {
        return (new SingleSignerListModel());
    }
}

void BaseTransaction::updateSignaturesForDummyTx() {
    if (m_signers.isNull()) return;
    DBG_INFO << isDummyTx() << m_txJson.isEmpty() << m_signers->rowCount();
    if (!m_txJson.isEmpty() || isDummyTx()) {// Use for dummy transaction
        if (!m_txJson.isEmpty()) {
            QJsonArray signatures = m_txJson["signatures"].toArray();
            for (auto js : signatures) {
                QString xfp = js.toObject()["xfp"].toString();
                QString signature = js.toObject()["signature"].toString().split(".").at(1);
                m_signers.data()->updateSignatures(xfp, true, signature);
            }
        } else if (m_signatures.size() > 0) {
            for (auto xfp : m_signatures.uniqueKeys()) {
                QString signature = m_signatures.value(xfp);
                m_signers.data()->updateSignatures(xfp, true, signature);
            }
        }
        for (int i = 0; i < m_signers->rowCount(); i++) {
            auto signer = m_signers->getSingleSignerByIndex(i);
            signer->setHasSignBtn(true);
            if (signer->signerType() == (int)ENUNCHUCK::SignerType::SERVER) {
                m_signers->removeSingleSignerByIndex(i);
                --i;
            }
            if (!dummyXfp().isEmpty()) {
                if (dummyXfp() != signer->masterFingerPrint() ) {
                    signer->setHasSignBtn(false);
                }
            }
            if (hideSignBtns().contains(signer->masterFingerPrint())) {
                signer->setHasSignBtn(false);
            }
        }
    }
}

int BaseTransaction::numberSigned()
{
    int number_signed = 0;
    std::map<std::string, bool> signers = m_transaction.get_signers();
    for ( std::map<std::string, bool>::iterator it = signers.begin(); it != signers.end(); it++ ){
        if(it->second) { number_signed++; }
    }

    if(AppModel::instance()->walletInfo() && AppModel::instance()->walletInfo()->utxoList()){
        for (auto &item : m_transaction.get_inputs()) {
            AppModel::instance()->walletInfo()->utxoList()->updateSelected(QString::fromStdString(item.txid), item.vout);
        }
    }

    DBG_INFO << number_signed;
    return number_signed;
}

int BaseTransaction::pendingSignatures()
{
    int pending_count = max(0, (m() - numberSigned()));
    return pending_count;
}

QString BaseTransaction::subtotalCurrency() const
{
    return qUtils::currencyLocale(subtotalSats());
}

QString BaseTransaction::subtotalDisplay() const
{
    if((int)AppSetting::Unit::SATOSHI == AppSetting::instance()->unit()){
        QLocale locale(QLocale::English);
        return locale.toString(subtotalSats());
    }
    else{
        return subtotalBTC();
    }
}

qint64 BaseTransaction::subtotalSats() const
{
    return (qint64)m_transaction.get_sub_amount();
}

QString BaseTransaction::subtotalBTC() const
{
    return qUtils::QValueFromAmount(subtotalSats());
}

QString BaseTransaction::totalCurrency() const
{
    return qUtils::currencyLocale(totalSats());
}

QString BaseTransaction::totalDisplay() const
{
    if((int)AppSetting::Unit::SATOSHI == AppSetting::instance()->unit()){
        QLocale locale(QLocale::English);
        return locale.toString(totalSats());
    }
    else{
        return totalBTC();
    }
}

qint64 BaseTransaction::totalSats() const {
    qint64 subtotal = subtotalSats();
    qint64 fee = feeSats();

    if ((fee > 0 && subtotal > INT64_MAX - fee) ||
        (fee < 0 && subtotal < INT64_MIN - fee)) {
        DBG_INFO << "Overflow detected!";
        return 0;
    }

    return subtotal + fee;
}

QString BaseTransaction::totalBTC() const
{
    return qUtils::QValueFromAmount(totalSats());
}

DestinationListModel *BaseTransaction::destinationList() {
    return m_destinations.data();
}

void BaseTransaction::createDestinationList() {
    if(m_destinations.isNull()) return;
    m_destinations.data()->clearAll();
    std::vector<nunchuk::TxOutput> addrs;
    if (isReceiveTx()) {
        addrs = m_transaction.get_receive_outputs();
        for (std::pair<std::string, nunchuk::Amount> item : addrs) {
            m_destinations.data()->addDestination(QString::fromStdString(item.first), item.second);
        }
    } else {
        addrs = m_transaction.get_outputs();
        int index_change = m_transaction.get_change_index();
        size_t maxSize = m_isClaimTx ? 1 : addrs.size();
        for (size_t i = 0; i < maxSize; i++) {
            if (index_change != i) {
                std::pair<std::string, nunchuk::Amount> item = addrs.at(i);
                m_destinations.data()->addDestination(QString::fromStdString(item.first), item.second);
            }
        }
    }
}

QString BaseTransaction::get_replaced_by_txid() const
{
    return QString::fromStdString(m_transaction.get_replaced_by_txid());
}

QString BaseTransaction::get_replace_txid()
{
    return QString::fromStdString(m_transaction.get_replace_txid());
}

nunchuk::Transaction BaseTransaction::nunchukTransaction() const
{
    return m_transaction;
}

void BaseTransaction::setNunchukTransaction(const nunchuk::Transaction &tx)
{
    DBG_INFO << "TAPROOT-TEST";
    m_transaction = tx;
    createDestinationList();
    emit nunchukTransactionChanged();
}

QString BaseTransaction::roomId()
{
    if(AppModel::instance()->walletList()){
        QWalletPtr wallet = AppModel::instance()->walletList()->getWalletById(walletId());
        if(wallet){
            m_roomId = wallet.data()->roomId();
        }
    }
    return m_roomId;
}

void BaseTransaction::setRoomId(const QString &roomId)
{
    if(m_roomId != roomId){
        m_roomId = roomId;
        emit roomIdChanged();
    }
}

QString BaseTransaction::initEventId() const
{
    return m_initEventId;
}

void BaseTransaction::setInitEventId(const QString &initEventId)
{
    if(m_initEventId != initEventId){
        m_initEventId = initEventId;
        emit initEventIdChanged();
    }
}

bool BaseTransaction::createByMe() const
{
    return m_createByMe;
}

void BaseTransaction::setCreateByMe(bool createByMe)
{
    if(m_createByMe != createByMe){
        m_createByMe = createByMe;
        emit createByMeChanged();
    }
}

QString BaseTransaction::serverKeyMessage() const
{
    return m_serverKeyMessage;
}

void BaseTransaction::setServerKeyMessage(const QJsonObject &transaction)
{
    if (status() == (int)ENUNCHUCK::TransactionStatus::PENDING_SIGNATURES) {
        QJsonObject spending_limit_reached  = transaction.value("spending_limit_reached").toObject();
        double time = transaction.value("sign_time_milis").toDouble();
        if (!spending_limit_reached.isEmpty()) {
            m_serverKeyMessage = spending_limit_reached.value("message").toString();
        }
        else if(time != 0) {
            m_serverKeyMessage = QString("Co-sign at %1").arg(QDateTime::fromMSecsSinceEpoch(time).toString( "hh:mm AP MMM d"));
        }
        else{}
        bool is_cosigning  = transaction.value("is_cosigning").toBool();
        DBG_INFO << "FIXME is_cosigning" << is_cosigning << m_serverKeyMessage << transaction;
        setIsCosigning(is_cosigning);
        emit serverKeyMessageChanged();
    } else {
        DBG_INFO << "FIXME Hide is_cosigning" << m_is_cosigning << m_serverKeyMessage;
        setIsCosigning(false);
    }
}

QString BaseTransaction::packageFeeRate()
{
    return QString::number((double)m_packageFeeRate/1000, 'f', 2);
}

void BaseTransaction::setPackageFeeRate(int satvKB)
{
    if (m_packageFeeRate == satvKB)
        return;

    m_packageFeeRate = satvKB;
    emit nunchukTransactionChanged();
}

QString BaseTransaction::destination()
{
    QString ret = "";
    std::vector<nunchuk::TxOutput> addrs;
    if(isReceiveTx()){
        addrs = m_transaction.get_receive_outputs();
        ret = addrs.size() > 0 ? QString::fromStdString(addrs.at(0).first) : "";
    }
    else{
        addrs = m_transaction.get_outputs();
        int index_change = m_transaction.get_change_index();
        if(index_change >= 0 && index_change < (int)m_transaction.get_outputs().size()) {
            addrs.erase(addrs.begin() + index_change);
        }
        ret = addrs.size() > 1 ? "Multiple addresses" : addrs.size() > 0 ? QString::fromStdString(addrs.at(0).first) : "";
    }
    return ret;
}

bool BaseTransaction::isCpfp()
{
    QWarningMessage msg;
    nunchuk::Amount packageFeeRate{0};
    bool ret = bridge::IsCPFP(walletId(), nunchukTransaction(), packageFeeRate, msg);
    if (msg.type() == (int)EWARNING::WarningType::NONE_MSG) {
        setPackageFeeRate(packageFeeRate);
    }
    DBG_INFO << ret << packageFeeRate;
    return ret;
}

int BaseTransaction::keysetSelected()
{
    QString tx_id = txid();
    if(m_keysetSelected == -1){
        m_keysetSelected = AppSetting::instance()->getTransactionKeysetIndex(tx_id);
    }
    return m_keysetSelected;
}

void BaseTransaction::setKeysetSelected(int index, bool cached)
{
    if (m_keysetSelected == index)
        return;
    m_keysetSelected = index;
    if(cached){
        QString tx_id = txid();
        AppSetting::instance()->setTransactionKeysetIndex(tx_id, m_keysetSelected);
    }
    emit keysetSelectedChanged();
}

int BaseTransaction::addressType() const {
    return static_cast<int>(m_transaction.get_address_type());
}

int BaseTransaction::walletType() const {
    return static_cast<int>(m_transaction.get_wallet_type());
}

void BaseTransaction::refreshScanDevices()
{
    emit nunchukTransactionChanged();
}
