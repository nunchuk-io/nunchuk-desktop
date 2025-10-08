#ifndef QSIGNERMANAGEMENT_H
#define QSIGNERMANAGEMENT_H

#include <QObject>
#include "PrimaryKeyAccount.h"

typedef bool (*Execute)();
class QSignerManagement : public PrimaryKeyAccount
{
    Q_OBJECT
private:
    QSignerManagement();
    ~QSignerManagement();
public:
    static QSignerManagement *instance();
    QSignerManagement(QSignerManagement &other) = delete;
    QSignerManagement(QSignerManagement const &other) = delete;
    void operator=(const QSignerManagement &other) = delete;

    void registerCreateMasterSigner(Execute func) override;
    bool finishCreateMasterSigner() override;

    void registerCreateRemoteSigner(Execute func) override;
    bool finishCreateRemoteSigner() override;

    void registerCreateSoftwareSigner(Execute func) override;
    bool finishCreateSoftwareSigner() override;

    void registerCreateSoftwareSignerXprv(Execute func) override;
    bool finishCreateSoftwareSignerXprv() override;

    void clearExecute();

    void requestCreateSoftwareSigner(const QString &name,
                                 const QString &mnemonic,
                                 const QString &passphrase,
                                 bool replace = false);
    void requestCreateSoftwareSignerXprv(const QString &name,
                                     const QString &xprv,
                                     bool replace = false);

    void registerCreateTopUpXpub(Execute func);
    bool finishCreateTopUpXpub();

public slots:
    void requestCreateSignerFromHomeScreen();
    void requestCreateSignerFromReplaceScreen();
    void requestCreateSignerFromClaimScreen();
signals:
private:
    Execute m_executeMaster {nullptr};
    Execute m_executeRemote {nullptr};
    Execute m_executeSoftware {nullptr};
    Execute m_executeSoftXprv {nullptr};
    Execute m_executeTopUpXpub {nullptr};
};

#endif // QSIGNERMANAGEMENT_H
