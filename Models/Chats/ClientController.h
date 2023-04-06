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

#ifndef CLIENTCONTROLLER_H
#define CLIENTCONTROLLER_H

#include <QObject>
#include <QStandardPaths>
#include <QSqlDatabase>
#include <QSqlError>
#include <QtQml>
#include "QContactModel.h"
#include "QLogginManager.h"
#include "QNunchukRoomModel.h"
#include "QNunchukImageProvider.h"
#include "QLoggedInDeviceModel.h"
// Matrix
#include <uriresolver.h>
#ifdef USE_KEYCHAIN
#include <qt5keychain/keychain.h>
#endif

namespace Quotient {
    class Room;
    class Connection;
    class AccountSettings;
}

class ClientController final : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QContactModel*   contacts         READ contacts          NOTIFY contactsChanged)
    Q_PROPERTY(QContactModel*   contactsSent     READ contactsSent      NOTIFY contactsSentChanged)
    Q_PROPERTY(QContactModel*   contactsReceived READ contactsReceived  NOTIFY contactsReceivedChanged)
    Q_PROPERTY(QLoggedInDeviceModel*  devicesLogged READ devicesLogged  NOTIFY devicesLoggedChanged)
    Q_PROPERTY(QNunchukRoomListModel*   rooms    READ rooms             NOTIFY roomsChanged)
    Q_PROPERTY(bool isNunchukLoggedIn            READ isNunchukLoggedIn NOTIFY isNunchukLoggedInChanged)
    Q_PROPERTY(bool isMatrixLoggedIn             READ isMatrixLoggedIn  NOTIFY isMatrixLoggedInChanged)
    Q_PROPERTY(bool isNewDevice                  READ isNewDevice       WRITE setIsNewDevice NOTIFY enableEncryptionChanged)
    Q_PROPERTY(QVariant user                     READ user              NOTIFY userChanged)
    Q_PROPERTY(bool attachmentEnable             READ attachmentEnable  NOTIFY attachmentEnableChanged)
    Q_PROPERTY(bool readySupport                 READ readySupport      NOTIFY readySupportChanged)
private:
    ClientController();
    ~ClientController();

public:
    static ClientController *instance();
    ClientController(ClientController &other) = delete;
    ClientController(ClientController const &other) = delete;
    void operator=(const ClientController &other) = delete;
    QString accessToken() const;
    Quotient::Connection *connection() const;
    void setConnection(Quotient::Connection *c);
    bool isNunchukLoggedIn() const;
    void setIsNunchukLoggedIn(bool isNunchukLoggedIn);
    bool isMatrixLoggedIn() const;
    void requestLogin();
    QLogginManager *loginHandler() const;
    void setLoginHandler(const QLogginManagerPtr &login);
    void syncContacts(QList<DracoUser> data);
    void syncDevices(QList<DracoDevice> data);
    QContactModel* contacts() const;
    QContactModelPtr contactsPtr() const;
    void setContacts(const QContactModelPtr &contacts);
    void syncContactsSent(QList<DracoUser> data);
    QContactModel* contactsSent() const;
    QContactModelPtr contactsSentPtr() const;
    void setContactsSent(const QContactModelPtr &contactsSent);
    void syncContactsReceived(QList<DracoUser> data);
    QContactModel* contactsReceived() const;
    QContactModelPtr contactsReceivedPtr() const;
    void setContactsReceived(const QContactModelPtr &contactsReceived);
    QLoggedInDeviceModel *devicesLogged() const;
    QNunchukRoom *currentRoom() const;
    QNunchukRoom *GetRoomById(const QString& id) const;
    QNunchukRoomListModel* rooms() const;
    void setRooms(const QNunchukRoomListModelPtr &rs);
    QString userAvatarById(const QString &id) const;
    QNunchukImageProvider* imageprovider() const;
    void setImageprovider(QNunchukImageProvider *imageprovider);
    QString getRoomIdByWalletId(const QString &wallet_id) const;
    void transactionChanged(const QString &wallet_id, const QString &tx_id, const int status, const int height);
    void updateTransactionMemo(const QString &wallet_id, const QString &tx_id, const QString &memo);
    QVariant user() const;
    DracoUser getMe() const;
    void setMe(const DracoUser &me);

    bool isNewDevice() const;
    void setIsNewDevice(bool isNewDevice);

    void saveStayLoggedInData();
    void deleteStayLoggedInData();
    QByteArray readDataFromKeyChain(const QString &key);
    bool saveDataToKeyChain(const QString &key, const QByteArray &data);
    bool deleteDataFromKeyChain(const QString &key);

    QJsonObject getSubCur();
    void setSubCur(const QJsonObject &sub);

    bool attachmentEnable() const;
    void setAttachmentEnable(bool AttachmentEnable);

    bool readySupport() const;
    void setReadySupport(bool ReadySupport);

signals:
    void isNunchukLoggedInChanged();
    void contactsChanged();
    void contactsSentChanged();
    void contactsReceivedChanged();
    void devicesLoggedChanged();
    void currentRoomChanged(QString name);
    void roomsChanged();
    void connectionChanged();
    void isMatrixLoggedInChanged();
    void enableEncryptionChanged();
    void userChanged();
    void guestModeChanged();
    void attachmentEnableChanged();
    void readySupportChanged();

public slots:
    void proxyAuthenticationRequired(const QNetworkProxy &proxy, QAuthenticator *authenticator);
    void sslErrors(QNetworkReply *reply, const QList<QSslError> &errors);
    void refreshContacts();
    void acceptFriendRequest(const QString& contact_id);
    void ignoreFriendRequest(const QString& contact_id);
    void cancelFriendRequest(const QString& contact_id);
    void removeContact(const QString& contact_id);
    void onUserAvatarChanged();
    void onUserDisplaynameChanged();

    QStringList contactsByStringList();
    void setUserDisplayname(const QString &name);
    void setUserAvatar(const QString &url);
    void requestSignout();
    void forgetRoom(const int index);
    void leaveCurrentRoom();
    void leaveRoom(const int index);
    void joinRoom(QString roomAliasOrId);
    void createRoomChat(const QStringList& invitees, const QStringList &name, QVariant firstMessage = QVariant());
    void createSupportRoom();
    void sendMessage(const QString& msg);
    QString getPlainText(const QString &msg);
    void copyMessage(const QString &msg);

    void UploadFile(const QString& file_name,
                    const QString& mine_type,
                    const QString& json_info,
                    const char* data,
                    int data_length);

    void DownloadFile(const QString& file_name,
                      const QString& mine_type,
                      const QString& json_info,
                      const QString& mxc_uri);
    bool checkStayLoggedIn();

private:
    Quotient::Connection*   m_connection;
    bool                    m_isNunchukLoggedIn;
    QLogginManagerPtr       m_loginHandler;
    QContactModelPtr        m_contacts;
    QContactModelPtr        m_contactsSent;
    QContactModelPtr        m_contactsReceived;
    QLoggedInDeviceModelPtr m_devicesLogged;
    QNunchukRoomListModelPtr m_rooms;
    QNunchukImageProvider  *m_imageprovider;
    DracoUser               m_me;
    bool                    m_isNewDevice;
    QJsonObject             m_subCur;
    bool                    m_AttachmentEnable;
    bool                    m_ReadySupport;
};

#define CLIENT_INSTANCE     ClientController::instance()
#define CLIENT_CURRENTROOM  ClientController::instance()->currentRoom()

#endif // CLIENTCONTROLLER_H
