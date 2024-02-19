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
import QtQuick.Controls 2.3
import QtGraphicalEffects 1.12
import Qt.labs.platform 1.1
import HMIEVENTS 1.0
import EWARNING 1.0
import NUNCHUCKTYPE 1.0
import DataPool 1.0
import "../../Components/origins"
import "../../Components/customizes"
import "../../Components/customizes/Texts"
import "../../Components/customizes/Buttons"
import "../../Components/customizes/services"
import "../../../localization/STR_QML.js" as STR

QScreen {
    property var reqiredSignature: ServiceSetting.servicesTag.reqiredSignatures
    Loader {
        anchors.centerIn: parent
        sourceComponent: {
            if (reqiredSignature.type === "SECURITY_QUESTION") {
                return enter_backup_password
            }
            else if (reqiredSignature.type === "CONFIRMATION_CODE") {
                return key_recovery_request_sent
            } else if (reqiredSignature.step === "RECOVER") {
                return _tapsigner_recovered
            }
            return null
        }
    }
    Component {
        id: enter_backup_password
        QEnterBackupPassword {
            onPrevClicked: {
                QMLHandle.sendEvent(EVT.EVT_ENTER_BACKUP_PASSWORD_BACK)
            }

            onNextClicked: {
                QMLHandle.sendEvent(EVT.EVT_INPUT_BACKUP_PASSWORD_REQUEST, backup_password.textInputted)
            }
        }
    }
    Component {
        id: key_recovery_request_sent
        QKeyRecoveryResult {
            title: STR.STR_QML_1052
            description: STR.STR_QML_1053
        }
    }
    Component {
        id: _tapsigner_recovered
        QTapsignerRecovered {
            onSignalGotIt: {
                AppModel.showToast(0, STR.STR_QML_739, EWARNING.SUCCESS_MSG);
                closeTo(NUNCHUCKTYPE.CURRENT_TAB)
            }
        }
    }
}
