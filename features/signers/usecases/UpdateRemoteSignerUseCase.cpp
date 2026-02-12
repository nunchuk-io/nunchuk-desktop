#include "UpdateRemoteSignerUseCase.h"
#include "qUtils.h"
#include "ifaces/bridgeifaces.h"

namespace features::signers::usecases {
using namespace core::usecase;

Result<UpdateRemoteSignerResult> UpdateRemoteSignerUseCase::execute(const UpdateRemoteSignerInput &input) {
    UpdateRemoteSignerResult result;
    QWarningMessage msg;
    QJsonObject js_signer = input.json.value_or(QJsonObject());
    QString xfp = js_signer["xfp"].toString();
    QString name = js_signer["name"].toString();
    bool is_visible = js_signer["is_visible"].toBool();
    QJsonArray wtags = js_signer["tags"].toArray();
    nunchuk::SingleSigner remote_signer = input.signer;
    std::vector<nunchuk::SignerTag> tags; // get tags from api signer.tags
    for (QJsonValue tag : wtags) {
        QString js_tag = tag.toString();
        tags.push_back(SignerTagFromStr(js_tag.toStdString()));
    }
    // Do update
    remote_signer.set_name(name.toStdString());
    remote_signer.set_tags(tags);
    remote_signer.set_visible(remote_signer.is_visible() || is_visible);
    bridge::UpdateRemoteSigner(remote_signer, msg);
    if (msg.isSuccess()) {
        result.signer = remote_signer;
        return Result<UpdateRemoteSignerResult>::success(result);
    } else {
        return Result<UpdateRemoteSignerResult>::failure(msg.what());
    }
}
}