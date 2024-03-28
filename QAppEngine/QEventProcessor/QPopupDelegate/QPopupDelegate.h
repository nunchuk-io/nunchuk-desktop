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

#ifndef QPOPUPDELEGATE_H
#define QPOPUPDELEGATE_H

#include <QObject>
#include <QStack>
#include <QSharedPointer>
#include <QTimer>
#include <QQmlEngine>
#include <QQuickItem>
#include <QQmlContext>
#include "QAppEngine.h"

class PopupObject;
class QPopupDelegate;
typedef QSharedPointer<PopupObject> PopupObjectPtr;

class QPopupDelegate : public QObject
{
    Q_OBJECT
public:
    explicit QPopupDelegate(QQuickItem* rootObject, QQmlContext* context);
    virtual ~QPopupDelegate();
    QList<uint> getCurrentPopups() const;
    bool showPopup(POPUP_DATA p);
    bool closePopup(POPUP_DATA p);
    bool closeAll();
    bool showToastMessage(QVariant msg);
private:
    QQuickItem              *m_rootObject;
    QQmlContext             *m_context;
    QList<PopupObjectPtr>   m_listPopup;
    QStringList             m_QmlOder;
    void qmlSyncup();

public slots:
    void onClosePopupTimeout(uint id);
};

class PopupObject: public QTimer
{
    Q_OBJECT
public:
    explicit PopupObject(POPUP_DATA p);
    virtual ~PopupObject();
    POPUP_DATA popupInfo() const;
private:
    POPUP_DATA   m_popInfo;
signals:
    void closePopupTimeout(uint id);
public slots:
    void timeout_exec();
};


#endif // QPOPUPDELEGATE_H
