#ifndef MATRIXBRIGDE_H
#define MATRIXBRIGDE_H
#include <room.h>
#include <connection.h>
#include <ssosession.h>
#include <user.h>
#include <uriresolver.h>
#include <events/encryptionevent.h>
#include <events/redactionevent.h>
#include <events/roomavatarevent.h>
#include <events/roomcreateevent.h>
#include <events/roommemberevent.h>
#include <events/roommessageevent.h>
#include <events/roomcanonicalaliasevent.h>
#include <events/simplestateevents.h>
#include "matrixifaces.h"
#include "bridgeifaces.h"
#include "Chats/QNunchukMatrixEvent.h"
#include "Chats/QRoomTransaction.h"
#include "Chats/QRoomWallet.h"
#include "Chats/QNunchukRoomModel.h"
#include "SingleSignerModel.h"
#include "TransactionModel.h"
#include "QWarningMessage.h"

namespace matrixbrigde {

static QMap<QString, int> signerTypeToInt {
    { "HARDWARE",           (int)ENUNCHUCK::SignerType::HARDWARE        },
    { "AIRGAP",             (int)ENUNCHUCK::SignerType::AIRGAP          },
    { "SOFTWARE",           (int)ENUNCHUCK::SignerType::SOFTWARE        },
    { "FOREIGN_SOFTWARE",   (int)ENUNCHUCK::SignerType::FOREIGN_SOFTWARE},
};

QJsonObject stringToJson(const QString& content);

void makeMatrixInstance(const QString &account,
                        const QString &device_id,
                        QWarningMessage &msg);

QNunchukMatrixEvent InitWallet(const QString& room_id,
                               const QString& name,
                               int m,
                               int n,
                               ENUNCHUCK::AddressType address_type,
                               bool is_escrow,
                               const QString& description,
                               QWarningMessage& msg);

QNunchukMatrixEvent JoinWallet(const QString& room_id,
                               const QSingleSignerPtr &signer,
                               QWarningMessage& msg);

QNunchukMatrixEvent LeaveWallet(const QString& room_id,
                                const QString& join_id,
                                const QString& reason,
                                QWarningMessage& msg);

QNunchukMatrixEvent CancelWallet(const QString& room_id,
                                 const QString& reason,
                                 QWarningMessage& msg);

QNunchukMatrixEvent CreateWallet(const QString& room_id,
                                 QWarningMessage& msg);

QNunchukMatrixEvent DeleteWallet(const QString& room_id,
                                 QWarningMessage& msg);

QNunchukMatrixEvent InitTransaction(const QString& room_id,
                                    const QMap<QString, qint64> outputs,
                                    const QString &memo,
                                    const QUTXOListModelPtr inputs,
                                    const int fee_rate,
                                    bool subtract_fee_from_amount,
                                    QWarningMessage& msg);

QNunchukMatrixEvent SignTransaction(const QString &room_id,
                                    const QString& init_event_id,
                                    const QDevicePtr &device,
                                    QWarningMessage& msg);

QNunchukMatrixEvent SignAirgapTransaction(const QString &init_event_id,
                                          const QString &master_fingerprint,
                                          QWarningMessage &msg);

QNunchukMatrixEvent RejectTransaction(const QString &room_id,
                                      const QString& init_event_id,
                                      const QString& reason,
                                      QWarningMessage& msg);

QNunchukMatrixEvent CancelTransaction(const QString &room_id,
                                      const QString& init_event_id,
                                      const QString& reason,
                                      QWarningMessage& msg);

QNunchukMatrixEvent BroadcastTransaction(const QString &room_id,
                                         const QString& init_event_id,
                                         QWarningMessage& msg);

QRoomTransactionPtr GetRoomTransaction(const QString &room_id,
                                       const QString& init_event_id,
                                       QWarningMessage& msg);

nunchuk::RoomTransaction GetOriginRoomTransaction(const QString &room_id,
                                                  const QString& init_event_id,
                                                  QWarningMessage& msg);

QString GetTransactionId(const QString &room_id,
                         const QString& event_id,
                         QWarningMessage& msg);

QRoomWalletPtr ReloadRoomWallet(QNunchukRoom * const room);

QList<QRoomWalletPtr> GetAllRoomWallets(QWarningMessage& msg);

QRoomWalletPtr GetRoomWallet(const QString& room_id,
                             QWarningMessage& msg);

QRoomTransactionModelPtr GetPendingTransactions(const QString& room_id);

QNunchukMatrixEvent GetEvent(const QString& room_id,
                             const QString& event_id,
                             QWarningMessage& msg);

void ConsumeEvent(const QString& room_id,const QNunchukMatrixEvent& event);

void ConsumeSyncEvent(const QString& room_id,const QNunchukMatrixEvent& event);

void RegisterAutoBackup(const QString &sync_room_id,
                        const QString &access_token);

void EnableGenerateReceiveEvent();

bool HasRoomWallet(const QString& room_id);

void UploadFileCallback(const QString& json_info,
                        const QString& file_url);

void DownloadFileCallback(const QString& json_info,
                          const std::vector<unsigned char> &file_data);

void EnableAutoBackup(bool state);

void Backup();

QNunchukMatrixEvent ImportWallet(const QString& room_id,
                                 const QString& name,
                                 const QString& description,
                                 const QString& filepath,
                                 QWarningMessage& msg);

QNunchukMatrixEvent ImportWalletQR(const QString &room_id,
                                   const QString &name,
                                   const QString &description,
                                   nunchuk::Chain chain,
                                   const QStringList qrtags,
                                   QWarningMessage& msg);
};

#endif // MATRIXBRIGDE_H
