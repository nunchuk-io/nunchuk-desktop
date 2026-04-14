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

#include "Shared.h"

Shared::Shared() : m_rest(new QRest()) {}

Shared::~Shared() {}

Shared *Shared::instance() {
    static Shared mInstance;
    return &mInstance;
}

bool Shared::SendSharedWalletInvitation(const QJsonObject &request_body, QJsonObject &output, QString &errormsg) {
    if (request_body.isEmpty())
        return false;
    int reply_code = -1;
    QString reply_msg = "";
    QString cmd = commands[Share::CMD_IDX::SEND_INVITATION];

    QJsonObject jsonObj = m_rest->postSync(cmd, {}, {}, request_body, reply_code, reply_msg);
    if (reply_code == DRACO_CODE::SUCCESSFULL) {
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if (response_code == DRACO_CODE::RESPONSE_OK) {
            output = jsonObj["data"].toObject();
            return true;
        } else {
            errormsg = response_msg;
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Shared::GetGroupInvitations(const QString group_id, QJsonObject &output, QString &errormsg) {
    if (group_id.isEmpty())
        return false;
    QJsonObject data;
    int reply_code = -1;
    QString reply_msg = "";
    QString cmd = commands[Share::CMD_IDX::GET_GROUP_INVITATIONS];
    cmd.replace("{group_id}", group_id);

    QJsonObject jsonObj = m_rest->getSync(cmd, data, reply_code, reply_msg);
    if (reply_code == DRACO_CODE::SUCCESSFULL) {
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if (response_code == DRACO_CODE::RESPONSE_OK) {
            output = jsonObj["data"].toObject();
            return true;
        } else {
            errormsg = response_msg;
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Shared::RemoveInvitation(const QString invitation_id, QJsonObject &output, QString &errormsg) {
    if (invitation_id.isEmpty())
        return false;
    QJsonObject data;
    int reply_code = -1;
    QString reply_msg = "";
    QString cmd = commands[Share::CMD_IDX::REMOVE_INVITATION];
    cmd.replace("{invitation_id}", invitation_id);

    QJsonObject jsonObj = m_rest->deleteSync(cmd, data, reply_code, reply_msg);
    if (reply_code == DRACO_CODE::SUCCESSFULL) {
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if (response_code == DRACO_CODE::RESPONSE_OK) {
            output = jsonObj["data"].toObject();
            return true;
        } else {
            errormsg = response_msg;
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Shared::GetSharedWalletInvitations(QJsonObject &output, QString &errormsg) {
    QJsonObject data;
    int reply_code = -1;
    QString reply_msg = "";
    QString cmd = commands[Share::CMD_IDX::GET_INVITATIONS];

    QJsonObject jsonObj = m_rest->getSync(cmd, data, reply_code, reply_msg);
    if (reply_code == DRACO_CODE::SUCCESSFULL) {
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if (response_code == DRACO_CODE::RESPONSE_OK) {
            output = jsonObj["data"].toObject();
            return true;
        } else {
            errormsg = response_msg;
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}

bool Shared::DenySharedWalletInvitation(const QString invitation_id, QJsonObject &output, QString &errormsg) {
    if (invitation_id.isEmpty())
        return false;
    int reply_code = -1;
    QString reply_msg = "";
    QString cmd = commands[Share::CMD_IDX::DENY_INVITATION];
    cmd.replace("{invitation_id}", invitation_id);

    QJsonObject jsonObj = m_rest->postSync(cmd, {}, reply_code, reply_msg);
    if (reply_code == DRACO_CODE::SUCCESSFULL) {
        QJsonObject errorObj = jsonObj["error"].toObject();
        int response_code = errorObj["code"].toInt();
        QString response_msg = errorObj["message"].toString();
        if (response_code == DRACO_CODE::RESPONSE_OK) {
            output = jsonObj["data"].toObject();
            return true;
        } else {
            errormsg = response_msg;
            return false;
        }
    }
    errormsg = reply_msg;
    return false;
}
