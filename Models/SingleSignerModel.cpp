#include "SingleSignerModel.h"
#include "qUtils.h"
#include <QQmlEngine>

SingleSigner::SingleSigner() :  name_(""),
    xpub_(""),
    public_key_(""),
    derivation_path_(""),
    master_fingerprint_(""),
    master_signer_id_(""),
    last_health_check_(QDateTime::fromTime_t(0)),
    isBeingUsed_(true),
    signed_(false),
    isRemote_(true),
    isValid_(false),
    message_(qUtils::QGenerateRandomMessage()),
    signature_(""),
    health_(-1),
    isColdCard_(false),
    warningMessage_(QSharedPointer<QWarningMessage>(new QWarningMessage())),
    checked_(false)
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

SingleSigner::SingleSigner(const QString &pr_name,
                           const QString &pr_xpub,
                           const QString &pr_public_key,
                           const QString &pr_derivation_path,
                           const QString &pr_master_fingerprint,
                           const QString &pr_master_signer_id,
                           const QDateTime& pr_last_health_check)
    : name_(pr_name),
      xpub_(pr_xpub),
      public_key_(pr_public_key),
      derivation_path_(pr_derivation_path),
      master_fingerprint_(pr_master_fingerprint),
      master_signer_id_(pr_master_signer_id),
      last_health_check_(pr_last_health_check),
      isBeingUsed_(true),
      signed_(false),
      isRemote_(true),
      isValid_(false),
      message_(qUtils::QGenerateRandomMessage()),
      signature_(""),
      health_(-1),
      isColdCard_(false),
      warningMessage_(QSharedPointer<QWarningMessage>(new QWarningMessage())),
      checked_(false)
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

SingleSigner::~SingleSigner() {
    warningMessage_.clear();
}

QString SingleSigner::name() const {return name_;}

QString SingleSigner::xpub() const {return xpub_;}

QString SingleSigner::publickey() const {return public_key_;}

QString SingleSigner::derivationPath() const {return derivation_path_;}

QString SingleSigner::masterFingerPrint() const {return  master_fingerprint_;}

QString SingleSigner::masterSignerId() const {return master_signer_id_;}

QString SingleSigner::lastHealthCheck() {
    if(0 == last_health_check_.toTime_t()){
        return "N/A"; // There is no time
    }
    return this->timeGapCalculationShort(last_health_check_); // last_health_check_.toString( "MM/dd/yyyy hh:mm AP" );
}

QDateTime SingleSigner::lastHealthCheckDateTime() const {
    return last_health_check_;
}

QString SingleSigner::timeGapCalculationShort(QDateTime in)
{
    QDateTime today = QDateTime::currentDateTime();
    qint64 secs = in.secsTo(today);
    const qint64 DAY = 86400;
    qint64 days = secs / DAY;
    QTime t = QTime(0,0).addSecs(secs % DAY);
    if(abs(days) > 0){
        return QString("%1 %2 ago").arg(abs(days)).arg(abs(days) > 1 ? "days" : "day");
    }
    else{
        if(t.hour() > 0){
            if(t.minute() > 0){
                return QString("%1 %2 %3 %4 ago").arg(t.hour()).arg(t.hour() > 1 ? "hours" : "hour").arg(t.minute()).arg(t.minute() > 1 ? "minutes" : "minute");
            }
            else{
                return QString("%1 %2ago").arg(t.hour()).arg(t.hour() > 1 ? "hours" : "hour");
            }
        }
        else{
            if(t.minute() == 0 && t.second() < 10){
                return QString("Just now");
            }
            else if(t.minute() == 0 && t.second() >= 10){
                return QString("Few seconds ago");
            }
            else{
                return QString("%1 %2 ago").arg(t.minute()).arg(t.minute() > 1 ? "minutes" : "minute");
            }
        }
    }
}

