#ifndef REPLACEDWALLET_H
#define REPLACEDWALLET_H

#include <QObject>
#include "WalletIO.h"

class ReplacedWallet : public WalletIO
{
    Q_OBJECT
    Q_PROPERTY(bool         isReplaced                              READ isReplaced                                             NOTIFY walletChanged)
    Q_PROPERTY(QString      replaceFlow                             READ replaceFlow                                            NOTIFY replaceFlowChanged)
    Q_PROPERTY(bool         tranReplace                             READ tranReplace                                            NOTIFY tranReplaceChanged)
public:
    ReplacedWallet(const nunchuk::Wallet &w);
    ~ReplacedWallet() override = default;
    virtual bool isReplaced() const;
    void MixMasterSignerAndSingleSignerAll();
    QString replaceFlow() const;
    void setReplaceFlow(const QString& replaceFlow);
    void SelectKeyToReplace(const QString& xfp, const int index);
    void CreateANewWallet();
    void walletCreateDone();
    bool replaceFree() const;
    void updateNewKeyReplace(const QString &xfp);
    void refreshScanDevices();
    QString walletName() override;

    bool ImportColdcardViaFile(const QString &fileName, int new_index);

    bool tranReplace() const;
    void setTranReplace(bool newTranReplace);
public slots:
    int  reuseKeyGetCurrentIndex(const QString &xfp);
    QString bip32path(const QString &xfp, int index);
    bool updateKeyReplace(const QString &xfp, const int index);
    bool removeKeyReplaced(const int index);
signals:
    void replaceFlowChanged();
    void tranReplaceChanged();
private:
    QString m_replaceFlow {"replace-key-info"};
    struct Signer_t {
        nunchuk::SingleSigner signer;
        int index;
    };
    Signer_t m_curReplaceKey;
    bool m_replaceFree {false};
    static bool m_tranReplace;
};

#endif // REPLACEDWALLET_H
