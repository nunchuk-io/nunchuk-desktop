#include "SandboxWallet.h"
#include "qUtils.h"
#include "AppSetting.h"
#include "ViewsEnums.h"
#include "AppModel.h"
#include "bridgeifaces.h"
#include "QThreadForwarder.h"
#include "Premiums/QSharedWallets.h"

SandboxWallet::SandboxWallet(const nunchuk::Wallet &w) :
    AssistedWallet{w},
    m_sandbox(QGroupSandboxPtr(new QGroupSandbox())),
    m_conversations(QGroupMessageModelPtr(new QGroupMessageModel()))
{}

bool SandboxWallet::isReplaced() const
{
    bool isGlobalReplaced = QSharedWallets::instance()->deprecatedWallets().contains(walletId());
    return isGlobalReplaced || AssistedWallet::isReplaced();
}

bool SandboxWallet::needBackup()
{
    if(isGlobalGroupWallet()){
        bool needBackup = true;
        QWalletCached<QString, QString, QString, QString, bool> data;
        bool ret = AppSetting::instance()->getwalletCached(walletId(), data);
        if(ret){
            needBackup = !data.backedup;
        }
        else {
            setNeedBackup(needBackup);
        }
        return needBackup;
    }
    else {
        return false;
    }
}

void SandboxWallet::setNeedBackup(const bool data)
{
    if(isGlobalGroupWallet()){
        QWalletCached<QString, QString, QString, QString, bool> cache;
        cache.groupId   = groupId();
        cache.slug      = slug();
        cache.myRole    = myRole();
        cache.status    = status();
        cache.backedup  = !data;
        AppSetting::instance()->setWalletCached(walletId(), cache);
        emit needBackupChanged();
    }
}

bool SandboxWallet::isGlobalGroupWallet() const
{
    if(!AppModel::instance()->groupWalletListPtr()){
        return false;
    }
    else {
        return AppModel::instance()->groupWalletListPtr()->containsId(walletId());
    }
}


bool SandboxWallet::isReplaceGroupWallet() const
{
    if (m_sandbox) {
        return !m_sandbox->sandbox().get_replace_wallet_id().empty();
    }
    return false;
}

QGroupSandboxPtr SandboxWallet::groupSandboxPtr()
{
    return m_sandbox;
}

void SandboxWallet::updateGroupSandbox(const nunchuk::GroupSandbox &value)
{
    if(m_sandbox){
        m_sandbox.data()->setSandbox(value);
        emit groupSandboxChanged();
    }
}

void SandboxWallet::setGroupSandbox(QGroupSandboxPtr sandbox)
{
    m_sandbox = sandbox;
    emit groupSandboxChanged();
}

QGroupSandbox *SandboxWallet::groupSandbox()
{
    return groupSandboxPtr().data();
}

void SandboxWallet::startGetUnreadMessage()
{
    QPointer<SandboxWallet> safeThis(this);
    runInConcurrent([safeThis]() -> int {
        SAFE_QPOINTER_CHECK(ptrLamda, safeThis)
        return bridge::GetUnreadMessagesCount(ptrLamda->walletId());
    },[safeThis](int number) {
                        SAFE_QPOINTER_CHECK_RETURN_VOID(ptrLamda, safeThis)
                        ptrLamda->setUnreadMessage(number);
                        if (auto list = AppModel::instance()->groupWalletList()) {
                            list->unReadMessageCountChanged();
                        }
                    });

}

void SandboxWallet::startDownloadConversation()
{
    if(conversations() && isGlobalGroupWallet()){
        QString wallet_id = walletId();
        conversations()->startDownloadConversation(wallet_id);
        startGetUnreadMessage();
    }
}

void SandboxWallet::startSendGroupMessage(const QString &message)
{
    QString wallet_id = walletId();
    QWarningMessage msg;
    bridge::SendGroupMessage(wallet_id.toStdString(), message.toStdString(), {}, msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        DBG_INFO << "SENT>" << wallet_id << message ;
    }
    else {
        AppModel::instance()->showToast(msg.code(), msg.what(), (EWARNING::WarningType)msg.type());
    }
}


void SandboxWallet::GetGroupWalletConfig()
{
    QWarningMessage msg;
    auto config = bridge::GetGroupWalletConfig(walletId().toStdString(), msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        m_nunchukConfig = config;
        emit walletConfigChanged();
    }
}

int SandboxWallet::numberOnline()
{
    QtConcurrent::run([this]() {
        if(groupSandbox()){
            int number = bridge::GetGroupOnline(groupSandbox()->groupId());
            QThreadForwarder::instance()->forwardInQueuedConnection([number, this](){
                DBG_INFO << number;
                this->setNumberOnline(number);
            });
        }
    });
    return m_numberOnline;
}

