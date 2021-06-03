#ifndef QPOPUPMANAGER_H
#define QPOPUPMANAGER_H

#include <QObject>
#include <QStack>
#include <QSharedPointer>
#include <QTimer>
#include <QQmlEngine>
#include <QQuickItem>
#include <QQmlContext>
#include "QAppEngine.h"

class PopupObject;
class QPopupManager;
typedef QSharedPointer<PopupObject> PopupObjectPtr;

class QPopupManager : public QObject
{
    Q_OBJECT
public:
    explicit QPopupManager(QQuickItem* rootObject, QQmlContext* context);
    virtual ~QPopupManager();
    QList<uint> getCurrentPopups() const;
    bool showPopup(POPUP_DATA p);
    bool closePopup(POPUP_DATA p);
    bool closeAll();
private:
    QQuickItem              *m_rootObject;
    QQmlContext             *m_context;
    QList<PopupObjectPtr>   m_listPopup;
    QStringList             m_QmlOder;
    void qmlSyncup();

public slots:
    void onClosePopupTimeout(uint id);
};

class PopupObject: public QTimer
{
    Q_OBJECT
public:
    explicit PopupObject(POPUP_DATA p);
    virtual ~PopupObject();
    POPUP_DATA popupInfo() const;
private:
    POPUP_DATA   m_popInfo;
signals:
    void closePopupTimeout(uint id);
public slots:
    void timeout_exec();
};


#endif // QPOPUPMANAGER_H
