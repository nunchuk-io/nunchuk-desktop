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
#include <QQmlEngine>
#include "QNunchukMatrixEvent.h"

QNunchukMatrixEvent::QNunchukMatrixEvent()
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

QNunchukMatrixEvent::QNunchukMatrixEvent(nunchuk::NunchukMatrixEvent e): evt_(e)
{

}

QNunchukMatrixEvent::~QNunchukMatrixEvent()
{

}

nunchuk::NunchukMatrixEvent QNunchukMatrixEvent::nunchukEvent() const
{
    return evt_;
}

QString QNunchukMatrixEvent::get_type() const
{
    return QString::fromStdString(evt_.get_type());
}

void QNunchukMatrixEvent::set_type(const QString &value)
{
    evt_.set_type(value.toStdString());
}

QString QNunchukMatrixEvent::get_content() const
{
    return QString::fromStdString(evt_.get_content());
}

void QNunchukMatrixEvent::set_content(const QString &value)
{
    evt_.set_content(value.toStdString());
}

QString QNunchukMatrixEvent::get_event_id() const
{
    return QString::fromStdString(evt_.get_event_id());
}

void QNunchukMatrixEvent::set_event_id(const QString &value)
{
    evt_.set_event_id(value.toStdString());
}

QString QNunchukMatrixEvent::get_room_id() const
{
    return QString::fromStdString(evt_.get_room_id());
}

void QNunchukMatrixEvent::set_room_id(const QString &value)
{
    evt_.set_room_id(value.toStdString());
}

QString QNunchukMatrixEvent::get_sender() const
{
    return QString::fromStdString(evt_.get_sender());
}

void QNunchukMatrixEvent::set_sender(const QString &value)
{
    evt_.set_sender(value.toStdString());
}

time_t QNunchukMatrixEvent::get_ts() const
{
    return evt_.get_ts();
}

void QNunchukMatrixEvent::set_ts(time_t value)
{
    evt_.set_ts(value);
}
