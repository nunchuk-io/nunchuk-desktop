#include "LoadingViewModel.h"

namespace core::viewmodels {
LoadingViewModel::LoadingViewModel(QObject *parent) : ActionViewModel(parent) {}

bool LoadingViewModel::isLoading() const {
    return m_isLoading;
}
void LoadingViewModel::setIsLoading(bool loading) {
    if (m_isLoading != loading) {
        m_isLoading = loading;
        emit isLoadingChanged();
    }
}

} // namespace core::viewmodels