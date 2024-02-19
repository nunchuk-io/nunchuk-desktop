#ifndef PINGTHREAD_H
#define PINGTHREAD_H

#include <QThread>
#include <QDebug>

class ObjectTracking : public QThread
{
    Q_OBJECT
public:
    ObjectTracking();
    void run();

signals:
    void pingSignal();
    void feedbackSignal();

public slots:
    void handleFeedback();

private:
    int m_pingCount;
};

class MonitoringThread : public QObject
{
    Q_OBJECT
public:
    MonitoringThread();
    void startTracking();

private slots:
    void handlePing();

private:
    ObjectTracking *m_objtracking;
};

#endif // PINGTHREAD_H
