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
#include "matrixlistener.h"
#include "Chats/QRoomCustomEvent.h"
#include "QOutlog.h"
#include "Chats/ClientController.h"
// event id
std::string SendFunction(const std::string &roomid, const std::string &eventtype, const std::string &eventContent, bool ignore_error )
{
    QString rid = QString::fromStdString(roomid);
    QString event = QString::fromStdString(eventtype);
    QString event_content = QString::fromStdString(eventContent);
    QJsonObject data = QJsonDocument::fromJson(event_content.toUtf8()).object();
    QNunchukRoom* room = CLIENT_INSTANCE->GetRoomById(QString::fromStdString(roomid));
    if(room){
        DBG_INFO << " Sender thread:" << QThread::currentThread() << ">> Main thread:" <<  qApp->thread();
        if(QThread::currentThread() != qApp->thread()){
            QString txnId;
            QMetaObject::invokeMethod(room, "postEvent", Qt::BlockingQueuedConnection, Q_RETURN_ARG(QString, txnId), Q_ARG(QString, event), Q_ARG(QJsonObject, data));
            return txnId.toStdString();
        }
        else{
            return room->postEvent(event, data).toStdString();
        }
    }
    return "";
}

bool consume_sync_event_listener(int percent)
{
    AppModel::instance()->startNunchukSync();
    return true;
}

string UploadFileFunc(const string &file_name, const string &mine_type, const string &json_info, const char *data, size_t data_length)
{
    if(QThread::currentThread() != qApp->thread()){
        QMetaObject::invokeMethod(CLIENT_INSTANCE,
                                  "UploadFile",
                                  Qt::BlockingQueuedConnection,
                                  Q_ARG(QString, QString::fromStdString(file_name)),
                                  Q_ARG(QString, QString::fromStdString(mine_type)),
                                  Q_ARG(QString, QString::fromStdString(json_info)),
                                  Q_ARG(const char*, data),
                                  Q_ARG(int, data_length));
    }
    else{
        CLIENT_INSTANCE->UploadFile(QString::fromStdString(file_name),
                                    QString::fromStdString(mine_type),
                                    QString::fromStdString(json_info),
                                    data,
                                    data_length);
    }
    return "";
}

std::vector<unsigned char> DownloadFileFunc(const string &file_name, const string &mine_type, const string &json_info, const string &mxc_uri)
{
    std::vector<unsigned char> ret;
    if(ClientController::instance()->rooms()){
        if(ClientController::instance()->rooms()->allHisLoaded() == false){
            CLIENT_INSTANCE->DownloadFile(QString::fromStdString(file_name), QString::fromStdString(mine_type), QString::fromStdString(json_info), QString::fromStdString(mxc_uri));
            return ret;
        }
    }

    if(QThread::currentThread() != qApp->thread()){
        QMetaObject::invokeMethod(CLIENT_INSTANCE,
                                  "DownloadFile",
                                  Qt::BlockingQueuedConnection,
                                  Q_ARG(QString, QString::fromStdString(file_name)),
                                  Q_ARG(QString, QString::fromStdString(mine_type)),
                                  Q_ARG(QString, QString::fromStdString(json_info)),
                                  Q_ARG(QString, QString::fromStdString(mxc_uri)));
    }
    else{
        CLIENT_INSTANCE->DownloadFile(QString::fromStdString(file_name), QString::fromStdString(mine_type), QString::fromStdString(json_info), QString::fromStdString(mxc_uri));
    }
    return ret;
}

bool DownloadFileProgress(int percent)
{
    return true;
}
