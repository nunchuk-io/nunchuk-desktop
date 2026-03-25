#include "NunchukMetaType.hpp"

namespace core::common::datatypes {

void registerNunchukMetaTypes() {
    qRegisterMetaType<nunchuk::Transaction>();
    qRegisterMetaType<nunchuk::Wallet>();
    qRegisterMetaType<nunchuk::SingleSigner>();
    qRegisterMetaType<nunchuk::MasterSigner>();
    qRegisterMetaType<nunchuk::Device>();
    qRegisterMetaType<nunchuk::SignerTag>();
    qRegisterMetaType<nunchuk::SignerType>();
    qRegisterMetaType<std::vector<nunchuk::Device>>();
    qRegisterMetaType<std::vector<nunchuk::Transaction>>();
    qRegisterMetaType<std::vector<nunchuk::SignerTag>>();
}
}