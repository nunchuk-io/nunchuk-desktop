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
import QtGraphicalEffects 1.12
import QtQuick.Controls 2.5
import QRCodeItem 1.0
import HMIEVENTS 1.0
import EWARNING 1.0
import NUNCHUCKTYPE 1.0
import DataPool 1.0
import "../../../Components/origins"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../../localization/STR_QML.js" as STR

Popup {
    id: _popup
    width: parent.width
    height: parent.height
    modal: true
    focus: true
    closePolicy: Popup.CloseOnEscape
    background: Item{}
    property string countryCode: ""
    property string countryName: ""
    Rectangle {
        id: qrmask
        width: 600
        height: 500
        radius: 24
        color: "#FFFFFF"
        anchors.centerIn: parent
        layer.enabled: true
        layer.effect: OpacityMask {
            maskSource: Rectangle {
                width: 600
                height: 500
                radius: 24
            }
        }
        QCloseButton {
            anchors {
                right: parent.right
                rightMargin: 16
                top: parent.top
                topMargin: 16
            }
            onClicked: _popup.close()
        }
        QMontserrat {
            anchors {
                top: parent.top
                topMargin: 36
                left: parent.left
                leftMargin: 36
            }
            font.weight: Font.Medium
            font.pixelSize: 32
            text: STR.STR_QML_1257
        }
        Item {
            anchors {
                top: parent.top
                topMargin: 100
                left: parent.left
                leftMargin: 36
            }
            width: 528
            height: 298
            QListView {
                width: 528
                height: 298
                model: OnBoarding.countries
                clip: true
                interactive: true
                ScrollBar.vertical: ScrollBar { active: true }
                delegate: Item {
                    width: 528
                    height: 48
                    QLato {
                        width: 504
                        height: 28
                        anchors.centerIn: parent
                        text: modelData.name
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                countryCode = modelData.code
                                countryName = modelData.name
                                _popup.close()
                            }
                        }
                    }
                }
            }
            QLine {
                width: parent.width
                anchors.bottom: parent.bottom
            }
        }
        Row {
            anchors {
                bottom: parent.bottom
                bottomMargin: 36
                left: parent.left
                leftMargin: 36
            }
            spacing: 12
            QTextButton {
                width: 80
                height: 48
                label.text: STR.STR_QML_245
                label.font.pixelSize: 16
                type: eTypeF
                onButtonClicked: {
                    _popup.close()
                }
            }
        }
    }
    DropShadow {
        anchors.fill: qrmask
        horizontalOffset: 3
        verticalOffset: 3
        radius: 8.0
        samples: 17
        color: "#80000000"
        source: qrmask
    }
}
