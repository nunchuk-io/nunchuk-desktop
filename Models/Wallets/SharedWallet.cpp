#include "SharedWallet.h"
#include "Servers/Draco.h"
#include "Servers/Byzantine.h"
#include "QThreadForwarder.h"

SharedWallet::SharedWallet(const nunchuk::Wallet &w) :
    HotWallet{w}
{
    m_roomMembers.clear();
    QObject::connect(this, &SharedWallet::signalSyncCollabKeyname, this, &SharedWallet::slotSyncCollabKeyname, Qt::QueuedConnection);
}

int SharedWallet::collabWalletN() const
{
    return m_collabWalletN;
}

void SharedWallet::setCollabWalletN(int data)
{
    if(data != m_collabWalletN){
        m_collabWalletN = qMax(0, data);
        emit walletChanged();
    }
}

bool SharedWallet::isSharedWallet() const
{
    return m_isSharedWallet;
}

void SharedWallet::setIsSharedWallet(bool isShared)
{
    if(m_isSharedWallet != isShared){
        m_isSharedWallet = isShared;
        emit isSharedWalletChanged();
    }
}

void SharedWallet::syncCollabKeyname()
{
    if(isSharedWallet()){
        QString room_id = roomId();
        if(room_id != ""){
            DBG_INFO << room_id;
            QList<DracoUser> users = Draco::instance()->getRoomMembers(room_id);
            if(users.count() > 0 && signersPtr()){
                emit signalSyncCollabKeyname(users);
            }
        }
    }
}

void SharedWallet::slotSyncCollabKeyname(QList<DracoUser> users){
    DBG_INFO << roomId();
    if(signersPtr()){
        signersPtr().data()->syncNunchukEmail(users);
    }
}


QString SharedWallet::roomId() const
{
    return m_roomId;
}

void SharedWallet::setRoomId(const QString &roomId)
{
    if(m_roomId != roomId){
        m_roomId = roomId;
        emit roomIdChanged();
    }
}

QString SharedWallet::initEventId() const
{
    return m_initEventId;
}

void SharedWallet::setInitEventId(const QString &initEventId)
{
    if(m_initEventId != initEventId){
        m_initEventId = initEventId;
        emit initEventIdChanged();
    }
}

QWalletPtr SharedWallet::clone() const
{
    auto to = CoinWallet::clone();
    to.data()->setIsSharedWallet(isSharedWallet());
    to.data()->setRoomId(roomId());
    return to;
}
