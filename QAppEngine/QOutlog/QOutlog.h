#ifndef QQOUTLOG_H
#define QQOUTLOG_H

#include <QString>
#include <QSharedPointer>
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
#include <QDebug>

enum class LOG_LEVEL : int
{
    LOG_FATAL = 0,
    LOG_ERROR,
    LOG_WARN,
    LOG_INFO,
};

#define DBG_FATAL    QOutlog().begin(LOG_LEVEL::LOG_FATAL) << '[' << QDateTime::currentDateTime() << ']' << '[' << __FUNCTION__ << "][" << __LINE__ << ']'
#define DBG_ERROR    QOutlog().begin(LOG_LEVEL::LOG_ERROR) << '[' << QDateTime::currentDateTime() << ']' << '[' << __FUNCTION__ << "][" << __LINE__ << ']'
#define DBG_WARN     QOutlog().begin(LOG_LEVEL::LOG_WARN)  << '[' << QDateTime::currentDateTime() << ']' << '[' << __FUNCTION__ << "][" << __LINE__ << ']'
#define DBG_INFO     QOutlog().begin(LOG_LEVEL::LOG_INFO)  << '[' << QDateTime::currentDateTime() << ']' << '[' << __FUNCTION__ << "][" << __LINE__ << ']'
#define DBG_QT_MSG   QOutlog().begin(LOG_LEVEL::LOG_ERROR) << '[' << QDateTime::currentDateTime() << ']' << "[QtMsg]"

static const QString logfilePath = "logfile_nunchuck-client-qt.log";

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
        mStream << "QPair(" << pair.first << ',' << pair.second << ')';
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

    inline QOutlog &operator<<(std::nullptr_t) { mStream << "(nullptr)"; return *this; }
    inline QOutlog &operator<<(const void * t) { mStream << t; return *this; }
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
    QSharedPointer<QFile> logfile;
};

static LogWriteToFile g_writer(logfilePath);

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

class FuncTime
{
public:
    FuncTime(QString _func) : mFunc(_func)
    {
        mTime.start();
    }

    ~FuncTime()
    {
        DBG_INFO << QString("%1 takes %2 ms").arg(mFunc).arg(mTime.elapsed());
    }
private:
    QString mFunc;
    QElapsedTimer mTime;
};

#endif // QQOutlog_H
