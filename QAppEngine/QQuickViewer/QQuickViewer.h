#ifndef QQUICKVIEWER_H
#define QQUICKVIEWER_H

#include <QObject>
#include <QGuiApplication>
#include <QQuickView>
#include <QQmlEngine>
#include <QQmlContext>
#include <QQmlComponent>

#include "QAppEngine.h"

class QQuickViewer : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int currentFlow READ currentFlow WRITE setCurrentFlow NOTIFY currentFlowChanged)

public:
    static QQuickViewer *instance();
    QQuickViewer(QQuickViewer &other) = delete;
    QQuickViewer(QQuickViewer const &other) = delete;
    void operator=(const QQuickViewer &other) = delete;

    void initialized();
    void initFonts(QStringList &fonts);
    void completed();
    bool registerContextProperty(const QString &str, const QVariant &var);
    QList<QString> contextPropertiesRegisted() const;
    bool updateContextProperty(const QString &str, const QVariant &var);
    Q_INVOKABLE void doRegisterQML(QObject* objPropose);
    Q_INVOKABLE void unRegisterQML(QObject* objPropose);
    Q_INVOKABLE uint onsRequester() const;
    bool setViewerSize(int width, int height);
    bool show();
    bool hide();
    QQmlContext *getQmlContext() const;
    QQuickView *getQuickWindow() const;
    QList<uint> getCurrentStates() const;
    static void registerStates(const STATE_SYSTEM tbl[], uint len);
    int currentFlow() const;
    void setCurrentFlow(int currentFlow);
    int popupTrigger(int popupId) const;
    void setPopupTrigger(int popupId, int trigger);
    void addImageProvider(const QString &id, QQmlImageProviderBase *provider);
    QList<QObject *> getQmlObj() const;
    bool closeAllPopup();
    QObject *getCurrentScreen() const;
private:
    static bool                                         m_register;
    static QHash<uint, const APPLICATION_STATE*>        m_stateRegisted;
    static QHash<uint, QHash<uint, STATE_TRIGGER> >     m_poolEvt;
    QQuickView                                          *m_viewer;
    QScreenManager                                      *m_scrMng;
    QPopupManager                                       *m_popMng;
    QList<QString>                                      m_ctxProperties;
    uint                                                m_RootState;
    QList<QObject*>                                     m_qmlObj;
    uint                                                m_popRequester;
    QSize                                               m_currentSize;
    double                                              m_currentScale;
    int                                                 m_currentFlow;
    QMap<int, int>                                      m_PopupTriger;
    QObject*                                            m_currentScreen;
private:
    explicit QQuickViewer();
    virtual ~QQuickViewer();
    QQuickItem* loadQml(QQuickView *view, const QUrl &url, QQmlContext *context);
    void handleTransition(const APPLICATION_STATE *from, const APPLICATION_STATE *to, QVariant msg = QVariant());
    bool showScreen(uint id, QVariant msg = QVariant());
    bool showPopup(uint id, QVariant msg = QVariant());
    bool closePopup(uint id, QVariant msg = QVariant());
    void setOnsRequester(const uint id);
    void collectGarbage();
signals:
    void visibleChanged(bool state);
    void eventReceiver(int event, int data);
    void signalNotifySendEvent(uint eventID, QVariant msg = QVariant());
    void currentFlowChanged();

public slots:
    void onVisibleChanged(bool state);
    void sendEvent(uint eventID, QVariant msg = QVariant());
    void notifySendEvent(uint eventID, QVariant msg = QVariant());
    // For resize
    void onWidthChanged(int w);
    void aboutToQuit(QPrivateSignal signal);
};

#endif // QQUICKVIEWER_H
