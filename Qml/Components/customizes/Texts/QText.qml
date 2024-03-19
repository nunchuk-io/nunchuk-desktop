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
import QtQuick 2.4

Text {
    verticalAlignment: Text.AlignVCenter
    renderType: Text.QtRendering
    font.preferShaping : false
    smooth: true
    antialiasing: true
    color: "#031F2B"
    fontSizeMode: Text.FixedSize
/*
Font.Thin
Font.Light
Font.ExtraLight
Font.Normal - the default -400
Font.Medium -500
Font.DemiBold -600
Font.Bold -700
Font.ExtraBold -800
Font.Black -900
*/
}
