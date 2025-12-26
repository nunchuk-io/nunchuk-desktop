#pragma once

#include "ActionViewModel.h"

namespace core::viewmodels {
class LoadingViewModel : public ActionViewModel {
    Q_OBJECT
    Q_PROPERTY(bool isLoading READ isLoading WRITE setIsLoading NOTIFY isLoadingChanged)
  public:
    explicit LoadingViewModel(QObject *parent = nullptr);
    virtual ~LoadingViewModel() = default;

    bool isLoading() const;
    void setIsLoading(bool loading);
  signals:
    void isLoadingChanged();
  private:
    bool m_isLoading{false};
};
} // namespace core::viewmodels