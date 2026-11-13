#include "DevicePairingController.h"

#include <QCoreApplication>
#include <QRegularExpression>
#include <QThread>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <mutex>

namespace core::hardware {
struct PairingState {
    std::weak_ptr<nunchuk::DevicePairingSession> session;
    std::mutex mutex;
    std::condition_variable changed;
    std::atomic_bool canceled{false}, finished{false}, timedOut{false}, busy{false};
    bool answered = false;
    nunchuk::DevicePairingResponse response;

    void cancel() {
        canceled = true;
        if (auto current = session.lock()) current->Cancel();
        changed.notify_all();
    }
};

DevicePairingController* DevicePairingController::instance() {
    static auto* controller = new DevicePairingController(qApp);
    return controller;
}

DevicePairingController::DevicePairingController(QObject* parent) : QObject(parent), m_timeout(this) {
    m_timeout.setSingleShot(true);
    m_timeout.setInterval(300000);
    connect(&m_timeout, &QTimer::timeout, this, [this]() {
        if (m_state) {
            m_state->timedOut = true;
            cancel();
        }
    });
}

void DevicePairingController::confirm(const QString& code) {
    if (!m_state || m_state->canceled) return;
    const bool enterCode = m_stage == "enter_code";
    if (!enterCode && m_stage != "confirm_code") return;
    static const QRegularExpression sixDigits(QStringLiteral("^[0-9]{6}$"));
    if (enterCode && !sixDigits.match(code).hasMatch()) return;
    {
        std::lock_guard lock(m_state->mutex);
        m_state->response = {true, enterCode ? code.toStdString() : std::string()};
        m_state->answered = true;
    }
    m_stage = "confirming";
    emit stateChanged();
    m_state->changed.notify_all();
}

void DevicePairingController::cancel() {
    if (!m_state) return;
    m_state->cancel();
    m_stage = "canceling";
    emit stateChanged();
}

void DevicePairingController::pairDevice(const QString& hwiPath, const nunchuk::Device& device) {
    if (!qApp || QCoreApplication::closingDown() || QThread::currentThread() == thread()) {
        throw nunchuk::HWIException(nunchuk::HWIException::RUN_ERROR, "Pairing must run on a background thread");
    }
    auto session = std::make_shared<nunchuk::DevicePairingSession>(hwiPath.toStdString(), device);
    auto state = std::make_shared<PairingState>();
    state->session = session;
    // Also cover shutdown before the queued popup update has been handled.
    const auto quitConnection = connect(qApp, &QCoreApplication::aboutToQuit, this, [state]() { state->cancel(); });
    const auto name = device.get_type() == "trezor" ? QStringLiteral("Trezor") : QStringLiteral("BitBox02");
    QMetaObject::invokeMethod(this, [this, state, name]() {
        if (state->finished || state->canceled) return;
        if (m_state) {
            state->busy = true;
            state->cancel();
            return;
        }
        m_state = state;
        m_deviceName = name;
        m_code.clear();
        m_stage = "connecting";
        m_timeout.start();
        emit stateChanged();
    }, Qt::QueuedConnection);
    auto finish = [this, state, quitConnection]() {
        state->finished = true;
        disconnect(quitConnection);
        QMetaObject::invokeMethod(this, [this, state]() {
            if (m_state != state) return;
            m_timeout.stop();
            m_state.reset();
            m_stage.clear();
            m_deviceName.clear();
            m_code.clear();
            emit stateChanged();
        }, Qt::QueuedConnection);
    };
    try {
        const auto request = session->Start();
        if (request) {
            QMetaObject::invokeMethod(this, [this, state, request = *request]() {
                if (m_state != state || state->finished || state->canceled) return;
                m_stage = request.method == nunchuk::DevicePairingMethod::ENTER_CODE ? "enter_code" : "confirm_code";
                m_code = QString::fromStdString(request.code);
                emit stateChanged();
            }, Qt::QueuedConnection);
            std::unique_lock lock(state->mutex);
            if (!state->changed.wait_for(lock, std::chrono::minutes(5), [&]() { return state->answered || state->canceled.load(); })) {
                state->timedOut = true;
                state->cancel();
            }
            const auto response = state->response;
            lock.unlock();
            if (state->canceled) throw nunchuk::HWIException(nunchuk::HWIException::ACTION_CANCELED, "Pairing canceled");
            session->Confirm(response);
        }
        finish();
    } catch (...) {
        session->Cancel();
        finish();
        if (state->busy) {
            throw nunchuk::HWIException(nunchuk::HWIException::DEVICE_BUSY, "Another pairing dialog is already open");
        }
        if (state->timedOut) {
            throw nunchuk::HWIException(nunchuk::HWIException::DEVICE_NOT_READY, "Pairing timed out. Please try again.");
        }
        throw;
    }
}
}
