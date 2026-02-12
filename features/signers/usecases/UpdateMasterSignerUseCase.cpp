#include "UpdateMasterSignerUseCase.h"
#include "qUtils.h"
#include "ifaces/bridgeifaces.h"

namespace features::signers::usecases {
using namespace core::usecase;

Result<UpdateMasterSignerResult> UpdateMasterSignerUseCase::execute(const UpdateMasterSignerInput &input) {
    UpdateMasterSignerResult result;
    QWarningMessage msg;
    QJsonObject js_signer = input.json.value_or(QJsonObject());
    QString xfp = js_signer["xfp"].toString();
    QString name = js_signer["name"].toString();
    bool is_visible = js_signer["is_visible"].toBool();
    QJsonArray wtags = js_signer["tags"].toArray();
    std::vector<nunchuk::SignerTag> tags; // get tags from api signer.tags
    for (QJsonValue tag : wtags) {
        QString js_tag = tag.toString();
        tags.push_back(SignerTagFromStr(js_tag.toStdString()));
    }
    // Do update
    nunchuk::MasterSigner master_signer = input.signer;
    master_signer.set_name(name.toStdString());
    master_signer.set_tags(tags);

    master_signer.set_visible(master_signer.is_visible() || is_visible);
    bridge::UpdateMasterSigner(master_signer, msg);
    if (msg.isSuccess()) {
        result.signer = master_signer;
        return Result<UpdateMasterSignerResult>::success(result);
    } else {
        return Result<UpdateMasterSignerResult>::failure(msg.what());
    }
}
}