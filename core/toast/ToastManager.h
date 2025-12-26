#pragma once

#include <QObject>
#include <QVariantList>
#include <QTimer>
#include "QWarningMessage.h"

namespace core::toast {

class ToastManager : public QObject {
    Q_OBJECT
    Q_PROPERTY(QVariantList toasts READ toasts NOTIFY toastsChanged)

public:
    explicit ToastManager(QObject* parent = nullptr);

    Q_INVOKABLE void show(const QString& source,
                          const QVariantMap& data = {},
                          int durationMs = 3000);

    Q_INVOKABLE void clear();

    QVariantList toasts() const;
public slots:
    void showToast(int code, const QString &what, EWARNING::WarningType type);
signals:
    void toastsChanged();

private:
    void removeToast(const QString& id);

private:
    QVariantList m_toasts;
};
} // namespace core::toast
