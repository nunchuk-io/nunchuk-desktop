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

#ifndef TRANSACTIONLISTMODEL_H
#define TRANSACTIONLISTMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include "SingleSignerModel.h"
#include "QOutlog.h"
#include <nunchuk.h>
#include <QTimer>
#include "UTXOModel.h"

class Wallet;

class Destination : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString address          READ address        NOTIFY addressChanged)
    Q_PROPERTY(QString amount           READ amountDisplay  NOTIFY amountChanged)
    Q_PROPERTY(QString amountBTC        READ amountBTC      NOTIFY amountChanged)
    Q_PROPERTY(QString amountCurrency   READ amountCurrency NOTIFY amountChanged)
public:
    Destination();
    Destination(const QString &address, const qint64 amount);
    ~Destination();
    QString amountDisplay() const;
    QString amountBTC() const;
    QString amountCurrency() const;
    qint64 amountSats() const;
    void setAmount(const qint64 value);

    QString address() const;
    void setAddress(const QString & value);
private:
    QString address_;
    qint64 amount_;

signals:
    void addressChanged();
    void amountChanged();
};
typedef OurSharedPointer<Destination> QDestinationPtr;

class DestinationListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QString reciever READ reciever CONSTANT)
public:
    DestinationListModel();
    ~DestinationListModel();
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QHash<int,QByteArray> roleNames() const;
    void addDestination(const QString& address, const qint64 amount);
    bool contains(const QString &address, qint64 &amount);
    qint64 getAmountByAddress(const QString &address);
    QMap<QString, qint64> getOutputs() const;
    void notifyUnitChanged();
    enum DestinationRoles {
        destination_address_role,
        destination_amount_role,
        destination_amount_btc_role,
        destination_amount_currency_role
    };
    QString reciever();
    void clearAll();
private:
    QList<QDestinationPtr> m_data;
};
typedef OurSharedPointer<DestinationListModel> QDestinationListModelPtr;

