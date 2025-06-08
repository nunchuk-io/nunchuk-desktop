#include "HotWallet.h"
#include "bridgeifaces.h"

HotWallet::HotWallet(const nunchuk::Wallet &w) :
    CoinWallet{w}
{
}

bool HotWallet::keyNeedBackup() const
{
    return nunchukWallet().need_backup();
}

void HotWallet::setKeyNeedBackup(const bool bVal)
{
    if (bVal != keyNeedBackup()) {
        auto wallet = nunchukWallet();
        wallet.set_need_backup(bVal);
        setNunchukWallet(wallet);
        emit walletChanged();
    }
}

bool HotWallet::allowBackup() {
    if(auto signers = singleSignersAssigned()){
        for (auto &signer : signers->fullList()) {
            bool has_signer = bridge::nunchukHasSinger(signer->originSingleSigner());
            if(signer && !has_signer && keyNeedBackup()) {
                return false;
            }
        }
    }
    return true;
}