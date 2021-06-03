#include "nunchuckiface.h"
#include "nunchucklistener.h"
#include "QOutlog.h"

nunchukiface::nunchukiface(){}

nunchukiface::~nunchukiface(){}

void nunchukiface::registerCallback()
{
#ifndef USING_STUB_API
    AddBalanceListener(balance_listener);
    AddDeviceListener(devices_listener);
    AddTransactionListener(transaction_listener);
    AddBlockListener(block_listener);
    AddBlockchainConnectionListener(blockchain_connection_listener);
#endif
}

nunchukiface *nunchukiface::instance() {
    static nunchukiface mInstance;
    return &mInstance;
}

bool nunchukiface::makeNunchukInstance(const nunchuk::AppSettings& appsettings, const std::string& passphrase, QWarningMessage& msg)
{
#ifdef USING_STUB_API
//    msg.setWarningMessage(-1014, "makeNunchuckInstance ERROR", EWARNING::WarningType::EXCEPTION_MSG);
    return true;
#else
    try {
        nunchuk_instance_ = nunchuk::MakeNunchuk(appsettings, passphrase);
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
        nunchuk_instance_ = NULL;
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION " << e.what();
        nunchuk_instance_ = NULL;
    }

    if(nunchuk_instance_){
        this->registerCallback();
        return true;
    }
    else{
        return false;
    }
#endif
}

