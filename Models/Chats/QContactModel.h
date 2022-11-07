#ifndef QCONTACTMODEL_H
#define QCONTACTMODEL_H

#include <QAbstractListModel>
#include "DracoDefines.h"

class QContactModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentIndexChanged)
    Q_PROPERTY(int count        READ count                              NOTIFY countChanged)
public:
    QContactModel();
    ~QContactModel();
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QHash<int, QByteArray> roleNames() const;

    void syncContacts(QList<DracoUser> data);
    void addContact(const DracoUser user);
    DracoUser getContactByIndex(const int index);
    QStringList contactsByStringList();
    QString getContactNameByChatId(const QString &id);
    void removeAll();
    DracoUser currentUser();
    Q_INVOKABLE QVariant get(int row);
    void removeContact(const QString &contact_id);
    void acceptFriendRequest(const QString& contact_id);
    void ignoreFriendRequest(const QString& contact_id);
    void cancelFriendRequest(const QString& contact_id);
    bool containsUserId(const QString& userid);
    enum ContactRoles {
        Contact_id,
        Contact_name,
        Contact_email,
        Contact_gender,
        Contact_avatar,
        Contact_status,
        Contact_status_text,
        Contact_chat_id,
    };
    int currentIndex() const;
    void setCurrentIndex(int currentIndex);

    int count() const;

private:
    int m_currentIndex;
    QList<DracoUser> m_data;

signals:
    void currentIndexChanged();
    void countChanged();
};

typedef QSharedPointer<QContactModel> QContactModelPtr;

#endif // QCONTACTMODEL_H
