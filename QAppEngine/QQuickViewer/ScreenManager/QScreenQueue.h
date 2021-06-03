#ifndef QSCREENQUEUE_H
#define QSCREENQUEUE_H

#include <QObject>
#include <QQmlComponent>
#include <QSharedPointer>
#include <QMutex>

#define MAX_DEEP_LIMIT 10

typedef QSharedPointer<QQmlComponent> QQmlComponentPtr;
struct SCREEN_T{
    QString             scrName;
    QQmlComponentPtr    qml;
};

class QScreenQueue : public QObject
{
    Q_OBJECT
    QMutex          m_mutex;
protected:
    int             m_MaxDeep;
    QList<SCREEN_T> m_screenList;
public:
    explicit QScreenQueue(uint max_deep = MAX_DEEP_LIMIT, QObject *parent = 0);
    virtual ~QScreenQueue();
    bool contains(QString& id);
    QQmlComponentPtr getScreen(QString& scrName);
    void insert(QString& scrName, QQmlComponentPtr qml);
    void setMaxDeep(int max_deep);
    QQmlComponentPtr operator[](QString scrName);
    void initScreenQueue();
};

#endif // QSCREENQUEUE_H
