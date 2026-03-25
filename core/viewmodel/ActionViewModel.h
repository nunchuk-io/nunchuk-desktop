#pragma once

#include "BaseViewModel.h"

namespace core::viewmodels {
class ActionViewModel : public BaseViewModel {
  public:
    explicit ActionViewModel(QObject *parent = nullptr) : BaseViewModel(parent) {}
};
} // namespace core::viewmodels