QString SingleSigner::timeGapCalculation(QDateTime in)
{
    QDateTime today = QDateTime::currentDateTime();
    qint64 secs = in.secsTo(today);
    const qint64 DAY = 86400;
    qint64 days = secs / DAY;
    QTime t = QTime(0,0).addSecs(secs % DAY);
    if(abs(days) > 0){
        if(t.hour() > 0){
            return QString("%1 %2 %3 %4 ago").arg(abs(days)).arg(abs(days) > 1 ? "days" : "day").arg(t.hour()).arg(t.hour() > 1 ? "hours" : "hour");
        }
        else{
            return QString("%1 %2 ago").arg(abs(days)).arg(abs(days) > 1 ? "days" : "day");
        }
    }
    else{
        if(t.hour() > 0){
            if(t.minute() > 0){
                return QString("%1 %2 %3 %4 ago").arg(t.hour()).arg(t.hour() > 1 ? "hours" : "hour").arg(t.minute()).arg(t.minute() > 1 ? "minutes" : "minute");
            }
            else{
                return QString("%1 %2ago").arg(t.hour()).arg(t.hour() > 1 ? "hours" : "hour");
            }
        }
        else{
            if(t.minute() == 0 && t.second() < 10){
                return QString("Just now");
            }
            else if(t.minute() == 0 && t.second() >= 10){
                return QString("Few seconds ago");
            }
            else{
                return QString("%1 %2 ago").arg(t.minute()).arg(t.minute() > 1 ? "minutes" : "minute");
            }
        }
    }
}

bool SingleSigner::signerSigned() const
{
    return signed_;
}

void SingleSigner::setName(const QString &d) {
    if(d != name_){
        name_ = d;
        emit nameChanged();
    }
}

void SingleSigner::setXpub(const QString &d) {
    if(d != xpub_){
        xpub_ = d;
        emit xpubChanged();
    }
}

void SingleSigner::setPublickey(const QString &d) {
    if(d != public_key_){
        public_key_ = d;
        emit publickeyChanged();
    }
}

void SingleSigner::setDerivationPath(const QString &d) {
    if(d != derivation_path_){
        derivation_path_ = d;
        emit derivationPathChanged();
    }
}

void SingleSigner::setMasterFingerPrint(const QString &d) {
    if(d != master_fingerprint_){
        master_fingerprint_ = d;
        emit masterFingerPrintChanged();
    }
}

void SingleSigner::setMasterSignerId(const QString &d) {
    if(d != master_signer_id_){
        master_signer_id_ = d;
        emit masterSignerIdChanged();
    }
}

void SingleSigner::setlastHealthCheck(const QDateTime& d) {
    if(d != last_health_check_){
        last_health_check_ = d;
        emit lastHealthCheckChanged();
    }
}

void SingleSigner::setSignerSigned(const bool d)
{
    if(d != signed_){
        signed_ = d;
        emit signerSignedChanged();
    }
}

bool SingleSigner::isRemote() const
{
    return isRemote_;
}

void SingleSigner::setIsRemote(bool isRemote)
{
    if(isRemote_ != isRemote){
        isRemote_ = isRemote;
        emit isRemoteChanged();
    }
}

bool SingleSigner::isValid() const
{
    return isValid_;
}

void SingleSigner::setIsValid(bool isValid)
{
    if(isValid_ != isValid){
        isValid_ = isValid;
        emit isValidChanged();
    }
}

QString SingleSigner::message() const
{
    return message_;
}

void SingleSigner::setMessage(const QString &message)
{
    if(message_ != message){
        message_ = message;
        emit messageChanged();
    }
}

QString SingleSigner::signature() const
{
    return signature_;
}

void SingleSigner::setSignature(const QString &signature)
{
    if(signature_ != signature){
        signature_ = signature;
        emit signatureChanged();
    }
}

int SingleSigner::health() const
{
    return health_;
}

void SingleSigner::setHealth(int health)
{
    if(health_ != health){
        health_ = health;
        emit healthChanged();
    }
}

bool SingleSigner::isColdCard() const
{
    return isColdCard_;
}

