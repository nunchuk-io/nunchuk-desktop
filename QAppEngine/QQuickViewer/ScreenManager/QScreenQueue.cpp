#include "QScreenQueue.h"
#include <QMutexLocker>
QScreenQueue::QScreenQueue(uint max_deep, QObject *parent) : QObject(parent),m_MaxDeep(max_deep)
{

}

QScreenQueue::~QScreenQueue()
{

}
bool QScreenQueue::contains(QString& id){
    QMutexLocker locker(&m_mutex);
    for(int i = 0; i < m_screenList.size(); i++){
        if( m_screenList.at(i).scrName ==  id)
            return  true;
    }
    return false;
}

QQmlComponentPtr QScreenQueue::getScreen(QString& scrName){
    QMutexLocker locker(&m_mutex);
    for(int i = 0; i < m_screenList.size(); i++){
        if( m_screenList.at(i).scrName ==  scrName)
            return  m_screenList.at(i).qml;
    }
    return QQmlComponentPtr();
}

void QScreenQueue::insert(QString& scrName, QQmlComponentPtr qml){
    QMutexLocker locker(&m_mutex);
    if(m_screenList.count() >= m_MaxDeep){
        m_screenList.removeLast();
    }
    SCREEN_T t = {scrName, qml};
    m_screenList.insert(0,t);
}


void QScreenQueue::setMaxDeep(int max_deep){
    QMutexLocker locker(&m_mutex);
    m_MaxDeep = max_deep;
    if(m_MaxDeep < m_screenList.count()){
        int gap = m_screenList.count() - m_MaxDeep;
        for(int i =0; i < gap; i++){
            m_screenList.removeLast();
        }
    }
}

QQmlComponentPtr QScreenQueue::operator[](QString scrName){
    return getScreen(scrName);
}

void QScreenQueue::initScreenQueue()
{
    QMutexLocker locker(&m_mutex);
    while (m_screenList.count() > 1) {
        m_screenList.removeLast();
    }
}
