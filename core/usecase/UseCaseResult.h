#pragma once
#include <QJsonObject>
#include <QJsonDocument>
#include <QString>

namespace core::usecase {

struct UseCaseResult {
    virtual ~UseCaseResult() = default;

    virtual QJsonObject toJson() const = 0;

    QString toLogString() const {
        return QString(
            QJsonDocument(toJson()).toJson(QJsonDocument::Compact)
        );
    }
};
}