#pragma once
#include <QObject>
#include <QString>
#include <QVector>
#include <QUrl>

namespace core::screen {
    class ScreenManager : public QObject
    {
        Q_OBJECT
        Q_PROPERTY(bool visible READ visible NOTIFY stateChanged)
        Q_PROPERTY(QString currentScreenId READ currentScreenId NOTIFY stateChanged)
        Q_PROPERTY(QUrl currentSource READ currentSource NOTIFY stateChanged)
    
    public:
        explicit ScreenManager(QObject* parent = nullptr);
    
        // Navigation API
        Q_INVOKABLE void show(const QString& screenId, const QUrl& source);
        Q_INVOKABLE void replace(const QString& screenId, const QUrl& source);
        Q_INVOKABLE void clearAndShow(const QString& screenId, const QUrl& source);
        Q_INVOKABLE void back();
        Q_INVOKABLE void clear();
    
        // State
        bool visible() const;
        QString currentScreenId() const;
        QUrl currentSource() const;
    
    signals:
        void stateChanged();
    
    private:
        struct Entry {
            QString screenId;
            QUrl source;
        };
    
        QVector<Entry> m_stack;
    };
}
