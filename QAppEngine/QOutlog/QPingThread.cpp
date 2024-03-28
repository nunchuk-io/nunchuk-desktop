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
            DBG_INFO << "NO FEEDBACK IN" << m_pingCount * TIMEOUT_TRACKING << "seconds";
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

QPingThread::QPingThread() : m_objtracking(NULL)
{
    m_objtracking = new ObjectTracking();
    connect(m_objtracking, &ObjectTracking::pingSignal, this, &QPingThread::handlePing);
}

void QPingThread::startPing() {
    if(m_objtracking){
        m_objtracking->start();
    }
}

void QPingThread::handlePing()
{
    DBG_INFO << "MONITORING: Handling ping signal ... >> OK MAIN THREAD FINE";
    emit m_objtracking->feedbackSignal();
}
