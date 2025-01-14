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

QOutlog::QOutlog()
{
    mStream.setString(&mLogString);
}

QOutlog::~QOutlog()
{
#ifndef RELEASE_MODE
    if(mLogString.endsWith(QLatin1Char(' '))) {
        mLogString.chop(1);
    }
    if (!mLogString.contains("QCoreApplication")) {
        std::cout << mLogString.toStdString() << std::endl;
    }
//    g_writer.writeLog(mLogString);
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
    qInstallMessageHandler(this->verboseMessageHandler); // Comment tempo
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
#ifndef RELEASE_MODE
    logfile = QSharedPointer<QFile>(new QFile(file));
    logfile.data()->open(QIODevice::WriteOnly);
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
#ifndef RELEASE_MODE
    DBG_FUNCTION_TIME_INFO << QString("%1 takes %2 ms").arg(mFunc).arg(mTime.elapsed());
#endif
}
