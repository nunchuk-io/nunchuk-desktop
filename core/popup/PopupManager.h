// core/popup/PopupManager.h
#pragma once
#include <QObject>
#include <QStack>
#include <QString>
#include <QVariantMap>
#include "PopupRequest.h"

namespace core::popup {

class PopupManager : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool hasPopup READ hasPopup NOTIFY popupChanged)
    Q_PROPERTY(QString currentPopupId READ currentPopupId NOTIFY popupChanged)
    Q_PROPERTY(QVariantMap currentData READ currentData NOTIFY popupChanged)

  public:
    explicit PopupManager(QObject* parent = nullptr);

    Q_INVOKABLE void show(const QString& popupId,
                          const QVariantMap& data = {});
    Q_INVOKABLE void close();
    Q_INVOKABLE void clear();

    bool hasPopup() const;
    QString currentPopupId() const;
    QVariantMap currentData() const;

  signals:
    void popupChanged();
    void popupAction(const QString& action,
                     const QVariantMap& payload);

  private:
    QStack<PopupRequest> m_stack;
};

} // namespace core::popup
