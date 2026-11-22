#include "QGuestMigration.h"
#include <algorithm>
#include <QApplication>
#include <QCryptographicHash>
#include <QCursor>
#include <QDir>
#include <QDirIterator>
#include <QFileInfo>
#include <QSettings>
#include <QFutureWatcher>
#include <QtConcurrent>
#include "AppModel.h"
#include "AppSetting.h"
#include "bridgeifaces.h"
#include "nunchuckiface.h"
#include "QWarningMessage.h"
#include "Servers/Draco.h"
#include "Chats/ClientController.h"
#include "QEventProcessor.h"
#include "QOutlog.h"

namespace {
const char *kPendingPromptKey = "guestMigration/pendingPromptAccount";

QString chainDirName(nunchuk::Chain chain)
{
    switch (chain) {
    case nunchuk::Chain::MAIN: return "mainnet";
    case nunchuk::Chain::TESTNET: return "testnet";
    case nunchuk::Chain::SIGNET: return "signet";
    case nunchuk::Chain::REGTEST: return "regtest";
    }
    return "mainnet";
}

const QList<nunchuk::Chain> &allChains()
{
    static const QList<nunchuk::Chain> chains {nunchuk::Chain::MAIN, nunchuk::Chain::TESTNET,
                                               nunchuk::Chain::SIGNET, nunchuk::Chain::REGTEST};
    return chains;
}

int countStorageIds(const QString &path)
{
    QDir dir(path);
    if (!dir.exists()) return 0;
    int count = 0;
    const QStringList names = dir.entryList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden);
    for (const QString &name : names) {
        if (name.size() == 8) count++;
    }
    return count;
}

bool hasAnyFile(const QString &path)
{
    QFileInfo info(path);
    if (!info.exists()) return false;
    if (!info.isDir()) return true;
    QDirIterator it(path, QDir::Files | QDir::Hidden | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
    return it.hasNext();
}

QSettings *rootSettings()
{
    return static_cast<QSettings *>(AppSetting::instance());
}
}

QGuestMigration::QGuestMigration(QObject *parent) : QObject(parent)
{
    connect(CLIENT_INSTANCE, &ClientController::isNunchukLoggedInChanged, this, &QGuestMigration::refresh);
    connect(AppModel::instance(), &AppModel::walletListChanged, this, &QGuestMigration::refresh);
    connect(AppModel::instance(), &AppModel::masterSignerListChanged, this, &QGuestMigration::refresh);
    connect(AppModel::instance(), &AppModel::remoteSignerListChanged, this, &QGuestMigration::refresh);
}

QGuestMigration *QGuestMigration::instance()
{
    static QGuestMigration mInstance;
    return &mInstance;
}

int QGuestMigration::walletCount() const { return m_walletCount; }

int QGuestMigration::keyCount() const { return m_keyCount; }

bool QGuestMigration::canMigrate() const { return m_canMigrate; }

bool QGuestMigration::busy() const { return m_busy; }

void QGuestMigration::setBusy(bool busy)
{
    if (m_busy == busy) return;
    m_busy = busy;
    emit busyChanged();
}

bool QGuestMigration::isAccountReady() const
{
    return CLIENT_INSTANCE->isNunchukLoggedIn()
           && bridge::nunchukCurrentMode() == ONLINE_MODE
           && !Draco::instance()->Uid().isEmpty();
}

bool QGuestMigration::guestEncrypted() const
{
    return rootSettings()->value("enableDBEncryption", false).toBool();
}

QString QGuestMigration::guestBasePath() const
{
    return AppSetting::instance()->storagePath();
}

QString QGuestMigration::accountDirName(const QString &account) const
{
    QByteArray digest = QCryptographicHash::hash(account.toLower().toUtf8(), QCryptographicHash::Sha256);
    std::reverse(digest.begin(), digest.end());
    return QString::fromLatin1(digest.toHex());
}

QList<nunchuk::Chain> QGuestMigration::guestChainsWithData() const
{
    QList<nunchuk::Chain> chains;
    const QString base = guestBasePath();
    for (nunchuk::Chain chain : allChains()) {
        const QString chainPath = QDir(base).filePath(chainDirName(chain));
        if (hasAnyFile(QDir(chainPath).filePath("wallets"))
            || hasAnyFile(QDir(chainPath).filePath("signers"))
            || QFileInfo::exists(QDir(chainPath).filePath("room"))) {
            chains.append(chain);
        }
    }
    return chains;
}

