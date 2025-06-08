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
#include "QEventProcessor.h"
#include "QOutlog.h"
#include <QFontDatabase>
#include <QScreen>

bool        QEventProcessor::m_register    = false;
QHash<uint, const APPLICATION_STATE*>     QEventProcessor::m_stateRegisted;
QHash<uint, QHash<uint, STATE_TRIGGER> >  QEventProcessor::m_poolEvt;

QEventProcessor::QEventProcessor() : m_viewer(new QQuickView()), m_scrMng(NULL), m_popMng(NULL), m_RootState(0),
    m_popRequester(0), m_currentSize(QSize(0,0)), m_currentScale(1.0), m_currentFlow(-1)
{
    if(!m_PopupTriger.isEmpty()){
        m_PopupTriger.clear();
    }
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
    connect(this, &QEventProcessor::signalNotifySendEvent,     this, &QEventProcessor::sendEvent,              Qt::QueuedConnection);
    connect(this, &QEventProcessor::signalNotifyToastMessage,  this, &QEventProcessor::slotNotifyToastMessage, Qt::QueuedConnection);
}

QEventProcessor::~QEventProcessor()
{
    m_viewer->deleteLater();
    m_scrMng->deleteLater();
    m_popMng->deleteLater();
}

QEventProcessor *QEventProcessor::instance(){
    static QEventProcessor mInstance;
    return &mInstance;
}

void QEventProcessor::initialized()
{
    QObject::connect(m_viewer, SIGNAL(visibleChanged(bool)), this, SIGNAL(visibleChanged(bool)));
    QObject::connect(m_viewer, SIGNAL(visibleChanged(bool)), this, SLOT(onVisibleChanged(bool)));
    QObject::connect(m_viewer, SIGNAL(widthChanged(int)), this, SLOT(onWidthChanged(int)));
    QObject::connect(m_viewer, SIGNAL(heightChanged(int)), this, SLOT(onHeightChanged(int)));
    m_qmlObj.clear();
    setContextProperty("QAPP_DEVICE_WIDTH", QAPP_WIDTH_EXPECTED);
    setContextProperty("QAPP_DEVICE_HEIGHT", QAPP_HEIGHT_EXPECTED);
    setContextProperty("QMLHandle", QVariant::fromValue(this));
}

void QEventProcessor::initFonts(QStringList &fonts)
{
    for (int i = 0; i < fonts.count(); i++) {
        int fontID = QFontDatabase::addApplicationFont(fonts.at(i));
        QString family = QFontDatabase::applicationFontFamilies(fontID).at(0);
    }

    QFont::insertSubstitution("Lato", "Lato");
    QFont::insertSubstitution("Montserrat", "Montserrat");
}

void QEventProcessor::completed()
{
    loadQml(m_viewer, QUrl(QStringLiteral(MAIN_VIEWPORT_QML)), m_viewer->rootContext());
    m_scrMng = new QScreenDelegate(m_viewer->rootObject(), m_viewer->rootContext());
    m_popMng = new QPopupDelegate(m_viewer->rootObject(), m_viewer->rootContext());

    for (int i = 0; i < m_stateRegisted.keys().count(); i++) {
        if(LAYER::LAYER_BASE == m_stateRegisted[m_stateRegisted.keys().at(i)]->layerbase){
            m_RootState = m_stateRegisted[m_stateRegisted.keys().at(i)]->id;
            m_stateRegisted[m_stateRegisted.keys().at(i)]->funcEntry(QVariant("START APPLICATION"));
            break;
        }
    }
}

bool QEventProcessor::setContextProperty(const QString &str, const QVariant &var)
{
    if(m_viewer->rootContext()){
        m_viewer->rootContext()->setContextProperty(str, var);
        return true;
    }
    return false;
}

