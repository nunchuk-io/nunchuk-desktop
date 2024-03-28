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

#ifndef QCOMMONSTRUCTS_H
#define QCOMMONSTRUCTS_H
#include <QString>
#include <QVariant>
#include <stddef.h>
#include "QCommonDefines.h"
#include "QCommonEnums.h"

typedef struct popup_data
{
    uint        id;
    LIMIT       duration;
    QString     QmlPath;
    QVariant    msg;
    void        (*funcEntry)(QVariant msg);
    void        (*funcExit)(QVariant msg);
} POPUP_DATA;

typedef struct st_application_states
{
    uint        id;
    void        (*funcEntry)(QVariant msg);
    void        (*funcExit)(QVariant msg);
    LAYER       layerbase;
    LIMIT       duration;
    QString     QmlPath;
} APPLICATION_STATE;

typedef struct _st_state_trigger
{
    uint                    evt;
    void                    (*func)(QVariant msg);
    const APPLICATION_STATE *trans;
}STATE_TRIGGER;

typedef struct _st_state_system
{
    uint                    id;
    const STATE_TRIGGER     *trigger;
    uint                    size;
    const APPLICATION_STATE *state;
}STATE_SYSTEM;

#endif // QCOMMONSTRUCTS_H
