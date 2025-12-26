// core/popup/PopupRequest.h
#pragma once
#include <QString>
#include <QVariantMap>

namespace core::popup {

struct PopupRequest {
    QString popupId;        // confirm / info / warning
    QVariantMap data;       // title, message, buttons, etc
};

} // namespace core::popup
