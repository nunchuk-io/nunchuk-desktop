#pragma once
#include <QMetaType>
#include <nunchuk.h>

// Q_DECLARE_METATYPE must be in global namespace
Q_DECLARE_METATYPE(nunchuk::Transaction)
Q_DECLARE_METATYPE(nunchuk::Wallet)
Q_DECLARE_METATYPE(nunchuk::SingleSigner)
Q_DECLARE_METATYPE(nunchuk::MasterSigner)
Q_DECLARE_METATYPE(nunchuk::Device)
Q_DECLARE_METATYPE(nunchuk::SignerTag)
Q_DECLARE_METATYPE(nunchuk::SignerType)
Q_DECLARE_METATYPE(std::vector<nunchuk::Device>)
Q_DECLARE_METATYPE(std::vector<nunchuk::Transaction>)
Q_DECLARE_METATYPE(std::vector<nunchuk::SignerTag>)

namespace core::common::datatypes {

void registerNunchukMetaTypes();

}
