#ifndef DRACO_H
#define DRACO_H

#include <QNetworkAccessManager>
#include <QObject>
#include <QUrl>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include "Chats/ClientController.h"
#include "DracoDefines.h"

class Draco : public QObject
{
    Q_OBJECT
    Q_ENUMS(RESPONSE_CODE)
    Q_PROPERTY(int expireSec            READ expireSec              WRITE setExpireSec              NOTIFY expireSecChanged)
    Q_PROPERTY(QString dracoToken       READ dracoToken             WRITE setDracoToken             NOTIFY dracoTokenChanged)
    Q_PROPERTY(QString chatId           READ chatId                 WRITE setChatId                 NOTIFY chatIdChanged)
    Q_PROPERTY(QString Uid              READ Uid                    WRITE setUid                    NOTIFY uidChanged)
    Q_PROPERTY(QString emailRequested   READ emailRequested         WRITE setEmailRequested         NOTIFY emailRequestedChanged)
    Q_PROPERTY(bool stayLoggedIn        READ stayLoggedIn           WRITE setStayLoggedIn           NOTIFY stayLoggedInChanged)
public:
    static Draco *instance();
    Draco(Draco &other) = delete;
    Draco(Draco const &other) = delete;
    void operator=(const Draco &other) = delete;
    void connectDragonServer(QUrl url);
    Q_INVOKABLE void refreshContacts();
    Q_INVOKABLE void createAccount(const QString& name, const QString& email);
    Q_INVOKABLE void singin(const QString &email, const QString &password);
    Q_INVOKABLE bool signout();
    Q_INVOKABLE void exchangeRates();
    Q_INVOKABLE void feeRates();
    Q_INVOKABLE void verifyNewDevice(const QString &pin);
    Q_INVOKABLE void resendVerifyNewDeviceCode();
    Q_INVOKABLE void inviteFriends(const QStringList& emails);
    Q_INVOKABLE void removeContact(const QString &contact_id);
    Q_INVOKABLE void cancelFriendRequest(const QString &contact_id);
    Q_INVOKABLE void accecptFriendRequest(const QString& id);
    Q_INVOKABLE void ignoreFriendRequest(const QString& id);
    Q_INVOKABLE bool deleteCurrentUser();
    Q_INVOKABLE bool sendDeleteConfirmation(const QString &code);
    Q_INVOKABLE void setUserProfile(const QString& name, const QString& avartaPath);
    void checkForUpdate();
    Q_INVOKABLE void signoutDeice(const QString &device_id);
    Q_INVOKABLE void signoutAllDeices();
    Q_INVOKABLE void markAsCompromised(const QString &device_id);
    Q_INVOKABLE void pkey_signup(const QString &address,const QString &username,const QString &signature);
    Q_INVOKABLE void pkey_signin(const QString &address,const QString &username,const QString &signature);
    QString get_pkey_nonce(const QString &address,const QString &username);
    QString pkey_manual_nonce(const QString &address, const QString &username, const QString &nonce, const QString &type = "none");
    Q_INVOKABLE bool pkey_username_availability(const QString &username);
    bool pkey_delete_confirmation(const QString &signed_message);
    bool pkey_change_pkey(const QString &new_key, const QString &old_signed_message, const QString &new_signed_message);
    void recoverPassword(const QString& email, const QString& forgotToken, const QString& newpassword);
    void forgotPassword(const QString &email);
    Q_INVOKABLE void changePassword(const QString &oldpassword, const QString &newpassword);
    void getMe();
    void getMepKey(const QString &public_address);
    //Frriend/Contacts
    DracoUser search(const QString& email);
    Q_INVOKABLE QVariant requestFriends(const QVariant emails);
    bool requestSingleFriend(const QString& email);
    bool requestMutipleFriend(const QStringList& email, QStringList &failed_emails);
    QList<DracoUser> getContacts();
    QList<DracoUser> getContactsSent();
    QList<DracoUser> getContactsReceived();
    QList<DracoUser> getRoomMembers(const QString& id);
    QList<DracoDevice> getDevices();
    QString Uid() const;
    void setUid(const QString &Uid);
    QString Pid() const;
    void setPid(const QString &pid);
    QString emailRequested() const;
    void setEmailRequested(const QString &value);
    int expireSec() const;
    void setExpireSec(int value);
    QString dracoToken() const;
    void setDracoToken(const QString &value);
    QString chatId() const;
    void setChatId(const QString &value);
    void reset();

    QString loginHalfToken() const;
    void setLoginHalfToken(const QString &value);
    QString deviceId() const;
    void setDeviceId(const QString &value);
    QByteArray machineUniqueId() const;
    bool stayLoggedIn() const;
    void setStayLoggedIn(bool value);
private:
    Draco();
    ~Draco();
    QJsonObject postSync(const QString &cmd, QJsonObject data, int &https_code);
    QJsonObject getSync(const QString &cmd, QJsonObject data, int& https_code);
    QJsonObject putSync(const QString &cmd, QJsonObject data, int& https_code);
    QJsonObject deleteSync(const QString &cmd, QJsonObject data, int& https_code);
    static Draco* m_instance;
    QNetworkAccessManager *m_dracoManager;
    QNetworkAccessManager *m_exchangesMng;
    QNetworkAccessManager *m_feeRateMng;
    QString m_uid;
    QString m_pid;
    QString m_emailRequested;
    int m_expireSec;
    QString m_dracoToken;
    QString m_ChatId;
    QString m_loginHalfToken;
    QString m_deviceId;
    QByteArray m_machineUniqueId;
    bool m_stayLoggedIn;

signals:
    void uidChanged();
    void pidChanged();
    void emailRequestedChanged();
    void expireSecChanged();
    void dracoTokenChanged();
    void chatIdChanged();
    void deviceIdChanged();
    void loginHalfTokenChanged();
    void createAccountResult(int https_code, int error_code, QString error_msg);
    void singinResult(int https_code, int error_code, QString error_msg);
    void recoverPasswordResult(int https_code, int error_code, QString error_msg);
    void forgotPasswordResult(int https_code, int error_code, QString error_msg);
    void changePasswordResult(int https_code, int error_code, QString error_msg);
    void verifyNewDeviceResult(int https_code, int error_code, QString error_msg);
    void resendVerifyNewDeviceCodeResult(int https_code, int error_code, QString error_msg);
    void loggedInDeviceChanged(int https_code, int error_code, QString error_msg);
    void updateProfileResult(int https_code, int error_code, QString error_msg);
    void startCheckForUpdate(int result, const QString &title,const QString &message,const QString &doItLaterCTALbl);
    void signalpkey_signup(int https_code, int error_code, QString error_msg);
    void signalpkey_signin(int https_code, int error_code, QString error_msg);
    void stayLoggedInChanged();
};

#endif // DRACO_H