bool nunchukiface::SetPassphrase(const std::string &passphrase, QWarningMessage& msg){
    DBG_INFO;
    bool ret = false;
#ifdef USING_STUB_API
    try {
        nunchuk::SetPassphrase(passphrase);
        ret = true;
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
        ret = false;
    }
#else
    try {
        if(nunchuk_instance_){
            nunchuk_instance_->SetPassphrase(passphrase);
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
#endif
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
    DBG_INFO << name;
    nunchuk::Wallet ret(name, m, n, signers, address_type, is_escrow, 0);
#ifdef USING_STUB_API
    try {
        ret = nunchuk::CreateWallet(name, m, n, signers, address_type, is_escrow, desc);
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
#else
    try {
        if(nunchuk_instance_){
            ret = nunchuk_instance_->CreateWallet(name, m, n, signers, address_type, is_escrow, desc);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
       DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
#endif
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
    DBG_INFO << name;
    std::string ret = "";
#ifdef USING_STUB_API
    ret = nunchuk::DraftWallet(name, m, n, signers, address_type, is_escrow, desc);
#else
    try {
        if(nunchuk_instance_){
            ret = nunchuk_instance_->DraftWallet(name, m, n, signers, address_type, is_escrow, desc);
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
#endif
    return ret;
}

std::vector<nunchuk::Wallet> nunchukiface::GetWallets(QWarningMessage& msg){

    std::vector<nunchuk::Wallet> ret;
#ifdef USING_STUB_API
    ret = nunchuk::GetWallets();
#else
    try {
        if(nunchuk_instance_){
            ret = nunchuk_instance_->GetWallets();
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
#endif
    DBG_INFO << ret.size();
    return ret;
}

nunchuk::Wallet nunchukiface::GetWallet(const std::string &wallet_id, QWarningMessage& msg){
    DBG_INFO << wallet_id;

    std::vector<nunchuk::SingleSigner> temp;
    nunchuk::Wallet ret("", 0, 0, temp , nunchuk::AddressType::ANY, 0, 0);
#ifdef USING_STUB_API
    ret = nunchuk::GetWallet(wallet_id);
#else
    try {
        if(nunchuk_instance_){
            ret = nunchuk_instance_->GetWallet(wallet_id);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
#endif
    return ret;
}

bool nunchukiface::DeleteWallet(const std::string &wallet_id, QWarningMessage &msg){
    DBG_INFO << wallet_id;
    bool ret = false;
#ifdef USING_STUB_API
    ret = nunchuk::DeleteWallet(wallet_id);
#else
    try {
        if(nunchuk_instance_){
            ret = nunchuk_instance_->DeleteWallet(wallet_id);
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
#endif
    return ret;
}

bool nunchukiface::UpdateWallet(nunchuk::Wallet &wallet, QWarningMessage &msg){
    DBG_INFO;
    bool ret = false;
#ifdef USING_STUB_API
    ret = nunchuk::UpdateWallet(wallet);
#else
    try {
        if(nunchuk_instance_){
            ret = nunchuk_instance_->UpdateWallet(wallet);
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
#endif
    return ret;
}

bool nunchukiface::ExportWallet(const std::string &wallet_id, const std::string &file_path, nunchuk::ExportFormat format, QWarningMessage &msg){
    DBG_INFO << wallet_id << file_path << (int)format;

    bool ret = false;
#ifdef USING_STUB_API
    ret = nunchuk::ExportWallet(wallet_id, file_path, format);
#else
    try {
        if(nunchuk_instance_){
            DBG_INFO << "START EXPORT";
            ret = nunchuk_instance_->ExportWallet(wallet_id, file_path, format);
            DBG_INFO << "END EXPORT";
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
#endif
    return ret;
}

nunchuk::Wallet nunchukiface::ImportWalletDb(const std::string &file_path, QWarningMessage& msg){
    DBG_INFO << file_path;

    std::vector<nunchuk::SingleSigner> temp;
    nunchuk::Wallet ret("", 0, 0, temp , nunchuk::AddressType::ANY, 0, 0);
#ifdef USING_STUB_API
    try {
        ret = nunchuk::ImportWalletDb(file_path);
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
#else
    try {
        if(nunchuk_instance_){
            ret = nunchuk_instance_->ImportWalletDb(file_path);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
#endif
    return ret;
}

nunchuk::Wallet nunchukiface::ImportWalletDescriptor(const std::string& file_path,
                                                      const std::string& name,
                                                      const std::string& description,
                                                      QWarningMessage& msg)
{
    DBG_INFO << file_path;
    std::vector<nunchuk::SingleSigner> temp;
    nunchuk::Wallet ret("", 0, 0, temp , nunchuk::AddressType::ANY, 0, 0);
#ifdef USING_STUB_API
    ret = nunchuk::ImportWalletDescriptor(file_path, name, description);
#else
    try {
        if(nunchuk_instance_){
            ret = nunchuk_instance_->ImportWalletDescriptor(file_path, name, description);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
#endif
    return ret;
}

std::vector<nunchuk::Device> nunchukiface::GetDevices(QWarningMessage& msg){
    std::vector<nunchuk::Device> ret;
#ifdef USING_STUB_API
    try {
        ret = nunchuk::GetDevices();
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
#else
    try {
        if(nunchuk_instance_){
            ret = nunchuk_instance_->GetDevices();
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
#endif
    DBG_INFO << ret.size();
    return ret;
}

nunchuk::MasterSigner nunchukiface::CreateMasterSigner(const std::string &name, const nunchuk::Device &device, QWarningMessage& msg){
    DBG_INFO << name;

    nunchuk::MasterSigner ret("", nunchuk::Device(""), 0);
#ifdef USING_STUB_API
    ret = nunchuk::CreateMasterSigner(name, device);
#else
    try {
        if(nunchuk_instance_){
            ret = nunchuk_instance_->CreateMasterSigner(name, device, create_master_signer_listener);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
#endif
    return ret;
}

nunchuk::SingleSigner nunchukiface::GetSignerFromMasterSigner(const std::string &mastersigner_id,
                                                                const nunchuk::WalletType &wallet_type,
                                                                const nunchuk::AddressType &address_type,
                                                                int index,
                                                                QWarningMessage& msg)
{
    DBG_INFO << mastersigner_id;

    nunchuk::SingleSigner ret("","","","","",0,"");
#ifdef USING_STUB_API
    ret = nunchuk::GetSignerFromMasterSigner(mastersigner_id, wallet_type, address_type, index);
#else
    try {
        if(nunchuk_instance_){
            ret = nunchuk_instance_->GetSignerFromMasterSigner(mastersigner_id, wallet_type, address_type, index);
        }
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
#endif
    return ret;
}

nunchuk::SingleSigner nunchukiface::CreateSigner(const std::string &name,
                                                   const std::string &xpub,
                                                   const std::string &public_key,
                                                   const std::string &derivation_path,
                                                   const std::string &master_fingerprint,
                                                   QWarningMessage& msg)
{
    DBG_INFO << name << xpub << public_key << derivation_path << master_fingerprint;
    nunchuk::SingleSigner ret("","","","","",0,"");
#ifdef USING_STUB_API
    try {
        ret = nunchuk::CreateSigner(name, xpub, public_key, derivation_path, master_fingerprint);
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
#else
    try {
        if(nunchuk_instance_){
            ret = nunchuk_instance_->CreateSigner(name, xpub, public_key, derivation_path, master_fingerprint);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
#endif
    return ret;
}

int nunchukiface::GetCurrentIndexFromMasterSigner(const std::string &mastersigner_id,
                                                   const nunchuk::WalletType &wallet_type,
                                                   const nunchuk::AddressType &address_type,
                                                   QWarningMessage &msg)
{
    DBG_INFO << mastersigner_id;

    int ret = -1;
#ifdef USING_STUB_API
    ret = nunchuk::GetCurrentIndexFromMasterSigner(mastersigner_id, wallet_type, address_type);
#else
    try {
        if(nunchuk_instance_){
            ret = nunchuk_instance_->GetCurrentIndexFromMasterSigner(mastersigner_id, wallet_type, address_type);
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
#endif
    return ret;
}

nunchuk::SingleSigner nunchukiface::GetUnusedSignerFromMasterSigner(const std::string &mastersigner_id,
                                                                      const nunchuk::WalletType &wallet_type,
                                                                      const nunchuk::AddressType &address_type,
                                                                      QWarningMessage& msg)
{
    DBG_INFO << mastersigner_id << (int)wallet_type << (int)address_type;
    nunchuk::SingleSigner ret("","","","","",0,"");
#ifdef USING_STUB_API
    try {
        ret = nunchuk::GetUnusedSignerFromMasterSigner(mastersigner_id, wallet_type, address_type);
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
#else
    try {
        if(nunchuk_instance_){
            ret = nunchuk_instance_->GetUnusedSignerFromMasterSigner(mastersigner_id, wallet_type, address_type);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
#endif
    return ret;
}

std::vector<nunchuk::SingleSigner> nunchukiface::GetSignersFromMasterSigner(const std::string &mastersigner_id, QWarningMessage& msg)
{
    DBG_INFO << mastersigner_id;
    std::vector<nunchuk::SingleSigner> ret;
#ifdef USING_STUB_API
    ret = nunchuk::GetSignersFromMasterSigner(mastersigner_id);
#else
    try {
        if(nunchuk_instance_){
            ret = nunchuk_instance_->GetSignersFromMasterSigner(mastersigner_id);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
#endif
    return ret;
}

int nunchukiface::GetNumberOfSignersFromMasterSigner(const std::string &mastersigner_id, QWarningMessage &msg)
{
    DBG_INFO << mastersigner_id;

    int ret = -1;
#ifdef USING_STUB_API
    ret = nunchuk::GetNumberOfSignersFromMasterSigner(mastersigner_id);
#else
    try {
        if(nunchuk_instance_){
            ret = nunchuk_instance_->GetNumberOfSignersFromMasterSigner(mastersigner_id);
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
#endif
    return ret;
}

std::vector<nunchuk::MasterSigner> nunchukiface::GetMasterSigners(QWarningMessage& msg)
{
    DBG_INFO;
    std::vector<nunchuk::MasterSigner> ret;
#ifdef USING_STUB_API
    ret = nunchuk::GetMasterSigners();
#else
    try {
        if(nunchuk_instance_){
            ret = nunchuk_instance_->GetMasterSigners();
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
#endif
    return ret;
}

nunchuk::MasterSigner nunchukiface::GetMasterSigner(const std::string &mastersigner_id, QWarningMessage& msg)
{
    DBG_INFO << mastersigner_id;
    nunchuk::MasterSigner ret("", nunchuk::Device(""), 0);
#ifdef USING_STUB_API
    ret = nunchuk::GetMasterSigner(mastersigner_id);
#else
    try {
        if(nunchuk_instance_){
            ret = nunchuk_instance_->GetMasterSigner(mastersigner_id);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
#endif
    return ret;
}

bool nunchukiface::DeleteMasterSigner(const std::string &mastersigner_id, QWarningMessage &msg)
{
    DBG_INFO << mastersigner_id;
    bool ret = false;
#ifdef USING_STUB_API
    ret = nunchuk::DeleteMasterSigner(mastersigner_id);
#else
    try {
        if(nunchuk_instance_){
            ret = nunchuk_instance_->DeleteMasterSigner(mastersigner_id);
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
#endif
    return ret;
}

bool nunchukiface::DeleteRemoteSigner(const std::string &master_fingerprint, const std::string &derivation_path, QWarningMessage &msg)
{
    DBG_INFO << master_fingerprint << derivation_path;
    bool ret = false;
#ifdef USING_STUB_API
    ret = nunchuk::DeleteRemoteSigner(master_fingerprint, derivation_path);
#else
    try {
        if(nunchuk_instance_){
            ret = nunchuk_instance_->DeleteRemoteSigner(master_fingerprint, derivation_path);
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
#endif
    return ret;
}

bool nunchukiface::UpdateRemoteSigner(const nunchuk::SingleSigner &remotesigner, QWarningMessage &msg)
{
    DBG_INFO;
    bool ret = false;
#ifdef USING_STUB_API
    ret = nunchuk::UpdateRemoteSigner(remotesigner);
#else
    try {
        if(nunchuk_instance_){
            ret = nunchuk_instance_->UpdateRemoteSigner(remotesigner);
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
#endif
    return ret;
}

bool nunchukiface::UpdateMasterSigner(nunchuk::MasterSigner &mastersigner, QWarningMessage &msg)
{
    DBG_INFO;
    bool ret = false;
#ifdef USING_STUB_API
    ret = nunchuk::UpdateMasterSigner(mastersigner);
#else
    try {
        if(nunchuk_instance_){
            ret = nunchuk_instance_->UpdateMasterSigner(mastersigner);
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
#endif
    return ret;
}

std::vector<nunchuk::SingleSigner> nunchukiface::GetRemoteSigners(QWarningMessage &msg)
{
    DBG_INFO;
    std::vector<nunchuk::SingleSigner> ret;
#ifdef USING_STUB_API
    ret = nunchuk::GetRemoteSigners();
#else
    try {
        if(nunchuk_instance_){
            ret = nunchuk_instance_->GetRemoteSigners();
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
#endif
    return ret;
}

std::string nunchukiface::GetHealthCheckPath()
{
    std::string ret = "N/A";
#ifdef USING_STUB_API
    try {
        ret = nunchuk::GetHealthCheckPath();
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        ret = "N/A";
    }
#else
    try {
        if(nunchuk_instance_){
            ret = nunchuk_instance_->GetHealthCheckPath();
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
#endif
    DBG_INFO << ret;
    return ret;
}

nunchuk::HealthStatus nunchukiface::HealthCheckMasterSigner(const std::string &fingerprint,
                                                             std::string &message,
                                                             std::string &signature,
                                                             std::string &path,
                                                             QWarningMessage &msg)
{
    DBG_INFO << fingerprint << message;
    nunchuk::HealthStatus ret = nunchuk::HealthStatus::FINGERPRINT_NOT_MATCHED;
#ifdef USING_STUB_API
    try {
        ret = nunchuk::HealthCheckMasterSigner(fingerprint, message, signature, path);
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
        ret = nunchuk::HealthStatus::FINGERPRINT_NOT_MATCHED;
    }
#else
    try {
        if(nunchuk_instance_){
            ret = nunchuk_instance_->HealthCheckMasterSigner(fingerprint, message, signature, path);
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
#endif
    return ret;
}

nunchuk::HealthStatus nunchukiface::HealthCheckSingleSigner(const nunchuk::SingleSigner &signer,
                                                              const std::string &message,
                                                              const std::string &signature, QWarningMessage &msg)
{
    DBG_INFO << message << signature;
    nunchuk::HealthStatus ret = nunchuk::HealthStatus::SIGNATURE_INVALID;
#ifdef USING_STUB_API
    ret = nunchuk::HealthCheckSingleSigner(signer, message, signature);
#else
    try {
        if(nunchuk_instance_){
            ret = nunchuk_instance_->HealthCheckSingleSigner(signer, message, signature);
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
#endif
    return ret;
}

std::vector<nunchuk::Transaction> nunchukiface::GetTransactionHistory(const std::string &wallet_id,
                                                                        int count,
                                                                        int skip,
                                                                        QWarningMessage& msg)
{
    std::vector<nunchuk::Transaction> ret;
#ifdef USING_STUB_API
    ret = nunchuk::GetTransactionHistory(wallet_id, count, skip);
#else
    try {
        if(nunchuk_instance_){
            ret = nunchuk_instance_->GetTransactionHistory(wallet_id, count, skip);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
#endif
    DBG_INFO << "Wallet ID: " << wallet_id << "TXs:" << ret.size();
    return ret;
}

nunchuk::AppSettings nunchukiface::GetAppSettings(QWarningMessage& msg)
{
    nunchuk::AppSettings ret;
#ifdef USING_STUB_API
    ret = nunchuk::GetAppSettings();
#else
    try {
        if(nunchuk_instance_){
            ret = nunchuk_instance_->GetAppSettings();
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
#endif
    return ret;
}

nunchuk::AppSettings nunchukiface::UpdateAppSettings(nunchuk::AppSettings &appSettings, QWarningMessage& msg)
{
    nunchuk::AppSettings ret;
#ifdef USING_STUB_API
    msg.setWarningMessage(nunchuk::NunchukException::APP_RESTART_REQUIRED, "Reboot", EWARNING::WarningType::EXCEPTION_MSG);
    ret = nunchuk::UpdateAppSettings(appSettings);
#else
    try {
        if(nunchuk_instance_){
            ret = nunchuk_instance_->UpdateAppSettings(appSettings);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::future_error &e) {
        DBG_INFO << "THROW EXCEPTION, CAN NOT UPDATE SETTING" << e.what();
        msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what();
        msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
#endif
    DBG_INFO;
    return ret;
}

std::vector<std::string> nunchukiface::GetAddresses(const std::string &wallet_id, bool used, bool internal, QWarningMessage& msg)
{
    DBG_INFO << wallet_id << used;
    std::vector<std::string> ret;
#ifdef USING_STUB_API
    ret = nunchuk::GetAddresses(wallet_id, used, internal);
#else
    try {
        if(nunchuk_instance_){
            ret = nunchuk_instance_->GetAddresses(wallet_id, used, internal);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
#endif
    return ret;
}

std::string nunchukiface::NewAddress(const std::string &wallet_id, bool internal, QWarningMessage &msg)
{
    DBG_INFO << wallet_id;
    std::string ret = "";
#ifdef USING_STUB_API
    ret = nunchuk::NewAddress(wallet_id, internal);
#else
    try {
        if(nunchuk_instance_){
            ret = nunchuk_instance_->NewAddress(wallet_id, internal);
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
#endif
    return ret;
}

std::vector<nunchuk::UnspentOutput> nunchukiface::GetUnspentOutputs(const std::string &wallet_id, QWarningMessage& msg)
{

    DBG_INFO << wallet_id;
    std::vector<nunchuk::UnspentOutput> ret;
#ifdef USING_STUB_API
    ret = nunchuk::GetUnspentOutputs(wallet_id);
#else
    try {
        if(nunchuk_instance_){
            ret = nunchuk_instance_->GetUnspentOutputs(wallet_id);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
#endif
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
    DBG_INFO << wallet_id << fee_rate;
    nunchuk::Transaction ret;
#ifdef USING_STUB_API
    try {
        ret = nunchuk::CreateTransaction(wallet_id, outputs, memo, inputs, fee_rate, subtract_fee_from_amount);
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
#else
    try {
        if(nunchuk_instance_){
            ret = nunchuk_instance_->CreateTransaction(wallet_id, outputs, memo, inputs, fee_rate, subtract_fee_from_amount);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
#endif
    return ret;
}

nunchuk::Transaction nunchukiface::DraftTransaction(const std::string &wallet_id,
                                                      const std::map<std::string, nunchuk::Amount> outputs,
                                                      const std::vector<nunchuk::UnspentOutput> inputs,
                                                      nunchuk::Amount fee_rate,
                                                      const bool subtract_fee_from_amount,
                                                      QWarningMessage& msg)
{
    DBG_INFO << wallet_id << fee_rate << subtract_fee_from_amount;
    nunchuk::Transaction ret;
#ifdef USING_STUB_API
    try {
        ret = nunchuk::DraftTransaction(wallet_id, outputs, inputs, fee_rate, subtract_fee_from_amount);
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
#else
    try {
        if(nunchuk_instance_){
            ret = nunchuk_instance_->DraftTransaction(wallet_id, outputs, inputs, fee_rate, subtract_fee_from_amount);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
#endif
    return ret;
}

nunchuk::Transaction nunchukiface::ReplaceTransaction(const std::string& wallet_id,
                                                        const std::string &tx_id,
                                                        nunchuk::Amount new_fee_rate,
                                                        QWarningMessage& msg)
{
    DBG_INFO << wallet_id << tx_id << new_fee_rate;
    nunchuk::Transaction ret;
#ifdef USING_STUB_API
    try {
        ret = nunchuk::ReplaceTransaction(tx_id, new_fee_rate);
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
#else
    try {
        if(nunchuk_instance_){
            ret = nunchuk_instance_->ReplaceTransaction(wallet_id, tx_id, new_fee_rate);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
#endif
    return ret;
}

bool nunchukiface::UpdateTransactionMemo(const std::string &wallet_id,
                                          const std::string &tx_id,
                                          const std::string &new_memo,
                                          QWarningMessage &msg)
{
    DBG_INFO << tx_id;
    bool ret = false;
#ifdef USING_STUB_API
    ret = nunchuk::UpdateTransactionMemo(wallet_id, tx_id, new_memo);
#else
    try {
        if(nunchuk_instance_){
            ret = nunchuk_instance_->UpdateTransactionMemo(wallet_id, tx_id, new_memo);
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
#endif
    return ret;
}

bool nunchukiface::ExportTransaction(const std::string &wallet_id,
                                      const std::string &tx_id,
                                      const std::string &file_path,
                                      QWarningMessage &msg)
{
    DBG_INFO << tx_id;
    bool ret = false;
#ifdef USING_STUB_API
    ret = nunchuk::ExportTransaction(wallet_id, tx_id, file_path);
#else
    try {
        if(nunchuk_instance_){
            ret = nunchuk_instance_->ExportTransaction(wallet_id, tx_id, file_path);
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
#endif
    return ret;
}

nunchuk::Transaction nunchukiface::ImportTransaction(const std::string &wallet_id,
                                                       const std::string &file_path,
                                                       QWarningMessage& msg)
{
    DBG_INFO << wallet_id << file_path;
    nunchuk::Transaction ret;
#ifdef USING_STUB_API
    ret = nunchuk::ImportTransaction(wallet_id, file_path);
#else
    try {
        if(nunchuk_instance_){
            ret = nunchuk_instance_->ImportTransaction(wallet_id, file_path);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
#endif
    return ret;
}

nunchuk::Transaction nunchukiface::SignTransaction(const std::string &wallet_id,
                                                     const std::string &tx_id,
                                                     const nunchuk::Device &device,
                                                     QWarningMessage& msg)
{
    DBG_INFO << wallet_id << tx_id;
    nunchuk::Transaction ret;
#ifdef USING_STUB_API
    try {
        ret = nunchuk::SignTransaction(wallet_id, tx_id, device);
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
#else
    try {
        if(nunchuk_instance_){
            ret = nunchuk_instance_->SignTransaction(wallet_id, tx_id, device);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
#endif
    return ret;
}

nunchuk::Transaction nunchukiface::BroadcastTransaction(const std::string &wallet_id,
                                                          const std::string &tx_id,
                                                          QWarningMessage& msg)
{
    DBG_INFO << wallet_id << tx_id;
    nunchuk::Transaction ret;
#ifdef USING_STUB_API
    try {
        ret = nunchuk::BroadcastTransaction(wallet_id, tx_id);
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
#else
    try {
        if(nunchuk_instance_){
            ret = nunchuk_instance_->BroadcastTransaction(wallet_id, tx_id);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
#endif
    return ret;
}

nunchuk::Transaction nunchukiface::GetTransaction(const std::string &wallet_id,
                                                    const std::string &tx_id,
                                                    QWarningMessage& msg)
{
    DBG_INFO << wallet_id << tx_id;
    nunchuk::Transaction ret;
#ifdef USING_STUB_API
    ret = nunchuk::GetTransaction(wallet_id, tx_id);
#else
    try {
        if(nunchuk_instance_){
            ret = nunchuk_instance_->GetTransaction(wallet_id, tx_id);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
#endif
    return ret;
}

bool nunchukiface::DeleteTransaction(const std::string &wallet_id,
                                      const std::string &tx_id,
                                      QWarningMessage &msg)
{
    DBG_INFO << wallet_id << tx_id;
    bool ret = false;
#ifdef USING_STUB_API
    ret = nunchuk::DeleteTransaction(wallet_id, tx_id);
#else
    try {
        if(nunchuk_instance_){
            ret = nunchuk_instance_->DeleteTransaction(wallet_id, tx_id);
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
#endif
    return ret;
}

void nunchukiface::CacheMasterSignerXPub(const std::string &mastersigner_id, QWarningMessage &msg)
{

#ifdef USING_STUB_API

#else
    try {
        if(nunchuk_instance_){
            nunchuk_instance_->CacheMasterSignerXPub(mastersigner_id, cache_master_signer_XPub);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
#endif
}

int nunchukiface::GetChainTip(QWarningMessage &msg)
{
    int ret = 0;
#ifdef USING_STUB_API
    ret = nunchuk::GetChainTip();
#else
    try {
        if(nunchuk_instance_){
            ret = nunchuk_instance_->GetChainTip();
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
#endif
    DBG_INFO << ret;
    return ret;
}

bool nunchukiface::ExportHealthCheckMessage(const std::string &message, const std::string &file_path, QWarningMessage &msg)
{
    bool ret = false;
#ifdef USING_STUB_API
    ret = nunchuk::ExportHealthCheckMessage(message, file_path);
#else
    try {
        if(nunchuk_instance_){
            ret = nunchuk_instance_->ExportHealthCheckMessage(message, file_path);
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
#endif
    DBG_INFO << message << file_path << ret;
    return ret;
}

std::string nunchukiface::ImportHealthCheckSignature(const std::string &file_path, QWarningMessage &msg)
{
    std::string ret = "";
#ifdef USING_STUB_API
    ret = nunchuk::ImportHealthCheckSignature(file_path);
#else
    try {
        if(nunchuk_instance_){
            ret = nunchuk_instance_->ImportHealthCheckSignature(file_path);
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
#endif
    DBG_INFO << file_path << ret;
    return ret;
}

nunchuk::Amount nunchukiface::EstimateFee(int conf_target, QWarningMessage &msg)
{
    DBG_INFO << conf_target;
    nunchuk::Amount ret = 0;
#ifdef USING_STUB_API
    ret = nunchuk::EstimateFee(conf_target);
#else
    try {
        if(nunchuk_instance_){
            ret = nunchuk_instance_->EstimateFee(conf_target);
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
#endif
    return ret;
}

bool nunchukiface::ExportUnspentOutputs(const std::string &wallet_id, const std::string &file_path, nunchuk::ExportFormat format, QWarningMessage& msg)
{
    DBG_INFO << wallet_id << file_path;
    bool ret = false;
#ifdef USING_STUB_API
    ret = nunchuk::ExportUnspentOutputs(wallet_id, file_path, format);
#else
    try {
        if(nunchuk_instance_){
            ret = nunchuk_instance_->ExportUnspentOutputs(wallet_id, file_path, format);
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
#endif
    return ret;
}

bool nunchukiface::ExportTransactionHistory(const std::string &wallet_id, const std::string &file_path, nunchuk::ExportFormat format, QWarningMessage& msg)
{
    DBG_INFO << wallet_id << file_path;
    bool ret = false;
#ifdef USING_STUB_API
    ret = nunchuk::ExportTransactionHistory(wallet_id, file_path, format);
#else
    try {
        if(nunchuk_instance_){
            ret = nunchuk_instance_->ExportTransactionHistory(wallet_id, file_path, format);
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
#endif
    return ret;
}

void nunchukiface::DisplayAddressOnDevice(const std::string &wallet_id, const std::string &address, const std::string &device_fingerprint, QWarningMessage &msg)
{
    DBG_INFO << wallet_id << address;
#ifdef USING_STUB_API
    nunchuk::DisplayAddressOnDevice(wallet_id, address, device_fingerprint);
#else
    try {
        if(nunchuk_instance_){
            nunchuk_instance_->DisplayAddressOnDevice(wallet_id, address, device_fingerprint);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
#endif
}

nunchuk::Amount nunchukiface::GetAddressBalance(const std::string &wallet_id, const std::string &address, QWarningMessage &msg)
{
    DBG_INFO << wallet_id << address;
    nunchuk::Amount ret = 0;
#ifdef USING_STUB_API
    ret = nunchuk::GetAddressBalance(wallet_id, address);
#else
    try {
        if(nunchuk_instance_){
            ret = nunchuk_instance_->GetAddressBalance(wallet_id, address);
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
#endif
    return ret;
}

std::string nunchukiface::GetSelectedWallet(QWarningMessage& msg)
{
    std::string ret = "";
#ifdef USING_STUB_API
    ret = nunchuk::GetSelectedWallet();
#else
    try {
        if(nunchuk_instance_){
            ret = nunchuk_instance_->GetSelectedWallet();
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
#endif
    DBG_INFO << ret;
    return ret;
}

void nunchukiface::PromtPinOnDevice(const nunchuk::Device &device, QWarningMessage &msg)
{
#ifndef USING_STUB_API
    try {
        if(nunchuk_instance_){
            nunchuk_instance_->PromtPinOnDevice(device);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
#endif
    DBG_INFO;
}

void nunchukiface::SendPinToDevice(const nunchuk::Device &device, const std::string &pin, QWarningMessage &msg)
{
#ifdef USING_STUB_API
//     msg.setWarningMessage(2021, "SendPinToDevice FALSE", EWARNING::WarningType::EXCEPTION_MSG);
#else
    try {
        if(nunchuk_instance_){
            nunchuk_instance_->SendPinToDevice(device, pin);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
#endif
    DBG_INFO << pin;
}

bool nunchukiface::SetSelectedWallet(const std::string &wallet_id, QWarningMessage &msg)
{
    DBG_INFO << wallet_id;
    bool ret = false;
#ifdef USING_STUB_API
    ret = nunchuk::SetSelectedWallet(wallet_id);
#else
    try {
        if(nunchuk_instance_){
            ret = nunchuk_instance_->SetSelectedWallet(wallet_id);
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
#endif
    return ret;
}

nunchuk::SingleSigner nunchukiface::CreateCoboSigner(const std::string &name, const std::string &json_info, QWarningMessage &msg)
{
    DBG_INFO << name << json_info;
    nunchuk::SingleSigner ret("","","","","",0,"");
#ifdef USING_STUB_API
    try {
//        ret = nunchuk::CreateCoboSigner(name, json_info);
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
#else
    try {
        if(nunchuk_instance_){
            ret = nunchuk_instance_->CreateCoboSigner(name, json_info);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
#endif
    return ret;
}

std::vector<std::string> nunchukiface::ExportCoboTransaction(const std::string &wallet_id, const std::string &tx_id, QWarningMessage &msg)
{
    DBG_INFO << wallet_id << tx_id;
    std::vector<std::string> ret;
#ifdef USING_STUB_API
    try {
        ret = nunchuk::ExportCoboTransaction(wallet_id, tx_id);
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
#else
    try {
        if(nunchuk_instance_){
            ret = nunchuk_instance_->ExportCoboTransaction(wallet_id, tx_id);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
#endif
    return ret;
}

nunchuk::Transaction nunchukiface::ImportCoboTransaction(const std::string &wallet_id, const std::vector<std::string> &qr_data, QWarningMessage &msg)
{
    DBG_INFO << wallet_id;
    nunchuk::Transaction ret;
#ifdef USING_STUB_API
    ret = nunchuk::ImportCoboTransaction(wallet_id, qr_data);
#else
    try {
        if(nunchuk_instance_){
            ret = nunchuk_instance_->ImportCoboTransaction(wallet_id, qr_data);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
#endif
    return ret;
}

std::vector<std::string> nunchukiface::ExportCoboWallet(const std::string &wallet_id, QWarningMessage &msg)
{
    DBG_INFO << wallet_id;
    std::vector<std::string> ret;
#ifdef USING_STUB_API
    try {
        ret = nunchuk::ExportCoboWallet(wallet_id);
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
#else
    try {
        if(nunchuk_instance_){
            ret = nunchuk_instance_->ExportCoboWallet(wallet_id);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
#endif
    return ret;
}

nunchuk::Wallet nunchukiface::ImportCoboWallet(const std::vector<std::string> &qr_data, const std::string &description, QWarningMessage &msg)
{
    std::vector<nunchuk::SingleSigner> temp;
    nunchuk::Wallet ret("", 0, 0, temp , nunchuk::AddressType::ANY, 0, 0);
#ifdef USING_STUB_API
    try {
        ret = nunchuk::ImportCoboWallet(qr_data, description);
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
#else
    try {
        if(nunchuk_instance_){
            ret = nunchuk_instance_->ImportCoboWallet(qr_data, description);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
#endif
    return ret;
}

nunchuk::Wallet nunchukiface::ImportWalletConfigFile(const std::string &file_path, const std::string &description, QWarningMessage &msg)
{
    std::vector<nunchuk::SingleSigner> temp;
    nunchuk::Wallet ret("", 0, 0, temp , nunchuk::AddressType::ANY, 0, 0);
#ifdef USING_STUB_API
    try {
        ret = nunchuk::ImportWalletConfigFile(file_path, description);
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
#else
    try {
        if(nunchuk_instance_){
            ret = nunchuk_instance_->ImportWalletConfigFile(file_path, description);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
#endif
    return ret;
}

nunchuk::MasterSigner nunchukiface::CreateSoftwareSigner(const std::string &name, const std::string &mnemonic, const std::string &passphrase, QWarningMessage& msg )
{
    DBG_INFO << name;
    nunchuk::MasterSigner ret("", nunchuk::Device(""), 0);
#ifdef USING_STUB_API
    try {
        ret = nunchuk::CreateSoftwareSigner(name, mnemonic, passphrase);
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
#else
    try {
        if(nunchuk_instance_){
            ret = nunchuk_instance_->CreateSoftwareSigner(name, mnemonic, passphrase, create_software_signer_listener );
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
#endif
    return ret;
}

void nunchukiface::SendSignerPassphrase(const std::string &mastersigner_id, const std::string &passphrase, QWarningMessage &msg)
{
    DBG_INFO << mastersigner_id;
#ifdef USING_STUB_API
    try {
        nunchuk::SendSignerPassphrase(mastersigner_id, passphrase);
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
#else
    try {
        if(nunchuk_instance_){
            nunchuk_instance_->SendSignerPassphrase(mastersigner_id, passphrase);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
#endif
}

void nunchukiface::ClearSignerPassphrase(const std::string &mastersigner_id, QWarningMessage &msg)
{
    DBG_INFO << mastersigner_id;
#ifdef USING_STUB_API

#else
    try {
        if(nunchuk_instance_){
            nunchuk_instance_->ClearSignerPassphrase(mastersigner_id);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
#endif
}

void nunchukiface::RescanBlockchain(int start_height, int stop_height, QWarningMessage &msg)
{
    DBG_INFO;
#ifndef USING_STUB_API
    try {
        if(nunchuk_instance_){
            nunchuk_instance_->RescanBlockchain(start_height, stop_height);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
        msg.setWarningMessage(ex.code(), ex.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION" << e.what(); msg.setWarningMessage(-1, e.what(), EWARNING::WarningType::EXCEPTION_MSG);
    }
#endif
}

void nunchukiface::AddBalanceListener(std::function<void (std::string, nunchuk::Amount)> listener)
{
    DBG_INFO;
#ifndef USING_STUB_API
    try {
        if(nunchuk_instance_){
            nunchuk_instance_->AddBalanceListener(listener);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION";
    }
#endif
}

void nunchukiface::AddTransactionListener(std::function<void (std::string, nunchuk::TransactionStatus)> listener)
{
    DBG_INFO;
#ifndef USING_STUB_API
    try {
        if(nunchuk_instance_){
            nunchuk_instance_->AddTransactionListener(listener);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION";
    }
#endif
}

void nunchukiface::AddDeviceListener(std::function<void (std::string, bool)> listener)
{
    DBG_INFO;
#ifndef USING_STUB_API
    try {
        if(nunchuk_instance_){
            nunchuk_instance_->AddDeviceListener(listener);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION";
    }
#endif
}

void nunchukiface::AddBlockListener(std::function<void (int, std::string)> listener)
{
    DBG_INFO;
#ifndef USING_STUB_API
    try {
        if(nunchuk_instance_){
            nunchuk_instance_->AddBlockListener(listener);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION";
    }
#endif
}

void nunchukiface::AddBlockchainConnectionListener(std::function<void(nunchuk::ConnectionStatus, int)> listener)
{
    DBG_INFO;
#ifndef USING_STUB_API
    try {
        if(nunchuk_instance_){
            nunchuk_instance_->AddBlockchainConnectionListener(listener);
        }
    }
    catch (const nunchuk::BaseException &ex) {
        DBG_INFO << "exception nunchuk::BaseException" << ex.code() << ex.what();
    }
    catch (std::exception &e) {
        DBG_INFO << "THROW EXCEPTION";
    }
#endif
}

