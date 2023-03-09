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
#include "nunchuckiface.h"
#include "nunchucklistener.h"
#include "QOutlog.h"
#include "utils/enumconverter.hpp"

nunchukiface::nunchukiface(): nunchukMode_(LOCAL_MODE){}

nunchukiface::~nunchukiface(){}

void nunchukiface::registerCallback()
{
//    AddBalanceListener(balance_listener);
    AddBalancesListener(balances_listener);
    AddDeviceListener(devices_listener);
    AddTransactionListener(transaction_listener);
    AddBlockListener(block_listener);
    AddBlockchainConnectionListener(blockchain_connection_listener);
}

nunchukiface *nunchukiface::instance() {
    static nunchukiface mInstance;
    return &mInstance;
}

void nunchukiface::setNunchukMode(int mode)
{
    nunchukMode_ = mode;
}

int nunchukiface::nunchukMode() const
{
    return nunchukMode_;
}

void nunchukiface::makeNunchukInstance(const nunchuk::AppSettings& appsettings,
                                       const std::string& passphrase,
                                       QWarningMessage& msg)
{
    nunchuk_instance_[LOCAL_MODE] = NULL;
    try {
        nunchuk_instance_[LOCAL_MODE] = nunchuk::MakeNunchuk(appsettings, passphrase);
        if(nunchuk_instance_[LOCAL_MODE]){
            this->registerCallback();
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
        nunchuk_instance_[LOCAL_MODE] = NULL;
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION " << e.what();
        nunchuk_instance_[LOCAL_MODE] = NULL;
    }
}

void nunchukiface::makeNunchukInstanceForAccount(const nunchuk::AppSettings &appsettings,
                                                 const std::string &passphrase,
                                                 const std::string &account,
                                                 QWarningMessage &msg)
{
    nunchuk_instance_[ONLINE_MODE] = NULL;
    try {
        nunchuk_instance_[ONLINE_MODE] = nunchuk::MakeNunchukForAccount(appsettings, passphrase, account);
        if(nunchuk_instance_[ONLINE_MODE]){
            this->registerCallback();
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
        nunchuk_instance_[ONLINE_MODE] = NULL;
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION " << e.what();
        nunchuk_instance_[ONLINE_MODE] = NULL;
    }
}

const std::unique_ptr<nunchuk::Nunchuk>& nunchukiface::nunchukinstance() const
{
    return nunchuk_instance_[nunchukMode()];
}

bool nunchukiface::SetPassphrase(const std::string &passphrase, QWarningMessage& msg){
    bool ret = false;
    try {
        if(nunchuk_instance_[nunchukMode()]){
            nunchuk_instance_[nunchukMode()]->SetPassphrase(passphrase);
            ret = true;
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
        ret = false;
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
        ret = false;
    }
    return ret;
}

nunchuk::Wallet nunchukiface::CreateWallet(const std::string &name,
                                           int m,
                                           int n,
                                           std::vector<nunchuk::SingleSigner> &signers,
                                           nunchuk::AddressType address_type,
                                           bool is_escrow,
                                           const std::string &desc,
                                           QWarningMessage& msg)
{
    nunchuk::Wallet ret(false);
    try {
        if(nunchuk_instance_[nunchukMode()]){
            ret = nunchuk_instance_[nunchukMode()]->CreateWallet(name, m, n, signers, address_type, is_escrow, desc);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

nunchuk::Wallet nunchukiface::CreateWallet(const nunchuk::Wallet &wallet, bool allow_used_signer, QWarningMessage &msg)
{
    nunchuk::Wallet ret(false);
    try {
        if(nunchuk_instance_[nunchukMode()]){
            ret = nunchuk_instance_[nunchukMode()]->CreateWallet(wallet,allow_used_signer);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

std::string nunchukiface::DraftWallet(const std::string &name,
                                      int m,
                                      int n,
                                      std::vector<nunchuk::SingleSigner> &signers,
                                      nunchuk::AddressType address_type,
                                      bool is_escrow,
                                      const std::string &desc,
                                      QWarningMessage &msg)
{
    std::string ret = "";
    try {
        if(nunchuk_instance_[nunchukMode()]){
            ret = nunchuk_instance_[nunchukMode()]->DraftWallet(name, m, n, signers, address_type, is_escrow, desc);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        ret = "";
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        ret = "";
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

std::vector<nunchuk::Wallet> nunchukiface::GetWallets(QWarningMessage& msg){

    std::vector<nunchuk::Wallet> ret;
    try {
        if(nunchuk_instance_[nunchukMode()]){
            ret = nunchuk_instance_[nunchukMode()]->GetWallets();
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

nunchuk::Wallet nunchukiface::GetWallet(const std::string &wallet_id, QWarningMessage& msg){
    nunchuk::Wallet ret(false);
    try {
        if(nunchuk_instance_[nunchukMode()]){
            ret = nunchuk_instance_[nunchukMode()]->GetWallet(wallet_id);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

bool nunchukiface::HasWallet(const std::string &wallet_id, QWarningMessage &msg)
{
    bool ret = false;
    try {
        if(nunchuk_instance_[nunchukMode()]){
            ret = nunchuk_instance_[nunchukMode()]->HasWallet(wallet_id);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

bool nunchukiface::DeleteWallet(const std::string &wallet_id, QWarningMessage &msg){
    bool ret = false;
    try {
        if(nunchuk_instance_[nunchukMode()]){
            ret = nunchuk_instance_[nunchukMode()]->DeleteWallet(wallet_id);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
        ret = false;
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
        ret = false;
    }
    return ret;
}

bool nunchukiface::UpdateWallet(const nunchuk::Wallet &wallet, QWarningMessage &msg){
    bool ret = false;
    try {
        if(nunchuk_instance_[nunchukMode()]){
            ret = nunchuk_instance_[nunchukMode()]->UpdateWallet(wallet);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
        ret = false;
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
        ret = false;
    }
    return ret;
}

bool nunchukiface::ExportWallet(const std::string &wallet_id, const std::string &file_path, nunchuk::ExportFormat format, QWarningMessage &msg){
    bool ret = false;
    try {
        if(nunchuk_instance_[nunchukMode()]){
            ret = nunchuk_instance_[nunchukMode()]->ExportWallet(wallet_id, file_path, format);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
        ret = false;
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
        ret = false;
    }
    return ret;
}

nunchuk::Wallet nunchukiface::ImportWalletDb(const std::string &file_path, QWarningMessage& msg){
    nunchuk::Wallet ret(false);
    try {
        if(nunchuk_instance_[nunchukMode()]){
            ret = nunchuk_instance_[nunchukMode()]->ImportWalletDb(file_path);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

nunchuk::Wallet nunchukiface::ImportWalletDescriptor(const std::string& file_path,
                                                     const std::string& name,
                                                     const std::string& description,
                                                     QWarningMessage& msg)
{
    nunchuk::Wallet ret(false);
    try {
        if(nunchuk_instance_[nunchukMode()]){
            ret = nunchuk_instance_[nunchukMode()]->ImportWalletDescriptor(file_path, name, description);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

std::vector<nunchuk::Device> nunchukiface::GetDevices(QWarningMessage& msg){
    std::vector<nunchuk::Device> ret;
    try {
        if(nunchuk_instance_[nunchukMode()]){
            ret = nunchuk_instance_[nunchukMode()]->GetDevices();
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

nunchuk::MasterSigner nunchukiface::CreateMasterSigner(const std::string &name, const nunchuk::Device &device, QWarningMessage& msg){
    nunchuk::MasterSigner ret("", nunchuk::Device(""), 0);
    try {
        if(nunchuk_instance_[nunchukMode()]){
            ret = nunchuk_instance_[nunchukMode()]->CreateMasterSigner(name, device, create_master_signer_listener);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

nunchuk::SingleSigner nunchukiface::GetSignerFromMasterSigner(const std::string &mastersigner_id,
                                                              const nunchuk::WalletType &wallet_type,
                                                              const nunchuk::AddressType &address_type,
                                                              int index,
                                                              QWarningMessage& msg)
{
    nunchuk::SingleSigner ret("","","","","",0,"");
    try {
        if(nunchuk_instance_[nunchukMode()]){
            ret = nunchuk_instance_[nunchukMode()]->GetSignerFromMasterSigner(mastersigner_id, wallet_type, address_type, index);
        }
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

nunchuk::SingleSigner nunchukiface::CreateSigner(const std::string &name,
                                                 const std::string &xpub,
                                                 const std::string &public_key,
                                                 const std::string &derivation_path,
                                                 const std::string &master_fingerprint,
                                                 const std::string& type,
                                                 QWarningMessage& msg)
{
    nunchuk::SingleSigner ret("","","","","",0,"");
    try {
        if(nunchuk_instance_[nunchukMode()]){
            ret = nunchuk_instance_[nunchukMode()]->CreateSigner(name, xpub, public_key, derivation_path, master_fingerprint,SignerTypeFromStr(type));
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

int nunchukiface::GetCurrentIndexFromMasterSigner(const std::string &mastersigner_id,
                                                  const nunchuk::WalletType &wallet_type,
                                                  const nunchuk::AddressType &address_type,
                                                  QWarningMessage &msg)
{
    int ret = -1;
    try {
        if(nunchuk_instance_[nunchukMode()]){
            ret = nunchuk_instance_[nunchukMode()]->GetCurrentIndexFromMasterSigner(mastersigner_id, wallet_type, address_type);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
        ret = false;
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
        ret = -1;
    }
    return ret;
}

nunchuk::SingleSigner nunchukiface::GetUnusedSignerFromMasterSigner(const std::string &mastersigner_id,
                                                                    const nunchuk::WalletType &wallet_type,
                                                                    const nunchuk::AddressType &address_type,
                                                                    QWarningMessage& msg)
{
    nunchuk::SingleSigner ret("","","","","",0,"");
    try {
        if(nunchuk_instance_[nunchukMode()]){
            ret = nunchuk_instance_[nunchukMode()]->GetUnusedSignerFromMasterSigner(mastersigner_id, wallet_type, address_type);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

std::vector<nunchuk::SingleSigner> nunchukiface::GetSignersFromMasterSigner(const std::string &mastersigner_id, QWarningMessage& msg)
{
    std::vector<nunchuk::SingleSigner> ret;
    try {
        if(nunchuk_instance_[nunchukMode()]){
            ret = nunchuk_instance_[nunchukMode()]->GetSignersFromMasterSigner(mastersigner_id);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

bool nunchukiface::HasSigner(const nunchuk::SingleSigner &signer,QWarningMessage& msg)
{
    bool ret = false;
    try {
        if(nunchuk_instance_[nunchukMode()]){
            ret = nunchuk_instance_[nunchukMode()]->HasSigner(signer);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
        ret = false;
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
        ret = false;
    }
    return ret;
}

void nunchukiface::AddTapsigner(const std::string &card_ident, const std::string &xfp, const std::string &name, const std::string &version, int birth_height, bool is_testnet, QWarningMessage& msg)
{
    try {
        if(nunchuk_instance_[nunchukMode()]){
            nunchuk_instance_[nunchukMode()]->AddTapsigner(card_ident,xfp,name,version,birth_height,is_testnet);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
}

int nunchukiface::GetNumberOfSignersFromMasterSigner(const std::string &mastersigner_id, QWarningMessage &msg)
{
    int ret = -1;
    try {
        if(nunchuk_instance_[nunchukMode()]){
            ret = nunchuk_instance_[nunchukMode()]->GetNumberOfSignersFromMasterSigner(mastersigner_id);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
        ret = false;
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
        ret = -1;
    }
    return ret;
}

std::vector<nunchuk::MasterSigner> nunchukiface::GetMasterSigners(QWarningMessage& msg)
{
    std::vector<nunchuk::MasterSigner> ret;
    try {
        if(nunchuk_instance_[nunchukMode()]){
            ret = nunchuk_instance_[nunchukMode()]->GetMasterSigners();
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

nunchuk::MasterSigner nunchukiface::GetMasterSigner(const std::string &mastersigner_id, QWarningMessage& msg)
{
    nunchuk::MasterSigner ret("", nunchuk::Device(""), 0);
    try {
        if(nunchuk_instance_[nunchukMode()]){
            ret = nunchuk_instance_[nunchukMode()]->GetMasterSigner(mastersigner_id);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

bool nunchukiface::DeleteMasterSigner(const std::string &mastersigner_id, QWarningMessage &msg)
{
    bool ret = false;
    try {
        if(nunchuk_instance_[nunchukMode()]){
            ret = nunchuk_instance_[nunchukMode()]->DeleteMasterSigner(mastersigner_id);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
        ret = false;
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
        ret = false;
    }
    return ret;
}

bool nunchukiface::DeleteRemoteSigner(const std::string &master_fingerprint, const std::string &derivation_path, QWarningMessage &msg)
{
    bool ret = false;
    try {
        if(nunchuk_instance_[nunchukMode()]){
            ret = nunchuk_instance_[nunchukMode()]->DeleteRemoteSigner(master_fingerprint, derivation_path);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
        ret = false;
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
        ret = false;
    }
    return ret;
}

bool nunchukiface::UpdateRemoteSigner(const nunchuk::SingleSigner &remotesigner, QWarningMessage &msg)
{
    bool ret = false;
    try {
        if(nunchuk_instance_[nunchukMode()]){
            ret = nunchuk_instance_[nunchukMode()]->UpdateRemoteSigner(remotesigner);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
        ret = false;
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
        ret = false;
    }
    return ret;
}

bool nunchukiface::UpdateMasterSigner(const nunchuk::MasterSigner &mastersigner, QWarningMessage &msg)
{
    bool ret = false;
    try {
        if(nunchuk_instance_[nunchukMode()]){
            ret = nunchuk_instance_[nunchukMode()]->UpdateMasterSigner(mastersigner);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
        ret = false;
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
        ret = false;
    }
    return ret;
}

std::vector<nunchuk::SingleSigner> nunchukiface::GetRemoteSigners(QWarningMessage &msg)
{
    std::vector<nunchuk::SingleSigner> ret;
    try {
        if(nunchuk_instance_[nunchukMode()]){
            ret = nunchuk_instance_[nunchukMode()]->GetRemoteSigners();
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

std::string nunchukiface::GetHealthCheckPath()
{
    std::string ret = "N/A";
    try {
        if(nunchuk_instance_[nunchukMode()]){
            ret = nunchuk_instance_[nunchukMode()]->GetHealthCheckPath();
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        ret = "N/A";
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
        ret = "N/A";
    }
    return ret;
}

nunchuk::HealthStatus nunchukiface::HealthCheckMasterSigner(const std::string &fingerprint,
                                                            std::string &message,
                                                            std::string &signature,
                                                            std::string &path,
                                                            QWarningMessage &msg)
{
    nunchuk::HealthStatus ret = nunchuk::HealthStatus::FINGERPRINT_NOT_MATCHED;
    try {
        if(nunchuk_instance_[nunchukMode()]){
            ret = nunchuk_instance_[nunchukMode()]->HealthCheckMasterSigner(fingerprint, message, signature, path);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
        ret = nunchuk::HealthStatus::FINGERPRINT_NOT_MATCHED;
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
        ret = nunchuk::HealthStatus::FINGERPRINT_NOT_MATCHED;
    }
    return ret;
}

nunchuk::HealthStatus nunchukiface::HealthCheckSingleSigner(const nunchuk::SingleSigner &signer,
                                                            const std::string &message,
                                                            const std::string &signature, QWarningMessage &msg)
{
    nunchuk::HealthStatus ret = nunchuk::HealthStatus::SIGNATURE_INVALID;
    try {
        if(nunchuk_instance_[nunchukMode()]){
            ret = nunchuk_instance_[nunchukMode()]->HealthCheckSingleSigner(signer, message, signature);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
        ret = nunchuk::HealthStatus::FINGERPRINT_NOT_MATCHED;
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTIONS";
        ret = nunchuk::HealthStatus::FINGERPRINT_NOT_MATCHED;
    }
    return ret;
}

std::vector<nunchuk::Transaction> nunchukiface::GetTransactionHistory(const std::string &wallet_id,
                                                                      int count,
                                                                      int skip,
                                                                      QWarningMessage& msg)
{
    std::vector<nunchuk::Transaction> ret;
    try {
        if(nunchuk_instance_[nunchukMode()]){
            ret = nunchuk_instance_[nunchukMode()]->GetTransactionHistory(wallet_id, count, skip);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

nunchuk::AppSettings nunchukiface::GetAppSettings(QWarningMessage& msg)
{
    nunchuk::AppSettings ret;
    try {
        if(nunchuk_instance_[nunchukMode()]){
            ret = nunchuk_instance_[nunchukMode()]->GetAppSettings();
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

nunchuk::AppSettings nunchukiface::UpdateAppSettings(nunchuk::AppSettings &appSettings, QWarningMessage& msg)
{
    nunchuk::AppSettings ret;
    try {
        if(nunchuk_instance_[nunchukMode()]){
            ret = nunchuk_instance_[nunchukMode()]->UpdateAppSettings(appSettings);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::future_error &e) {
        DBG_INFO << "THROW EXCEPTION, COULD NOT UPDATE SETTING" << e.what();
        msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
        msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

std::vector<std::string> nunchukiface::GetAddresses(const std::string &wallet_id, bool used, bool internal, QWarningMessage& msg)
{
    std::vector<std::string> ret;
    try {
        if(nunchuk_instance_[nunchukMode()]){
            ret = nunchuk_instance_[nunchukMode()]->GetAddresses(wallet_id, used, internal);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

std::string nunchukiface::NewAddress(const std::string &wallet_id, bool internal, QWarningMessage &msg)
{
    std::string ret = "";
    try {
        if(nunchuk_instance_[nunchukMode()]){
            ret = nunchuk_instance_[nunchukMode()]->NewAddress(wallet_id, internal);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
        ret = "";
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
        ret = "";
    }
    return ret;
}

std::vector<nunchuk::UnspentOutput> nunchukiface::GetUnspentOutputs(const std::string &wallet_id, QWarningMessage& msg)
{
    std::vector<nunchuk::UnspentOutput> ret;
    try {
        if(nunchuk_instance_[nunchukMode()]){
            ret = nunchuk_instance_[nunchukMode()]->GetUnspentOutputs(wallet_id);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

nunchuk::Transaction nunchukiface::CreateTransaction(const std::string& wallet_id,
                                                     const std::map<std::string, nunchuk::Amount> outputs,
                                                     const std::string& memo,
                                                     const std::vector<nunchuk::UnspentOutput> inputs,
                                                     nunchuk::Amount fee_rate,
                                                     bool subtract_fee_from_amount,
                                                     QWarningMessage& msg)
{
    nunchuk::Transaction ret;
    try {
        if(nunchuk_instance_[nunchukMode()]){
            ret = nunchuk_instance_[nunchukMode()]->CreateTransaction(wallet_id, outputs, memo, inputs, fee_rate, subtract_fee_from_amount);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

nunchuk::Transaction nunchukiface::DraftTransaction(const std::string &wallet_id,
                                                    const std::map<std::string, nunchuk::Amount> outputs,
                                                    const std::vector<nunchuk::UnspentOutput> inputs,
                                                    nunchuk::Amount fee_rate,
                                                    const bool subtract_fee_from_amount,
                                                    QWarningMessage& msg)
{
    nunchuk::Transaction ret;
    try {
        if(nunchuk_instance_[nunchukMode()]){
            ret = nunchuk_instance_[nunchukMode()]->DraftTransaction(wallet_id, outputs, inputs, fee_rate, subtract_fee_from_amount);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

nunchuk::Transaction nunchukiface::ReplaceTransaction(const std::string& wallet_id,
                                                      const std::string &tx_id,
                                                      nunchuk::Amount new_fee_rate,
                                                      QWarningMessage& msg)
{
    nunchuk::Transaction ret;
    try {
        if(nunchuk_instance_[nunchukMode()]){
            ret = nunchuk_instance_[nunchukMode()]->ReplaceTransaction(wallet_id, tx_id, new_fee_rate);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

bool nunchukiface::UpdateTransactionMemo(const std::string &wallet_id,
                                         const std::string &tx_id,
                                         const std::string &new_memo,
                                         QWarningMessage &msg)
{
    bool ret = false;
    try {
        if(nunchuk_instance_[nunchukMode()]){
            ret = nunchuk_instance_[nunchukMode()]->UpdateTransactionMemo(wallet_id, tx_id, new_memo);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
        ret = false;
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
        ret = false;
    }
    return ret;
}

bool nunchukiface::ExportTransaction(const std::string &wallet_id,
                                     const std::string &tx_id,
                                     const std::string &file_path,
                                     QWarningMessage &msg)
{
    bool ret = false;
    try {
        if(nunchuk_instance_[nunchukMode()]){
            ret = nunchuk_instance_[nunchukMode()]->ExportTransaction(wallet_id, tx_id, file_path);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
        ret = false;
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
        ret = false;
    }
    return ret;
}

nunchuk::Transaction nunchukiface::ImportTransaction(const std::string &wallet_id,
                                                     const std::string &file_path,
                                                     QWarningMessage& msg)
{
    nunchuk::Transaction ret;
    try {
        if(nunchuk_instance_[nunchukMode()]){
            ret = nunchuk_instance_[nunchukMode()]->ImportTransaction(wallet_id, file_path);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

nunchuk::Transaction nunchukiface::UpdateTransaction(const std::string &wallet_id, const std::string &tx_id, const std::string &new_txid, const std::string &raw_tx, const std::string &reject_msg, QWarningMessage &msg)
{
    nunchuk::Transaction ret;
    try {
        if(nunchuk_instance_[nunchukMode()]){
            ret = nunchuk_instance_[nunchukMode()]->UpdateTransaction(wallet_id, tx_id, new_txid, raw_tx, reject_msg);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

nunchuk::Transaction nunchukiface::ImportPsbt(const std::string &wallet_id, const std::string &psbt, QWarningMessage &msg)
{
    nunchuk::Transaction ret;
    try {
        if(nunchuk_instance_[nunchukMode()]){
            ret = nunchuk_instance_[nunchukMode()]->ImportPsbt(wallet_id, psbt);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

nunchuk::Transaction nunchukiface::SignTransaction(const std::string &wallet_id,
                                                   const std::string &tx_id,
                                                   const nunchuk::Device &device,
                                                   QWarningMessage& msg)
{
    nunchuk::Transaction ret;
    try {
        if(nunchuk_instance_[nunchukMode()]){
            ret = nunchuk_instance_[nunchukMode()]->SignTransaction(wallet_id, tx_id, device);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

nunchuk::Transaction nunchukiface::BroadcastTransaction(const std::string &wallet_id,
                                                        const std::string &tx_id,
                                                        QWarningMessage& msg)
{
    nunchuk::Transaction ret;
    try {
        if(nunchuk_instance_[nunchukMode()]){
            ret = nunchuk_instance_[nunchukMode()]->BroadcastTransaction(wallet_id, tx_id);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

nunchuk::Transaction nunchukiface::GetTransaction(const std::string &wallet_id,
                                                  const std::string &tx_id,
                                                  QWarningMessage& msg)
{
    nunchuk::Transaction ret;
    try {
        if(nunchuk_instance_[nunchukMode()]){
            ret = nunchuk_instance_[nunchukMode()]->GetTransaction(wallet_id, tx_id);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

nunchuk::TapsignerStatus nunchukiface::GetTapsignerStatusFromMasterSigner(const std::string &master_signer_id, QWarningMessage &msg)
{
    nunchuk::TapsignerStatus ret;
    try {
        if(nunchuk_instance_[nunchukMode()]){
            ret = nunchuk_instance_[nunchukMode()]->GetTapsignerStatusFromMasterSigner(master_signer_id);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

bool nunchukiface::DeleteTransaction(const std::string &wallet_id,
                                     const std::string &tx_id,
                                     QWarningMessage &msg)
{
    bool ret = false;
    try {
        if(nunchuk_instance_[nunchukMode()]){
            ret = nunchuk_instance_[nunchukMode()]->DeleteTransaction(wallet_id, tx_id);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
        ret = false;
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
        ret = false;
    }
    return ret;
}

void nunchukiface::CacheMasterSignerXPub(const std::string &mastersigner_id, QWarningMessage &msg)
{
    try {
        if(nunchuk_instance_[nunchukMode()]){
            nunchuk_instance_[nunchukMode()]->CacheMasterSignerXPub(mastersigner_id, cache_master_signer_XPub);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
}

int nunchukiface::GetChainTip(QWarningMessage &msg)
{
    int ret = 0;
    try {
        if(nunchuk_instance_[nunchukMode()]){
            ret = nunchuk_instance_[nunchukMode()]->GetChainTip();
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
        ret = 0;
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
        ret = 0;
    }
    return ret;
}

bool nunchukiface::ExportHealthCheckMessage(const std::string &message, const std::string &file_path, QWarningMessage &msg)
{
    bool ret = false;
    try {
        if(nunchuk_instance_[nunchukMode()]){
            ret = nunchuk_instance_[nunchukMode()]->ExportHealthCheckMessage(message, file_path);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
        ret = false;
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
        ret = false;
    }
    return ret;
}

std::string nunchukiface::ImportHealthCheckSignature(const std::string &file_path, QWarningMessage &msg)
{
    std::string ret = "";
    try {
        if(nunchuk_instance_[nunchukMode()]){
            ret = nunchuk_instance_[nunchukMode()]->ImportHealthCheckSignature(file_path);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
        ret = "";
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
        ret = "";
    }
    return ret;
}

nunchuk::Amount nunchukiface::EstimateFee(int conf_target, QWarningMessage &msg)
{
    nunchuk::Amount ret = 0;
    try {
        if(nunchuk_instance_[nunchukMode()]){
            ret = nunchuk_instance_[nunchukMode()]->EstimateFee(conf_target);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
        ret = 0;
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
        ret = 0;
    }
    return ret;
}

bool nunchukiface::ExportUnspentOutputs(const std::string &wallet_id, const std::string &file_path, nunchuk::ExportFormat format, QWarningMessage& msg)
{
    bool ret = false;
    try {
        if(nunchuk_instance_[nunchukMode()]){
            ret = nunchuk_instance_[nunchukMode()]->ExportUnspentOutputs(wallet_id, file_path, format);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
        ret = false;
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
        ret = false;
    }
    return ret;
}

bool nunchukiface::ExportTransactionHistory(const std::string &wallet_id, const std::string &file_path, nunchuk::ExportFormat format, QWarningMessage& msg)
{
    bool ret = false;
    try {
        if(nunchuk_instance_[nunchukMode()]){
            ret = nunchuk_instance_[nunchukMode()]->ExportTransactionHistory(wallet_id, file_path, format);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
        ret = false;
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
        ret = false;
    }
    return ret;
}

void nunchukiface::DisplayAddressOnDevice(const std::string &wallet_id, const std::string &address, const std::string &device_fingerprint, QWarningMessage &msg)
{
    try {
        if(nunchuk_instance_[nunchukMode()]){
            nunchuk_instance_[nunchukMode()]->DisplayAddressOnDevice(wallet_id, address, device_fingerprint);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
}

nunchuk::Amount nunchukiface::GetAddressBalance(const std::string &wallet_id, const std::string &address, QWarningMessage &msg)
{
    nunchuk::Amount ret = 0;
    try {
        if(nunchuk_instance_[nunchukMode()]){
            ret = nunchuk_instance_[nunchukMode()]->GetAddressBalance(wallet_id, address);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
        ret = 0;
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
        ret = 0;
    }
    return ret;
}

std::string nunchukiface::GetSelectedWallet(QWarningMessage& msg)
{
    std::string ret = "";
    try {
        if(nunchuk_instance_[nunchukMode()]){
            ret = nunchuk_instance_[nunchukMode()]->GetSelectedWallet();
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
        ret = "";
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
        ret = "";
    }
    return ret;
}

void nunchukiface::PromtPinOnDevice(const nunchuk::Device &device, QWarningMessage &msg)
{
    try {
        if(nunchuk_instance_[nunchukMode()]){
            nunchuk_instance_[nunchukMode()]->PromtPinOnDevice(device);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
}

void nunchukiface::SendPinToDevice(const nunchuk::Device &device, const std::string &pin, QWarningMessage &msg)
{
    try {
        if(nunchuk_instance_[nunchukMode()]){
            nunchuk_instance_[nunchukMode()]->SendPinToDevice(device, pin);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
}

void nunchukiface::SendPassphraseToDevice(const nunchuk::Device &device, const std::string &passphrase, QWarningMessage &msg)
{
    try {
        if(nunchuk_instance_[nunchukMode()]){
            nunchuk_instance_[nunchukMode()]->SendPassphraseToDevice(device, passphrase);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
}

bool nunchukiface::SetSelectedWallet(const std::string &wallet_id, QWarningMessage &msg)
{
    bool ret = false;
    try {
        if(nunchuk_instance_[nunchukMode()]){
            ret = nunchuk_instance_[nunchukMode()]->SetSelectedWallet(wallet_id);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
        ret = false;
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
        ret = false;
    }
    return ret;
}

nunchuk::SingleSigner nunchukiface::CreateCoboSigner(const std::string &name, const std::string &json_info, QWarningMessage &msg)
{
    nunchuk::SingleSigner ret("","","","","",0,"");
    try {
        if(nunchuk_instance_[nunchukMode()]){
            ret = nunchuk_instance_[nunchukMode()]->CreateCoboSigner(name, json_info);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

std::vector<std::string> nunchukiface::ExportCoboTransaction(const std::string &wallet_id, const std::string &tx_id, QWarningMessage &msg)
{
    std::vector<std::string> ret;
    try {
        if(nunchuk_instance_[nunchukMode()]){
            ret = nunchuk_instance_[nunchukMode()]->ExportCoboTransaction(wallet_id, tx_id);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

nunchuk::Transaction nunchukiface::ImportCoboTransaction(const std::string &wallet_id, const std::vector<std::string> &qr_data, QWarningMessage &msg)
{
    nunchuk::Transaction ret;
    try {
        if(nunchuk_instance_[nunchukMode()]){
            ret = nunchuk_instance_[nunchukMode()]->ImportCoboTransaction(wallet_id, qr_data);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

std::vector<std::string> nunchukiface::ExportCoboWallet(const std::string &wallet_id, QWarningMessage &msg)
{
    std::vector<std::string> ret;
    try {
        if(nunchuk_instance_[nunchukMode()]){
            ret = nunchuk_instance_[nunchukMode()]->ExportCoboWallet(wallet_id);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

nunchuk::Wallet nunchukiface::ImportCoboWallet(const std::vector<std::string> &qr_data, const std::string &description, QWarningMessage &msg)
{
    nunchuk::Wallet ret(false);
    try {
        if(nunchuk_instance_[nunchukMode()]){
            ret = nunchuk_instance_[nunchukMode()]->ImportCoboWallet(qr_data, description);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

nunchuk::Wallet nunchukiface::ImportWalletConfigFile(const std::string &file_path, const std::string &description, QWarningMessage &msg)
{
    nunchuk::Wallet ret(false);
    try {
        if(nunchuk_instance_[nunchukMode()]){
            ret = nunchuk_instance_[nunchukMode()]->ImportWalletConfigFile(file_path, description);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

nunchuk::MasterSigner nunchukiface::CreateSoftwareSigner(const std::string &name, const std::string &mnemonic, const std::string &passphrase, QWarningMessage& msg, bool isPrimaryKey)
{
    nunchuk::MasterSigner ret("", nunchuk::Device(""), 0);
    try {
        if(nunchuk_instance_[nunchukMode()]){
            ret = nunchuk_instance_[nunchukMode()]->CreateSoftwareSigner(name, mnemonic, passphrase, create_software_signer_listener,isPrimaryKey);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

bool nunchukiface::DeletePrimaryKey()
{
    if(nunchuk_instance_[nunchukMode()]){
        return nunchuk_instance_[nunchukMode()]->DeletePrimaryKey();
    }
    return false;
}

std::string nunchukiface::SignLoginMessage(const std::string& mastersigner_id, const std::string& message, QWarningMessage& msg)
{
    std::string signature;
    try {
        if(nunchuk_instance_[nunchukMode()]){
            signature = nunchuk_instance_[nunchukMode()]->SignLoginMessage(mastersigner_id, message);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return signature;
}

void nunchukiface::SendSignerPassphrase(const std::string &mastersigner_id, const std::string &passphrase, QWarningMessage &msg)
{
    try {
        if(nunchuk_instance_[nunchukMode()]){
            nunchuk_instance_[nunchukMode()]->SendSignerPassphrase(mastersigner_id, passphrase);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
}

void nunchukiface::ClearSignerPassphrase(const std::string &mastersigner_id, QWarningMessage &msg)
{
    try {
        if(nunchuk_instance_[nunchukMode()]){
            nunchuk_instance_[nunchukMode()]->ClearSignerPassphrase(mastersigner_id);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
}

void nunchukiface::RescanBlockchain(int start_height, int stop_height, QWarningMessage &msg)
{
    try {
        if(nunchuk_instance_[nunchukMode()]){
            nunchuk_instance_[nunchukMode()]->RescanBlockchain(start_height, stop_height);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
}

void nunchukiface::AddBalanceListener(std::function<void (std::string, nunchuk::Amount)> listener)
{
    try {
        if(nunchuk_instance_[nunchukMode()]){
            nunchuk_instance_[nunchukMode()]->AddBalanceListener(listener);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION";
    }
}

void nunchukiface::AddBalancesListener(std::function<void (std::string, nunchuk::Amount, nunchuk::Amount)> listener)
{
    try {
        if(nunchuk_instance_[nunchukMode()]){
            nunchuk_instance_[nunchukMode()]->AddBalancesListener(listener);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION";
    }
}

void nunchukiface::AddTransactionListener( std::function<void(std::string, nunchuk::TransactionStatus, std::string)> listener)
{
    try {
        if(nunchuk_instance_[nunchukMode()]){
            nunchuk_instance_[nunchukMode()]->AddTransactionListener(listener);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION";
    }
}

void nunchukiface::AddDeviceListener(std::function<void (std::string, bool)> listener)
{
    try {
        if(nunchuk_instance_[nunchukMode()]){
            nunchuk_instance_[nunchukMode()]->AddDeviceListener(listener);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION";
    }
}

void nunchukiface::AddBlockListener(std::function<void (int, std::string)> listener)
{
    try {
        if(nunchuk_instance_[nunchukMode()]){
            nunchuk_instance_[nunchukMode()]->AddBlockListener(listener);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION";
    }
}

void nunchukiface::AddBlockchainConnectionListener(std::function<void(nunchuk::ConnectionStatus, int)> listener)
{
    try {
        if(nunchuk_instance_[nunchukMode()]){
            nunchuk_instance_[nunchukMode()]->AddBlockchainConnectionListener(listener);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION";
    }
}

nunchuk::SingleSigner nunchukiface::ParseKeystoneSigner(const std::string &qr_data,
                                                        QWarningMessage& msg)
{
    nunchuk::SingleSigner ret;
    try {
        if(nunchuk_instance_[nunchukMode()]){
            ret = nunchuk_instance_[nunchukMode()]->ParseKeystoneSigner(qr_data);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

std::vector<std::string> nunchukiface::ExportKeystoneWallet(const std::string &wallet_id,
                                                            QWarningMessage& msg)
{
    std::vector<std::string> ret;
    try {
        if(nunchuk_instance_[nunchukMode()]){
            ret = nunchuk_instance_[nunchukMode()]->ExportKeystoneWallet(wallet_id);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

std::vector<std::string> nunchukiface::ExportKeystoneTransaction(const std::string &wallet_id,
                                                                 const std::string &tx_id,
                                                                 QWarningMessage& msg)
{
    std::vector<std::string> ret;
    try {
        if(nunchuk_instance_[nunchukMode()]){
            ret = nunchuk_instance_[nunchukMode()]->ExportKeystoneTransaction(wallet_id, tx_id);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

nunchuk::Transaction nunchukiface::ImportKeystoneTransaction(const std::string &wallet_id,
                                                             const std::vector<std::string> &qr_data,
                                                             QWarningMessage& msg)
{
    nunchuk::Transaction ret;
    try {
        if(nunchuk_instance_[nunchukMode()]){
            ret = nunchuk_instance_[nunchukMode()]->ImportKeystoneTransaction(wallet_id, qr_data);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

nunchuk::Wallet nunchukiface::ImportKeystoneWallet(const std::vector<std::string> &qr_data,
                                                   const std::string &description,
                                                   QWarningMessage &msg)
{
    nunchuk::Wallet ret(false);
    try {
        if(nunchuk_instance_[nunchukMode()]){
            ret = nunchuk_instance_[nunchukMode()]->ImportKeystoneWallet(qr_data, description);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

std::vector<nunchuk::SingleSigner> nunchukiface::ParsePassportSigners(const std::vector<std::string> &qr_data,
                                                                      QWarningMessage &msg)
{
    std::vector<nunchuk::SingleSigner> ret;
    try {
        if(nunchuk_instance_[nunchukMode()]){
            ret = nunchuk_instance_[nunchukMode()]->ParsePassportSigners(qr_data);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

std::vector<nunchuk::SingleSigner> nunchukiface::ParseQRSigners(const std::vector<std::string> &qr_data, QWarningMessage &msg)
{
    std::vector<nunchuk::SingleSigner> ret;
    try {
        if(nunchuk_instance_[nunchukMode()]){
            ret = nunchuk_instance_[nunchukMode()]->ParseQRSigners(qr_data);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

std::vector<nunchuk::SingleSigner> nunchukiface::ParseJSONSigners(const std::string &json_str, QWarningMessage &msg)
{
    std::vector<nunchuk::SingleSigner> ret;
    try {
        if(nunchuk_instance_[nunchukMode()]){
            ret = nunchuk_instance_[nunchukMode()]->ParseJSONSigners(json_str);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

std::vector<std::string> nunchukiface::ExportPassportWallet(const std::string &wallet_id,
                                                            QWarningMessage &msg)
{
    std::vector<std::string> ret;
    try {
        if(nunchuk_instance_[nunchukMode()]){
            ret = nunchuk_instance_[nunchukMode()]->ExportPassportWallet(wallet_id);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

std::vector<std::string> nunchukiface::ExportPassportTransaction(const std::string &wallet_id,
                                                                 const std::string &tx_id,
                                                                 QWarningMessage &msg)
{
    std::vector<std::string> ret;
    try {
        if(nunchuk_instance_[nunchukMode()]){
            ret = nunchuk_instance_[nunchukMode()]->ExportPassportTransaction(wallet_id, tx_id);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

nunchuk::Transaction nunchukiface::ImportPassportTransaction(const std::string &wallet_id,
                                                             const std::vector<std::string> &qr_data,
                                                             QWarningMessage &msg)
{
    nunchuk::Transaction ret;
    try {
        if(nunchuk_instance_[nunchukMode()]){
            ret = nunchuk_instance_[nunchukMode()]->ImportPassportTransaction(wallet_id, qr_data);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

void nunchukiface::stopInstance()
{
    if(nunchuk_instance_[nunchukMode()]){
        nunchuk_instance_[nunchukMode()].reset();
    }
}

bool nunchukiface::UpdateTransactionSchedule(const std::string &wallet_id, const std::string &tx_id, time_t ts, QWarningMessage& msg)
{
    bool ret;
    try {
        if(nunchuk_instance_[nunchukMode()]){
            ret = nunchuk_instance_[nunchukMode()]->UpdateTransactionSchedule(wallet_id, tx_id, ts);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    return ret;
}

void nunchukiface::ForceRefreshWallet(const std::string &wallet_id, QWarningMessage &msg)
{
    try {
        if(nunchuk_instance_[nunchukMode()]){
            nunchuk_instance_[nunchukMode()]->ForceRefreshWallet(wallet_id);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
}

