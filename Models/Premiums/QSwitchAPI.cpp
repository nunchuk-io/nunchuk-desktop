#include "QSwitchAPI.h"
#include "QOutlog.h"
#include <QQmlEngine>

static bool m_IsByzantine = false;
QSwitchAPI::QSwitchAPI(int mode) : m_mode(mode)
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

bool QSwitchAPI::IsByzantine()
{
    return m_IsByzantine;
}

bool QSwitchAPI::getIsByzantine() const
{
    return m_IsByzantine;
}

void QSwitchAPI::setIsByzantine(bool val)
{
    m_IsByzantine = val;
    emit IsByzantineChanged();
}

int QSwitchAPI::getAddHardware() const
{
    return mAddHardware;
}

void QSwitchAPI::setAddHardware(int val)
{
    if (mAddHardware == val) return;
    mAddHardware = val;
    setIsByzantine(m_mode == 1);
    DBG_INFO << IsByzantine();
    emit AddHardwareChanged();
}