void SandboxWallet::setNumberOnline(int number)
{
    if((number >= 0) && (m_numberOnline != number)){
        m_numberOnline = number;
        emit numberOnlineChanged();
    }
}

int SandboxWallet::unreadMessage()
{
    return m_unreadMessage;
}

void SandboxWallet::setUnreadMessage(int number)
{
    if((number >= 0) && (m_unreadMessage != number)){
        m_unreadMessage = number;
        emit unreadMessageChanged();
    }
}

int SandboxWallet::retentionDays()
{
    return m_nunchukConfig.get_chat_retention_days();
}

void SandboxWallet::setRetentionDays(const int days)
{
    m_nunchukConfig.set_chat_retention_days(days);
    QWarningMessage msg;
    bridge::SetGroupWalletConfig(walletId().toStdString(), m_nunchukConfig, msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()){
        bridge::GetGroupWalletConfig(walletId().toStdString(), msg);
        emit walletConfigChanged();
    }
    else {
        AppModel::instance()->showToast(msg.code(), msg.what(), (EWARNING::WarningType)msg.type());
    }
}

QGroupMessageModel *SandboxWallet::conversations()
{
    if(!m_conversations){
        m_conversations = QGroupMessageModelPtr(new QGroupMessageModel());
    }
    return m_conversations.data();
}


bool SandboxWallet::showbubbleChat() const
{
    return m_showbubbleChat;
}

void SandboxWallet::setShowbubbleChat(bool data)
{
    if (m_showbubbleChat == data)
        return;
    m_showbubbleChat = data;
    emit showbubbleChatChanged();
}

void SandboxWallet::markAsReadMessage(const QString &msg_id)
{
    bridge::SetLastReadMessage(walletId(), msg_id);
}

void SandboxWallet::markFiveMessagesAsRead()
{
    if(conversations()){
        conversations()->markFiveMessagesAsRead();
        int number = bridge::GetUnreadMessagesCount(walletId());
        if (auto list = AppModel::instance()->walletListPtr()) {
            list->updateUnreadMessage(walletId(),number);
        }
        if (auto list = AppModel::instance()->groupWalletListPtr()) {
            list->updateUnreadMessage(walletId(),number);
        }
    }
}

void SandboxWallet::markAllMessagesAsRead()
{
    if(conversations()){
        conversations()->markAllMessagesAsRead();
        if (auto list = AppModel::instance()->walletListPtr()) {
            list->updateUnreadMessage(walletId(),0);
        }
        if (auto list = AppModel::instance()->groupWalletListPtr()) {
            list->updateUnreadMessage(walletId(),0);
        }
    }
}


void SandboxWallet::GetReplaceGroups()
{
    QPointer<SandboxWallet> safeThis(this);
    runInThread([safeThis]() ->QMap<QString, bool>{
        SAFE_QPOINTER_CHECK(ptrLamda, safeThis)
        QWarningMessage msg;
        return bridge::GetReplaceGroups(ptrLamda->walletId(), msg);
    },[safeThis](QMap<QString, bool> groups) {
                    SAFE_QPOINTER_CHECK_RETURN_VOID(ptrLamda, safeThis)
                    QJsonArray array;
                    for (auto it = groups.begin(); it != groups.end(); ++it) {
                        QJsonObject js;
                        js["group_id"] = it.key();
                        js["accepted"] = it.value();
                        array.append(js);
                    }
                    ptrLamda->m_replaceGroups = array;
                    DBG_INFO << array;
                    emit ptrLamda->replaceGroupsChanged();
                });
}

QVariantList SandboxWallet::replaceGroups()
{
    return m_replaceGroups.toVariantList();
}


void SandboxWallet::requestAcceptReplaceGroup(const QString &sandbox_id)
{
    if (m_sandbox) {
        if (m_sandbox->AcceptReplaceGroup(walletId(), sandbox_id)) {
            GetReplaceGroups();
            m_sandbox->setScreenFlow("setup-group-wallet");
            AppModel::instance()->setNewWalletInfo(dynamic_cast<Wallet*>(this));
            QSharedWallets::instance()->GetAllGroups();
            timeoutHandler(500,[=]{
                QEventProcessor::instance()->sendEvent(E::EVT_SETUP_GROUP_WALLET_REQUEST);
            });
        }
    }
}

void SandboxWallet::requestDeclineReplaceGroup(const QString &sandbox_id)
{
    if (m_sandbox) {
        if (m_sandbox->DeclineReplaceGroup(walletId(), sandbox_id)) {
            GetReplaceGroups();
        }
    }
}
