#pragma once
#include "core/flow/BaseFlow.h"
#include "core/viewmodel/DefinePropertyMacros.h"

namespace features::draftwallets::sandbox::flows {
using core::flow::FlowContext;

class GroupSandboxInvitesFlow : public core::flow::BaseFlow {
    Q_OBJECT
  public:
    explicit GroupSandboxInvitesFlow(FlowContext *ctx, QObject *parent = nullptr);

    DEFINE_SET_GET(QStringList, invitesList)
    DEFINE_SET_GET(QString, groupId)

  public:
    QString id() const override {
        return "GroupSandboxInvitesFlow";
    }

  public slots:
    void bind(QObject *vm) override;

  signals:

  private:
};

} // namespace features::draftwallets::sandbox::flows