void QGuestMigration::refresh()
{
    if (m_busy) return;
    int wallets = 0;
    int keys = 0;
    bool can = false;
    const QString base = guestBasePath();
    if (!base.isEmpty() && isAccountReady()) {
        for (nunchuk::Chain c : allChains()) {
            const QString chainPath = QDir(base).filePath(chainDirName(c));
            wallets += countStorageIds(QDir(chainPath).filePath("wallets"));
            keys += countStorageIds(QDir(chainPath).filePath("signers"));
        }
        bool accountHasData = false;
        const QString accountPath = QDir(base).filePath(accountDirName(Draco::instance()->Uid()));
        if (QFileInfo::exists(QDir(accountPath).filePath("decoy"))) {
            accountHasData = true;
        }
        for (nunchuk::Chain c : allChains()) {
            if (accountHasData) break;
            const QString accChain = QDir(accountPath).filePath(chainDirName(c));
            accountHasData = hasAnyFile(QDir(accChain).filePath("wallets"))
                             || hasAnyFile(QDir(accChain).filePath("signers"));
        }
        can = (wallets + keys) > 0 && !accountHasData;
    }
    if (m_walletCount != wallets || m_keyCount != keys || m_canMigrate != can) {
        m_walletCount = wallets;
        m_keyCount = keys;
        m_canMigrate = can;
        emit statusChanged();
    }
}

void QGuestMigration::markNewAccount(const QString &email)
{
    if (email.isEmpty()) return;
    rootSettings()->setValue(kPendingPromptKey, email.toLower());
}

void QGuestMigration::markFederatedSignin()
{
    m_federatedCandidate = true;
}

void QGuestMigration::prepareAccountInstance(const QString &account)
{
    m_instanceAccount = account.toLower();
    if (!m_federatedCandidate) return;
    m_federatedCandidate = false;
    const QString base = guestBasePath();
    if (account.isEmpty() || base.isEmpty()) return;
    if (QFileInfo::exists(QDir(base).filePath(accountDirName(account)))) return;
    markNewAccount(account);
}

void QGuestMigration::onAccountInstanceReady()
{
    refresh();
    const QString pending = rootSettings()->value(kPendingPromptKey).toString();
    if (pending.isEmpty() || m_busy) return;
    const QString email = Draco::instance()->emailRequested().toLower();
    const QString uid = Draco::instance()->Uid().toLower();
    if (pending != email && pending != uid && pending != m_instanceAccount) return;
    rootSettings()->remove(kPendingPromptKey);
    m_promptActive = true;
    timeoutHandler(1000, [this]() {
        refresh();
        if (m_canMigrate) {
            emit promptRequested(m_walletCount, m_keyCount);
        } else {
            releasePrompt(false);
        }
    });
}

bool QGuestMigration::deferOnboarding()
{
    if (m_promptActive || m_busy) {
        m_onboardingDeferred = true;
        return true;
    }
    return false;
}

void QGuestMigration::releasePrompt(bool migrated)
{
    m_promptActive = false;
    if (migrated) {
        AppSetting::instance()->setIsFirstTimeOnboarding(true);
    }
    if (m_onboardingDeferred) {
        m_onboardingDeferred = false;
        AppModel::instance()->requestOnboarding();
    }
}

void QGuestMigration::requestPrompt()
{
    if (m_busy) return;
    refresh();
    if (m_canMigrate) {
        m_promptActive = true;
        emit promptRequested(m_walletCount, m_keyCount);
    }
}

void QGuestMigration::dismissPrompt()
{
    if (m_busy) return;
    releasePrompt(false);
}

void QGuestMigration::restoreGuestPassphrase(const std::string &storage, const QList<nunchuk::Chain> &chains, const std::string &passphrase)
{
    for (nunchuk::Chain chain : chains) {
        QWarningMessage msg;
        nunchukiface::instance()->SetStoragePassphrase(storage, "", chain, "", passphrase, msg);
        if ((int)EWARNING::WarningType::NONE_MSG != msg.type()) {
            DBG_ERROR << "Restore guest passphrase failed" << chainDirName(chain) << msg.code();
        }
    }
}

