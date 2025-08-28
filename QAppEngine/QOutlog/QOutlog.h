/**************************************************************************
 * This file is part of the Nunchuk software (https://nunchuk.io/)        *
 * Copyright (C) 2020-2022 Enigmo								          *
 * Copyright (C) 2022 Nunchuk								              *
 *                                                                        *
 * This program is free software; you can redistribute it and/or          *
 * modify it under the terms of the GNU General Public License            *
 * as published by the Free Software Foundation; either version 3         *
 * of the License, or (at your option) any later version.                 *
 *                                                                        *
 * This program is distributed in the hope that it will be useful,        *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 * GNU General Public License for more details.                           *
 *                                                                        *
 * You should have received a copy of the GNU General Public License      *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.  *
 *                                                                        *
 **************************************************************************/

#ifndef QQOUTLOG_H
#define QQOUTLOG_H

#include <QString>
#include <QSharedPointer>
#include <QtCore/qdir.h>
#include <QtCore/qstandardpaths.h>
#include <QtMsgHandler>
#include <QMessageLogContext>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QObject>
#include <QString>
#include <QQmlError>
#include <QDateTime>
#include <QElapsedTimer>
#include <QMutex>
#include <QMutexLocker>
#include <QThread>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDateTime>

enum class LOG_LEVEL : int
{
    LOG_FATAL = 0,
    LOG_ERROR,
    LOG_WARN,
    LOG_INFO,
};

#if !defined(__PRETTY_FUNCTION__) && !defined(__GNUC__)
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

#define DBG_FATAL    QOutlog().begin(LOG_LEVEL::LOG_FATAL) << '[' << QDateTime::currentDateTime() << ']' << '[' << __PRETTY_FUNCTION__ << "][" << __LINE__ << ']'
#define DBG_ERROR    QOutlog().begin(LOG_LEVEL::LOG_ERROR) << '[' << QDateTime::currentDateTime() << ']' << '[' << __PRETTY_FUNCTION__ << "][" << __LINE__ << ']'
#define DBG_WARN     QOutlog().begin(LOG_LEVEL::LOG_WARN)  << '[' << QDateTime::currentDateTime() << ']' << '[' << __PRETTY_FUNCTION__ << "][" << __LINE__ << ']'
#define DBG_INFO     QOutlog().begin(LOG_LEVEL::LOG_INFO)  << '[' << QDateTime::currentDateTime() << ']' << '[' << __PRETTY_FUNCTION__ << "][" << __LINE__ << ']'
#define DBG_QT_MSG   QOutlog().begin(LOG_LEVEL::LOG_ERROR)
#define DBG_FUNCTION_TIME_INFO     QOutlog().begin(LOG_LEVEL::LOG_INFO)  << '[' << QDateTime::currentDateTime() << ']' << "[ Function time ]"

class QOutlog
{
public:
    explicit QOutlog();
    virtual ~QOutlog();

    // info log
    QOutlog &begin(LOG_LEVEL level);

    inline QOutlog &operator<<(bool t) { mStream << (t ? "true" : "false") << ' '; return *this;}
    inline QOutlog &operator<<(QVariant t) { mStream << t.toString() << ' '; return *this; }
    inline QOutlog &operator<<(char t) { mStream << t << ' '; return *this; }
    inline QOutlog &operator<<(QChar t) { mStream << t << ' '; return *this; }
    inline QOutlog &operator<<(signed short t) { mStream << t << ' '; return *this; }
    inline QOutlog &operator<<(unsigned short t) { mStream << t << ' '; return *this; }
    inline QOutlog &operator<<(signed int t) { mStream << t << ' '; return *this; }
    inline QOutlog &operator<<(unsigned int t) { mStream << t << ' '; return *this; }
    inline QOutlog &operator<<(signed long t) { mStream << t << ' '; return *this; }
    inline QOutlog &operator<<(unsigned long t) { mStream << t << ' '; return *this; }
    inline QOutlog &operator<<(qint64 t) { mStream << t << ' '; return *this; }
    inline QOutlog &operator<<(quint64 t) { mStream << t << ' '; return *this; }
    inline QOutlog &operator<<(float t) { mStream << t << ' '; return *this; }
    inline QOutlog &operator<<(double t) { mStream << t << ' '; return *this; }
    inline QOutlog &operator<<(const char* t) { mStream << QString::fromUtf8(t) << ' '; return *this; }
    inline QOutlog &operator<<(const QString & t) { mStream << t << ' '; return *this; }
    inline QOutlog &operator<<(const std::string & t) { mStream << QString::fromStdString(t) << ' '; return *this; }
    inline QOutlog &operator<<(const QStringRef & t) { return operator<<(t.toString()); }
    inline QOutlog &operator<<(const QQmlError & t) { return operator<<(t.toString()); }
    inline QOutlog &operator<<(const QList<QQmlError> &list){
        mStream << '(';
        for (typename QList<QQmlError>::size_type i = 0; i < list.count(); ++i) {
            if (i) {mStream << ",";}
            mStream << list.at(i).toString();
        }
        mStream << ')' << ' ';
        return *this;
    }

    template <class T>
    inline QOutlog &operator<<(const QList<T> &list)
    {
        mStream << '(';
        for (typename QList<T>::size_type i = 0; i < list.count(); ++i) {
            if (i){mStream << ",";}
            mStream << list.at(i);
        }
        mStream << ')' << ' ';
        return *this;
    }

