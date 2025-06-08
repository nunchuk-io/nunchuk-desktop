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
import "../../../Components/origins"
import "../../../Components/customizes"
import "../../../Components/customizes/Chats"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../../localization/STR_QML.js" as STR

QOnScreenContentTypeB {
    property var alert: GroupWallet.dashboardInfo.alert
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_995
    extraHeader: Item {}
    onCloseClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
    content: Item {
        Row {
            spacing: 36
            Rectangle {
                width: 346
                height: 464
                radius: 24
                color: "#D0E2FF"
                QPicture {
                    anchors.verticalCenter: parent.verticalCenter
                    source: "qrc:/Images/Images/COLDCARD-register.svg"
                }
            }
            Item {
                width: 346
                height: 464
                QLato {
                    width: parent.width
                    height: paintedHeight
                    text: STR.STR_QML_991
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    wrapMode: Text.WordWrap
                    lineHeight: 28
                    lineHeightMode: Text.FixedHeight
                }
            }
        }
    }
    onPrevClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
    bottomRight: QTextButton {
        width: 184
        height: 48
        label.text: STR.STR_QML_994
        label.font.pixelSize: 16
        type: eTypeE
        onButtonClicked: {
            exportwalletDialog.currentFile = StandardPaths.writableLocation(StandardPaths.DocumentsLocation) + "/"
                    + RoomWalletData.getValidFilename(AppModel.walletInfo.walletName)
                    + "-Coldcard-Config.txt"
            exportwalletDialog.open()
        }
    }
    FileDialog {
        id: exportwalletDialog
        property int exportFormat: -1
        fileMode: FileDialog.SaveFile
        onAccepted: {
            var _input = {
                type: "coldcard-export-wallet",
                currentFile: exportwalletDialog.currentFile
            }
            QMLHandle.sendEvent(EVT.EVT_DASHBOARD_ALERT_INFO_ENTER, _input)
        }
    }
}
