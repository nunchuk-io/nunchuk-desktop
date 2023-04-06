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

#ifndef SCREENMANAGER_H
#define SCREENMANAGER_H

#include <QObject>
#include <QQmlEngine>
#include <QQuickItem>
#include <QQmlContext>

#include "QAppEngine.h"
#include "QScreenQueue.h"

class QScreenManager : public QObject
{
public:
    explicit QScreenManager(QQuickItem* rootObject, QQmlContext* context);
    virtual ~QScreenManager();

    bool showScreen(const APPLICATION_STATE *scr, QVariant msg = QVariant());
    uint getCurrentScreen() const;
private:
    static QScreenQueue cacheScreen;
    QQuickItem          *m_rootObject;
    QQmlContext         *m_context;
    const APPLICATION_STATE *m_CurrentScreen;

private:
    QQmlComponentPtr getComponent(QObject *parent, QString screenFile);
};

#endif // SCREENMANAGER_H