class Transaction : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString                      txid                    READ txid                                               NOTIFY nunchukTransactionChanged)
    Q_PROPERTY(QString                      memo                    READ memo                                               NOTIFY nunchukTransactionChanged)
    Q_PROPERTY(int                          height                  READ height                                             NOTIFY nunchukTransactionChanged)
    Q_PROPERTY(int                          status                  READ status                                             NOTIFY nunchukTransactionChanged)
    Q_PROPERTY(int                          m                       READ m                                                  NOTIFY nunchukTransactionChanged)
    Q_PROPERTY(bool                         hasChange               READ hasChange                                          NOTIFY nunchukTransactionChanged)
    Q_PROPERTY(QString                      fee                     READ feeDisplay                                         NOTIFY nunchukTransactionChanged)
    Q_PROPERTY(QString                      feeBTC                  READ feeBTC                                             NOTIFY nunchukTransactionChanged)
    Q_PROPERTY(QString                      feeCurrency             READ feeCurrency                                        NOTIFY nunchukTransactionChanged)
    Q_PROPERTY(QString                      feeRate                 READ feeRate                                            NOTIFY nunchukTransactionChanged)
    Q_PROPERTY(Destination*                 change                  READ change                                             NOTIFY nunchukTransactionChanged)
    Q_PROPERTY(DestinationListModel*        destinationList         READ destinationList                                    NOTIFY nunchukTransactionChanged)
    Q_PROPERTY(SingleSignerListModel*       singleSignersAssigned   READ singleSignersAssigned                              NOTIFY nunchukTransactionChanged)
    Q_PROPERTY(QString                      subtotal                READ subtotalDisplay                                    NOTIFY nunchukTransactionChanged)
    Q_PROPERTY(QString                      subtotalCurrency        READ subtotalCurrency                                   NOTIFY nunchukTransactionChanged)
    Q_PROPERTY(QString                      total                   READ totalDisplay                                       NOTIFY nunchukTransactionChanged)
    Q_PROPERTY(QString                      totalCurrency           READ totalCurrency                                      NOTIFY nunchukTransactionChanged)
    Q_PROPERTY(int                          numberSigned            READ numberSigned                                       NOTIFY nunchukTransactionChanged)
    Q_PROPERTY(QString                      blocktime               READ blocktimeDisplay                                   NOTIFY nunchukTransactionChanged)
    Q_PROPERTY(bool                         isReceiveTx             READ isReceiveTx                                        NOTIFY nunchukTransactionChanged)
    Q_PROPERTY(bool                         subtractFromFeeAmount   READ subtractFromFeeAmount                              NOTIFY nunchukTransactionChanged)
    Q_PROPERTY(QString                      replacedByTxid          READ get_replaced_by_txid                               NOTIFY replacedTxidChanged)
    Q_PROPERTY(QString                      replaceTxid             READ get_replace_txid                                   NOTIFY replacedTxidChanged)
    Q_PROPERTY(QString                      roomId                  READ roomId                                             NOTIFY roomIdChanged)
    Q_PROPERTY(QString                      initEventId             READ initEventId                                        NOTIFY initEventIdChanged)
    Q_PROPERTY(bool                         createByMe              READ createByMe                                         NOTIFY createByMeChanged)
    Q_PROPERTY(QString                      psbt                    READ psbt                                               NOTIFY psbtChanged)
    Q_PROPERTY(QString                      serverKeyMessage        READ serverKeyMessage                                   NOTIFY serverKeyMessageChanged)
    Q_PROPERTY(QString                      destination             READ destination                                        NOTIFY nunchukTransactionChanged)
    Q_PROPERTY(bool                         isCosigning             READ isCosigning                                        NOTIFY serverKeyMessageChanged)
    Q_PROPERTY(bool                         enableRequestSignature  READ enableRequestSignature                             CONSTANT)
    Q_PROPERTY(bool                         enableScheduleBroadcast READ enableScheduleBroadcast                            CONSTANT)
    Q_PROPERTY(bool                         enableCancelTransaction READ enableCancelTransaction                            CONSTANT)
    Q_PROPERTY(bool                         hasMoreBtn              READ hasMoreBtn                                         NOTIFY hasMoreBtnChanged)
    Q_PROPERTY(QCoinTagsModel*              changeCoinsTag          READ changeCoinsTag                                     CONSTANT)
    Q_PROPERTY(QUTXOListModel*              manualCoins             READ manualCoins                                        NOTIFY manualCoinsChanged)
    Q_PROPERTY(QUTXOListModel*              inputCoins              READ inputCoins                                         CONSTANT)
    Q_PROPERTY(QCoinTagsModel*              parentCoinsTag          READ parentCoinsTag                                     NOTIFY parentCoinsTagChanged)
    Q_PROPERTY(int                          keysetsCount            READ keysetsCount                                       NOTIFY nunchukTransactionChanged)
    Q_PROPERTY(bool                         isCpfp                  READ isCpfp                                             NOTIFY nunchukTransactionChanged)
    Q_PROPERTY(QString                      packageFeeRate          READ packageFeeRate                                     NOTIFY nunchukTransactionChanged)
    Q_PROPERTY(QString                      scriptPathFeeRate       READ scriptPathFeeRate                                  NOTIFY nunchukTransactionChanged)
    Q_PROPERTY(QString                      txidReplacing           READ txidReplacing                                      NOTIFY txidReplacingChanged)
    Q_PROPERTY(bool                         useScriptPath           READ useScriptPath                                      NOTIFY useScriptPathChanged)
    Q_PROPERTY(QString                      feeOtherKeyset          READ feeOtherKeysetDisplay                              NOTIFY feeOtherKeysetChanged)
    Q_PROPERTY(QString                      feeOtherKeysetBTC       READ feeOtherKeysetBTC                                  NOTIFY feeOtherKeysetChanged)
    Q_PROPERTY(QString                      feeOtherKeysetCurrency  READ feeOtherKeysetCurrency                             NOTIFY feeOtherKeysetChanged)
    Q_PROPERTY(qint64                       feeOtherKeysetSats      READ feeOtherKeyset                                     NOTIFY feeOtherKeysetChanged)

