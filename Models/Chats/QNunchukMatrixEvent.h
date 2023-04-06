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

#ifndef QNUNCHUKMATRIXEVENT_H
#define QNUNCHUKMATRIXEVENT_H

#include <QObject>
#include "nunchukmatrix.h"

class QNunchukMatrixEvent : public QObject
{
    Q_OBJECT
public:
    QNunchukMatrixEvent();
    QNunchukMatrixEvent(nunchuk::NunchukMatrixEvent e);
    ~QNunchukMatrixEvent();
    nunchuk::NunchukMatrixEvent nunchukEvent() const;

    QString get_type() const;
    QString get_content() const;
    QString get_event_id() const;
    QString get_room_id() const;
    QString get_sender() const;
    time_t get_ts() const;

    void set_type(const QString& value);
    void set_content(const QString& value);
    void set_event_id(const QString& value);
    void set_room_id(const QString& value);
    void set_sender(const QString& value);
    void set_ts(time_t value);

private:
    nunchuk::NunchukMatrixEvent evt_;
};

#endif // QNUNCHUKMATRIXEVENT_H
