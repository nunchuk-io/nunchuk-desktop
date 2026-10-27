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
import QtQuick
import QtQuick.Controls
import "../../../../Components/origins"
import "../../../../Components/customizes"
import "../../../../Components/customizes/Texts"
import "../../../../Components/customizes/Buttons"
import "../../../../Components/customizes/Popups"
import "../../../../../localization/STR_QML.js" as STR

QPopup {
    id: _popup
    property string title: ""
    property int valueM: 1
    property int valueN: 1
    property int maxN: 15
    signal saved(int m, int n)

    function openWith(popupTitle, m, n) {
        title = popupTitle
        valueN = Math.max(1, n)
        valueM = Math.max(1, Math.min(m, valueN))
        open()
    }

    content: Item {
        width: 400
        height: _col.implicitHeight
        Column {
            id: _col
            width: parent.width
            spacing: 16
            QLato {
                width: parent.width
                font.weight: Font.Bold
                text: _popup.title
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                wrapMode: Text.WordWrap
            }
            Repeater {
                model: [
                    { title: STR.STR_QML_015, description: STR.STR_QML_2360, isN: true },
                    { title: STR.STR_QML_1652, description: STR.STR_QML_1653, isN: false }
                ]
                Item {
                    id: _stepper
                    width: _col.width
                    height: 56
                    readonly property int value: modelData.isN ? _popup.valueN : _popup.valueM
                    readonly property bool canMinus: modelData.isN ? _popup.valueN > 1 : _popup.valueM > 1
                    readonly property bool canPlus: modelData.isN ? _popup.valueN < _popup.maxN : _popup.valueM < _popup.valueN
                    function minus() {
                        if (modelData.isN) {
                            _popup.valueN--
                            if (_popup.valueM > _popup.valueN) _popup.valueM = _popup.valueN
                        } else {
                            _popup.valueM--
                        }
                    }
                    function plus() {
                        if (modelData.isN) _popup.valueN++
                        else _popup.valueM++
                    }
                    Column {
                        anchors.verticalCenter: parent.verticalCenter
                        width: 220
                        spacing: 4
                        QLato {
                            width: parent.width
                            text: modelData.title
                            horizontalAlignment: Text.AlignLeft
                        }
                        QLato {
                            width: parent.width
                            text: modelData.description
                            font.pixelSize: 12
                            color: "#757575"
                            wrapMode: Text.WordWrap
                            horizontalAlignment: Text.AlignLeft
                        }
                    }
                    Row {
                        anchors {
                            right: parent.right
                            verticalCenter: parent.verticalCenter
                        }
                        spacing: 12
                        Rectangle {
                            width: 36
                            height: 36
                            radius: 18
                            border.color: "#031F2B"
                            border.width: 2
                            enabled: _stepper.canMinus
                            opacity: enabled ? 1.0 : 0.4
                            anchors.verticalCenter: parent.verticalCenter
                            Rectangle { width: 10.5; height: 1.5; color: "#031F2B"; anchors.centerIn: parent }
                            MouseArea {
                                anchors.fill: parent
                                cursorShape: Qt.PointingHandCursor
                                onClicked: _stepper.minus()
                            }
                        }
                        Rectangle {
                            width: 50
                            height: 48
                            radius: 8
                            border.color: "#DEDEDE"
                            border.width: 1
                            anchors.verticalCenter: parent.verticalCenter
                            QLato {
                                anchors.centerIn: parent
                                text: _stepper.value
                            }
                        }
                        Rectangle {
                            width: 36
                            height: 36
                            radius: 18
                            border.color: "#031F2B"
                            border.width: 2
                            enabled: _stepper.canPlus
                            opacity: enabled ? 1.0 : 0.4
                            anchors.verticalCenter: parent.verticalCenter
                            Rectangle { width: 10.5; height: 1.5; color: "#031F2B"; anchors.centerIn: parent }
                            Rectangle { width: 1.5; height: 10.5; color: "#031F2B"; anchors.centerIn: parent }
                            MouseArea {
                                anchors.fill: parent
                                cursorShape: Qt.PointingHandCursor
                                onClicked: _stepper.plus()
                            }
                        }
                    }
                }
            }
            Item {
                width: parent.width
                height: 48
                QTextButton {
                    anchors.left: parent.left
                    width: label.paintedWidth + 2*16
                    height: 48
                    label.text: STR.STR_QML_035
                    label.font.pixelSize: 16
                    type: eTypeF
                    onButtonClicked: _popup.close()
                }
                QTextButton {
                    anchors.right: parent.right
                    width: label.paintedWidth + 2*16
                    height: 48
                    label.text: STR.STR_QML_835
                    label.font.pixelSize: 16
                    type: eTypeE
                    onButtonClicked: {
                        _popup.close()
                        _popup.saved(_popup.valueM, _popup.valueN)
                    }
                }
            }
        }
    }
}
