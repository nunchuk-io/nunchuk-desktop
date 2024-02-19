#include "QPingThread.h"
#include "QOutlog.h"

#define TIMEOUT_TRACKING 5000

ObjectTracking::ObjectTracking() : m_pingCount(0)
{
    connect(this, &ObjectTracking::feedbackSignal   , this, &ObjectTracking::handleFeedback);
}

void ObjectTracking::run()
{
    while (true)
    {
        msleep(TIMEOUT_TRACKING);
        if(m_pingCount > 4){
            DBG_INFO << "MONITORING: NO FEEDBACK FROM MAINTHREAD IN" << m_pingCount * TIMEOUT_TRACKING << "seconds";
            abort();
            // trace bt with: thread apply all bt
        }
        m_pingCount++;
        emit pingSignal();
    }
}

void ObjectTracking::handleFeedback()
{
    m_pingCount = 0;
}

MonitoringThread::MonitoringThread() : m_objtracking(NULL)
{
    m_objtracking = new ObjectTracking();
    connect(m_objtracking, &ObjectTracking::pingSignal, this, &MonitoringThread::handlePing);
}

void MonitoringThread::startTracking() {
    if(m_objtracking){
        m_objtracking->start();
    }
}

void MonitoringThread::handlePing()
{
    DBG_INFO << "MONITORING: Handling ping signal ... >> OK MAIN THREAD FINE";
    emit m_objtracking->feedbackSignal();
}
