#include "QOutlog.h"
#include <iostream>

QOutlog::QOutlog()
{
    mStream.setString(&mLogString);
}

QOutlog::~QOutlog()
{
    if(mLogString.endsWith(QLatin1Char(' '))) {
        mLogString.chop(1);
    }
    std::cout << mLogString.toStdString() << std::endl;
    g_writer.writeLog(mLogString);
}

QOutlog &QOutlog::begin(LOG_LEVEL level)
{
    m_level = level;
#ifdef USE_3RD_DEBUG
    // To request write log to the 3rd party of debuger (such as dlt)
#else
    mStream << "[#" << QThread::currentThreadId() << ']';
#endif
    return *this;
}

LogVerbose::LogVerbose()
{
#ifdef USE_3RD_DEBUG
    // To request write log to the 3rd party of debuger (such as dlt)
#else
    qInstallMessageHandler(this->verboseMessageHandler); // Comment tempo
#endif
}

LogVerbose::~LogVerbose()
{

}

void LogVerbose::verboseMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &_msg)
{
    DBG_QT_MSG << QString("[%1][%2][%3][%4] %5").arg(context.category).arg(context.file).arg(context.function).arg(context.line).arg(_msg) ;
}

LogVerbose g_verbose;

void LogWriteToFile::writeLog(const QString &log)
{
    QTextStream st(logfile.data());
    st.setCodec("UTF-8");
    st << log << endl;
    st.flush();
}

LogWriteToFile::LogWriteToFile(const QString &file)
{
    logfile = QSharedPointer<QFile>(new QFile(file));
    logfile.data()->open(QIODevice::WriteOnly);
}

LogWriteToFile::~LogWriteToFile()
{

}
