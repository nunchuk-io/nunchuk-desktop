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

#ifndef COMMONDEFINES_H
#define COMMONDEFINES_H


#define QAPP_WIDTH_EXPECTED   1280
#define QAPP_HEIGHT_EXPECTED  910
#define QAPP_GAP_HEIGHT       120
#define QWINDOW_HEIGHT_PREFER (QAPP_HEIGHT_EXPECTED + QAPP_GAP_HEIGHT)
#define QWINDOW_WIDTH_PREFER  (QAPP_WIDTH_EXPECTED + QAPP_GAP_HEIGHT)

#define POPUP_NUM_MAX   4
#define ALEN(array)     (sizeof(array)/sizeof(*(array)))

#define MAIN_VIEWPORT_QML               "qrc:/main.qml"
#define JS_SCREEN_TRANSITION_FUNCTION   "screen_Transition"
#define JS_POPUPS_TRANSITION_FUNCTION   "load_Popup"
#define JS_TOASTS_TRANSITION_FUNCTION   "load_Toast"


#endif // COMMONDEFINES_H
