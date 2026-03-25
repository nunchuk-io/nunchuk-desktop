#pragma once
#include "features/signers/viewmodels/AddExistingKeyViewModel.h"

namespace features::signers::viewmodels {
class WhichTypeOfKeySelectionViewModel : public AddExistingKeyViewModel {
    Q_OBJECT
    DEFINE_QT_PROPERTY(int, heightOffset)
    DEFINE_QT_PROPERTY(QVariantList, supportedList)
    DEFINE_QT_PROPERTY(int, keyType)
  public:
    explicit WhichTypeOfKeySelectionViewModel(QObject *parent = nullptr);
    ~WhichTypeOfKeySelectionViewModel();
    void setupGuide();
    void setupSupportedList();
    void continueOffChain();
    void continueOffChain(bool isExisting);
  protected:
    void onInit() override;
  public slots:
    void selectKeyType(int type);
    void onContinueClicked();
};
} // namespace features::signers::viewmodels
