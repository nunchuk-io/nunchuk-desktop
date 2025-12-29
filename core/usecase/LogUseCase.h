#pragma once
#include <QJsonObject>
#include <QJsonDocument>
#include <QString>
#include "UseCaseInput.h"
#include "UseCaseResult.h"

namespace core::usecase {
    inline QString valueToString(const QJsonObject& v) {
        return QString(QJsonDocument(v).toJson(QJsonDocument::Compact));
    }
    
    inline QString valueToString(const QString& v) {
        return v;
    }
    
    inline QString valueToString(const QByteArray& v) {
        return QString::fromUtf8(v);
    }

    inline QString valueToString(const UseCaseResult& v) {
        return v.toLogString();
    }

    inline QString valueToString(const UseCaseInput& v) {
        return v.toLogString();
    }
}