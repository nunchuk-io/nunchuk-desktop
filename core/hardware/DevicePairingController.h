#pragma once

#include <QObject>
#include <QString>
#include <QTimer>
#include <memory>
#include <nunchuk.h>

namespace core::hardware {
struct PairingState;

class DevicePairingController : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool active READ active NOTIFY stateChanged)
    Q_PROPERTY(QString stage READ stage NOTIFY stateChanged)
    Q_PROPERTY(QString deviceName READ deviceName NOTIFY stateChanged)
    Q_PROPERTY(QString code READ code NOTIFY stateChanged)

public:
    // Created on the application thread when registering QML context properties.
    static DevicePairingController* instance();
    bool active() const { return bool(m_state); }
    QString stage() const { return m_stage; }
    QString deviceName() const { return m_deviceName; }
    QString code() const { return m_code; }
    Q_INVOKABLE void confirm(const QString& code);
    Q_INVOKABLE void cancel();

    // Blocks the existing scan worker; UI updates are queued to this object's thread.
    void pairDevice(const QString& hwiPath, const nunchuk::Device& device);

signals:
    void stateChanged();

private:
    explicit DevicePairingController(QObject* parent);
    std::shared_ptr<PairingState> m_state;
    QString m_stage, m_deviceName, m_code;
    QTimer m_timeout;
};
}
