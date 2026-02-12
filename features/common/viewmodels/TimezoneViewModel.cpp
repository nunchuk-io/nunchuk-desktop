#include "TimezoneViewModel.h"
#include "core/ui/UiServices.inc"
#include "generated_qml_keys.hpp"

namespace features::common::viewmodels {
using namespace core::viewmodels;

TimezoneViewModel::TimezoneViewModel(QObject *parent) 
: ActionViewModel(parent),
m_timezones(QWalletTimezoneModelPtr(new QWalletTimezoneModel()))
{
}


void TimezoneViewModel::onInit() {
    // ctx()->flowManager()->startFlow<SyncWalletFromRemoteFlow>();
}

void TimezoneViewModel::onDispose() {
    // ctx()->flowManager()->stopFlow<SyncWalletFromRemoteFlow>();
}

QWalletTimezoneModel *TimezoneViewModel::timezones() const
{
    return m_timezones.data();
}

QWalletTimezoneModelPtr TimezoneViewModel::timezonesPtr() const
{
    return m_timezones;
}

} // namespace features::common::viewmodels
