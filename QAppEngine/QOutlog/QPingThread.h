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

class QPingThread : public QObject
{
    Q_OBJECT
public:
    QPingThread();
    void startPing();

private slots:
    void handlePing();

private:
    ObjectTracking *m_objtracking;
};

#endif // PINGTHREAD_H