void SingleSigner::setIsColdCard(const bool isColdCard)
{
    if(isColdCard_ != isColdCard){
        isColdCard_ = isColdCard;
        emit isColdCardChanged();
    }
}

QSharedPointer<QWarningMessage> SingleSigner::warningMessagePtr() const
{
    return warningMessage_;
}

QWarningMessage* SingleSigner::warningMessage() const
{
    return warningMessage_.data();
}

void SingleSigner::setWarningMessage(const QSharedPointer<QWarningMessage> &warningMessage)
{
    warningMessage_ = warningMessage;
    emit warningMessageChanged();
}

bool SingleSigner::checked() const
{
    return checked_;
}

void SingleSigner::setChecked(const bool checked)
{
    if(checked != checked_){
        checked_ = checked;
        emit checkedChanged();
    }
}

bool SingleSigner::isBeingUsed() const
{
    return isBeingUsed_;
}

void SingleSigner::setIsBeingUsed(bool isBeingUsed)
{
    if(isBeingUsed_ != isBeingUsed){
        isBeingUsed_ = isBeingUsed;
        emit isBeingUsedChanged();
    }
}

SingleSignerListModel::SingleSignerListModel(){
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

SingleSignerListModel::~SingleSignerListModel(){d_.clear();}

int SingleSignerListModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return d_.count();
}

QVariant SingleSignerListModel::data(const QModelIndex &index, int role) const
{
    switch (role) {
    case single_signer_name_Role:
        return d_[index.row()]->name();
    case single_signer_xpub_Role:
        return d_[index.row()]->xpub();
    case single_signer_public_key_Role:
        return d_[index.row()]->publickey();
    case single_signer_derivation_path_Role:
        return d_[index.row()]->derivationPath();
    case single_signer_master_fingerprint_Role:
        return d_[index.row()]->masterFingerPrint();
    case single_signer_master_signer_id_Role:
        return d_[index.row()]->masterSignerId();
    case single_signer_last_health_check_Role:
        return d_[index.row()]->lastHealthCheck();
    case single_signer_signed_status_Role:
        return d_[index.row()]->signerSigned();
    case single_signer_isRemote_Role:
        return d_[index.row()]->isRemote();
    case single_signer_signed_isValid_Role:
        return d_[index.row()]->isValid();
    case single_signer_signed_message_Role:
        return d_[index.row()]->message();
    case single_signer_signed_signature_Role:
        return d_[index.row()]->signature();
    case single_signer_health_Role:
        return d_[index.row()]->health();
    case single_signer_isColdcard_Role:
        return d_[index.row()]->isColdCard();
    case single_signer_checked_Role:
        return d_[index.row()]->checked();
    default:
        return QVariant();
    }
}

bool SingleSignerListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(role == single_signer_signed_message_Role) {
        d_[index.row()]->setMessage(value.toString());
    }
    else if(role == single_signer_signed_signature_Role){
        d_[index.row()]->setSignature(value.toString());
    }
    else{ }
    emit dataChanged(index, index, { role } );
    return true;
}

QHash<int, QByteArray> SingleSignerListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[single_signer_name_Role]                = "singleSigner_name";
    roles[single_signer_xpub_Role]                = "singleSigner_xpub";
    roles[single_signer_public_key_Role]          = "singleSigner_publickey";
    roles[single_signer_derivation_path_Role]     = "singleSigner_derivationPath";
    roles[single_signer_master_fingerprint_Role]  = "singleSigner_masterFingerPrint";
    roles[single_signer_master_signer_id_Role]    = "singleSigner_masterSignerId";
    roles[single_signer_last_health_check_Role]   = "singleSigner_lastHealthCheck";
    roles[single_signer_signed_status_Role]       = "single_signer_signed_status";
    roles[single_signer_isRemote_Role]            = "single_signer_isRemote";
    roles[single_signer_signed_isValid_Role]      = "single_signer_signed_isValid";
    roles[single_signer_signed_message_Role]      = "single_signer_signed_message";
    roles[single_signer_signed_signature_Role]    = "single_signer_signed_signature";
    roles[single_signer_health_Role]              = "single_signer_signed_health";
    roles[single_signer_isColdcard_Role]          = "single_signer_isColdcard";
    roles[single_signer_checked_Role]             = "single_signer_checked";
    return roles;
}

