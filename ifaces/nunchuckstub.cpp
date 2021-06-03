#include "nunchuckstub.h"
#include "QOutlog.h"

#ifdef USING_STUB_API
nunchuk::Device::Device(const std::string &fingerprint) :
    type_(""),
    path_(""),
    model_(""),
    master_fingerprint_(fingerprint),
    connected_((rand()%2)),
    needs_pass_phrase_sent_((rand()%2)),
    needs_pin_sent_((rand()%2)){}

nunchuk::Device::Device(const std::string &type, const std::string &path, const std::string &model, const std::string &master_fingerprint, bool needs_pass_phrase_sent, bool needs_pin_sent) :
    type_(type),
    path_(path),
    model_(model),
    master_fingerprint_(master_fingerprint),
    connected_((rand()%2)),
    needs_pass_phrase_sent_(needs_pass_phrase_sent),
    needs_pin_sent_(needs_pin_sent){}

std::string nunchuk::Device::get_type() const {
    if(random()%3){
        return "ledger";
    }
    return type_;
}

std::string nunchuk::Device::get_path() const { return path_;}

std::string nunchuk::Device::get_model() const { return model_;}

std::string nunchuk::Device::get_master_fingerprint() const { return master_fingerprint_;}

bool nunchuk::Device::connected() const { return connected_;}

bool nunchuk::Device::needs_pass_phrase_sent() const { return needs_pass_phrase_sent_;}

bool nunchuk::Device::needs_pin_sent() const { return needs_pin_sent_;}

nunchuk::SingleSigner::SingleSigner(const std::string &name,
                                     const std::string &xpub,
                                     const std::string &public_key,
                                     const std::string &derivation_path,
                                     const std::string &master_fingerprint,
                                     time_t last_health_check,
                                     const std::string &master_signer_id) :
    name_(name),
    xpub_(xpub),
    public_key_(public_key),
    derivation_path_(derivation_path),
    master_fingerprint_(master_fingerprint),
    master_signer_id_(master_signer_id),
    last_health_check_(last_health_check){}

bool nunchuk::SingleSigner::is_used() const { return true; }

std::string nunchuk::SingleSigner::get_name() const { return name_;}

std::string nunchuk::SingleSigner::get_xpub() const { return xpub_;}

std::string nunchuk::SingleSigner::get_public_key() const { return public_key_;}

std::string nunchuk::SingleSigner::get_derivation_path() const { return derivation_path_;}

std::string nunchuk::SingleSigner::get_master_fingerprint() const { return master_fingerprint_;}

std::string nunchuk::SingleSigner::get_master_signer_id() const { return master_signer_id_;}

bool nunchuk::SingleSigner::has_master_signer() const { return (rand()%2);}

time_t nunchuk::SingleSigner::get_last_health_check() const { return last_health_check_;}

void nunchuk::SingleSigner::set_name(const std::string &value) {name_ = value;}

nunchuk::MasterSigner::MasterSigner(const std::string& id, const Device& device, time_t last_health_check, bool software) :
    id_(id), name_(random_string(10)), device_(device),last_health_check_(last_health_check), software_(software) {}

std::string nunchuk::MasterSigner::get_id() const { return random_number_as_string(1);}

std::string nunchuk::MasterSigner::get_name() const { return random_string(50);}

nunchuk::Device nunchuk::MasterSigner::get_device() const {return device_;}

void nunchuk::MasterSigner::set_name(const std::string &value) { name_ = value;}

bool nunchuk::MasterSigner::is_software() const {return software_;}

nunchuk::Wallet::Wallet(const std::string& id, int m, int n,
                         std::vector<SingleSigner>& signers, AddressType address_type,
                         bool is_escrow, time_t create_date):
    id_(id),
    name_(random_string(10)),
    m_(m),
    n_(n),
    signers_(signers),
    address_type_(address_type),
    escrow_(is_escrow),
    balance_(random_int(1000, 1000000)),
    create_date_(create_date)
{}

std::string nunchuk::Wallet::get_id() const {return id_;}

std::string nunchuk::Wallet::get_name() const {return name_;}

int nunchuk::Wallet::get_m() const {return m_;}

