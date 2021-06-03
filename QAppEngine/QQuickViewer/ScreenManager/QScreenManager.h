#ifndef SCREENMANAGER_H
#define SCREENMANAGER_H

#include <QObject>
#include <QQmlEngine>
#include <QQuickItem>
#include <QQmlContext>

#include "QAppEngine.h"
#include "QScreenQueue.h"

class QScreenManager : public QObject
{
public:
    explicit QScreenManager(QQuickItem* rootObject, QQmlContext* context);
    virtual ~QScreenManager();

    bool showScreen(const APPLICATION_STATE *scr, QVariant msg = QVariant());
    uint getCurrentScreen() const;
private:
    static QScreenQueue cacheScreen;
    QQuickItem          *m_rootObject;
    QQmlContext         *m_context;
    const APPLICATION_STATE *m_CurrentScreen;

private:
    QQmlComponentPtr getComponent(QObject *parent, QString screenFile);
};

#endif // SCREENMANAGER_H
