#ifndef STRINGURL_H
#define STRINGURL_H
#include <QString>

class StringUrl
{
public:
    StringUrl(const QString& url);
    StringUrl& add(const QString& url);
    StringUrl& replace(const QString& src, const QString& dest);
    QString toString();
    virtual StringUrl& self() = 0;
protected:
    QString m_url;
};

#endif // STRINGURL_H
