#pragma once
#include "features/signers/viewmodels/AddKeyBaseViewModel.h"

namespace features::signers::viewmodels {
class AddBlockstreamJadeViaQRViewModel : public AddKeyBaseViewModel {
    Q_OBJECT
  public:
    explicit AddBlockstreamJadeViaQRViewModel(QObject *parent = nullptr);
};
} // namespace features::signers::viewmodels
