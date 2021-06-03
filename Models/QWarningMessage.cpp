#include "QWarningMessage.h"
#include "QOutlog.h"
#include <QQmlEngine>

QWarningMessage::QWarningMessage() : code_(0), what_(""), type_((int)EWARNING::WarningType::NONE_MSG), explaination_("")
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

QWarningMessage::~QWarningMessage()
{}

QString QWarningMessage::contentDisplay()
{
    if((int)EWARNING::WarningType::EXCEPTION_MSG == type_){
        return QString("%1 : [%2] %3").arg(explaination_).arg(code_).arg(what_);
    }
    else{
        return QString("%1").arg(what_);
    }
}

int QWarningMessage::type() const
{
    return type_;
}

void QWarningMessage::setType(int type)
{
    if(type_ != type){
        type_ = type;
        if((int)EWARNING::WarningType::NONE_MSG == type_){
            resetWarningMessage();
        }
        emit typeChanged();
    }
}

QString QWarningMessage::explaination() const
{
    return explaination_;
}

void QWarningMessage::setExplaination(const QString &explaination)
{
    if(explaination_ != explaination){
        explaination_ = explaination;
        emit explainationChanged();
    }
}

QString QWarningMessage::what() const
{
    return what_;
}

void QWarningMessage::setWhat(const QString &what)
{
    if(what_ != what){
        what_ = what;
        emit whatChanged();
    }
}

int QWarningMessage::code() const
{
    return code_;
}

void QWarningMessage::setCode(int code)
{
    if(code_ != code){
        code_ = code;
        emit codeChanged();
    }
}

void QWarningMessage::setWarningMessage(int code, const QString &what, EWARNING::WarningType type, const QString& explain)
{
    setExplaination(explain);
    setCode(code);
    setWhat(what);
    setType((int)type);
    emit contentDisplayChanged();
}

void QWarningMessage::resetWarningMessage()
{
    setExplaination("");
    setCode(0);
    setWhat("NO ERROR");
    setType((int)EWARNING::WarningType::NONE_MSG);
    emit contentDisplayChanged();
}