    template <typename T>
    inline QOutlog &operator<<(const QVector<T> &vec)
    {
        mStream << "QVector";
        return operator<<(vec.toList());
    }

    template <class aKey, class aT>
    inline QOutlog &operator<<(const QMap<aKey, aT> &map)
    {
        mStream << "QMap(";
        for (typename QMap<aKey, aT>::const_iterator it = map.constBegin(); it != map.constEnd(); ++it) {
            mStream << '(' << it.key() << "," << it.value() << ')';
        }
        mStream << ')' << ' ';
        return *this;
    }

    template <class aKey>
    inline QOutlog &operator<<(const QMap<aKey, QVariant> &map)
    {
        mStream << "QMap(";
        for (typename QMap<aKey, QVariant>::const_iterator it = map.constBegin(); it != map.constEnd(); ++it) {
            mStream << '(' << it.key() << "," << it.value().toString() << ')';
        }
        mStream << ')' << ' ';
        return *this;
    }

    template <class aKey, class aT>
    inline QOutlog &operator<<(const QHash<aKey, aT> &hash)
    {
        mStream << "QHash(";
        for (typename QHash<aKey, aT>::const_iterator it = hash.constBegin(); it != hash.constEnd(); ++it){
            mStream << '(' << it.key() << "," << it.value() << ')';
        }
        mStream << ')' << ' ';
        return *this;
    }

    template <class T1, class T2>
    inline QOutlog &operator<<(const QPair<T1, T2> &pair)
    {
        mStream << "QPair(" << pair.first << ',' << pair.second << ')' << ' ';
        return *this;
    }

    template <typename T>
    inline QOutlog &operator<<(const QSet<T> &set)
    {
        mStream << "QSet" << ' ';
        return operator<<(set.toList());
    }

    template <class T>
    inline QOutlog &operator<<(const QContiguousCache<T> &cache)
    {
        mStream << "QContiguousCache(";
        for (int i = cache.firstIndex(); i <= cache.lastIndex(); ++i) {
            mStream << cache[i];
            if (i != cache.lastIndex())
                mStream << ", ";
        }
        mStream << ')' << ' ';
        return *this;
    }

    template <class T>
    inline QOutlog &operator<<(const QSharedPointer<T> &ptr)
    {
        mStream << "QSharedPointer(" << ptr.data() << ")" << ' ';
        return *this;
    }

    inline QOutlog &operator<<(const QByteArray &t)
    {
        for(int i = 0; i < t.size(); i++) {
            mStream << QString("0x%1 ").arg((uchar)t.at(i), 2, 16, QChar('0'));
        }
        mStream  << ' ';
        return *this;
    }

    inline QOutlog &operator<<(std::nullptr_t) { mStream << "(nullptr)" << ' ';; return *this; }
    inline QOutlog &operator<<(const void * t) { mStream << t << ' '; return *this; }
    inline QOutlog &operator<<(const QJsonObject & t) {
        QJsonDocument doc(t);
        QString strJson(doc.toJson(QJsonDocument::Compact));
        return operator<<(strJson);
    }
    inline QOutlog &operator<<(const QJsonArray & t) {
        QJsonDocument doc(t);
        QString strJson(doc.toJson(QJsonDocument::Compact));
        return operator<<(strJson);
    }
    inline QOutlog &operator<<(QUrl t) { mStream << t.toString() << ' '; return *this; }

    template <class T>
    inline QOutlog &operator<<(const std::vector<T> &t)
    {
        for(int i = 0; i < (int)t.size(); i++) {
            mStream << QString("0x%1 ").arg((uchar)t.at(i), 2, 16, QChar('0'));
        }
        mStream  << ' ';
        return *this;
    }

    inline QOutlog &operator<<(const QVariantList &list)
    {
        mStream << '(';
        for (int i = 0; i < list.count(); ++i) {
            if (i) { mStream << ","; }
            mStream << list.at(i).toString();  // Chuyển QVariant thành chuỗi để hiển thị
        }
        mStream << ')' << ' ';
        return *this;
    }
private:
    QTextStream mStream;
    QString     mLogString;
    LOG_LEVEL   m_level;
};

class LogVerbose
{
public:
    static void verboseMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &_msg);
    explicit LogVerbose();
    virtual ~LogVerbose();
};

class LogWriteToFile
{
public:
    void writeLog(const QString& log);
    LogWriteToFile(const QString &file);
    ~LogWriteToFile();
private:
    QMutex mutex;
    QSharedPointer<QFile> logfile;
};

#ifndef DEFINE_SAFE_DELETE
#define DEFINE_SAFE_DELETE
template< class T > inline void safeDelete( T*& pVal )
{
    if(NULL != pVal){
        delete pVal;
        pVal = NULL;
    }
}
#endif

class QFunctionTime
{
public:
    QFunctionTime(QString _func);
    ~QFunctionTime();
private:
    QString mFunc;
    QElapsedTimer mTime;
};

template<typename T>
class DeleterWithDeleteLater
{
public:
    void operator()(T* obj) const
    {
        obj->deleteLater();
    }
};

template<typename T>
class OurSharedPointer : public QSharedPointer<T>
{
public:
    OurSharedPointer(T* ptr = nullptr) : QSharedPointer<T>(ptr, DeleterWithDeleteLater<T>()) {}
    bool isValid() const { return !this->isNull(); }
};

#endif // QQOutlog_H
