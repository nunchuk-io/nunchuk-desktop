#include "StringUrl.h"

StringUrl::StringUrl(const QString &url) {
    m_url = url;
}

StringUrl &StringUrl::add(const QString &url) {
    m_url = QString("%1/%2").arg(m_url).arg(url);
    return *this;
}

StringUrl& StringUrl::replace(const QString& src, const QString& dest)
{
    m_url.replace(src, dest);
    return *this;
}

QString StringUrl::toString() {
    return m_url;
}
