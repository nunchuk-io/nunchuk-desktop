#pragma once
#include "features/signers/viewmodels/AddKeyBaseViewModel.h"

namespace features::signers::viewmodels {
class AddSoftwareKeyViewModel : public AddKeyBaseViewModel {
    Q_OBJECT
    DEFINE_QT_PROPERTY(QVariantList, options)
    DEFINE_QT_PROPERTY(int, option)
  public:
    explicit AddSoftwareKeyViewModel(QObject *parent = nullptr);

  public slots:
    void selectedOption(int option);
    void onContinueClicked();
};
} // namespace features::signers::viewmodels
