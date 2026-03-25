#include "SupportedSignersUseCase.h"
#include "core/ui/UiServices.inc"
#include "core/bridge/ExternalBridges.h"

namespace features::signers::usecases {
using namespace core::usecase;

SupportedSignersUseCase &SupportedSignersUseCase::addParameter(AppModel *model) {
    m_model = model;
    return *this;
}

core::usecase::Result<SupportedSignersResult> SupportedSignersUseCase::execute(const SupportedSignersInput &input) {
    std::vector<nunchuk::SingleSigner> allSigners {};
    std::function<bool(const nunchuk::SingleSigner&)> push_back = 
        [&allSigners](const nunchuk::SingleSigner& signer) -> bool {
            auto it = std::find_if(allSigners.begin(), allSigners.end(), 
                [&signer](const nunchuk::SingleSigner& s) {
                    // Skip add if either XFP or path matches an existing signer
                    return s.get_master_fingerprint() == signer.get_master_fingerprint() ||
                           s.get_derivation_path() == signer.get_derivation_path();
                });
            if (it == allSigners.end()) {
                allSigners.push_back(signer);
                return true;
            }
            return false;
        };


    auto masterList = m_model->masterSignerListPtr();
    auto remoteList = m_model->remoteSignerListPtr();
    if (masterList && remoteList) {
        for (auto master : masterList->fullList()) {
            QString masterId = master->id();
            std::vector<nunchuk::SingleSigner> signers = bridge::GetSignersFromMasterSigner(masterId);
            if (signers.empty()) {
                for (const auto& address_type : input.address_types) {
                    push_back(master->cloneSingleSigner(input.wallet_type, address_type));
                }                
            } else {
                for (const auto& k : signers) {
                    push_back(k);
                }
            }
        }
        for (auto single : remoteList->fullList()) {
            push_back(single->originSingleSigner());
        }        
    }
    std::vector<nunchuk::SingleSigner> supportedSigners {};
    for (const auto& k : allSigners) {
        bool matchType = isType(k, input.signer_type);
        bool matchTag = input.signer_tags.empty() ? true : isTag(k, input.signer_tags.front());
        if (matchType && matchTag) {
            supportedSigners.push_back(k);
        }
    }
    if (supportedSigners.empty()) {
        return core::usecase::Result<SupportedSignersResult>::failure("No supported signers found.");
    } else {
        SupportedSignersResult result;
        result.signers = supportedSigners;
        return core::usecase::Result<SupportedSignersResult>::success(result);
    }
}

bool SupportedSignersUseCase::isTag(const nunchuk::SingleSigner& signer, const nunchuk::SignerTag& tag) {
    const auto& tags = signer.get_tags();
    return std::find(tags.begin(), tags.end(), tag) != tags.end();
}

bool SupportedSignersUseCase::isType(const nunchuk::SingleSigner& signer, const nunchuk::SignerType& type) {
    return signer.get_type() == type;
}

}