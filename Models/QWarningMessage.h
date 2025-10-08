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

#ifndef QERRORCODE_H
#define QERRORCODE_H

#include <QObject>

class EWARNING: public QObject
{
    Q_OBJECT
    Q_ENUMS(WarningType)
public:
    enum class WarningType {
        NONE_MSG = 0,
        SUCCESS_MSG,
        WARNING_MSG,
        ERROR_MSG,
        EXCEPTION_MSG
    };
};
Q_DECLARE_METATYPE(EWARNING::WarningType)

class POPUP: public QObject
{
    Q_OBJECT
    Q_ENUMS(PopupType)
    Q_ENUMS(WalletImportType)
public:
    enum class PopupType {
        PopupNone,
        PopupTop,
        PopupCenter,
        PopupBottom,
        PopupBottomA
    };
    enum class WalletImportType {
        IMPORT_WALLET_DB = 0,
        IMPORT_WALLET_DESCRIPTOR,
        IMPORT_WALLET_CONFIGFILE,
        IMPORT_WALLET_QRCODE,
    };
};
Q_DECLARE_METATYPE(POPUP::PopupType)

class QWarningMessage : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString  contentDisplay  READ contentDisplay                     NOTIFY contentDisplayChanged)
    Q_PROPERTY(QString  explaination    READ explaination                       NOTIFY explainationChanged)
    Q_PROPERTY(QString  what            READ what                               NOTIFY whatChanged)
    Q_PROPERTY(int      code            READ code                               NOTIFY codeChanged)
    Q_PROPERTY(int      type            READ type           WRITE setType       NOTIFY typeChanged)
    Q_PROPERTY(int      popupType       READ popupType      WRITE setPopupType  NOTIFY popupTypeChanged)
public:
    QWarningMessage();
    ~QWarningMessage();

    QString contentDisplay();

    int code() const;
    void setCode(int code);

    QString what() const;
    void setWhat(const QString &what);

    void setWarningMessage(int code, const QString &what, EWARNING::WarningType type = EWARNING::WarningType::EXCEPTION_MSG);
    void resetWarningMessage();

    int type() const;
    void setType(int type);

    QString explaination() const;
    void setExplaination(const QString &explaination);

    int popupType() const;
    void setPopupType(int popup);

private:
    int code_;
    QString what_;
    int type_;
    QString explaination_;
    QString content_;
    int popupType_;
signals:
    void codeChanged();
    void whatChanged();
    void typeChanged();
    void explainationChanged();
    void contentDisplayChanged();
    void popupTypeChanged();
};
typedef QSharedPointer<QWarningMessage> QWarningMessagePtr;
Q_DECLARE_METATYPE(QWarningMessagePtr)

#endif // QERRORCODE_H
