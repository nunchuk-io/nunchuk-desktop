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
import QtQuick.Controls 1.4
import QtQuick.Controls 2.3
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.12
import HMIEVENTS 1.0
import EWARNING 1.0
import QRCodeItem 1.0
import DataPool 1.0
import "../../origins"
import "../../customizes"
import "../../customizes/Texts"
import "../../customizes/Buttons"
import "../../../../localization/STR_QML.js" as STR

Popup {
    id: _infoPopup
    width: parent.width
    height: parent.height
    modal: true
    focus: true
    property string contentText: ""
    property string title: STR.STR_QML_339
    signal gotItClicked()
    closePolicy: Popup.CloseOnReleaseOutside | Popup.CloseOnEscape
    background: Item{}
    property var historyConfigs: [
        {id: 1, label: STR.STR_QML_1676 },
        {id: 7, label: STR.STR_QML_1677 },
        {id: 14, label: STR.STR_QML_1678 },
    ]
    property var walletInfo: AppModel.walletInfo
    property int retentionDays: walletInfo.retentionDays
    Rectangle {
        width: 391
        height: 308
        radius: 8
        color: Qt.rgba(255, 255, 255, 0)
        anchors.centerIn: parent
        layer.enabled: true
        layer.effect: OpacityMask {
            maskSource: Rectangle {
                width: 391
                height: 308
                radius: 24
            }
        }
        Rectangle {
            id: bgcontentlimit
            width: 391
            height: 308
            anchors.centerIn: parent
            color: "#FFFFFF"
            radius: 24
            Column {
                anchors.fill: parent
                anchors.margins: 24
                spacing: 24
                QLato {
                    width: parent.width
                    height: 20
                    font.pixelSize: 16
                    font.weight: Font.Bold
                    text: STR.STR_QML_1300
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                }
                Column {
                    width: parent.width
                    Repeater {
                        model: historyConfigs
                        QRadioButtonTypeA {
                            id: btn
                            width: 343
                            height: 48
                            label: modelData.label
                            layoutDirection: Qt.LeftToRight
                            fontFamily: "Lato"
                            fontPixelSize: 16
                            fontWeight: Font.Normal
                            selected: retentionDays === modelData.id
                            onButtonClicked: {
                                retentionDays = modelData.id
                            }
                        }
                    }
                }

                Row {
                    anchors.horizontalCenter: parent.horizontalCenter
                    spacing: 16
                    QTextButton {
                        width: 163
                        height: 48
                        label.text: STR.STR_QML_035
                        label.font.pixelSize: 16
                        type: eTypeP
                        onButtonClicked: {
                            close()
                        }
                    }
                    QTextButton {
                        width: 163
                        height: 48
                        label.text: STR.STR_QML_835
                        label.font.pixelSize: 16
                        type: eTypeE
                        onButtonClicked: {
                            walletInfo.retentionDays = retentionDays
                            close()
                        }
                    }
                }
            }
        }
        DropShadow {
            anchors.fill: bgcontentlimit
            horizontalOffset: 3
            verticalOffset: 5
            spread: 0
            radius: 8
            samples: 30
            color: "#aa000000"
            source: bgcontentlimit
        }
    }
}
