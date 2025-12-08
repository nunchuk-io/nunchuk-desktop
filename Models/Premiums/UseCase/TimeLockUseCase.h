#pragma once

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include "TypeDefine.h"
#include "TypeDefine.h"


class TimeLockUseCase {
public:
    explicit TimeLockUseCase(QGroupDashboardPtr dashboard)
        : m_dashboard(std::move(dashboard)) {}
    ~TimeLockUseCase() = default;

    void SetupTimeLock(const QVariant &datetime, bool isPutServer, std::function<void()> callback = nullptr);
    QJsonObject timeLockInfo();
    void clearTimeLock(std::function<void()> callback);
    QJsonObject timeLockSet() const;
private:
    QGroupDashboardPtr m_dashboard;
    QJsonObject m_timeLockSet;
};