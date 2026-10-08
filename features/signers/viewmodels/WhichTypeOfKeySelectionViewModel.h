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
  private:
    // Guards against re-entrant continueOffChain() calls (e.g. double-click on Continue):
    // m_supportedSignersUC's underlying WorkerConcurrent silently drops a 2nd request while
    // the 1st is in flight (run() returns false, callback never stored), so the 2nd
    // setOverrideCursor() would never get its matching restoreOverrideCursor().
    bool m_isSubmitting{false};
};
} // namespace features::signers::viewmodels
