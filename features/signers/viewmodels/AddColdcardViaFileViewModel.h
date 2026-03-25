#pragma once
#include "features/signers/viewmodels/AddKeyBaseViewModel.h"
#include "features/signers/usecases/ImportViaFileUseCase.h"

namespace features::signers::viewmodels {
using features::signers::usecases::ImportViaFileUseCase;
class AddColdcardViaFileViewModel : public AddKeyBaseViewModel {
    Q_OBJECT
  public:
    explicit AddColdcardViaFileViewModel(QObject *parent = nullptr);

  public slots:
    void importColdcardFile(const QString &filePath);
  private:
    ImportViaFileUseCase m_importViaFileUC;
};
} // namespace features::signers::viewmodels
