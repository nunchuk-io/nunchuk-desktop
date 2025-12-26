#include "ScreenManager.h"
#include <QDebug>

namespace core::screen {
    ScreenManager::ScreenManager(QObject* parent)
    : QObject(parent)
{
}

bool ScreenManager::visible() const
{
    return !m_stack.isEmpty();
}

QString ScreenManager::currentScreenId() const
{
    return m_stack.isEmpty() ? QString() : m_stack.last().screenId;
}

QUrl ScreenManager::currentSource() const
{
    return m_stack.isEmpty() ? QUrl() : m_stack.last().source;
}

void ScreenManager::show(const QString& screenId, const QUrl& source)
{
    m_stack.push_back({ screenId, source });
    emit stateChanged();
}

void ScreenManager::replace(const QString& screenId, const QUrl& source)
{
    if (!m_stack.isEmpty())
        m_stack.pop_back();

    m_stack.push_back({ screenId, source });
    emit stateChanged();
}

void ScreenManager::clearAndShow(const QString& screenId, const QUrl& source)
{
    m_stack.clear();
    m_stack.push_back({ screenId, source });
    emit stateChanged();
}

void ScreenManager::back()
{
    if (m_stack.isEmpty())
        return;

    m_stack.pop_back();
    emit stateChanged();
}

void ScreenManager::clear()
{
    if (m_stack.isEmpty())
        return;

    m_stack.clear();
    emit stateChanged();
}
} // namespace core::screen