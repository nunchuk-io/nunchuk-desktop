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

#ifndef LOCALIZATION_H
#define LOCALIZATION_H

#include <QObject>
#include <QTranslator>
#include <QGuiApplication>


class Localization : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString language READ language  NOTIFY languageChanged)
public:
    static Localization *instance();
    Localization(Localization &other) = delete;
    Localization(Localization const &other) = delete;
    void operator=(const Localization &other) = delete;

    void initLanguage();
    QString language() const;
    void setLanguage(const QString &lang);
private:
    Localization();
    ~Localization();
    QTranslator* translator;

signals:
    void languageChanged();
};

#endif // LOCALIZATION_H