public:
    Transaction();
    Transaction(const nunchuk::Transaction &tx);
    ~Transaction();

    bool isReceiveTx() const;
    void setIsReceiveTx(bool receive);

    QString txid() const;

    QString memo() const;
    void setMemo(const QString &memo);

    int status() const;
    void setStatus(int status);

    std::vector<nunchuk::KeysetStatus> const& keysetStatus() const;
    int keysetsCount();

    int m() const; // number of signatures required

    int height() const;
    void setHeight(const int value);

    QString feeDisplay() const;
    QString feeBTC() const;
    QString feeCurrency() const;
    qint64  feeSats() const;
    void    setFee(const qint64 fee);

    bool subtractFromFeeAmount() const;

    QString feeRate() const;
    qint64  feeRateSats() const;

    QString psbt() const;

    QString subtotalCurrency() const;
    QString subtotalDisplay() const;
    qint64  subtotalSats() const;
    QString subtotalBTC() const;

    QString totalCurrency() const;
    QString totalDisplay() const;
    qint64  totalSats() const;
    QString totalBTC() const;

    DestinationListModel* destinationList();

    bool hasChange() const;

    Destination* change();

    QString blocktimeDisplay() const;
    time_t  blocktime() const;

    QString walletId() const;
    void setWalletId(const QString &walletId);

    SingleSignerListModel* singleSignersAssigned();
    int numberSigned();

    QString get_replaced_by_txid() const;
    QString get_replace_txid();
    void set_replace_txid(const QString &txid);

    nunchuk::Transaction nunchukTransaction() const;
    void setNunchukTransaction(const nunchuk::Transaction &tx);
    QString roomId();
    void setRoomId(const QString &roomId);
    QString initEventId() const;
    void setInitEventId(const QString &initEventId);
    bool createByMe() const;
    void setCreateByMe(bool createByMe);
    QString serverKeyMessage() const;
    void setServerKeyMessage(const QJsonObject &data);
    QString packageFeeRate() ;
    void setPackageFeeRate(int satvKB);

    QString destination();
    bool isCpfp();
    time_t scheduleTime();

    bool isCosigning() const;
    void setIsCosigning(bool is_cosigning);
    void setTxJson(const QJsonObject &txJs);
    void setSignatures(const QMap<QString, QString>& signatures);
    QMap<QString, QString> signatures() const;

    bool hasMoreBtn() const;
    void setHasMoreBtn(bool has);

    virtual QString dummyXfp() const;
    virtual bool isDummyTx() const;

    virtual QStringList hideSignBtns() const;

    bool enableRequestSignature();
    bool enableScheduleBroadcast();
    bool enableCancelTransaction();

    nunchuk::TxInput changeInfo();

    QCoinTagsModel* changeCoinsTag();
    QCoinTagsModel* parentCoinsTag();

    QUTXOListModel* inputCoins();
    QUTXOListModel* manualCoins();
    void createFilteringCoinInCoinSelection();

    QUTXOListModelPtr GetUtxoListSelected();

    // For taproot
    std::map<std::string, bool> taprootFinalSigners();

    QString scriptPathFeeRate();
    QString txidReplacing() const;
    void setTxidReplacing(const QString &id);

    bool isClaimTx() const;
    void setIsClaimTx(bool is_claim_tx);

    bool useScriptPath() const;
    void setUseScriptPath(bool newUseScriptPath);

    QString feeOtherKeysetDisplay() const;
    QString feeOtherKeysetBTC() const;
    QString feeOtherKeysetCurrency() const;
    qint64  feeOtherKeyset() const;
    void setFeeOtherKeyset(qint64 data);

public slots:
    bool parseQRTransaction(const QStringList& qrtags);
    void copyTransactionID();
    void requestSignatures(const QString& membership_id);
    void scheduleBroadcast();
    void cancelTransaction();
    bool hasDraftCoinChange();
    void createParentCoinTag();
protected:
    bool ImportQRTransaction(const QStringList& qrtags);

