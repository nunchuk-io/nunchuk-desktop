/**************************************************************************
 * This file is part of the Nunchuk software (https://nunchuk.io/)        *
 * Copyright (C) 2020-2022 Enigmo								          *
 * Copyright (C) 2022 Nunchuk								              *
 *                                                                        *
 * This program is free software; you can redistribute it and/or          *
 * modify it under the terms of the GNU General Public License            *
 * as published by the Free Software Foundation; either version 3         *
 * of the License, or (at your option) any later version.                 *
 *                                                                        *
 * This program is distributed in the hope that it will be useful,        *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 * GNU General Public License for more details.                           *
 *                                                                        *
 * You should have received a copy of the GNU General Public License      *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.  *
 *                                                                        *
 **************************************************************************/
#include "QWarningMessage.h"
#include <QQmlEngine>

QWarningMessage::QWarningMessage() : code_(0), what_(""), type_((int)EWARNING::WarningType::NONE_MSG), explaination_("")
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

QWarningMessage::~QWarningMessage()
{}

QString QWarningMessage::contentDisplay()
{
    switch(type()){
    case (int)EWARNING::WarningType::ERROR_MSG:
        return QString("%1").arg(what());
        break;
    case (int)EWARNING::WarningType::EXCEPTION_MSG:
        return QString("%1: [%2] %3").arg(explaination()).arg(code()).arg(what());
        break;
    default:
        return QString("%1").arg(what());
        break;
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

int QWarningMessage::popupType() const
{
    return popupType_;
}

void QWarningMessage::setPopupType(int popup)
{
    if (popupType_ == popup)
        return;

    popupType_ = popup;
    emit popupTypeChanged();
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

void QWarningMessage::setWarningMessage(int code, const QString &what, EWARNING::WarningType type)
{
    setCode(code);
    setWhat(what);
    setType((int)type);
    switch(type){
    case EWARNING::WarningType::ERROR_MSG:
        setExplaination("");
        break;
    case EWARNING::WarningType::EXCEPTION_MSG:
        setExplaination("Error");
        break;
    case EWARNING::WarningType::WARNING_MSG:
        setExplaination("Warning");
        break;
    default:
        setExplaination("");
        break;
    }
    emit contentDisplayChanged();
}

void QWarningMessage::resetWarningMessage()
{
    setExplaination("");
    setCode(0);
    setWhat("");
    setType((int)EWARNING::WarningType::NONE_MSG);
    emit contentDisplayChanged();
}

bool QWarningMessage::isSuccess() const
{
    return type_ == (int)EWARNING::WarningType::NONE_MSG;
}