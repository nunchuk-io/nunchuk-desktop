/**************************************************************************
 * This file is part of the Nunchuk software (https://nunchuk.io/)        *
 * Copyright (C) 2020-2022 Enigmo								          *
 * Copyright (C) 2022 Nunchuk								              *
 *                                                                        *
 * This program is free software; you can redistribute it and/or          *
 * modify it under the terms of the GNU General Public License            *
 * as published by the Free Software Foundation; either version 3         *
 * of the License, or (at your option) any later version.                 *
 *                                                                        *
 * This program is distributed in the hope that it will be useful,        *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 * GNU General Public License for more details.                           *
 *                                                                        *
 * You should have received a copy of the GNU General Public License      *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.  *
 *                                                                        *
 **************************************************************************/
#include "matrixlistener.h"
#include "Chats/QRoomCustomEvent.h"
#include "QOutlog.h"
#include "Chats/ClientController.h"
#include <QSemaphore>
#include <atomic>
#include <memory>
#include <mutex>
#include <utility>

namespace {
constexpr int MAIN_THREAD_DISPATCH_POLL_MS = 25;

std::atomic_bool &mainDispatchStopping()
{
    static auto *stopping = new std::atomic_bool{false};
    return *stopping;
}

std::mutex &mainDispatchAdmissionMutex()
{
    static auto *mutex = new std::mutex;
    return *mutex;
}

void installShutdownHook(QCoreApplication *application)
{
    static std::once_flag once;
    std::call_once(once, [application]() {
        QObject::connect(application, &QCoreApplication::aboutToQuit,
                         application, []() {
                             std::lock_guard<std::mutex> lock(mainDispatchAdmissionMutex());
                             mainDispatchStopping().store(true, std::memory_order_release);
                         }, Qt::DirectConnection);
    });
}

enum MainThreadCallStatus {
    CALL_PENDING,
    CALL_RUNNING,
    CALL_CANCELLED,
    CALL_COMPLETED
};

template <typename Result>
struct MainThreadCallState {
    std::atomic<int> status{CALL_PENDING};
    QSemaphore finished;
    Result result{};
};

template <typename Result, typename Function>
std::pair<bool, Result> invokeOnMainThread(Function &&function)
{
    std::unique_lock<std::mutex> admissionLock(mainDispatchAdmissionMutex());
    QCoreApplication *application = QCoreApplication::instance();
    if (!application || QCoreApplication::closingDown()) {
        return {};
    }
    installShutdownHook(application);
    if (mainDispatchStopping().load(std::memory_order_acquire)) {
        return {};
    }
    if (QThread::currentThread() == application->thread()) {
        admissionLock.unlock();
        return {true, function()};
    }

    auto state = std::make_shared<MainThreadCallState<Result>>();
    // Keep the arguments/results owned by the queued call. This avoids raw
    // QObject access from the callback thread and lets shutdown cancel a call
    // that the main event loop can no longer service.
    const bool invoked = QMetaObject::invokeMethod(
        application,
        [state, function = std::forward<Function>(function)]() mutable {
            int expected = CALL_PENDING;
            if (!state->status.compare_exchange_strong(expected, CALL_RUNNING)) {
                return;
            }
            state->result = function();
            state->status.store(CALL_COMPLETED);
            state->finished.release();
        },
        Qt::QueuedConnection);
    admissionLock.unlock();
    if (!invoked) {
        return {};
    }

    while (!state->finished.tryAcquire(1, MAIN_THREAD_DISPATCH_POLL_MS)) {
        const bool shouldCancel = mainDispatchStopping().load(std::memory_order_acquire)
                                  || !QCoreApplication::instance()
                                  || QCoreApplication::closingDown();
        if (!shouldCancel) {
            continue;
        }

        int expected = CALL_PENDING;
        if (state->status.compare_exchange_strong(expected, CALL_CANCELLED)) {
            DBG_INFO << "Matrix main-thread dispatch cancelled";
            return {};
        }
        if (expected == CALL_RUNNING) {
            // The queued closure owns its state and captures, so the caller can
            // leave safely even if the main-thread operation has just started.
            return {};
        }
        if (expected == CALL_COMPLETED || state->status.load() == CALL_COMPLETED) {
            return {true, std::move(state->result)};
        }
        return {};
    }
    return {true, std::move(state->result)};
}

}

void matrixlistener::prepareMainThreadDispatch()
{
    std::lock_guard<std::mutex> admissionLock(mainDispatchAdmissionMutex());
    if (auto *application = QCoreApplication::instance()) {
        if (!QCoreApplication::closingDown() &&
            !mainDispatchStopping().load(std::memory_order_acquire)) {
            installShutdownHook(application);
        }
    }
}

bool matrixlistener::canSendErrorEvent()
{
    const auto result = invokeOnMainThread<bool>([]() {
        auto *rooms = ClientController::instance()->rooms();
        return !rooms || rooms->allHisLoaded();
    });
    return result.first && result.second;
}

// event id
std::string SendFunction(const std::string &roomid, const std::string &eventtype, const std::string &eventContent, bool ignore_error )
{
    Q_UNUSED(ignore_error);
    const QString roomId = QString::fromStdString(roomid);
    const QString event = QString::fromStdString(eventtype);
    const QJsonObject data = QJsonDocument::fromJson(QByteArray::fromStdString(eventContent)).object();
    const auto result = invokeOnMainThread<QString>([roomId, event, data]() {
        if (QNunchukRoom *room = CLIENT_INSTANCE->GetRoomById(roomId)) {
            return room->postEvent(event, data);
        }
        return QString();
    });

    return result.first ? result.second.toStdString() : std::string();
}

bool consume_sync_event_listener(int percent)
{
    Q_UNUSED(percent);
    const auto result = invokeOnMainThread<bool>([]() {
        AppModel::instance()->startNunchukSync();
        return true;
    });
    return result.first && result.second;
}

string UploadFileFunc(const string &file_name, const string &mine_type, const string &json_info, const char *data, size_t data_length)
{
    const QString fileName = QString::fromStdString(file_name);
    const QString mimeType = QString::fromStdString(mine_type);
    const QString jsonInfo = QString::fromStdString(json_info);
    const int payloadSize = static_cast<int>(data_length);
    const QByteArray payload(data, payloadSize);
    invokeOnMainThread<bool>([fileName, mimeType, jsonInfo, payload]() {
        CLIENT_INSTANCE->UploadFile(fileName,
                                    mimeType,
                                    jsonInfo,
                                    payload.constData(),
                                    payload.size());
        return true;
    });
    return "";
}

std::vector<unsigned char> DownloadFileFunc(const string &file_name, const string &mine_type, const string &json_info, const string &mxc_uri)
{
    std::vector<unsigned char> ret;
    const QString fileName = QString::fromStdString(file_name);
    const QString mimeType = QString::fromStdString(mine_type);
    const QString jsonInfo = QString::fromStdString(json_info);
    const QString mxcUri = QString::fromStdString(mxc_uri);
    invokeOnMainThread<bool>([fileName, mimeType, jsonInfo, mxcUri]() {
        // Preserve the existing branch and request exactly once; only move the
        // QObject reads and method call onto their owning thread.
        auto *rooms = ClientController::instance()->rooms();
        if (rooms && !rooms->allHisLoaded()) {
            CLIENT_INSTANCE->DownloadFile(fileName, mimeType, jsonInfo, mxcUri);
            return true;
        }
        CLIENT_INSTANCE->DownloadFile(fileName, mimeType, jsonInfo, mxcUri);
        return true;
    });
    return ret;
}

bool DownloadFileProgress(int percent)
{
    return true;
}
