#include "CancelInheritancePlanViewModel.h"
#include "core/ui/UiServices.inc"
#include "generated_qml_keys.hpp"

namespace features::inheritance::common::viewmodels {
using namespace core::viewmodels;

CancelInheritancePlanViewModel::CancelInheritancePlanViewModel(QObject *parent) : InheritancePlanViewModel(parent) {}

void CancelInheritancePlanViewModel::onInit() {}

void CancelInheritancePlanViewModel::confirmCancel() {}

void CancelInheritancePlanViewModel::cancelOperation() {}
} // namespace features::inheritance::common::viewmodels
