#include "WhichTypeOfKeySelectionViewModel.h"
#include "core/ui/UiServices.inc"
#include "generated_qml_keys.hpp"
#include "core/common/resources/AppStrings.h"
#include "features/signers/flows/KeySetupFlow.h"
namespace features::signers::viewmodels {
using namespace features::signers::flows;
using namespace features::signers::usecases;

static const QJsonArray allKeys = {
    QJsonObject{{ "type", (int)SignerKeyType::BitBoxHW },    { "name", "BitBox" },           { "device_type", "bitbox02" },  { "tag", "BITBOX" }},
    QJsonObject{{ "type", (int)SignerKeyType::ColdcardHW }, { "name", "COLDCARD" },         { "device_type", "coldcard" },  { "tag", "COLDCARD" }},
    QJsonObject{{ "type", (int)SignerKeyType::JadeHW },     { "name", "Blockstream Jade" }, { "device_type", "jade" },      { "tag", "JADE" }},
    QJsonObject{{ "type", (int)SignerKeyType::LedgerHW },   { "name", "Ledger" },           { "device_type", "ledger" },    { "tag", "LEDGER" }},
    QJsonObject{{ "type", (int)SignerKeyType::TrezorHW },   { "name", "Trezor" },           { "device_type", "trezor" },    { "tag", "TREZOR" }},
    QJsonObject{{ "type", (int)SignerKeyType::TapSignerHW },{ "name", "TAPSIGNER" },        { "device_type", "tapsigner" }, { "tag", "INHERITANCE" }},
    QJsonObject{{ "type", (int)SignerKeyType::Software },   { "name", "Software" },         { "device_type", "software" },  { "tag", "SOFTWARE" }}
};

QJsonObject add(const SignerKeyType& type, bool isEnabled = true) {
    for (const auto& key : allKeys) {
        QJsonObject keyCopy = key.toObject();
        if (keyCopy.value("type").toInt() == (int)type) {            
            keyCopy.insert("is_enabled", isEnabled);
            return keyCopy;
        }
    }
    return QJsonObject{};
}

QJsonObject getKeyInfoByType(const SignerKeyType& type) {
    for (const auto& key : allKeys) {
        QJsonObject keyCopy = key.toObject();
        if (keyCopy.value("type").toInt() == (int)type) {            
            return keyCopy;
        }
    }
    return QJsonObject{};
}

WhichTypeOfKeySelectionViewModel::WhichTypeOfKeySelectionViewModel(QObject *parent) 
: AddExistingKeyViewModel(parent) {
    setheightOffset(546);
}

WhichTypeOfKeySelectionViewModel::~WhichTypeOfKeySelectionViewModel() {
    qApp->restoreOverrideCursor();
}

void WhichTypeOfKeySelectionViewModel::onInit() {
    setupGuide();
    setupSupportedList();
}

void WhichTypeOfKeySelectionViewModel::setupGuide() {
    switch (setupOption())
    {
    case FeatureOption::ClaimOffChain:{
        setheadline(Strings.STR_QML_1601());
        settitle(Strings.STR_QML_942());        
        setdescription(Strings.STR_QML_1603());
        break;
    }
    
    default:
        break;
    }
}

void WhichTypeOfKeySelectionViewModel::setupSupportedList() {
    QJsonArray list;
    switch (setupOption())
    {
    case FeatureOption::ClaimOffChain:{
        setheightOffset(516);
        list.append(add(SignerKeyType::ColdcardHW));
        list.append(add(SignerKeyType::Software));
        list.append(add(SignerKeyType::TapSignerHW, false));
        setsupportedList(list.toVariantList());
        setkeyType((int)SignerKeyType::ColdcardHW);
        break;
    }
    
    default:
        break;
    }
}

void WhichTypeOfKeySelectionViewModel::selectKeyType(int type)  {
    setkeyType(type);
}

void WhichTypeOfKeySelectionViewModel::continueOffChain() {
    qApp->setOverrideCursor(Qt::WaitCursor);
    setwalletType(nunchuk::WalletType::MULTI_SIG);
    GUARD_APP_MODEL()
    SupportedSignersInput input;
    input.wallet_type = walletType();
    switch (static_cast<SignerKeyType>(keyType()))
    {
    case SignerKeyType::ColdcardHW:        
        setsignerType(nunchuk::SignerType::HARDWARE);
        break;
    case SignerKeyType::Software: {
        setsignerType(nunchuk::SignerType::SOFTWARE);
    }   break;
    default:
        break;
    }
    input.address_types = addressTypes();
    input.signer_type = signerType();
    
    std::vector<nunchuk::SignerTag> tags {};
    if (!hardwareTag().isEmpty()) {
        if (signerType() != nunchuk::SignerType::SOFTWARE) {
            tags.push_back(SignerTagFromStr(hardwareTag().toStdString()));
        }
    }
    input.signer_tags = tags;
    m_supportedSignersUC.addParameter(appModel).executeAsync(input, [this](core::usecase::Result<SupportedSignersResult> result) {
        continueOffChain(result.isSuccess());
    });
}

void WhichTypeOfKeySelectionViewModel::continueOffChain(bool isExisting) {
    GUARD_SUB_SCREEN_MANAGER()
    if (isExisting) {
        switch (static_cast<SignerKeyType>(keyType()))
        {
        case SignerKeyType::ColdcardHW:        
            subMng->show(qml::features::signers::qaddhardwareexistingkey);
            break;
        case SignerKeyType::Software: {
            subMng->show(qml::features::signers::qaddsoftwareexistingkey);
        }   break;
        default:
            break;
        }
    } else {
        switch (static_cast<SignerKeyType>(keyType()))
        {
        case SignerKeyType::ColdcardHW:        
            subMng->show(qml::features::signers::qcoldcardrefreshdevices);
            break;
        case SignerKeyType::Software: {
            subMng->show(qml::features::signers::qrecoveryaddsoftwarekey);
        }   break;
        default:
            break;
        }
    }
}

void WhichTypeOfKeySelectionViewModel::onContinueClicked() {
    auto keyInfo = getKeyInfoByType(static_cast<SignerKeyType>(keyType()));
    if (keyInfo.isEmpty()) return;
    setsignerName(keyInfo.value("name").toString());
    sethardwareTag(keyInfo.value("tag").toString());

    GUARD_FLOW_MANAGER()
    auto flow = flowMng->startFlow<KeySetupFlow>();
    flow->setkeyName(keyInfo.value("name").toString());
    flow->setkeyTag(keyInfo.value("tag").toString());
    flow->setkeyType(keyType());

    switch (setupOption())
    {
    case FeatureOption::ClaimOffChain:{
        continueOffChain();
        break;
    }
    
    default:
        break;
    }
}

} // namespace features::signers::viewmodels
