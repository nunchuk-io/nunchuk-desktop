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
import QtQuick 2.12
import HMIEVENTS 1.0
import NUNCHUCKTYPE 1.0
import "../../../Components/origins"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../../localization/STR_QML.js" as STR

QGroupWalletArea {
    width: 352
    height: 269
    readonly property var types: [
        {type: NUNCHUCKTYPE.NATIVE_SEGWIT,   displayName: STR.STR_QML_062 },
        {type: NUNCHUCKTYPE.TAPROOT,         displayName: STR.STR_QML_553 },
        {type: NUNCHUCKTYPE.NESTED_SEGWIT,   displayName: STR.STR_QML_063 },
        {type: NUNCHUCKTYPE.LEGACY,          displayName: STR.STR_QML_064 },
    ]
    signal copySandboxUrl()
    signal showQrSandboxUrl()
    signal clickSetting()
    Column {
        anchors.fill: parent
        width: parent.width
        padding: 16
        spacing: 16
        Item {
            width: 320
            height: 60
            Column {
                width: parent.width
                spacing: 4
                QLato {
                    width: parent.width
                    height: 28
                    font.weight: Font.Bold
                    font.pixelSize: 20
                    color: "#FFFFFF"
                    text: sandbox.groupName
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                }
                Row {
                    width: parent.width
                    height: 28
                    spacing: 8
                    QLato {
                        width: paintedWidth
                        height: 28
                        font.weight: Font.Normal
                        font.pixelSize: 16
                        color: "#FFFFFF"
                        text: qsTr("%1/%2 %3").arg(sandbox.groupM).arg(sandbox.groupN).arg(STR.STR_QML_069)
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignLeft
                    }

                    QLato {
                        width: paintedWidth
                        height: 16
                        font.weight: Font.Medium
                        font.pixelSize: 12
                        color: "#FFFFFF"
                        text: "•"
                        opacity: 0.4
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignLeft
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 6
                    }

                    QLato {
                        width: paintedWidth
                        height: 28
                        font.weight: Font.Normal
                        font.pixelSize: 16
                        color: "#FFFFFF"
                        text: types.find(function(e) {return e.type === sandbox.addressType }).displayName
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignLeft
                    }
                }
            }
        }
        QLine {
            width: 320
            color: "#EAEAEA"
            opacity: 0.4
        }
        Column {
            width: parent.width
            spacing: 12
            QIconTextButton {
                width: 320
                height: 40
                label: STR.STR_QML_1558
                icons: ["link-light.svg", "link-light.svg", "link-light.svg", "link-light.svg"]
                fontPixelSize: 12
                iconSize: 18
                type: eTypeQ
                enabled: sandbox.isCreate
                opacity: enabled ? 1.0 : 0.4
                onButtonClicked: {
                    copySandboxUrl()
                }
            }
            QIconTextButton {
                width: 320
                height: 40
                label: STR.STR_QML_1559
                icons: ["show-qr-light.svg", "show-qr-light.svg", "show-qr-light.svg", "show-qr-light.svg"]
                fontPixelSize: 12
                iconSize: 18
                type: eTypeQ
                enabled: sandbox.isCreate
                opacity: enabled ? 1.0 : 0.4
                onButtonClicked: {
                    showQrSandboxUrl()
                }
            }
            QIconTextButton {
                width: 320
                height: 40
                label: STR.STR_QML_157
                icons: ["setting-light.svg", "setting-light.svg", "setting-light.svg", "setting-light.svg"]
                fontPixelSize: 12
                iconSize: 18
                type: eTypeQ
                onButtonClicked: {
                    clickSetting()
                }
            }
        }
    }

}