void QEventProcessor::sendEvent(uint eventID, QVariant msg)
{
    // CHECK IN CURRENT POPUP
    DBG_INFO << "eventID: " << eventID;
    if(NULL != m_popMng){
        QList<uint> pops = m_popMng->getCurrentPopups();
        for (int i = 0; i < pops.count(); i++) {
            if(m_poolEvt[pops.at(i)].contains(eventID)){
                if(NULL != m_poolEvt[pops.at(i)][eventID].func){
                    m_poolEvt[pops.at(i)][eventID].func(msg);
                }

                if(NULL != m_poolEvt[pops.at(i)][eventID].trans){
                    handleTransition(m_stateRegisted[pops.at(i)], m_poolEvt[pops.at(i)][eventID].trans, msg);
                }
                emit stateChanged();
                return;
            }
        }
    }

    // CHECK IN CURRENT SCREEN
    if(NULL != m_scrMng){
        uint scr = m_scrMng->getCurrentScreen();
        if(m_poolEvt[scr].contains(eventID)){
            if(NULL != m_poolEvt[scr][eventID].func){
                m_poolEvt[scr][eventID].func(msg);
            }

            if(NULL != m_poolEvt[scr][eventID].trans){
                handleTransition(m_stateRegisted[scr], m_poolEvt[scr][eventID].trans, msg);
            }
            emit stateChanged();
            return;
        }
    }

    // CHECK IN ROOT
    if(m_poolEvt[m_RootState].contains(eventID)){
        if(NULL != m_poolEvt[m_RootState][eventID].func){
            m_poolEvt[m_RootState][eventID].func(msg);
        }

        if(NULL != m_poolEvt[m_RootState][eventID].trans){
            handleTransition(m_stateRegisted[m_RootState], m_poolEvt[m_RootState][eventID].trans, msg);
        }
        emit stateChanged();
        return;
    }
}

void QEventProcessor::notifySendEvent(uint eventID, QVariant msg)
{
    emit signalNotifySendEvent(eventID, msg);
}

void QEventProcessor::sendToastMessage(QVariant msg)
{
    emit signalNotifyToastMessage(msg);
}

void QEventProcessor::onWidthChanged(int w)
{
    if(w > m_currentSize.width()){
        DBG_INFO << "FULL SIZE REQUEST" << w;
        setContextProperty("QAPP_DEVICE_WIDTH", m_viewer->geometry().width());
    }
    else{
        DBG_INFO << "ORIGIN SIZE REQUEST" << w;
        setContextProperty("QAPP_DEVICE_WIDTH", m_currentSize.width());
    }
}

void QEventProcessor::onHeightChanged(int h)
{
    if(h > m_currentSize.height()){
        DBG_INFO << "FULL SIZE REQUEST" << h << (double)h/(double)QAPP_HEIGHT_EXPECTED;
        setContextProperty("QAPP_DEVICE_HEIGHT", m_viewer->geometry().height());
        setContextProperty("QAPP_DEVICE_HEIGHT_RATIO", qMin(1.0, (double)h/(double)QAPP_HEIGHT_EXPECTED));
    }
    else{
        DBG_INFO << "ORIGIN SIZE REQUEST" << h ;
        setContextProperty("QAPP_DEVICE_HEIGHT", m_currentSize.height());
    }
}

void QEventProcessor::aboutToQuit(QEventProcessor::QPrivateSignal signal)
{
    DBG_INFO;
    emit stateChanged();
}

void QEventProcessor::doRegisterQML(QObject *objPropose)
{
    m_currentScreen = objPropose;
    m_qmlObj.insert(0, objPropose);
    for (QObject* obj: m_qmlObj) {
        if( m_qmlObj.first() == obj){
            obj->setProperty("isOnTop", true);
            QObject::connect(this, SIGNAL(eventReceiver(int,int)), obj, SIGNAL(eventReceiver(int, int)));
        }
        else{
            obj->setProperty("isOnTop", false);
            this->disconnect(obj);
        }
    }
}

void QEventProcessor::unRegisterQML(QObject *objPropose)
{
    m_qmlObj.removeAll(objPropose);
    for (QObject* obj: m_qmlObj) {
        if( m_qmlObj.first() == obj){
            obj->setProperty("isOnTop", true);
            QObject::connect(this, SIGNAL(eventReceiver(int,int)), obj, SIGNAL(eventReceiver(int, int)));
        }
        else{
            obj->setProperty("isOnTop", false);
            this->disconnect(obj);
        }
    }
}

