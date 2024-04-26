#include "OnBoardingModel.h"
#include "ViewsEnums.h"
#include "Servers/Draco.h"
#include "bridgeifaces.h"

OnBoardingModel *OnBoardingModel::instance()
{
    static OnBoardingModel mInstance;
    return &mInstance;
}

OnBoardingModel::OnBoardingModel()
{

}

OnBoardingModel::~OnBoardingModel()
{

}

QString OnBoardingModel::state() const
{
    return m_state;
}

void OnBoardingModel::setState(const QString &newState)
{
    if (m_state == newState)
        return;

    m_state = newState;
    emit stateChanged();
}

QWalletPtr OnBoardingModel::ImportWalletDescriptor(const QString &file_path)
{
    QWarningMessage msg;
    QString walletName = "Imported";
    auto w = bridge::nunchukImportWalletDescriptor(file_path, walletName, "", msg);
    if(msg.type() == (int)EWARNING::WarningType::NONE_MSG){
        AppModel::instance()->startReloadUserDb();
        return w;
    }
    if (msg.type() != (int)EWARNING::WarningType::NONE_MSG) {
        AppModel::instance()->showToast(msg.code(), msg.what(), (EWARNING::WarningType)msg.type());
    }
    return w;
}

void OnBoardingModel::requestBackupHotWallet()
{
    QWarningMessage msg;
    if (auto w = AppModel::instance()->walletInfoPtr()) {
        if (w->needBackup()) {
            w->setNeedBackup(false);
            nunchukiface::instance()->UpdateWallet(w->wallet(), msg);
            if (msg.type() != (int)EWARNING::WarningType::NONE_MSG) {
                AppModel::instance()->showToast(msg.code(), msg.what(), (EWARNING::WarningType)msg.type());
            }
            else {
                AppModel::instance()->startReloadUserDb();
            }
        }
    }
}

bool OnBoardingModel::importQrHotWallet(const QStringList qrtags)
{
    QStringList in = qrtags;
    in.removeDuplicates();
    if(in.isEmpty()){
        return false;
    }
    QWarningMessage msg;
    bridge::nunchukImportKeystoneWallet(in, "", msg);
    if(msg.type() == (int)EWARNING::WarningType::NONE_MSG){
        msg.resetWarningMessage();
        if(msg.type() == (int)EWARNING::WarningType::NONE_MSG){
            AppModel::instance()->startReloadUserDb();
            return true;
        }
    }
    return false;
}

QVariantList OnBoardingModel::countries() const
{
    return m_countries.toVariantList();
}

void OnBoardingModel::setCountries(const QJsonArray &countries)
{
    if (m_countries == countries)
        return;
    m_countries = countries;
    emit countryListChanged();
}

bool OnBoardingModel::GetCountryCodeList()
{
    QJsonObject output;
    QString errormsg;
    bool ret = Draco::instance()->GetCountryCodeList(output, errormsg);
    if (ret) {
        QJsonArray countryList = output["countries"].toArray();
        setCountries(countryList);
    }
    else {

    }
    return ret;
}

bool OnBoardingModel::RequestOnboardingNoAdvisor(const QString &country_code, const QString &email, const QString &note)
{
    QString errormsg;
    bool ret = Draco::instance()->RequestOnboardingNoAdvisor(country_code, email, note, errormsg);
    if (ret) {
        AppModel::instance()->showToast(0, "Query sent", EWARNING::WarningType::SUCCESS_MSG);
    }
    return ret;
}

QWalletPtr OnBoardingModel::CreateAHotWallet(const QString &mnemonic, bool need_backup)
{
    QWarningMessage msg;
    QWalletPtr newWalletInfo = bridge::nunchukCreateHotWallet(mnemonic, "", need_backup, false, msg);
    if((int)EWARNING::WarningType::NONE_MSG == msg.type()) {
        AppModel::instance()->startReloadUserDb();
        return newWalletInfo;
    }

    if (msg.type() != (int)EWARNING::WarningType::NONE_MSG) {
        AppModel::instance()->showToast(msg.code(), msg.what(), (EWARNING::WarningType)msg.type());
    }
    return newWalletInfo;
}

