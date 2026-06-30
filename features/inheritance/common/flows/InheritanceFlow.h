#pragma once
#include "core/flow/BaseFlow.h"
#include "core/viewmodel/DefinePropertyMacros.h"
#include "features/inheritance/common/usecases/GetOrCreateAssociateMagicUseCase.h"

namespace features::inheritance::common::flows {
using core::flow::FlowContext;
using features::inheritance::usecases::GetOrCreateAssociateMagicUseCase;

class InheritanceFlow : public core::flow::BaseFlow {
    Q_OBJECT
    DEFINE_QT_PROPERTY(QJsonObject, inheritance)
  public:
    explicit InheritanceFlow(FlowContext *ctx, QObject *parent = nullptr);
    virtual ~InheritanceFlow() = default;

    QString id() const override {
        return "InheritanceFlow";
    }

    virtual bool checkValidCanGoNext() {
        Q_UNUSED(this)
        return true;
    };

  public slots:
    void bind(QObject *vm) override;
    void discardChanges();
    void finalizeChanges();
    void securityQuestionVerified();
    void cancelInheritancePlan();
    void finalizeInheritanceChanges();

  private:
    GetOrCreateAssociateMagicUseCase getOrCreateAssociateMagicUC;
};
} // namespace features::inheritance::common::flows