void SingleSignerListModel::addSingleSigner(const QString &pr_name,
                                            const QString &pr_xpub,
                                            const QString &pr_public_key,
                                            const QString &pr_derivation_path,
                                            const QString &pr_master_fingerprint,
                                            const QString &pr_master_signer_id,
                                            const QDateTime& pr_last_health_check)
{
    beginResetModel();
    if(!containsSigner(pr_master_fingerprint, pr_derivation_path)){
        QSharedPointer<SingleSigner> tpsigner = QSharedPointer<SingleSigner>(new SingleSigner(pr_name,
                                                                                              pr_xpub,
                                                                                              pr_public_key,
                                                                                              pr_derivation_path,
                                                                                              pr_master_fingerprint,
                                                                                              pr_master_signer_id,
                                                                                              pr_last_health_check));
        if(tpsigner){
            if(pr_master_signer_id.isEmpty() || pr_master_signer_id.isNull() || pr_master_signer_id == "-1"  || pr_master_signer_id == ""){
                tpsigner.data()->setIsRemote(true);
            }
            else{
                tpsigner.data()->setIsRemote(false);
            }
            d_.append(tpsigner);
        }
    }
    endResetModel();
}

void SingleSignerListModel::replaceSingleSigner(int index, const QSharedPointer<SingleSigner> &value)
{
    if(index < 0 || index >= d_.count()){
        DBG_INFO << "Index out of range";
    }
    else {
        beginResetModel();
        d_.replace(index, value);
        endResetModel();
    }
}

void SingleSignerListModel::addSingleSigner(const QSharedPointer<SingleSigner> &d)
{
    beginResetModel();
    if(d){
        if(!containsSigner(d.data()->masterFingerPrint(), d.data()->derivationPath())){
            if(d.data()->masterSignerId().isEmpty()
                || d.data()->masterSignerId().isNull()
                || d.data()->masterSignerId() == "-1"
                || d.data()->masterSignerId() == "")
            {
                d->setIsRemote(true);
            }
            d_.append(d);
        }
    }
    endResetModel();
}

void SingleSignerListModel::initSignatures()
{
    beginResetModel();
    for (int var = 0; var < d_.count(); var++) {
        d_[var]->setSignerSigned(false);
    }
    endResetModel();
}


void SingleSignerListModel::updateSignatures(const QString &masterfingerprint, const bool value)
{
    for (int var = 0; var < d_.count(); var++) {
        if(0 == QString::compare(masterfingerprint, d_.at(var).data()->masterFingerPrint(), Qt::CaseInsensitive)){
            d_[var]->setSignerSigned(value);
        }
    }
}

bool SingleSignerListModel::needTopUpXpubs() const
{
    foreach (QSharedPointer<SingleSigner> i , d_ ){
        if((!i.data()->isRemote()) && (!i.data()->isValid())){
            return true;
        }
    }
    return false;
}

void SingleSignerListModel::initIsColdCard()
{
    beginResetModel();
    for (int var = 0; var < d_.count(); var++) {
        d_[var]->setIsColdCard(false);
    }
    endResetModel();
}

void SingleSignerListModel::setIsColdCard(const QString &id)
{
    beginResetModel();
    if(id != ""){
        foreach (QSharedPointer<SingleSigner> i , d_ ){
            if(i.data()->masterSignerId() == id){
                i.data()->setIsColdCard(true);
            }
        }
    }
    endResetModel();
}

void SingleSignerListModel::updateSingleSignerList(const QSharedPointer<SingleSignerListModel> &d)
{
    beginResetModel();
    d_.clear();
    for (int var = 0; var < d.data()->rowCount(); var++) {
        d_.append(d.data()->getSingleSignerByIndex(var));
    }
    endResetModel();
}

