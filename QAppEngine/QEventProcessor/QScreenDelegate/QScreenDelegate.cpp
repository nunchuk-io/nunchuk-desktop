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
#include "QScreenDelegate.h"

QScreenQueue QScreenDelegate::cacheScreen;

QScreenDelegate::QScreenDelegate(QQuickItem* rootObject, QQmlContext* context): m_rootObject(rootObject), m_context(context), m_CurrentScreen(NULL)
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

QScreenDelegate::~QScreenDelegate()
{}

bool QScreenDelegate::showScreen(const APPLICATION_STATE *scr, QVariant msg)
{
    bool ret = false;
    if((NULL != m_rootObject) && (NULL != scr) && (m_CurrentScreen != scr)){
        QString path = scr->QmlPath;
        DBG_INFO << path;
        if(!cacheScreen.contains(path)){
            QQmlComponentPtr newComp = getComponent(m_rootObject, path);
            m_context->setContextProperty("QAppScreen", nullptr);
            m_context->setContextProperty("QAppScreen", newComp.data());
            cacheScreen.insert(path, newComp);
            newComp.clear();
        }
        else{
            m_context->setContextProperty("QAppScreen", nullptr);
            m_context->setContextProperty("QAppScreen", cacheScreen[path].data());
        }

        if(cacheScreen[path].data()->isError()){
            DBG_ERROR << "SCREEN_STATE ERROR:" << path << cacheScreen[path].data()->errors();
        }
        else{
            if(NULL != scr){
                scr->funcEntry(msg);
            }
            ret = QMetaObject::invokeMethod(m_rootObject, JS_SCREEN_TRANSITION_FUNCTION/*, Qt::QueuedConnection*/);
            if(NULL != m_CurrentScreen){
                m_CurrentScreen->funcExit(msg);
            }
            m_CurrentScreen = scr;
        }
    }
    return ret;
}

uint QScreenDelegate::getCurrentScreen() const
{
    return (NULL != m_CurrentScreen) ? m_CurrentScreen->id : 0;
}

QQmlComponentPtr QScreenDelegate::getComponent(QObject *parent, QString screenFile) {
    if(NULL != parent){
        QQmlEngine *engine = qmlEngine(parent);
        if(engine) {
            QQmlComponentPtr component = QQmlComponentPtr(new QQmlComponent(engine, screenFile, this));
            QQmlEngine::setObjectOwnership(component.data(), QQmlEngine::CppOwnership);
            return component;
        }
    }
    return QQmlComponentPtr(NULL);
}

