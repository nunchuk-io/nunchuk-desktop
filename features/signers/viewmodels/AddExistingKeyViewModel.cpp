#include "AddExistingKeyViewModel.h"
#include "core/ui/UiServices.inc"
#include "core/bridge/ExternalBridges.h"
#include "core/utils/Utils.h"

namespace features::signers::viewmodels {
using namespace features::signers::usecases;

AddExistingKeyViewModel::AddExistingKeyViewModel(QObject *parent) 
: AddKeyBaseViewModel(parent) {
    setaddressTypes({nunchuk::AddressType::NATIVE_SEGWIT, nunchuk::AddressType::TAPROOT, nunchuk::AddressType::NESTED_SEGWIT, nunchuk::AddressType::LEGACY, nunchuk::AddressType::ANY});
}

void AddExistingKeyViewModel::createExistingKey() {
    GUARD_APP_MODEL()
    SupportedSignersInput input;
    input.wallet_type = walletType();
    input.address_types = addressTypes();
    input.signer_type = signerType();
    std::vector<nunchuk::SignerTag> tags {};
    // DBG_INFO << "Hardware Tag:" << hardwareTag() << " Signer Type:" << (int)signerType();
    if (!hardwareTag().isEmpty()) {
        if (signerType() != nunchuk::SignerType::SOFTWARE) {
            tags.push_back(SignerTagFromStr(hardwareTag().toStdString()));
        }
    }
    input.signer_tags = tags;
    m_supportedSignersUC.addParameter(appModel).executeAsync(input, [this](core::usecase::Result<SupportedSignersResult> result) {
        if (result.isSuccess()) {
            auto supportedSigners = result.value().signers;
            QSingleSignerListModelPtr signerListModel = bridge::nunchukConvertRemoteSigners(supportedSigners);
            setsignerList(signerListModel);
        } else {
            emit showToast(0, result.error(), EWARNING::WarningType::ERROR_MSG);
        }
    });
}

void AddExistingKeyViewModel::onInit() {
    createExistingKey();
}

void AddExistingKeyViewModel::selectExistingKey(const QString& xfp, const int account_index) {
    setselectedXfp(xfp);
    setaccountIndex(account_index);
    auto signer = signerList()->getSingleSignerBy(xfp, account_index);
    if (signer) {
        nunchuk::SingleSigner selected = signer->originSingleSigner();
        setselectedSigner(selected);
    }
}

} // namespace features::signers::viewmodels