QSharedPointer<SingleSigner> SingleSignerListModel::getSingleSignerByIndex(const int index) {
    if(index < 0 || index >= d_.count()){
        DBG_INFO << "Index out of range";
        QSharedPointer<SingleSigner> fail(NULL);
        return fail;
    }
    else {
        return d_.at(index);
    }
}

bool SingleSignerListModel::containsMasterSignerId(const QString &masterSignerId) {
    foreach (QSharedPointer<SingleSigner> i , d_ ){
        if(masterSignerId == i.data()->masterSignerId()){
            return true;
        }
    }
    return false;
}

bool SingleSignerListModel::removeSingleSignerByIndex(const int index)
{
    beginResetModel();
    d_.removeAt(index);
    endResetModel();
    return false;
}

bool SingleSignerListModel::removeSingleSignerByFingerprint(const QString &fp)
{
    bool ret = false;
    beginResetModel();
    foreach (QSharedPointer<SingleSigner> it, d_) {
        if(0 == QString::compare(fp, it.data()->masterFingerPrint(), Qt::CaseInsensitive)){
            d_.removeAll(it);
            ret = true;
            break;
        }
    }
    endResetModel();
    return ret;
}

QString SingleSignerListModel::getMasterSignerIdByIndex(const int index)
{
    QString ret = "";
    if(index < 0 || index >= d_.count()){
        DBG_INFO << "Index out of range";
    }
    else {
        ret = d_.at(index)->masterSignerId();
    }

    return ret;
}

QString SingleSignerListModel::getMasterSignerXfpByIndex(const int index)
{
    QString ret = "";
    if(index < 0 || index >= d_.count()){
        DBG_INFO << "Index out of range";
    }
    else {
        ret = d_.at(index)->masterFingerPrint();
    }

    return ret;
}

QSharedPointer<SingleSigner> SingleSignerListModel::getSingleSignerByFingerPrint(const QString &fp) {

    foreach (QSharedPointer<SingleSigner> i , d_ ){
        if(0 == QString::compare(fp, i.data()->masterFingerPrint(), Qt::CaseInsensitive)){
            return i;
        }
    }
    return NULL;
}

int SingleSignerListModel::getnumberSigned() {
    int ret = 0;
    foreach (QSharedPointer<SingleSigner> i , d_ ){
        if(true == i.data()->signerSigned()){
            ret++;
        }
    }
    return ret;
}

bool SingleSignerListModel::containsSigner(const QString &xfp, const QString &path)
{
    foreach (QSharedPointer<SingleSigner> i , d_ ){
        if(0 == QString::compare(path, i.data()->derivationPath(), Qt::CaseInsensitive)
                && 0 == QString::compare(xfp, i.data()->masterFingerPrint(), Qt::CaseInsensitive))
        {
            return true;
        }
    }
    return false;
}

bool SingleSignerListModel::contains(const QString &masterFingerPrint)
{
    foreach (QSharedPointer<SingleSigner> i , d_ ){
        if(0 == QString::compare(masterFingerPrint, i.data()->masterFingerPrint(), Qt::CaseInsensitive)){
            return true;
        }
    }
    return false;
}

bool SingleSignerListModel::checkUsableToSign(const QString &masterFingerPrint)
{
    bool isContains = contains(masterFingerPrint);
    if(isContains){
        foreach (QSharedPointer<SingleSigner> i , d_ ){
            if(0 == QString::compare(masterFingerPrint, i.data()->masterFingerPrint(), Qt::CaseInsensitive)) {
                if(!i.data()->signerSigned()){
                    return true;
                }
            }
        }
    }
    return false;
}

