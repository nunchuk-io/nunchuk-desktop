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
import "../../../../../localization/STR_QML.js" as STR

QCheckYourHardwareFirmware {
    property var firmwareInfo: SignerManagement.miniscriptSupportedFirmwares("LEDGER")
    label.text: STR.STR_QML_1897
    linkWeb: "https://support.ledger.com/article/7103926130845-zd"
    guideText: STR.STR_QML_1898.arg(firmwareInfo.version)
    guideWeb: STR.STR_QML_1899
    offsetWidth: 40
}
