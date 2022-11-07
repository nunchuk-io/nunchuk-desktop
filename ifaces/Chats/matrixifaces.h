#ifndef NUNCHUKMATRIXIFACE_H
#define NUNCHUKMATRIXIFACE_H

#include <QObject>
#include "QWarningMessage.h"
#include "nunchukmatrix.h"

class matrixifaces
{
public:
    static matrixifaces *instance();
    void makeMatrixInstance(const nunchuk::AppSettings &appsettings,
                            const std::string &passphrase,
                            const std::string &account,
                            const std::string &device_id,
                            QWarningMessage &msg);
    matrixifaces(matrixifaces &other) = delete;
    matrixifaces(matrixifaces const &other) = delete;
    void operator=(const matrixifaces &other) = delete;

    nunchuk::NunchukMatrixEvent SendErrorEvent( const std::string& room_id,
                                                int code,
                                                const QString& message);

    nunchuk::NunchukMatrixEvent InitWallet( const std::string& room_id,
                                            const std::string& name,
                                            int m,
                                            int n,
                                            nunchuk::AddressType address_type,
                                            bool is_escrow,
                                            const std::string& description,
                                            QWarningMessage& msg);


    nunchuk::NunchukMatrixEvent ImportWallet( const std::string& room_id,
                                              const std::string& name,
                                              int m,
                                              int n,
                                              nunchuk::AddressType address_type,
                                              bool is_escrow,
                                              const std::string& description,
                                              const std::vector<nunchuk::SingleSigner>& signers,
                                              QWarningMessage& msg);

    nunchuk::NunchukMatrixEvent JoinWallet(const std::string& room_id,
                                           const nunchuk::SingleSigner& signer,
                                           QWarningMessage& msg);

    nunchuk::NunchukMatrixEvent LeaveWallet(const std::string& room_id,
                                            const std::string& join_id,
                                            const std::string& reason,
                                            QWarningMessage& msg);

    nunchuk::NunchukMatrixEvent CancelWallet(const std::string& room_id,
                                             const std::string& reason,
                                             QWarningMessage& msg);

    nunchuk::NunchukMatrixEvent CreateWallet(const std::unique_ptr<nunchuk::Nunchuk>& nu,
                                             const std::string& room_id,
                                             QWarningMessage& msg);

    nunchuk::NunchukMatrixEvent DeleteWallet(const std::unique_ptr<nunchuk::Nunchuk>& nu,
                                             const std::string& room_id,
                                             QWarningMessage& msg);

    nunchuk::NunchukMatrixEvent InitTransaction(const std::unique_ptr<nunchuk::Nunchuk>& nu,
                                                const std::string& room_id,
                                                const std::map<std::string, nunchuk::Amount> outputs,
                                                const std::string& memo,
                                                const std::vector<nunchuk::UnspentOutput> inputs,
                                                nunchuk::Amount fee_rate,
                                                bool subtract_fee_from_amount,
                                                QWarningMessage& msg);

    nunchuk::NunchukMatrixEvent SignTransaction(const std::string& room_id,
                                                const std::unique_ptr<nunchuk::Nunchuk> &nu,
                                                const std::string& init_event_id,
                                                const nunchuk::Device &device,
                                                QWarningMessage& msg);

    nunchuk::NunchukMatrixEvent SignAirgapTransaction(const std::unique_ptr<nunchuk::Nunchuk>& nu,
                                                      const std::string& init_event_id,
                                                      const std::string& master_fingerprint,
                                                      QWarningMessage &msg);

    nunchuk::NunchukMatrixEvent RejectTransaction( const std::string &room_id,
                                                   const std::string& init_id,
                                                   const std::string& reason,
                                                   QWarningMessage& msg);

    nunchuk::NunchukMatrixEvent CancelTransaction(const std::string &room_id,
                                                  const std::string& init_id,
                                                  const std::string& reason,
                                                  QWarningMessage& msg);

    nunchuk::NunchukMatrixEvent BroadcastTransaction(const std::string &room_id,
                                                     const std::unique_ptr<nunchuk::Nunchuk> &nu,
                                                     const std::string& init_event_id,
                                                     QWarningMessage& msg);

    nunchuk::RoomTransaction GetRoomTransaction(const std::string &room_id,
                                                const std::string& init_event_id,
                                                QWarningMessage& msg);

    std::string GetTransactionId(const std::string &room_id,
                                 const std::string& event_id,
                                 QWarningMessage& msg);

    bool HasRoomWallet(const std::string& room_id, QWarningMessage &msg);

    std::vector<nunchuk::RoomWallet> GetAllRoomWallets(QWarningMessage& msg);

    nunchuk::RoomWallet GetRoomWallet(const std::string& room_id,
                                      QWarningMessage& msg);

    std::vector<nunchuk::RoomTransaction> GetPendingTransactions(const std::string& room_id,
                                                                 QWarningMessage& msg);

    nunchuk::NunchukMatrixEvent GetEvent(const std::string& room_id,
                                         const std::string& event_id,
                                         QWarningMessage& msg);

    void ConsumeEvent(const std::string& room_id,
                      const std::unique_ptr<nunchuk::Nunchuk>& nu,
                      const nunchuk::NunchukMatrixEvent& event,
                      QWarningMessage& msg);

    void ConsumeSyncEvent(const std::string& room_id,
                          const std::unique_ptr<nunchuk::Nunchuk>& nu,
                          const nunchuk::NunchukMatrixEvent& event,
                          QWarningMessage &msg);

    void RegisterAutoBackup(const std::unique_ptr<nunchuk::Nunchuk>& nu,
                            const std::string& sync_room_id,
                            const std::string& access_token,
                            QWarningMessage &msg);

    void EnableGenerateReceiveEvent(const std::unique_ptr<nunchuk::Nunchuk>& nu,
                                    QWarningMessage &msg);

    void RegisterFileFunc(nunchuk::UploadFileFunc upload,
                          nunchuk::DownloadFileFunc download);

    nunchuk::NunchukMatrixEvent UploadFileCallback(const std::string& json_info,
                                                   const std::string& file_url);

    void DownloadFileCallback(const std::unique_ptr<nunchuk::Nunchuk>& nu,
                              const std::string& json_info,
                              const std::vector<unsigned char>& file_data,
                              std::function<bool /* stop */ (int /* percent */)> progress);

    void EnableAutoBackup(bool state,
                          QWarningMessage &msg);

    void Backup(const std::unique_ptr<nunchuk::Nunchuk> &nu,
                QWarningMessage &msg);
private:
    matrixifaces();
    ~matrixifaces();
    std::unique_ptr<nunchuk::NunchukMatrix> matrix_instance = NULL;
};

#endif // NUNCHUKMATRIXIFACE_H