void SingleSignerListModel::updateSignerHealthStatus(const QString &masterSignerId, const int status, const time_t time)
{
    beginResetModel();
    foreach (QSharedPointer<SingleSigner> i , d_ ){
        if(i.data() && masterSignerId == i.data()->masterSignerId()){
            i.data()->setHealth(status);
            i.data()->setlastHealthCheck(QDateTime::fromTime_t(time));
        }
    }
    endResetModel();
}

void SingleSignerListModel::notifyMasterSignerDeleted(const QString &masterSignerId)
{
    beginResetModel();
    foreach (QSharedPointer<SingleSigner> i , d_ ){
        if(i.data() && masterSignerId == i.data()->masterSignerId()){
            i.data()->setIsRemote(true);
        }
    }
    endResetModel();
}

void SingleSignerListModel::notifyMasterSignerRenamed(const QString &masterSignerId, const QString &newname)
{
    beginResetModel();
    foreach (QSharedPointer<SingleSigner> i , d_ ){
        if(i.data() && masterSignerId == i.data()->masterSignerId()){
            i.data()->setName(newname);
        }
    }
    endResetModel();
}

void SingleSignerListModel::notifyRemoteSignerRenamed(const QString &fingerprint, const QString &newname)
{
    beginResetModel();
    foreach (QSharedPointer<SingleSigner> i , d_ ){
        if(0 == QString::compare(fingerprint, i.data()->masterFingerPrint(), Qt::CaseInsensitive)){
            i.data()->setName(newname);
        }
    }
    endResetModel();
}

void SingleSignerListModel::resetUserChecked()
{
    beginResetModel();
    foreach (QSharedPointer<SingleSigner> it, d_) {
        it.data()->setChecked(false);
    }
    endResetModel();
}

void SingleSignerListModel::setUserCheckedByFingerprint(const bool state, const QString fp)
{
    beginResetModel();
    foreach (QSharedPointer<SingleSigner> it, d_) {
        DBG_INFO << fp << it.data()->masterFingerPrint();
        if(0 == QString::compare(fp, it.data()->masterFingerPrint(), Qt::CaseInsensitive)){
            it.data()->setChecked(state);
        }
    }
    endResetModel();
}

void SingleSignerListModel::setUserChecked(const bool state, const int index)
{
    if(index < 0 || index >= d_.count()){
        DBG_INFO << "Index out of range";
    }
    else {
        beginResetModel();
        d_.at(index)->setChecked(state);
        endResetModel();
    }
}

void SingleSignerListModel::updateHealthCheckTime()
{
    beginResetModel();
    foreach (QSharedPointer<SingleSigner> it, d_) {
        it.data()->lastHealthCheckChanged();
    }
    endResetModel();
}

bool SingleSignerListModel::containsDevicesConnected(const QStringList xfp)
{
    for (int i = 0; i < xfp.count(); i++) {
        foreach (QSharedPointer<SingleSigner> it, d_) {
            if(!it.data()->isRemote() && (0 == QString::compare(xfp.at(i), it.data()->masterFingerPrint(), Qt::CaseInsensitive))){
                return true;
            }
        }
    }
    return false;
}

void SingleSignerListModel::requestSort(int role, int order)
{
    beginResetModel();
    if(d_.count() > 1){
        switch (role) {
        case single_signer_name_Role:
        {
            if(Qt::DescendingOrder == order){
                qSort(d_.begin(), d_.end(), sortSingleSignerByNameDescending);
            }
            else{
                qSort(d_.begin(), d_.end(), sortSingleSignerByNameAscending);
            }
        }
            break;
        default:
            break;
        }
    }
    endResetModel();
}

bool sortSingleSignerByNameAscending(const QSharedPointer<SingleSigner> &v1, const QSharedPointer<SingleSigner> &v2)
{
    return (QString::compare((v1.data()->name()), (v2.data()->name())) < 0);
}

bool sortSingleSignerByNameDescending(const QSharedPointer<SingleSigner> &v1, const QSharedPointer<SingleSigner> &v2)
{
    return (QString::compare((v1.data()->name()), (v2.data()->name())) > 0);
}