private:
    QDestinationListModelPtr    m_destinations;
    QSingleSignerListModelPtr   m_signers;
    QSingleSignerListModelPtr   m_keysets;
    QDestinationPtr             m_change;
    QUTXOListModelPtr           m_inputCoins;
    QUTXOListModelPtr           m_manualCoins;
    QCoinTagsModelPtr           m_changeCoinsTag;
    QCoinTagsModelPtr           m_parentCoinsTag;

    nunchuk::Transaction        m_transaction;
    QString                     m_walletId;
    QString                     m_roomId;
    QString                     m_initEventId;
    bool                        m_createByMe;
    QString                     m_serverKeyMessage;
    int                         m_packageFeeRate {0};
    bool                        m_is_cosigning {false};
    QJsonObject                 m_txJson = {};
    QMap<QString, QString>      m_signatures = {};
    bool                        m_hasMoreBtn {true};
    QString                     m_txidReplacing {""};
    bool                        m_isClaimTx {false};
    bool                        m_useScriptPath {false};
    qint64                      m_fee_otherKeyset {0};

signals:
    void nunchukTransactionChanged();
    void isValidChanged();
    void replacedTxidChanged();
    void roomIdChanged();
    void initEventIdChanged();
    void createByMeChanged();
    void walletIdChanged();
    void psbtChanged();
    void serverKeyMessageChanged();
    void scan_percentChanged();
    void hasMoreBtnChanged();
    void manualCoinsChanged();
    void parentCoinsTagChanged();
    void txidReplacingChanged();
    void requestFeeSelection();
    void useScriptPathChanged();
    void feeOtherKeysetChanged();
};
typedef OurSharedPointer<Transaction> QTransactionPtr;

class TransactionListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int      count   READ count      NOTIFY countChanged)

public:
    TransactionListModel();
    ~TransactionListModel();

    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QHash<int,QByteArray> roleNames() const;
    QTransactionPtr getTransactionByIndex(const int index);
    QTransactionPtr getTransactionByTxid(const QString& txid);
    void requestSort(int role, int order);
    void addTransaction(const QTransactionPtr &d);
    void updateTransactionMemo(const QString &tx_id, const QString &memo);
    void updateTransaction(const QString &tx_id, const QTransactionPtr &tx);
    void updateTransaction(const QString &wallet_id, std::vector<nunchuk::Transaction> txs);
    void removeTransaction(const QString &tx_id);
    void notifyUnitChanged();
    void linkingReplacedTransactions();
    void cleardata();
    int  count() const;
    bool contains(const QString &tx_id);
    enum TransactionRoles {
        transaction_txid_role,
        transaction_memo_role,
        transaction_status_role,
        transaction_fee_role,
        transaction_m_role,
        transaction_hasChange_role,
        transaction_destinationList_role,
        transaction_destinationDisp_role,
        transaction_change_role,
        transaction_singleSignersAssigned_role,
        transaction_subtotal_role,
        transaction_total_role,
        transaction_numberSigned_role,
        transaction_blocktime_role,
        transaction_height_role,
        transaction_isReceiveTx_role,
        transaction_replacedTx_role,
        transaction_totalCurrency_role,
        transaction_subtotalCurrency_role,
        transaction_isRbf_role,
    };

signals:
    void countChanged();

private:
    QList<QTransactionPtr> m_data;
};
typedef OurSharedPointer<TransactionListModel> QTransactionListModelPtr;

// Sort Block time
bool sortTXsByBlocktimeAscending(const QTransactionPtr &v1, const QTransactionPtr &v2);
bool sortTXsByBlocktimeDescending(const QTransactionPtr &v1, const QTransactionPtr &v2);
bool sortTXsByBlocktimeDescendingSkipZero(const QTransactionPtr &v1, const QTransactionPtr &v2);

// Sort Amount
bool sortTXsByAmountAscending(const QTransactionPtr &v1, const QTransactionPtr &v2);
bool sortTXsByAmountDescending(const QTransactionPtr &v1, const QTransactionPtr &v2);

// Sort Memo
bool sortTXsByMemoAscending(const QTransactionPtr &v1, const QTransactionPtr &v2);
bool sortTXsByMemoDescending(const QTransactionPtr &v1, const QTransactionPtr &v2);

// Sort Status
bool sortTXsByStatusAscending(const QTransactionPtr &v1, const QTransactionPtr &v2);
bool sortTXsByStatusDescending(const QTransactionPtr &v1, const QTransactionPtr &v2);

#endif // TRANSACTIONLISTMODEL_H
