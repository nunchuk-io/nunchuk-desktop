#include "InheritanceFlow.h"
#include "Premiums/QInheritancePlan.h"
#include "core/ui/UiServices.inc"

namespace features::inheritance::common::flows {
using core::flow::FlowContext;
using namespace features::inheritance::usecases;

InheritanceFlow::InheritanceFlow(FlowContext *ctx, QObject *parent) : core::flow::BaseFlow(ctx, parent) {}

void InheritanceFlow::bind(QObject *vm) {
    Q_UNUSED(vm)
}

void InheritanceFlow::discardChanges() {
    GUARD_SERVICE_TAG()
    emit serviceTag->inheritanceDiscardChangeAlert();
}

void InheritanceFlow::finalizeChanges() {
    GUARD_SERVICE_WALLET_INHERITANCE_PLAN()
    if (ihPlan->IsActived()) {
        wallet->setFlow((int)AlertEnum::E_Alert_t::SERVICE_TAG_INHERITANCE_PLAN_UPDATE);
        ihPlan->RequestInheritancePlanUpdate();
    } else {
        wallet->setFlow((int)AlertEnum::E_Alert_t::SERVICE_TAG_INHERITANCE_PLAN_CREATE);
        ihPlan->RequestInheritancePlanCreate();
    }
}

void InheritanceFlow::securityQuestionVerified() {
    GUARD_SERVICE_WALLET_INHERITANCE_PLAN()
    int flow = wallet->flow();
    switch ((AlertEnum::E_Alert_t)flow) {
    case AlertEnum::E_Alert_t::SERVICE_TAG_INHERITANCE_PLAN_UPDATE:
        ihPlan->InheritancePlanUpdateSucceed();
        break;
    case AlertEnum::E_Alert_t::SERVICE_TAG_INHERITANCE_PLAN_CANCEL:
        ihPlan->InheritancePlanCancelSucceed();
        break;
    default:
        break;
    }
}

void InheritanceFlow::cancelInheritancePlan() {
    GUARD_SERVICE_WALLET_INHERITANCE_PLAN()
    wallet->setFlow((int)AlertEnum::E_Alert_t::SERVICE_TAG_INHERITANCE_PLAN_CANCEL);
    ihPlan->RequestInheritancePlanCancel();
}

void InheritanceFlow::finalizeInheritanceChanges() {
    GUARD_SERVICE_WALLET_INHERITANCE_PLAN()
    bool isOnChain = wallet->walletType() == (int)nunchuk::WalletType::MINISCRIPT;
    if (isOnChain) {
        finalizeChanges();
    } else {
        QJsonObject inheritance = this->inheritance();
        QString distribution_method = inheritance.value("distribution_method").toString();
        if (!qUtils::strCompare(distribution_method, "CUSTOMIZE")) {
            finalizeChanges();
        } else {
            if (!checkStageCanGoNext()) return;
            QString beneficiary_mode = inheritance.value("beneficiary_mode").toString();
            QString release_method = inheritance.value("release_method").toString();
            if (!(qUtils::strCompare(beneficiary_mode, "MULTIPLE") && qUtils::strCompare(release_method, "INDIVIDUAL"))) {
                // For single beneficiary with timeline release method, we can directly finalize changes without associate magic
                finalizeChanges();
            } else {
                QString wallet_id = ihPlan->wallet_id();
                QString group_id = wallet->groupId();
                DBG_INFO << "wallet_id: " << wallet_id << ", group_id: " << group_id << inheritance;
                QJsonArray beneficiaries = inheritance.value("beneficiaries").toArray();
                GetOrCreateAssociateMagicInput input;
                input.wallet_id = wallet_id;
                input.group_id = group_id;
                input.beneficiaries = beneficiaries;
                getOrCreateAssociateMagicUC.executeAsync(input, [this](const core::usecase::Result<GetOrCreateAssociateMagicResult> &result) {
                    if (result.isSuccess()) {
                        QJsonArray beneficiaries = result.value().output.value("beneficiaries").toArray();
                        QJsonObject inheritance = this->inheritance();
                        inheritance["beneficiaries"] = beneficiaries;
                        this->setinheritance(inheritance);
                        finalizeChanges();
                    } else {
                        emit showToast(0, result.error(), EWARNING::WarningType::ERROR_MSG);
                    }
                });
            }
        }
    }
}

} // namespace features::inheritance::common::flows
