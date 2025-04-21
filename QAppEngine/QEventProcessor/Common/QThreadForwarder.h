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

#ifndef QTHREADFORWARDER_H
#define QTHREADFORWARDER_H

#include <QObject>
#include <QCoreApplication>
#include "QOutlog.h"
class QThreadForwarder : public QObject
{
    Q_OBJECT
public:
    static QThreadForwarder* instance() {
        static QThreadForwarder forwarder;
        return &forwarder;
    }

    template <typename Func>
    void forwardInQueuedConnection(Func func) {
        if (QThread::currentThread() == qApp->thread()) {
            func();
        }
        else {
            if (qApp) {
                std::function<void()> safeFunc = std::move(func);
                QMetaObject::invokeMethod(qApp, [safeFunc]() { safeFunc(); }, Qt::QueuedConnection);
            } else {
                DBG_ERROR << "QCoreApplication instance is null!";
            }
        }
    }

    template <typename Func>
    void forwardInBlockingQueuedConnection(Func func)
    {
        if (QThread::currentThread() == qApp->thread()) {
            func();
        }
        else {
            if (qApp) {
                std::function<void()> safeFunc = std::move(func);
                QMetaObject::invokeMethod(qApp, [safeFunc]() { safeFunc(); }, Qt::BlockingQueuedConnection);
            } else {
                DBG_ERROR << "QCoreApplication instance is null!";
            }
        }
    }

private:
    QThreadForwarder() {}
    QThreadForwarder(const QThreadForwarder&) = delete;
    QThreadForwarder& operator=(const QThreadForwarder&) = delete;
};

#endif // QTHREADFORWARDER_H
