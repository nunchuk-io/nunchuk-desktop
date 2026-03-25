#pragma once
#include "core/enums/EnumMacros.h"
#include "features/common/viewmodels/BaseGuideViewModel.h"
#include <nunchuk.h>

DECLARE_QML_ENUM(features::signers::viewmodels, FeatureOption, None, OnChain, OffChain, ClaimOffChain)

DECLARE_QML_ENUM(features::signers::viewmodels, SignerKeyType, InheritancePlan, KeystoneHW, JadeHW, PassportHW, SeedSignerHW, ColdcardHW, TrezorHW, LedgerHW,
                 BitBoxHW, TapSignerHW, Software)

DECLARE_QML_ENUM(features::signers::viewmodels, RecoveryOption, ViaSeed, ViaXPRV)

namespace features::signers::viewmodels {
using features::common::viewmodels::BaseGuideViewModel;
class AddKeyBaseViewModel : public BaseGuideViewModel {
    Q_OBJECT
    DEFINE_QT_PROPERTY(QString, selectedXfp)
    DEFINE_QT_PROPERTY(QString, signerName)
    DEFINE_QT_PROPERTY(QString, hardwareTag)
    DEFINE_QT_PROPERTY(SignerKeyType, keyType)
  public:
    explicit AddKeyBaseViewModel(QObject *parent = nullptr);
    DEFINE_SET_GET(FeatureOption, setupOption)
    DEFINE_SET_GET(nunchuk::Device, device)
  protected:
    void onInit() override;
  public slots:
    virtual void slotProceedAddSigner(int progress) {};
    void cancel() override;
    void close() override;
};
} // namespace features::signers::viewmodels
