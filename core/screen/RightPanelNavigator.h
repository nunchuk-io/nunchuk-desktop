#pragma once
#include <QObject>
#include <QString>
#include <QQuickView>
#include <QStack>

namespace core::screen {
class RightPanelNavigator : public QObject {
    Q_OBJECT
  public:
    explicit RightPanelNavigator(QObject *parent = nullptr);

    // Navigation API
    void request(const QString &screenId);
    // Like request(), but marks the screen as a flow's terminal/completed state (e.g. claim finished) -
    // nothing is expected to resume from here. See isTerminalScreen().
    void requestTerminal(const QString &screenId);
    void back();
    // Clears bookkeeping entirely so the next registerObject()/fresh entry starts over instead of
    // resuming. Safe to call only from a genuine navigation action (e.g. a sidebar click), never from
    // object register/unregister - those also fire on incidental rebuilds unrelated to user intent.
    void reset();

    // Register/Unregister API
    void registerObject(QObject *object);
    void unregisterObject();

    // True if a screen was already requested (e.g. host was recreated mid-flow) - callers use this
    // to avoid blindly re-requesting their initial screen and clobbering an in-progress one.
    bool hasActiveScreen() const {
        return !m_currentScreenId.isEmpty();
    }

    // True if the current screen was requested via requestTerminal() - i.e. the active flow has
    // already reached a terminal/completed state with nothing left to resume into.
    bool isTerminalScreen() const {
        return m_isTerminal;
    }

  private:
    void qmlSyncup();

  private:
    // Carries isTerminal alongside each history entry so back() restores the correct terminal state
    // of whatever screen it returns to, instead of always assuming non-terminal.
    struct HistoryEntry {
        QString screenId;
        bool isTerminal;
    };
    QString m_currentScreenId;
    QList<QObject*> m_registeredObjects;
    QStack<HistoryEntry> m_historyStack;
    bool m_isTerminal = false;
};
} // namespace core::screen