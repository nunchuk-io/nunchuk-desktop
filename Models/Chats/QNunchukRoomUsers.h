#ifndef QNUNCHUKROOMUSERS_H
#define QNUNCHUKROOMUSERS_H

#include <QQmlEngine>
#include <QAbstractListModel>
#include <QObject>
#include <QRegularExpression>
#include <room.h>
#include <connection.h>
#include <ssosession.h>
#include <user.h>
#include <uriresolver.h>
#include <events/encryptionevent.h>
#include <events/redactionevent.h>
#include <events/roomavatarevent.h>
#include <events/roomcreateevent.h>
#include <events/roommemberevent.h>
#include <events/roommessageevent.h>
#include <events/roomcanonicalaliasevent.h>
#include <events/simplestateevents.h>
using namespace Quotient;

class QNunchukRoomUsers : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QString change READ change NOTIFY changeChanged)
public:
    QNunchukRoomUsers(Room* r);
    ~QNunchukRoomUsers();
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QHash<int, QByteArray> roleNames() const;
    enum UserRoles {
        user_id,
        user_name,
        user_avatar
    };
    Q_INVOKABLE  QVariant get(int row);
    QString change() const;

public slots:
    void refresh();
    bool HasContact(const QString &id);
signals:
    void changeChanged();

private:
    Quotient::Room  *m_room;
};
typedef QSharedPointer<QNunchukRoomUsers> QNunchukRoomUsersPtr;

#endif // QNUNCHUKROOMUSERS_H
