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
#include "QPopupDelegate.h"

QPopupDelegate::QPopupDelegate(QQuickItem *rootObject, QQmlContext *context): m_rootObject(rootObject), m_context(context)
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
    if(!m_listPopup.isEmpty()){
        m_listPopup.clear();
    }
    if(!m_QmlOder.isEmpty()){
        m_QmlOder.clear();
    }
}

QPopupDelegate::~QPopupDelegate()
{

}

QList<uint> QPopupDelegate::getCurrentPopups() const
{
    QList<uint> list;
    for (int i = 0; i < m_listPopup.count(); i++) {
        list.append(m_listPopup.at(i).data()->popupInfo().id);
    }
    return list;
}

bool QPopupDelegate::showPopup(POPUP_DATA p)
{
    int cIdx = -1;
    bool existed = false;
    for (int i = 0; i < m_listPopup.count(); i++)
    {
        if(p.id == m_listPopup.at(i).data()->popupInfo().id){
            cIdx = i;
            existed = true;
            break;
        }
    }

    if(!existed){
        PopupObjectPtr popup = PopupObjectPtr(new PopupObject(p));
        m_listPopup.append(popup);
        if(LIMIT::NONE != popup.data()->popupInfo().duration){
            popup.data()->start((int)p.duration);
        }
        QObject::connect(popup.data() , SIGNAL(closePopupTimeout(uint)),this, SLOT(onClosePopupTimeout(uint)), Qt::QueuedConnection);
    }
    else{
        if(cIdx != (m_listPopup.count() - 1)){
            m_listPopup.move(cIdx, m_listPopup.count() - 1);
        }

        m_listPopup.last().data()->start((int)p.duration);
    }

    // SHOW QML
    qmlSyncup();
    if((NULL != p.funcEntry) && !existed){
        p.funcEntry(p.msg);
    }
    return true;
}

bool QPopupDelegate::closePopup(POPUP_DATA p)
{
    bool ret = false;
    for (int i = 0; i < m_listPopup.count(); i++) {
        if(p.id == m_listPopup.at(i).data()->popupInfo().id){
            if(m_listPopup.at(i).data()->isActive()){
                m_listPopup.at(i).data()->stop();
            }
            ret = true;
            onClosePopupTimeout(m_listPopup.at(i).data()->popupInfo().id);
        }
    }
    return ret;
}

bool QPopupDelegate::closeAll()
{
    for (int i = 0; i < m_listPopup.count(); i++) {
        if(m_listPopup.at(i).data()->isActive()){
            m_listPopup.at(i).data()->stop();
        }
        onClosePopupTimeout(m_listPopup.at(i).data()->popupInfo().id);
    }
    m_listPopup.clear();
    return m_listPopup.isEmpty();
}

bool QPopupDelegate::showToastMessage(QVariant msg)
{
    if(m_rootObject){
        QObject *toastHost = m_rootObject->findChild<QObject*>("toastHost");
        if (!toastHost) {
            DBG_WARN << "ToastHost not found";
            return false;
        }
        return QMetaObject::invokeMethod(toastHost, JS_TOASTS_TRANSITION_FUNCTION, Q_ARG(QVariant, msg));
    }
    return false;
}

void QPopupDelegate::qmlSyncup()
{
    QStringList m_PopupQmlData;
    m_PopupQmlData.clear();

    for (int i = 0; i < m_listPopup.count(); i++) {
        m_PopupQmlData << m_listPopup.at(i).data()->popupInfo().QmlPath;
    }

    if(m_QmlOder != m_PopupQmlData){
        m_QmlOder.clear();
        m_QmlOder = m_PopupQmlData;
        DBG_INFO << m_QmlOder;
        QObject *popupHost = m_rootObject->findChild<QObject*>("popupHost");
        if (!popupHost) {
            DBG_WARN << "PopupHost not found";
            return;
        }
        QMetaObject::invokeMethod(popupHost, JS_POPUPS_TRANSITION_FUNCTION, Q_ARG(QVariant, QVariant::fromValue(m_QmlOder)), Q_ARG(QVariant,QVariant::fromValue( m_QmlOder.count())));
    }
    m_PopupQmlData.clear();
}

void QPopupDelegate::onClosePopupTimeout(uint id)
{
    QMutableListIterator<PopupObjectPtr> i(m_listPopup);
    while (i.hasNext()) {
        PopupObjectPtr bs = i.next();
        if(id == bs.data()->popupInfo().id){
            i.remove();
        }
    }
    qmlSyncup();
}

PopupObject::PopupObject(POPUP_DATA p): m_popInfo(p)
{
    if(LIMIT::NONE != p.duration){
        QObject::connect(this , SIGNAL(timeout()),this, SLOT(timeout_exec()));
    }
}

PopupObject::~PopupObject()
{
    if(NULL != m_popInfo.funcEntry){
        m_popInfo.funcExit(m_popInfo.msg);
    }
    stop();
}

POPUP_DATA PopupObject::popupInfo() const
{
    return m_popInfo;
}

void PopupObject::timeout_exec()
{
    this->stop();
    emit closePopupTimeout(m_popInfo.id);
}
