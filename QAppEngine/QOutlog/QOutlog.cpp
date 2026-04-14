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
#include "QOutlog.h"
#include <iostream>
#include <QFileInfo>
#ifdef Q_OS_UNIX
#include <unistd.h>
#endif

#ifdef ENABLE_OUTLOG
const char *debugForDev = "debugForDev";
static const QString logfilePath = []() -> QString {
    QString logDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(logDir);

#ifdef Q_OS_UNIX
    bool debugFileExists = (access(debugForDev, F_OK) == 0);
#else
    bool debugFileExists = QFileInfo(QString::fromUtf8(debugForDev)).exists();
#endif
    return debugFileExists ? "logfile_nunchuck-client-qt.log" : logDir + "/logfile_nunchuck-client-qt.log";
}();

static LogWriteToFile g_writer(logfilePath);
#endif

QOutlog::QOutlog()
{
    mStream.setString(&mLogString);
}

QOutlog::~QOutlog()
{
#ifdef ENABLE_OUTLOG
    if(mLogString.endsWith(QLatin1Char(' '))) {
        mLogString.chop(1);
    }
    if (!mLogString.contains("QCoreApplication")) {
        std::cout << mLogString.toStdString() << std::endl;
        g_writer.writeLog(mLogString);
    }
#endif
}

QOutlog &QOutlog::begin(LOG_LEVEL level)
{
    m_level = level;
#ifdef USE_3RD_DEBUG
    // To request write log to the 3rd party of debuger (such as dlt)
#else
    mStream << "[#" << QThread::currentThread()->currentThreadId() << ']';
#endif
    return *this;
}

LogVerbose::LogVerbose()
{
#ifdef USE_3RD_DEBUG
    // To request write log to the 3rd party of debuger (such as dlt)
#else
    // qInstallMessageHandler(this->verboseMessageHandler); // Comment tempo
    qInstallMessageHandler([](QtMsgType type, const QMessageLogContext& context, const QString& msg) {
        const char* level = "";
        switch (type) {
        case QtDebugMsg:    level = "Debug"; break;
        case QtInfoMsg:     level = "Info"; break;
        case QtWarningMsg:  level = "Warning"; break;
        case QtCriticalMsg: level = "Critical"; break;
        case QtFatalMsg:    level = "Fatal"; break;
        }

        fprintf(stderr, "[Qt][%s] %s\n", level, msg.toUtf8().constData());

        if (context.file || context.line || context.function) {
            fprintf(stderr, "  at %s:%d (%s)\n",
                    context.file ? context.file : "unknown",
                    context.line,
                    context.function ? context.function : "unknown");
        }

        if (context.category) {
            fprintf(stderr, "  category: %s\n", context.category);
        }
    });

    // backtrace info
    // qInstallMessageHandler([](QtMsgType type, const QMessageLogContext& context, const QString& msg) {
    //     QByteArray text = msg.toUtf8();
    //     fprintf(stderr, "[Qt] %s\n", text.constData());

    //     if (msg.contains("Unexpected null receiver")) {
    //         void* frames[64];
    //         int n = backtrace(frames, 64);
    //         char** syms = backtrace_symbols(frames, n);

    //         fprintf(stderr, "=== stack trace for null receiver ===\n");
    //         for (int i = 0; i < n; ++i) {
    //             fprintf(stderr, "%s\n", syms[i]);
    //         }
    //         fprintf(stderr, "=====================================\n");

    //         free(syms);
    //     }
    // });
#endif
}

LogVerbose::~LogVerbose()
{

}

void LogVerbose::verboseMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &_msg)
{
    DBG_QT_MSG << QString("[%1][%2] %5").arg(context.function).arg(context.line).arg(_msg) ;
}

LogVerbose g_verbose;

void LogWriteToFile::writeLog(const QString &log)
{
    mutex.lock();
    QTextStream st(logfile.data());
    st.setCodec("UTF-8");
    st << log << endl;
    st.flush();
    mutex.unlock();
}

LogWriteToFile::LogWriteToFile(const QString &file)
{
#ifdef ENABLE_OUTLOG
    QFile::remove(file);
    logfile = QSharedPointer<QFile>(new QFile(file));
    if (!logfile->open(QIODevice::WriteOnly)) {
        qWarning() << "Cannot open log file" << file << logfile->errorString();
    } else {
        qDebug() << "Log file created fresh:" << file;
    }
#endif
}

LogWriteToFile::~LogWriteToFile()
{

}

QFunctionTime::QFunctionTime(QString _func) : mFunc(_func)
{
    mTime.start();
}

QFunctionTime::~QFunctionTime()
{
#ifdef ENABLE_OUTLOG
    DBG_FUNCTION_TIME_INFO << QString("%1 takes %2 ms").arg(mFunc).arg(mTime.elapsed());
#endif
}
