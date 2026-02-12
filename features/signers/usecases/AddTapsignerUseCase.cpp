#include "AddTapsignerUseCase.h"
#include "qUtils.h"
#include "ifaces/bridgeifaces.h"

namespace features::signers::usecases {
using namespace core::usecase;

Result<AddTapsignerResult> AddTapsignerUseCase::execute(const AddTapsignerInput &input) {
    AddTapsignerResult result;
    result.tapsigner = input.tapsigner;
    QJsonObject tapsigner = input.tapsigner;
    if(!tapsigner.isEmpty()){
        QString card_id     = tapsigner["card_id"].toString();
        QString version     = tapsigner["version"].toString();
        int birth_height    = tapsigner["birth_height"].toInt();
        bool is_testnet     = tapsigner["is_testnet"].toBool();
        bridge::AddTapsigner(card_id, input.signer_xfp, input.signer_name, version, birth_height, is_testnet);
        return Result<AddTapsignerResult>::success(result);
    } else if(!input.card_id.isEmpty()){
        bridge::AddTapsigner(input.card_id, input.signer_xfp, input.signer_name, input.version, input.birth_height, input.is_testnet);
        return Result<AddTapsignerResult>::success(result);
    } else {
        return Result<AddTapsignerResult>::failure(QString("Invalid tapsigner data"));
    }
}
}