bool QEventProcessor::setViewerSize(int width, int height)
{
    m_viewer->setMinimumSize(QSize(width, height));
    setContextProperty("QAPP_DEVICE_HEIGHT_RATIO", qMin(1.0, (double)height/(double)QAPP_HEIGHT_EXPECTED));
    m_currentSize = QSize(width, height);
    return true;
}

bool QEventProcessor::show()
{
    bool state = false;
    if(NULL != m_viewer){
        m_viewer->show();
        state = true;
    }
    return state;
}

bool QEventProcessor::hide()
{
    bool state = false;
    if(NULL != m_viewer){
        m_viewer->hide();
        state = true;
    }
    return state;
}

QQmlContext *QEventProcessor::getQmlContext() const
{
    return m_viewer->rootContext();
}

QQuickView *QEventProcessor::getQuickWindow() const
{
    return m_viewer;
}

QList<uint> QEventProcessor::getCurrentStates() const
{
    QList<uint> list ;
    if(NULL != m_scrMng){
        list.append(m_scrMng->getCurrentScreen());
    }

    if(NULL != m_popMng){
        QList<uint> tmp = m_popMng->getCurrentPopups();
        for (int j = 0; j < tmp.count(); ++j) {
            list.append(tmp.at(j));
        }
    }
    return list;
}

void QEventProcessor::registerStates(const STATE_SYSTEM tbl[], uint len)
{
    if(false == m_register){
        if(!m_poolEvt.isEmpty()){
            m_poolEvt.clear();
        }

        if(!m_stateRegisted.isEmpty()){
            m_stateRegisted.clear();
        }

        // Looking for all child
        for (uint i = 0; i < len; i++) {
            m_stateRegisted[tbl[i].id] = tbl[i].state;
            for (uint j = 0; j < tbl[i].size; j++) {
                m_poolEvt[tbl[i].id][tbl[i].trigger[j].evt] = tbl[i].trigger[j];
            }           
        }

        m_register = true;
        DBG_INFO << "THERE ARE " << m_stateRegisted.count() << "STATES REGISTED";
    }
}

int QEventProcessor::currentFlow() const
{
    return m_currentFlow;
}

void QEventProcessor::setCurrentFlow(int currentFlow)
{
    if(m_currentFlow != currentFlow){
        DBG_INFO << currentFlow;
        m_currentFlow = currentFlow;
        emit currentFlowChanged();
    }
}

int QEventProcessor::popupTrigger(int popupId) const
{
    return m_PopupTriger[popupId];
}

void QEventProcessor::setPopupTrigger(int popupId, int trigger)
{
    m_PopupTriger[popupId] = trigger;
}

void QEventProcessor::addImageProvider(const QString &id, QQmlImageProviderBase *provider)
{
    if(m_viewer && m_viewer->engine()){
        m_viewer->engine()->addImageProvider(id, provider);
    }
}

QList<QObject *> QEventProcessor::getQmlObj() const
{
    return m_qmlObj;
}

bool QEventProcessor::closeAllPopup()
{
    bool ret = false;
    if(NULL != m_popMng){
        ret = m_popMng->closeAll();
    }
    return ret;
}

QObject *QEventProcessor::getCurrentScreen() const
{
    return m_currentScreen;
}

uint QEventProcessor::onsRequester() const
{
    return m_popRequester;
}

QQuickItem* QEventProcessor::loadQml(QQuickView *view, const QUrl &url, QQmlContext *context)
{
    auto component = new QQmlComponent(view->engine(), url, view);
    if (component->isError()) {
        QList<QQmlError> errorList = component->errors();
        foreach (const QQmlError &error, errorList) {
            QMessageLogger(error.url().toString().toLatin1().constData(), error.line(), 0).warning() << error;
        }
        return NULL;
    }
    auto *obj = component->create(context);
    if (component->isError()) {
        QList<QQmlError> errorList = component->errors();
        foreach (const QQmlError &error, errorList) {
            QMessageLogger(error.url().toString().toLatin1().constData(), error.line(), 0).warning() << error;
        }
        return NULL;
    }
    view->setContent(url, component, obj);
    return view->rootObject();
}

