#pragma once

#include "core/viewmodel/BaseViewModel.h"
#include "core/viewmodel/DefinePropertyMacros.h"

namespace features::common::viewmodels {
using core::viewmodels::BaseViewModel;
class BaseGuideViewModel : public BaseViewModel {
  Q_OBJECT
  public:
    explicit BaseGuideViewModel(QObject *parent = nullptr) : BaseViewModel(parent) {}
    virtual ~BaseGuideViewModel() = default;
    DEFINE_QT_PROPERTY(QString, headline)
    DEFINE_QT_PROPERTY(QString, title)
    DEFINE_QT_PROPERTY(QString, description)

};
} // namespace features::common::viewmodels