int nunchuk::Wallet::get_n() const {return n_;}

std::vector<nunchuk::SingleSigner> nunchuk::Wallet::get_signers() const { return signers_;}

nunchuk::AddressType nunchuk::Wallet::get_address_type() const {return address_type_;}

std::string nunchuk::Wallet::get_description() const { return ""; }

bool nunchuk::Wallet::is_escrow() const {return escrow_;}

nunchuk::Amount nunchuk::Wallet::get_balance() const {return (nunchuk::Amount)random_int(1000, 1000000);}

time_t nunchuk::Wallet::get_create_date() const {return create_date_;}

void nunchuk::Wallet::set_name(const std::string &value) {name_ = value;}

void nunchuk::Wallet::set_description(const std::string& value) { description_ = value; }

void nunchuk::Wallet::set_balance(const Amount& value){}

std::string random_string(size_t length)
{
    auto randchar = []() -> char
    {
            const char charset[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";
            const size_t max_index = (sizeof(charset) - 1);
            return charset[ rand() % max_index ];
};
std::string str(length,0);
std::generate_n( str.begin(), length, randchar );
return str;
}

std::string random_number_as_string(size_t length)
{
    auto randchar = []() -> char
    {
            const char charset[] = "0123456789";
            const size_t max_index = (sizeof(charset) - 1);
            return charset[ rand() % max_index ];
};
std::string str(length,0);
std::generate_n( str.begin(), length, randchar );
return str;
}

time_t random_time(){

    QString format = "MM/dd/yyyy hh:mm";
    int yyyy = 2020;//random_int(2019, 2020);
    int mm = 12;//random_int(1, 12);
    int dd = random_int(1, 28);
    int h = random_int(1, 24);
    int m = random_int(1, 60);
    int s = random_int(1, 60);
    QDateTime time;
    time.setDate(QDate(yyyy, mm, dd));
    time.setTime(QTime(h,m,s));
    return time.toTime_t();
}

int random_int(int min, int max)
{
    return qrand() % ((max + 1) - min) + min;
}

std::vector<nunchuk::Wallet> nunchuk::GetWallets() {
    std::vector<nunchuk::Wallet> ret;
//    return ret;
    for (int i = 0; i < 5; i++) {
        std::string id = random_number_as_string(1);
        int m = i;
        int n = i;
        AddressType address_type = AddressType::LEGACY;
        bool escrow = rand()%2;
        time_t create_date =  random_time();
        DBG_INFO << QDateTime::fromTime_t(create_date).toString("MM/dd/yyyy hh:mm");

        std::vector<SingleSigner> signers;
        for (int j = 0; j < 5; j++) {
            SingleSigner sn(random_string(10),
                            random_string(10),
                            random_string(10),
                            random_string(10),
                            random_number_as_string(1),
                            random_time(),
                            random_number_as_string(1));
            signers.push_back(sn);
        }
        nunchuk::Wallet wl(id, m, n, signers, address_type, escrow, create_date);
        ret.push_back(wl);
    }
    return ret;
}

std::vector<nunchuk::MasterSigner> nunchuk::GetMasterSigners()
{
    std::vector<nunchuk::MasterSigner> ret;
//    return ret;
    for (int i = 0; i < 3; i++) {
        std::string device_type = random_string(5);
        std::string device_path = random_string(1);
        std::string device_model = random_string(5);
        std::string device_master_fingerprint = random_number_as_string(1);
        bool device_needs_pass_phrase_sent = false;
        bool device_needs_pin_sent = true;//rand()%2;
        nunchuk::Device device(device_type, device_path, device_model, device_master_fingerprint, device_needs_pass_phrase_sent, device_needs_pin_sent);
        std::string id = random_number_as_string(3);
        nunchuk::MasterSigner masterSigner(id, device, random_time());
        ret.push_back(masterSigner);
    }

    for (int j = 0; j < 20; j++) {
        std::string device_type = random_string(5);
        std::string device_path = random_string(1);
        std::string device_model = random_string(5);
        std::string device_master_fingerprint = random_number_as_string(1);
        bool device_needs_pass_phrase_sent = true;
        bool device_needs_pin_sent = false;//rand()%2;
        nunchuk::Device device(device_type, device_path, device_model, device_master_fingerprint, device_needs_pass_phrase_sent, device_needs_pin_sent);
        std::string id = random_number_as_string(3);
        nunchuk::MasterSigner masterSigner(id, device, random_time(), true);
        ret.push_back(masterSigner);
    }
    return ret;
}

std::vector<nunchuk::Device> nunchuk::GetDevices()
{
    DBG_INFO;
//    sleep(5);
//    if(random()%2){
//        throw BaseException(2203, "GetDevices ERROR");
//    }
    std::vector<nunchuk::Device> ret;
    for (int i = 0; i < 5; i++) {
        std::string type = random_string(5);
        std::string path = random_number_as_string(1);
        std::string mode = random_string(5);
        std::string master_fingerprint = random_number_as_string(1);
        bool needs_pass_phrase_sent = rand()%2;
        bool needs_pin_sent = false;//rand()%2;

        Device dv(type, path, mode, master_fingerprint, needs_pass_phrase_sent, needs_pin_sent);
        ret.push_back(dv);
    }
    return ret;
}

nunchuk::MasterSigner nunchuk::CreateMasterSigner(const std::string &name, const nunchuk::Device &device)
{
    std::string id = random_number_as_string(5);
    DBG_INFO << QString::fromStdString(id);
    return MasterSigner(id,
                        device,
                        random_time());
}

nunchuk::Wallet nunchuk::GetWallet(const std::string &wallet_id)
{
    std::vector<nunchuk::SingleSigner> temp;
    nunchuk::Wallet ret("", 0, 0, temp , nunchuk::AddressType::ANY, 0, 0);
    return ret;
}

bool nunchuk::DeleteWallet(const std::string &wallet_id)
{
    return true;
}

bool nunchuk::UpdateWallet(nunchuk::Wallet &wallet)
{
    return true;
}

nunchuk::HealthStatus nunchuk::HealthCheckMasterSigner(const std::string &master_fingerprint,  const std::string &message, std::string &signature, std::string &path)
{
//    if(random()%2){
//        throw BaseException(9900, "HealthCheckMasterSigner ERROR");
//    }
    sleep(5);
    signature = random_string(30);
    path = random_string(30);
    return nunchuk::HealthStatus::SUCCESS;//(nunchuk::HealthStatus)random_int(0, 3);
}

int nunchuk::GetCurrentIndexFromMasterSigner(const std::string &mastersigner_id, const nunchuk::WalletType &wallet_type, const nunchuk::AddressType &address_type)
{
    return random_int(0, 100);
}

nunchuk::SingleSigner nunchuk::GetSignerFromMasterSigner(const std::string &mastersigner_id, const nunchuk::WalletType &wallet_type, const nunchuk::AddressType &address_type, int index)
{
    return SingleSigner(random_string(10),
                        random_string(10),
                        random_string(10),
                        random_string(10),
                        random_string(10),
                        random_time(),
                        mastersigner_id);
}

nunchuk::SingleSigner nunchuk::CreateSigner(const std::string &name, const std::string &xpub, const std::string &public_key, const std::string &derivation_path, const std::string &master_fingerprint)
{
//    throw BaseException(3311, "CreateSigner ERROR");
    return SingleSigner(name,
                        xpub,
                        public_key,
                        derivation_path,
                        master_fingerprint,
                        random_time(),
                        "");
}

nunchuk::SingleSigner nunchuk::GetUnusedSignerFromMasterSigner(const std::string &mastersigner_id, const nunchuk::WalletType &wallet_type, const nunchuk::AddressType &address_type)
{
//    if(random()%2){
//        throw BaseException(1102, "DraftTransaction ERROR");
//    }
    SingleSigner ret(random_string(10),
                     random_string(10),
                     random_string(10),
                     random_string(10),
                     random_string(10),
                     random_time(),
                     mastersigner_id);
     return ret;
}

void nunchuk::SetPassphrase(const std::string &passphrase) {
    sleep(5);
//    throw BaseException(nunchuk::NunchukException::PASSPHRASE_ALREADY_USED, "PASSPHRASE_ALREADY_USED");
}

nunchuk::Wallet nunchuk::CreateWallet(const std::string &name, int m, int n, std::vector<nunchuk::SingleSigner> &signers, nunchuk::AddressType address_type, bool is_escrow, const std::string &desc)
{
//    throw BaseException(3333, "CreateWallet ERROR");
    return nunchuk::Wallet(random_number_as_string(1),
                            m,
                            n,
                            signers,
                            address_type,
                            is_escrow,
                            random_time());
}

bool nunchuk::ExportWallet(const std::string &wallet_id, const std::string &file_path, nunchuk::ExportFormat format){return true;}

nunchuk::Wallet nunchuk::ImportWalletDb(const std::string &file_path)
{
//    throw BaseException(1122, "ImportWalletDb ERROR");

    std::string id = random_number_as_string(1);
    int m = random_int(1, 10);
    int n = random_int(1, 10);
    AddressType address_type = AddressType::LEGACY;
    bool escrow = rand()%2;
    time_t create_date =  random_time();

    std::vector<SingleSigner> signers;
    for (int j = 0; j < 5; j++) {
        SingleSigner sn(random_string(10),
                        random_string(10),
                        random_string(10),
                        random_string(10),
                        random_string(10),
                        random_time(),
                        random_number_as_string(1));
        signers.push_back(sn);
    }
    nunchuk::Wallet wl(id, m, n, signers, address_type, escrow, create_date);
    DBG_INFO << QString::fromStdString(wl.get_name());
    return wl;
}

nunchuk::Wallet nunchuk::ImportWalletDescriptor(const std::string &file_path, const std::string &name, const std::string &description)
{
    std::string id = random_number_as_string(1);
    int m = random_int(1, 10);
    int n = random_int(1, 10);
    AddressType address_type = AddressType::LEGACY;
    bool escrow = rand()%2;
    time_t create_date =  random_time();

    std::vector<SingleSigner> signers;
    for (int j = 0; j < 5; j++) {
        SingleSigner sn(random_string(10),
                        random_string(10),
                        random_string(10),
                        random_string(10),
                        random_string(10),
                        random_time(),
                        random_number_as_string(1));
        signers.push_back(sn);
    }
    nunchuk::Wallet wl(id, m, n, signers, address_type, escrow, create_date);
    DBG_INFO << QString::fromStdString(wl.get_name());
    return wl;
}

std::vector<nunchuk::SingleSigner> nunchuk::GetSignersFromMasterSigner(const std::string &mastersigner_id)
{

}

int nunchuk::GetNumberOfSignersFromMasterSigner(const std::string &mastersigner_id)
{

}

nunchuk::MasterSigner nunchuk::GetMasterSigner(const std::string &mastersigner_id)
{
    std::string device_type = random_string(5);
    std::string device_path = random_string(7);
    std::string device_model = random_string(5);
    std::string device_master_fingerprint = random_string(5);
    bool device_needs_pass_phrase_sent = rand()%2;
    bool device_needs_pin_sent = rand()%2;
    nunchuk::Device device(device_type, device_path, device_model, device_master_fingerprint, device_needs_pass_phrase_sent, device_needs_pin_sent);

    std::string id = random_number_as_string(1);
    nunchuk::MasterSigner masterSigner(id, device, random_time());
    return masterSigner;
}

bool nunchuk::DeleteMasterSigner(const std::string &mastersigner_id)
{
    return true;
}

bool nunchuk::UpdateMasterSigner(nunchuk::MasterSigner &mastersigner)
{
    return true;
}

std::string nunchuk::GetHealthCheckPath() {return random_string(15);}

nunchuk::HealthStatus nunchuk::HealthCheckSingleSigner(const nunchuk::SingleSigner &signer, const std::string &message, const std::string &signature)
{
    return (nunchuk::HealthStatus)(rand()%2);
}

std::vector<nunchuk::Transaction> nunchuk::GetTransactionHistory(const std::string &wallet_id, int count, int skip)
{
    std::vector<nunchuk::Transaction> ret;
    for (int i=0; i < 15; i++) {
        nunchuk::Transaction trans;
        ret.push_back(trans);
    }
    return ret;
}

nunchuk::AppSettings nunchuk::GetAppSettings()
{
    return nunchuk::AppSettings();
}

nunchuk::AppSettings nunchuk::UpdateAppSettings(nunchuk::AppSettings &appSettings)
{
    return nunchuk::AppSettings();
}

nunchuk::AppSettings::AppSettings(){}

nunchuk::Chain nunchuk::AppSettings::get_chain() const {return chain_;}
std::vector<std::string> nunchuk::AppSettings::get_mainnet_servers() const {return mainnet_servers_;}
std::vector<std::string> nunchuk::AppSettings::get_testnet_servers() const {return testnet_servers_;}
std::string nunchuk::AppSettings::get_hwi_path() const {return hwi_path_;}
std::string nunchuk::AppSettings::get_storage_path() const {return storage_path_;}
bool nunchuk::AppSettings::use_proxy() const {return enable_proxy_;}
std::string nunchuk::AppSettings::get_proxy_host() const {return proxy_host_;}
int nunchuk::AppSettings::get_proxy_port() const {return proxy_port_;}
std::string nunchuk::AppSettings::get_proxy_username() const {return proxy_username_;}
std::string nunchuk::AppSettings::get_proxy_password() const {return proxy_password_;}
std::string nunchuk::AppSettings::get_certificate_file() const {return certificate_file_;}

void nunchuk::AppSettings::set_chain(nunchuk::Chain value) {chain_ = value;}
void nunchuk::AppSettings::set_mainnet_servers(const std::vector<std::string>& value) {mainnet_servers_ = value;}
void nunchuk::AppSettings::set_testnet_servers(const std::vector<std::string>& value) {testnet_servers_ = value;}
void nunchuk::AppSettings::set_hwi_path(const std::string& value) {hwi_path_ = value;}
void nunchuk::AppSettings::set_storage_path(const std::string& value) {storage_path_ = value;}
void nunchuk::AppSettings::enable_proxy(bool value) {enable_proxy_ = value;}
void nunchuk::AppSettings::set_proxy_host(const std::string& value) {proxy_host_ = value;}
void nunchuk::AppSettings::set_proxy_port(int value) {proxy_port_ = value;}
void nunchuk::AppSettings::set_proxy_username(const std::string& value) {proxy_username_ = value;}
void nunchuk::AppSettings::set_proxy_password(const std::string& value) {proxy_password_ = value;}
void nunchuk::AppSettings::set_certificate_file(const std::string& value) {certificate_file_ = value;}

nunchuk::BackendType nunchuk::AppSettings::get_backend_type() const {return backend_type_;}
std::string nunchuk::AppSettings::get_corerpc_host() const {return corerpc_host_;}
int nunchuk::AppSettings::get_corerpc_port() const {return corerpc_port_;}
std::string nunchuk::AppSettings::get_corerpc_username() const {return corerpc_username_;}
std::string nunchuk::AppSettings::get_corerpc_password() const {return corerpc_password_;}
void nunchuk::AppSettings::set_backend_type(nunchuk::BackendType value){ backend_type_ = value;}
void nunchuk::AppSettings::set_corerpc_host(const std::string& value){ corerpc_host_ = value;}
void nunchuk::AppSettings::set_corerpc_port(int value){ corerpc_port_ = value;}
void nunchuk::AppSettings::set_corerpc_username(const std::string& value){ corerpc_username_ = value;}
void nunchuk::AppSettings::set_corerpc_password(const std::string& value){ corerpc_password_ = value;}

std::vector<std::string> nunchuk::GetAddresses(const std::string &wallet_id, bool used, bool internal)
{
    std::vector<std::string> ret;
    for (int j = 0; j < 5; j++) {
        ret.push_back((internal ? "changeXXXX" : "") + random_string(50));
    }
    return ret;
}

std::vector<nunchuk::UnspentOutput> nunchuk::GetUnspentOutputs(const std::string &wallet_id)
{
    std::vector<nunchuk::UnspentOutput> ret;
    for (int j = 0; j < 53; j++) {
        nunchuk::UnspentOutput temp;
        ret.push_back(temp);
    }
    return ret;
}

std::string nunchuk::NewAddress(const std::string &wallet_id, bool internal)
{
    return random_string(50);
}

nunchuk::Transaction nunchuk::CreateTransaction(const std::string &wallet_id,
                                                  const std::map<std::string, nunchuk::Amount> outputs,
                                                  const std::string &memo,
                                                  const std::vector<nunchuk::UnspentOutput> inputs,
                                                  nunchuk::Amount fee_rate,
                                                  bool subtract_fee_from_amount)
{
    nunchuk::Transaction ret;
    return ret;
}

bool nunchuk::ExportTransaction(const std::string &wallet_id, const std::string &tx_id, const std::string &file_path)
{
    return true;
}

nunchuk::Transaction nunchuk::ImportTransaction(const std::string &wallet_id, const std::string &file_path)
{
    nunchuk::Transaction ret;
    return ret;
}

nunchuk::Transaction nunchuk::SignTransaction(const std::string &wallet_id, const std::string &tx_id, const nunchuk::Device &device)
{
//    if(random()%2){
//    }
    sleep(5);
//    throw BaseException(3304, "SignTransaction ERROR");
    nunchuk::Transaction ret;
    ret.set_receive(false);
    return ret;
}

nunchuk::Transaction nunchuk::BroadcastTransaction(const std::string &wallet_id, const std::string &tx_id)
{
    if(random()%2){
        throw BaseException(3304, "BroadcastTransaction ERROR");
    }
    nunchuk::Transaction ret;
    ret.set_receive(false);
    return ret;
}

nunchuk::Transaction nunchuk::GetTransaction(const std::string &wallet_id, const std::string &tx_id)
{
    nunchuk::Transaction ret;
    return ret;
}

bool nunchuk::DeleteTransaction(const std::string &wallet_id, const std::string &tx_id)
{
    return true;
}

nunchuk::Transaction::Transaction() {}

std::string nunchuk::Transaction::get_txid() const {
    return  random_number_as_string(3);
}

int nunchuk::Transaction::get_height() const {
    return random_int(1, 10);
}

const std::vector<nunchuk::TxInput> nunchuk::Transaction::get_inputs() const {
    std::vector<nunchuk::TxInput> ret;
    for (int i = 0; i < 3; i++) {
        TxInput t;
        t.first = "A";//random_string(1);
        t.second = random_int(1000, 1000000);
        ret.push_back(t);
    }
    return ret;
}

const std::vector<nunchuk::TxOutput> nunchuk::Transaction::get_outputs() const {
    std::vector<nunchuk::TxOutput> ret;
    for (int i = 0; i < 5; i++) {
        TxOutput t;
        t.first = random_string(50);
        t.second = (Amount)random_int(1000, 1000000);
        ret.push_back(t);
    }
    return ret;
}


const std::vector<nunchuk::TxOutput> nunchuk::Transaction::get_receive_outputs() const {
    std::vector<nunchuk::TxOutput> ret;
    TxOutput t;
    t.first = random_string(50);
    t.second = (Amount)random_int(1000, 1000000);
    ret.push_back(t);
    return ret;
}

int nunchuk::Transaction::get_change_index() const { return 4; }

int nunchuk::Transaction::get_m() const { return random_int(1, 10); }

const std::map<std::string, bool> nunchuk::Transaction::get_signers() const {
    std::map<std::string, bool> ret;
    for (int var = 0; var < 5; ++var) {
        ret[random_number_as_string(1)] = rand()%2;
    }
    return ret;
}

std::string nunchuk::Transaction::get_replaced_by_txid() const {
    return  random_number_as_string(3);
}

nunchuk::UnspentOutput::UnspentOutput() {}

std::string nunchuk::UnspentOutput::get_txid() const {
    return random_string(50);
}

int nunchuk::UnspentOutput::get_vout() const {
    return random_int(1, 500);
}

std::string nunchuk::UnspentOutput::get_address() const {
    int lenght = 40;
//    if(random()%2){
//        lenght = random_int(30, 50);
//    }
    return random_string(lenght);
}

//2100000000000000 MAX
nunchuk::Amount nunchuk::UnspentOutput::get_amount() const{
    return 2112340000000;//random_int(100000, 100000000);
}

int nunchuk::UnspentOutput::get_height() const{
    return random_int(1, 100);
}

std::string nunchuk::UnspentOutput::get_memo() const {
    return random_string(100);
}

void nunchuk::UnspentOutput::set_txid(const std::string& value){}
void nunchuk::UnspentOutput::set_vout(int value) {}
void nunchuk::UnspentOutput::set_address(const std::string& value) {}
void nunchuk::UnspentOutput::set_amount(const nunchuk::Amount& value) {}
void nunchuk::UnspentOutput::set_height(int value) {}

std::string nunchuk::Transaction::get_memo() const { return random_string(20); }

nunchuk::TransactionStatus nunchuk::Transaction::get_status() const { return (TransactionStatus)(rand()%6); }

nunchuk::Amount nunchuk::Transaction::get_fee() const { return random_int(0, 10); }
nunchuk::Amount nunchuk::Transaction::get_fee_rate() const { return random_int(0, 10); }

time_t nunchuk::Transaction::get_blocktime() const { return random_time();}

bool nunchuk::Transaction::subtract_fee_from_amount() const {
    return random()%2;
}

bool nunchuk::Transaction::is_receive() const {return random()%2;}

void nunchuk::Transaction::set_receive(bool value) { is_receive_ = value; }

nunchuk::Amount nunchuk::Transaction::get_sub_amount() const {return random_int(1000, 1000000); }

nunchuk::Transaction nunchuk::DraftTransaction(const std::string &wallet_id,
                                                 const std::map<std::string,
                                                 nunchuk::Amount> outputs,
                                                 const std::vector<nunchuk::UnspentOutput> inputs,
                                                 nunchuk::Amount fee_rate,
                                                 bool subtract_fee_from_amount)
{
//    throw BaseException(1102, "DraftTransaction ERROR");
    return nunchuk::Transaction();
}

nunchuk::Transaction nunchuk::ReplaceTransaction(const std::string &tx_id, nunchuk::Amount new_fee_rate)
{
    const char* error = "ReplaceTransaction ERROR";
//    throw BaseException(2203, error);
    return nunchuk::Transaction();
}

bool nunchuk::UpdateTransactionMemo(const std::string &wallet_id, const std::string &tx_id, const std::string &new_memo)
{
    return true;
}

int nunchuk::GetChainTip() {return random_int(1000, 10000);}

bool nunchuk::ExportHealthCheckMessage(const std::string &message, const std::string &file_path){return true;}

std::string nunchuk::ImportHealthCheckSignature(const std::string &file_path) {return random_string(20);}

nunchuk::Amount nunchuk::EstimateFee(int conf_target) {random_int(1000, 1000000);}


std::string nunchuk::DraftWallet(const std::string &name, int m, int n, std::vector<nunchuk::SingleSigner> &signers, nunchuk::AddressType address_type, bool is_escrow, const std::string &desc)
{
    return random_string(20);
}

bool nunchuk::ExportUnspentOutputs( const std::string& wallet_id,
                                    const std::string& file_path,
                                    nunchuk::ExportFormat format){
    return true;
}

bool nunchuk::ExportTransactionHistory(const std::string& wallet_id,
                                       const std::string& file_path,
                                       nunchuk::ExportFormat format){
    return true;
}

void nunchuk::DisplayAddressOnDevice(const std::string &wallet_id, const std::string &address, const std::string &device_fingerprint)
{
    sleep(5);
}

nunchuk::Amount nunchuk::GetAddressBalance(const std::string &wallet_id, const std::string &address){
    return random_int(1,1000000000);
}

std::vector<nunchuk::SingleSigner> nunchuk::GetRemoteSigners()
{
    std::vector<nunchuk::SingleSigner> ret;
//    return ret;
    for (int i = 0; i < 5; i++) {
        std::string name = random_string(50);
        std::string xpub = random_string(7);
        std::string public_key = random_string(5);
        std::string derivation_path = random_number_as_string(1);
        std::string master_fingerprint = random_number_as_string(3);
        time_t last_health_check = random_time();
        SingleSigner tmp(name, xpub, public_key, derivation_path, master_fingerprint, last_health_check);
        ret.push_back(tmp);
    }
    return ret;
}

bool nunchuk::DeleteRemoteSigner(const std::string &master_fingerprint, const std::string &derivation_path){
    return true;
}

bool nunchuk::UpdateRemoteSigner(const nunchuk::SingleSigner &remotesigner)
{
     return true;
}

std::string nunchuk::GetSelectedWallet() {return "1";}

bool nunchuk::SetSelectedWallet(const std::string& wallet_id) {return true;}

nunchuk::Transaction nunchuk::ImportCoboTransaction(const std::string &wallet_id, const std::vector<std::string> &qr_data)
{
    return nunchuk::Transaction();
}

std::vector<std::string> nunchuk::ExportCoboTransaction(const std::string &wallet_id, const std::string &tx_id)
{
    std::vector<std::string> ret;
    for (int i = 0; i < 5; i++) {
        std::string tag = random_string(100);
        ret.push_back(tag);
    }
    return ret;
}

std::vector<std::string> nunchuk::ExportCoboWallet(const std::string &wallet_id)
{
    std::vector<std::string> ret;
    for (int i = 0; i < 5; i++) {
        std::string tag = random_string(100);
        ret.push_back(tag);
    }
    return ret;
}

nunchuk::Wallet nunchuk::ImportCoboWallet(const std::vector<std::string>& qr_data,
                                 const std::string& description)
{
    std::string id = random_number_as_string(1);
    int m = random_int(1, 10);
    int n = random_int(1, 10);
    AddressType address_type = AddressType::LEGACY;
    bool escrow = rand()%2;
    time_t create_date =  random_time();

    std::vector<SingleSigner> signers;
    for (int j = 0; j < 5; j++) {
        SingleSigner sn(random_string(10),
                        random_string(10),
                        random_string(10),
                        random_string(10),
                        random_string(10),
                        random_time(),
                        random_number_as_string(1));
        signers.push_back(sn);
    }
    nunchuk::Wallet wl(id, m, n, signers, address_type, escrow, create_date);
    DBG_INFO << QString::fromStdString(wl.get_name());
    return wl;
}

nunchuk::Wallet nunchuk::ImportWalletConfigFile( const std::string& file_path,
                                        const std::string& description)
{
    throw BaseException(2021, "ImportWalletConfigFile FALSE");
    std::string id = random_number_as_string(1);
    int m = random_int(1, 10);
    int n = random_int(1, 10);
    AddressType address_type = AddressType::LEGACY;
    bool escrow = rand()%2;
    time_t create_date =  random_time();

    std::vector<SingleSigner> signers;
    for (int j = 0; j < 5; j++) {
        SingleSigner sn(random_string(10),
                        random_string(10),
                        random_string(10),
                        random_string(10),
                        random_string(10),
                        random_time(),
                        random_number_as_string(1));
        signers.push_back(sn);
    }
    nunchuk::Wallet wl(id, m, n, signers, address_type, escrow, create_date);
    DBG_INFO << QString::fromStdString(wl.get_name());
    return wl;
}

nunchuk::MasterSigner nunchuk::CreateSoftwareSigner(const std::string &name, const std::string &mnemonic, const std::string &passphrase)
{
    std::string device_type = random_string(5);
    std::string device_path = random_string(7);
    std::string device_model = random_string(5);
    std::string device_master_fingerprint = random_string(5);
    bool device_needs_pass_phrase_sent = rand()%2;
    bool device_needs_pin_sent = rand()%2;
    nunchuk::Device device(device_type, device_path, device_model, device_master_fingerprint, device_needs_pass_phrase_sent, device_needs_pin_sent);

    std::string id = random_number_as_string(1);
    nunchuk::MasterSigner masterSigner(id, device, random_time(), true);
    masterSigner.set_name(name);

    return masterSigner;
}

void nunchuk::SendSignerPassphrase(const std::string& mastersigner_id,
                                   const std::string& passphrase)
{
    if(qrand()%2){
        throw BaseException(9052021, "SendSignerPassphrase FALSE");
    }
}

#endif // USING_STUB_API
