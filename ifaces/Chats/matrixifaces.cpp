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
#include "matrixifaces.h"
#include "matrixlistener.h"
#include "QOutlog.h"
#include "Chats/ClientController.h"

matrixifaces::matrixifaces()
{

}

matrixifaces::~matrixifaces()
{

}

matrixifaces *matrixifaces::instance()
{
    static matrixifaces mInstance;
    return &mInstance;
}

void matrixifaces::makeMatrixInstance(const nunchuk::AppSettings &appsettings,
                                      const std::string &passphrase,
                                      const std::string &account,
                                      const std::string& device_id,
                                      QWarningMessage &msg)
{
    matrix_instance = NULL;
    try {
        matrix_instance = nunchuk::MakeNunchukMatrixForAccount(appsettings, passphrase, account, device_id, SendFunction);
        if(matrix_instance){
            this->RegisterFileFunc(UploadFileFunc, DownloadFileFunc);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
        matrix_instance = NULL;
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION " << e.what();
        matrix_instance = NULL;
    }
}

nunchuk::NunchukMatrixEvent matrixifaces::SendErrorEvent(const string &room_id, int code, const QString &message)
{
    nunchuk::NunchukMatrixEvent ret;
    if(ClientController::instance()->rooms()){
        if(ClientController::instance()->rooms()->allHisLoaded() == false){
            return ret;
        }
    }
    QWarningMessage msg;
    try {
        if(matrix_instance){
            ret = matrix_instance->SendErrorEvent(room_id, QSysInfo::productType().toStdString(), (QString("%1").arg(code)).toStdString(), message.toStdString());
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
        msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

nunchuk::NunchukMatrixEvent matrixifaces::InitWallet(const std::string &room_id,
                                                     const std::string &name,
                                                     int m,
                                                     int n,
                                                     nunchuk::AddressType address_type,
                                                     bool is_escrow,
                                                     const std::string &description, QWarningMessage &msg)
{
    nunchuk::NunchukMatrixEvent ret;
    try {
        if(matrix_instance){
            ret = matrix_instance->InitWallet(room_id, name, m, n, address_type, is_escrow, description);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
        SendErrorEvent(room_id,ex.code(),ex.what());
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
        msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

nunchuk::NunchukMatrixEvent matrixifaces::ImportWallet(const string &room_id, const string &name, int m, int n, nunchuk::AddressType address_type, bool is_escrow, const string &description, const std::vector<nunchuk::SingleSigner> &signers, QWarningMessage &msg)
{
    nunchuk::NunchukMatrixEvent ret;
    try {
        if(matrix_instance){
            ret = matrix_instance->InitWallet(room_id, name, m, n, address_type, is_escrow, description,signers);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
        SendErrorEvent(room_id,ex.code(),ex.what());
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
        msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

nunchuk::NunchukMatrixEvent matrixifaces::JoinWallet(const std::string &room_id,
                                                     const nunchuk::SingleSigner &signer,
                                                     QWarningMessage &msg)
{
    nunchuk::NunchukMatrixEvent ret;
    try {
        if(matrix_instance){
            ret = matrix_instance->JoinWallet(room_id, signer);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
        SendErrorEvent(room_id,ex.code(),ex.what());
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
        msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

nunchuk::NunchukMatrixEvent matrixifaces::LeaveWallet(const std::string &room_id,
                                                      const std::string &join_id,
                                                      const std::string &reason,
                                                      QWarningMessage &msg)
{
    nunchuk::NunchukMatrixEvent ret;
    try {
        if(matrix_instance){
            ret = matrix_instance->LeaveWallet(room_id, join_id, reason);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
        SendErrorEvent(room_id,ex.code(),ex.what());
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
        msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

nunchuk::NunchukMatrixEvent matrixifaces::CancelWallet(const std::string &room_id,
                                                       const std::string &reason,
                                                       QWarningMessage &msg)
{
    nunchuk::NunchukMatrixEvent ret;
    try {
        if(matrix_instance){
            ret = matrix_instance->CancelWallet(room_id, reason);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
        SendErrorEvent(room_id,ex.code(),ex.what());
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
        msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

nunchuk::NunchukMatrixEvent matrixifaces::CreateWallet(const std::unique_ptr<nunchuk::Nunchuk> &nu,
                                                       const std::string &room_id,
                                                       QWarningMessage &msg)
{
    nunchuk::NunchukMatrixEvent ret;
    try {
        if(matrix_instance){
            ret = matrix_instance->CreateWallet(nu, room_id);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
        SendErrorEvent(room_id,ex.code(),ex.what());
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
        msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

nunchuk::NunchukMatrixEvent matrixifaces::DeleteWallet(const std::unique_ptr<nunchuk::Nunchuk> &nu, const string &room_id, QWarningMessage &msg)
{
    nunchuk::NunchukMatrixEvent ret;
    try {
        if(matrix_instance){
            ret = matrix_instance->DeleteWallet(nu, room_id);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
        SendErrorEvent(room_id,ex.code(),ex.what());
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
        msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

nunchuk::NunchukMatrixEvent matrixifaces::InitTransaction(const std::unique_ptr<nunchuk::Nunchuk>& nu,
                                                          const std::string& room_id,
                                                          const std::map<std::string, nunchuk::Amount> outputs,
                                                          const std::string& memo,
                                                          const std::vector<nunchuk::UnspentOutput> inputs,
                                                          nunchuk::Amount fee_rate,
                                                          bool subtract_fee_from_amount,
                                                          QWarningMessage& msg)
{
    nunchuk::NunchukMatrixEvent ret;
    try {
        if(matrix_instance){
            ret = matrix_instance->InitTransaction(nu, room_id, outputs, memo, inputs, fee_rate, subtract_fee_from_amount);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
        SendErrorEvent(room_id,ex.code(),ex.what());
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
        msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

nunchuk::NunchukMatrixEvent matrixifaces::SignTransaction(const std::string& room_id,
                                                          const std::unique_ptr<nunchuk::Nunchuk>& nu,
                                                          const std::string& init_event_id,
                                                          const nunchuk::Device& device,
                                                          QWarningMessage &msg)
{
    nunchuk::NunchukMatrixEvent ret;
    try {
        if(matrix_instance){
            ret = matrix_instance->SignTransaction(nu, init_event_id, device);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
        SendErrorEvent(room_id,ex.code(),ex.what());
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
        msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

nunchuk::NunchukMatrixEvent matrixifaces::SignAirgapTransaction(const std::unique_ptr<nunchuk::Nunchuk> &nu,
                                                                const string &init_event_id,
                                                                const string &master_fingerprint,
                                                                QWarningMessage &msg)
{
    nunchuk::NunchukMatrixEvent ret;
    try {
        if(matrix_instance){
            ret = matrix_instance->SignAirgapTransaction(nu, init_event_id, master_fingerprint);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
        msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

nunchuk::NunchukMatrixEvent matrixifaces::RejectTransaction(const std::string &room_id,
                                                            const std::string &init_id,
                                                            const std::string &reason,
                                                            QWarningMessage &msg)
{
    nunchuk::NunchukMatrixEvent ret;
    try {
        if(matrix_instance){
            ret = matrix_instance->RejectTransaction(init_id, reason);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
        SendErrorEvent(room_id,ex.code(),ex.what());
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
        msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

nunchuk::NunchukMatrixEvent matrixifaces::CancelTransaction(const std::string &room_id,
                                                            const std::string &init_id,
                                                            const std::string &reason,
                                                            QWarningMessage &msg)
{
    nunchuk::NunchukMatrixEvent ret;
    try {
        if(matrix_instance){
            ret = matrix_instance->CancelTransaction(init_id, reason);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
        SendErrorEvent(room_id,ex.code(),ex.what());
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
        msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

nunchuk::NunchukMatrixEvent matrixifaces::BroadcastTransaction(const std::string &room_id,
                                                               const std::unique_ptr<nunchuk::Nunchuk>& nu,
                                                               const std::string& init_event_id,
                                                               QWarningMessage &msg)
{
    nunchuk::NunchukMatrixEvent ret;
    try {
        if(matrix_instance){
            ret = matrix_instance->BroadcastTransaction(nu, init_event_id);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
        SendErrorEvent(room_id,ex.code(),ex.what());
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
        msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

nunchuk::RoomTransaction matrixifaces::GetRoomTransaction(const std::string &room_id,const string &init_event_id, QWarningMessage &msg)
{
    nunchuk::RoomTransaction ret;
    try {
        if(matrix_instance){
            ret = matrix_instance->GetRoomTransaction(init_event_id);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
        SendErrorEvent(room_id,ex.code(),ex.what());
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
        msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

string matrixifaces::GetTransactionId(const std::string &room_id,const string &event_id, QWarningMessage &msg)
{
    std::string ret = "";
    try {
        if(matrix_instance){
            ret = matrix_instance->GetTransactionId(event_id);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
        SendErrorEvent(room_id,ex.code(),ex.what());
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
        msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

bool matrixifaces::HasRoomWallet(const string &room_id, QWarningMessage &msg)
{
    bool ret = false;
    try {
        if(matrix_instance){
            ret = matrix_instance->HasRoomWallet(room_id);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
        SendErrorEvent(room_id,ex.code(),ex.what());
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
        msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

std::vector<nunchuk::RoomWallet> matrixifaces::GetAllRoomWallets(QWarningMessage &msg)
{
    std::vector<nunchuk::RoomWallet> ret;
    try {
        if(matrix_instance){
            ret = matrix_instance->GetAllRoomWallets();
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
        msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

nunchuk::RoomWallet matrixifaces::GetRoomWallet(const std::string &room_id,
                                                QWarningMessage &msg)
{
    nunchuk::RoomWallet ret;
    try {
        if(matrix_instance){
            ret = matrix_instance->GetRoomWallet(room_id);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
        SendErrorEvent(room_id,ex.code(),ex.what());
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
        msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

std::vector<nunchuk::RoomTransaction> matrixifaces::GetPendingTransactions(const std::string &room_id,
                                                                           QWarningMessage &msg)
{
    std::vector<nunchuk::RoomTransaction> ret;
    try {
        if(matrix_instance){
            ret = matrix_instance->GetPendingTransactions(room_id);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
        SendErrorEvent(room_id,ex.code(),ex.what());
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
        msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

nunchuk::NunchukMatrixEvent matrixifaces::GetEvent(const std::string& room_id,
                                                   const string &event_id,
                                                   QWarningMessage &msg)
{
    nunchuk::NunchukMatrixEvent ret;
    try {
        if(matrix_instance){
            ret = matrix_instance->GetEvent(event_id);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
        SendErrorEvent(room_id,ex.code(),ex.what());
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
        msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

void matrixifaces::ConsumeEvent(const std::string& room_id,
                                const std::unique_ptr<nunchuk::Nunchuk> &nu,
                                const nunchuk::NunchukMatrixEvent &event,
                                QWarningMessage &msg)
{
    try {
        if(matrix_instance){
            matrix_instance->ConsumeEvent(nu, event);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
        SendErrorEvent(room_id,ex.code(),ex.what());
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
        msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
}

void matrixifaces::ConsumeSyncEvent(const std::string& room_id,
                                    const std::unique_ptr<nunchuk::Nunchuk> &nu,
                                    const nunchuk::NunchukMatrixEvent &event,
                                    QWarningMessage &msg)
{
    try {
        if(matrix_instance){
            matrix_instance->ConsumeSyncEvent(nu, event, consume_sync_event_listener);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
        SendErrorEvent(room_id,ex.code(),ex.what());
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
        msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
}

void matrixifaces::RegisterAutoBackup(const std::unique_ptr<nunchuk::Nunchuk> &nu,
                                    const string &sync_room_id,
                                    const string &access_token,
                                    QWarningMessage &msg)
{
    try {
        if(matrix_instance){
            matrix_instance->RegisterAutoBackup(nu, sync_room_id, access_token);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
        SendErrorEvent(sync_room_id,ex.code(),ex.what());
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
        msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
}

void matrixifaces::EnableGenerateReceiveEvent(const std::unique_ptr<nunchuk::Nunchuk> &nu, QWarningMessage &msg)
{
    try {
        if(matrix_instance){
            matrix_instance->EnableGenerateReceiveEvent(nu);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
        msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
}

void matrixifaces::RegisterFileFunc(nunchuk::UploadFileFunc upload, nunchuk::DownloadFileFunc download)
{
    try {
        if(matrix_instance){
            matrix_instance->RegisterFileFunc(upload, download);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
    }
}

nunchuk::NunchukMatrixEvent matrixifaces::UploadFileCallback(const string &json_info,
                                                             const string &file_url)
{
    nunchuk::NunchukMatrixEvent ret;
    try {
        if(matrix_instance){
            ret = matrix_instance->UploadFileCallback(json_info, file_url);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
    }
    return ret;
}

void matrixifaces::DownloadFileCallback(const std::unique_ptr<nunchuk::Nunchuk> &nu,
                                        const string &json_info,
                                        const std::vector<unsigned char> &file_data,
                                        std::function<bool (int)> progress)
{
    try {
        if(matrix_instance){
            matrix_instance->DownloadFileCallback(nu, json_info, file_data, progress);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
    }
}

void matrixifaces::EnableAutoBackup(bool state, QWarningMessage &msg)
{
    try {
        if(matrix_instance){
            matrix_instance->EnableAutoBackup(state);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
        msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
}

void matrixifaces::Backup(const std::unique_ptr<nunchuk::Nunchuk> &nu, QWarningMessage &msg)
{
    try {
        if(matrix_instance){
            matrix_instance->Backup(nu);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
        msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
}