void QEventProcessor::handleTransition(const APPLICATION_STATE *from, const APPLICATION_STATE *to, QVariant msg)
{
    if(((NULL != from) && (NULL != to))){
        if((LAYER::LAYER_POPUP == to->layerbase) || (LAYER::LAYER_TOAST == to->layerbase)){
            this->showPopup(to->id, msg);
        }
        else if(LAYER::LAYER_SCREEN == to->layerbase)
        {
            if((NULL != m_scrMng) &&
                    (to->id == m_scrMng->getCurrentScreen()) &&
                    (LAYER::LAYER_BASE != from->layerbase) &&
                    (LAYER::LAYER_SCREEN != from->layerbase)&&
                    (LAYER::LAYER_ONSCREEN != from->layerbase))
            {
                if(LAYER::LAYER_ONSCREEN != from->layerbase){
                    this->setOnsRequester(0);
                }
                this->closePopup(from->id, msg);
            }
            else{
                this->closeAllPopup();
                this->showScreen(to->id, msg);
            }
        }
        else if(LAYER::LAYER_ONSCREEN == to->layerbase){
            this->setOnsRequester(from->id);
            this->closeAllPopup();
            this->showPopup(to->id, msg);
        }
        else{
            /* LAYER::LAYER_BASE Do nothing */
            this->closePopup(from->id, msg);
        }
    }
}

bool QEventProcessor::showScreen(uint id, QVariant msg)
{
    bool ret = false;
    if((NULL != m_scrMng) && m_stateRegisted.contains(id)){
        if(LAYER::LAYER_SCREEN == m_stateRegisted[id]->layerbase){
            ret = m_scrMng->showScreen(m_stateRegisted[id], msg);
        }
    }
    return ret;
}

bool QEventProcessor::showPopup(uint id, QVariant msg)
{
    bool ret = false;
    if((NULL != m_popMng) && m_stateRegisted.contains(id)){
        if((LAYER::LAYER_TOAST == m_stateRegisted[id]->layerbase)
                || (LAYER::LAYER_POPUP == m_stateRegisted[id]->layerbase)
                || (LAYER::LAYER_ONSCREEN == m_stateRegisted[id]->layerbase))
        {
            POPUP_DATA pop;
            pop.duration    = m_stateRegisted[id]->duration;
            pop.id          = m_stateRegisted[id]->id;
            pop.QmlPath     = m_stateRegisted[id]->QmlPath;
            pop.funcEntry   = m_stateRegisted[id]->funcEntry;
            pop.funcExit    = m_stateRegisted[id]->funcExit;
            pop.msg         = msg;
            ret = m_popMng->showPopup(pop);
        }
    }
    return ret;
}

bool QEventProcessor::closePopup(uint id, QVariant msg)
{
    bool ret = false;
    if((NULL != m_popMng) && m_stateRegisted.contains(id)){
        if((LAYER::LAYER_TOAST == m_stateRegisted[id]->layerbase)
                || (LAYER::LAYER_POPUP == m_stateRegisted[id]->layerbase)
                || (LAYER::LAYER_ONSCREEN == m_stateRegisted[id]->layerbase))
        {
            POPUP_DATA pop;
            pop.duration    = m_stateRegisted[id]->duration;
            pop.id          = m_stateRegisted[id]->id;
            pop.QmlPath     = m_stateRegisted[id]->QmlPath;
            pop.funcEntry   = m_stateRegisted[id]->funcEntry;
            pop.funcExit    = m_stateRegisted[id]->funcExit;
            pop.msg         = msg;
            ret = m_popMng->closePopup(pop);
        }
    }
    return ret;
}

bool QEventProcessor::showToastMessage(QVariant msg)
{
    if(m_popMng){
        return m_popMng->showToastMessage(msg);
    }
    return false;
}

void QEventProcessor::setOnsRequester(const uint id)
{
    m_popRequester = id;
}

void QEventProcessor::collectGarbage()
{
    if(NULL != m_viewer->engine()){
        m_viewer->engine()->collectGarbage();
    }
}

void QEventProcessor::onVisibleChanged(bool state)
{
    DBG_INFO << "Visibility changed to" << state;
    Q_UNUSED(state);
}

void QEventProcessor::slotNotifyToastMessage(QVariant msg) {
    showToastMessage(msg);
}
