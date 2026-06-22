#include "SignaturesRequiredBaseViewModel.h"

namespace features::common::viewmodels {

SignaturesRequiredBaseViewModel::SignaturesRequiredBaseViewModel(QObject *parent) : ActionViewModel(parent) {}

void SignaturesRequiredBaseViewModel::onInit() {
    ActionViewModel::onInit();
}

} // namespace features::common::viewmodels
