#include "HomeReminderViewModel.h"

#include "Models/AppModel.h"
#include "Servers/Draco.h"
#include "ifaces/nunchuckiface.h"
#include <QDesktopServices>
#include <QTimer>
#include <QUrl>

namespace features::home::viewmodels {

HomeReminderViewModel::HomeReminderViewModel(QObject *parent)
    : QObject(parent), m_sessionKey(currentSessionKey()) {
    connect(CLIENT_INSTANCE, &ClientController::isNunchukLoggedInChanged, this, [this]() {
        syncSession();
        // Some login paths can reach Home before the authenticated identity is
        // fully available. Refetch on the identity transition so an anonymous
        // response cannot make a newly logged-in Free user miss the reminder.
        fetch();
    });
    const auto syncSubscriptionAudience = [this]() {
        syncSession();
        if (!isEligibleAudience()) {
            clearReminder();
        } else {
            // An empty, resolved subscription list represents a Free user.
            // This also covers login paths where Home was created first.
            fetch();
        }
    };
    connect(CLIENT_INSTANCE, &ClientController::subscriptionsChanged, this, syncSubscriptionAudience);
    connect(CLIENT_INSTANCE, &ClientController::subscriptionsReadyChanged, this, syncSubscriptionAudience);
    connect(AppModel::instance(), &AppModel::nunchukModeChanged, this, [this]() {
        syncSession();
        if (!isEligibleAudience()) {
            clearReminder();
        } else {
            fetch();
        }
    });
}

QString HomeReminderViewModel::reminderId() const {
    return m_reminder.value("id").toString();
}

QString HomeReminderViewModel::title() const {
    return m_reminder.value("title").toString();
}

QString HomeReminderViewModel::description() const {
    return m_reminder.value("description").toString();
}

QString HomeReminderViewModel::imageUrl() const {
    const QString url = m_reminder.value("image_url").toString();
    return isValidHttpsUrl(url) ? url : QString();
}

QVariantList HomeReminderViewModel::actions() const {
    return m_actions.toVariantList();
}

bool HomeReminderViewModel::loading() const {
    return m_loading;
}

void HomeReminderViewModel::fetch() {
    if (!m_initialFetchGateResolved) {
        if (!m_initialFetchScheduled) {
            m_initialFetchScheduled = true;
            QTimer::singleShot(200, this, [this]() {
                m_initialFetchGateResolved = true;
                fetch();
            });
        }
        return;
    }

    syncSession();
    if (m_loading) {
        return;
    }
    if (!isEligibleAudience()) {
        clearReminder();
        return;
    }
    if (m_ready) {
        emit reminderReady();
        return;
    }
    if (m_fetchedCurrentSession) {
        return;
    }

    m_loading = true;
    const quint64 requestGeneration = ++m_requestGeneration;
    const quint64 sessionGeneration = m_sessionGeneration;
    const bool anonymous = !CLIENT_INSTANCE->isNunchukLoggedIn();
    emit loadingChanged();

    features::home::usecases::GetHomeReminderInput input;
    input.anonymous = anonymous;
    m_getHomeReminderUseCase.executeAsync(input, [this, requestGeneration, sessionGeneration](const core::usecase::Result<features::home::usecases::GetHomeReminderResult> &result) {
        m_loading = false;
        emit loadingChanged();

        // A monotonic generation guard also rejects ABA transitions such as
        // Guest -> account -> Guest while this request is in flight.
        syncSession();
        if (requestGeneration != m_requestGeneration || sessionGeneration != m_sessionGeneration) {
            if (isEligibleAudience()) {
                fetch();
            }
            return;
        }
        if (!isEligibleAudience()) {
            clearReminder();
            return;
        }
        if (result.isFailure()) {
            clearReminder();
            return;
        }

        // Both a null reminder and a valid object complete this session's
        // optional fetch. This avoids polling the endpoint on every Home focus.
        m_fetchedCurrentSession = true;
        const QJsonObject &reminder = result.value().reminder;
        if (reminder.isEmpty()) {
            clearReminder();
            return;
        }
        applyReminder(reminder);
    });
}

void HomeReminderViewModel::markShown() {
    const QString id = reminderId();
    if (!id.isEmpty()) {
        m_shownReminderIds.insert(id);
    }
    m_ready = false;
}

void HomeReminderViewModel::dismiss(const QString &presentedReminderId) {
    if (presentedReminderId.isEmpty() || presentedReminderId != reminderId()) {
        return;
    }
    m_ready = false;
    clearReminder();
}

void HomeReminderViewModel::triggerAction(const QVariantMap &actionSnapshot) {
    const QJsonObject action = normalizeAction(
        QJsonValue(QJsonObject::fromVariantMap(actionSnapshot)), false);
    if (action.isEmpty()) {
        return;
    }

    const QString type = action.value("type").toString();
    const QString target = action.value("target").toString();
    if ((type == "OPEN_LINK" || type == "OPEN_PAGE") && isValidHttpsUrl(target)) {
        QDesktopServices::openUrl(QUrl(target));
    }
}

QString HomeReminderViewModel::currentSessionKey() {
    if (!CLIENT_INSTANCE->isNunchukLoggedIn()) {
        return QString("guest:%1").arg(AppModel::instance()->nunchukMode());
    }

    const QString audience = !CLIENT_INSTANCE->subscriptionsReady()
                                 ? "unknown"
                                 : (CLIENT_INSTANCE->slugs().isEmpty() ? "free" : "subscribed");
    return QString("account:%1:%2").arg(Draco::instance()->Uid(), audience);
}

bool HomeReminderViewModel::isEligibleAudience() {
    if (!CLIENT_INSTANCE->isNunchukLoggedIn()) {
        return AppModel::instance()->nunchukMode() == LOCAL_MODE;
    }
    return CLIENT_INSTANCE->subscriptionsReady() && CLIENT_INSTANCE->slugs().isEmpty();
}

bool HomeReminderViewModel::isValidHttpsUrl(const QString &target) {
    const QUrl url(target, QUrl::StrictMode);
    return url.isValid() && url.scheme().compare("https", Qt::CaseInsensitive) == 0 && !url.host().isEmpty() && url.userInfo().isEmpty();
}

QJsonObject HomeReminderViewModel::normalizeAction(const QJsonValue &value, bool primary) {
    if (!value.isObject()) {
        return {};
    }

    const QJsonObject source = value.toObject();
    const QString label = source.value("label").toString().trimmed();
    const QString type = source.value("type").toString().trimmed().toUpper();
    const QString target = source.value("target").toString().trimmed();
    if (label.isEmpty() || (type != "OPEN_LINK" && type != "OPEN_PAGE") || !isValidHttpsUrl(target)) {
        return {};
    }

    QJsonObject action;
    action.insert("label", label);
    action.insert("type", type);
    action.insert("target", target);
    action.insert("primary", primary);
    return action;
}

bool HomeReminderViewModel::syncSession() {
    const QString key = currentSessionKey();
    if (key == m_sessionKey) {
        return false;
    }

    m_sessionKey = key;
    ++m_sessionGeneration;
    ++m_requestGeneration;
    m_fetchedCurrentSession = false;
    clearReminder();
    return true;
}

void HomeReminderViewModel::applyReminder(const QJsonObject &reminder) {
    const QString id = reminder.value("id").toString().trimmed();
    const QString reminderTitle = reminder.value("title").toString().trimmed();
    const QString reminderDescription = reminder.value("description").toString().trimmed();
    if (id.isEmpty() || (reminderTitle.isEmpty() && reminderDescription.isEmpty()) || m_shownReminderIds.contains(id)) {
        clearReminder();
        return;
    }

    QJsonArray normalizedActions;
    const QJsonObject primary = normalizeAction(reminder.value("action"), true);
    if (!primary.isEmpty()) {
        normalizedActions.append(primary);
    }
    const QJsonArray extraActions = reminder.value("extra_actions").toArray();
    for (const QJsonValue &value : extraActions) {
        const QJsonObject action = normalizeAction(value, false);
        if (!action.isEmpty()) {
            normalizedActions.append(action);
        }
    }

    m_reminder = reminder;
    m_actions = normalizedActions;
    m_ready = true;
    emit reminderChanged();
    emit reminderReady();
}

void HomeReminderViewModel::clearReminder() {
    const bool hadReminder = !m_reminder.isEmpty() || !m_actions.isEmpty();
    m_reminder = {};
    m_actions = {};
    m_ready = false;
    if (hadReminder) {
        emit reminderChanged();
    }
}

} // namespace features::home::viewmodels
