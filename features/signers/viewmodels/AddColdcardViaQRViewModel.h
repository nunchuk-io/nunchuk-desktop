#pragma once
#include "features/signers/viewmodels/AddKeyBaseViewModel.h"
#include "features/signers/usecases/ImportViaQrUseCase.h"

namespace features::signers::viewmodels {
using features::signers::usecases::ImportViaQrUseCase;
class AddColdcardViaQRViewModel : public AddKeyBaseViewModel {
    Q_OBJECT
  public:
    explicit AddColdcardViaQRViewModel(QObject *parent = nullptr);

  public slots:
    void importQR(const QStringList &tags);
  private:
    ImportViaQrUseCase m_importViaQrUC;
};
} // namespace features::signers::viewmodels