QGuestMigration::Result QGuestMigration::runMigration(const std::string &storage, const std::string &account, bool keep,
                                                      bool encrypted, const std::string &passphrase, const QList<nunchuk::Chain> &chains)
{
    Result result;
    QList<nunchuk::Chain> decrypted;
    if (encrypted) {
        for (nunchuk::Chain chain : chains) {
            QWarningMessage msg;
            nunchukiface::instance()->SetStoragePassphrase(storage, "", chain, passphrase, "", msg);
            if ((int)EWARNING::WarningType::NONE_MSG != msg.type()) {
                restoreGuestPassphrase(storage, decrypted, passphrase);
                result.code = msg.code();
                result.message = msg.what();
                result.invalidPassphrase = msg.code() == nunchuk::NunchukException::INVALID_PASSPHRASE;
                return result;
            }
            decrypted.append(chain);
        }
    }

    QWarningMessage migrateMsg;
    nunchukiface::instance()->MigrateGuestToAccount(storage, account, keep, migrateMsg);
    result.success = (int)EWARNING::WarningType::NONE_MSG == migrateMsg.type();
    if (!result.success) {
        result.code = migrateMsg.code();
        result.message = migrateMsg.what();
    }
    if (encrypted && (keep || !result.success)) {
        restoreGuestPassphrase(storage, decrypted, passphrase);
    }
    return result;
}

QString QGuestMigration::failReason(const Result &result)
{
    if (result.success) return QString();
    if (result.code == nunchuk::NunchukException::INVALID_STATE
        && result.message.contains("already has data", Qt::CaseInsensitive)) {
        return QStringLiteral("ACCOUNT_HAS_DATA");
    }
    if (result.code == nunchuk::NunchukException::INVALID_PARAMETER) {
        return QStringLiteral("INVALID_ACCOUNT");
    }
    return QStringLiteral("GENERIC");
}

void QGuestMigration::migrate(bool keep, const QString &passphrase)
{
    if (m_busy) return;
    m_promptActive = true;
    refresh();
    if (!m_canMigrate) {
        emit migrateFinished(false, keep, 0, 0, QStringLiteral("UNAVAILABLE"), QString());
        releasePrompt(false);
        return;
    }
    const bool encrypted = guestEncrypted();
    if (encrypted && passphrase.isEmpty()) {
        emit passphraseRequired(keep);
        return;
    }

    QWarningMessage pathMsg;
    nunchuk::AppSettings settings;
    if (!bridge::configureFilesystemPaths(settings, pathMsg)) {
        emit migrateFinished(false, keep, 0, 0, QStringLiteral("GENERIC"), pathMsg.what());
        releasePrompt(false);
        return;
    }
    const std::string storage = settings.get_storage_path();
    const std::string account = Draco::instance()->Uid().toStdString();
    const std::string guestPass = passphrase.toStdString();
    const int wallets = m_walletCount;
    const int keys = m_keyCount;
    const QList<nunchuk::Chain> chains = guestChainsWithData();

    setBusy(true);
    nunchukiface::instance()->stopInstance(LOCAL_MODE);
    nunchukiface::instance()->stopInstance(ONLINE_MODE);

    auto watcher = new QFutureWatcher<Result>(this);
    connect(watcher, &QFutureWatcher<Result>::finished, this, [this, watcher, keep, wallets, keys]() {
        const Result result = watcher->result();
        watcher->deleteLater();
        AppModel::instance()->makeInstanceForAccount(QVariant(), "");
        setBusy(false);
        refresh();
        if (result.invalidPassphrase) {
            emit passphraseInvalid(keep);
            return;
        }
        emit migrateFinished(result.success, keep, wallets, keys, failReason(result), result.message);
        releasePrompt(result.success);
    });
    watcher->setFuture(QtConcurrent::run([storage, account, keep, encrypted, guestPass, chains]() {
        return runMigration(storage, account, keep, encrypted, guestPass, chains);
    }));
}
