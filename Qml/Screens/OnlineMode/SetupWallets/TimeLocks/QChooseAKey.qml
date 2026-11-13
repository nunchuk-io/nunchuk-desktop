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
import NUNCHUCKTYPE 1.0
import DataPool 1.0
import "../../../../Components/origins"
import "../../../../Components/customizes"
import "../../../../Components/customizes/Texts"
import "../../../../Components/customizes/Buttons"
import "../../../../../localization/STR_QML.js" as STR

QOnScreenContentTypeB {
    id: _choose
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    property string titleText: STR.STR_QML_2441
    property string descriptionText: STR.STR_QML_2442
    property string addNewLabel: STR.STR_QML_2443
    property bool addNewIcon: true
    label.text: titleText
    property var keys: []
    property string selectedXfp: ""
    signal addNewKeyClicked()
    signal keySelected(var key)

    function selectedKey() {
        for (var i = 0; i < keys.length; i++) {
            if (keys[i].xfp === selectedXfp) return keys[i]
        }
        return null
    }

    content: Item {
        Column {
            width: parent.width
            spacing: 16
            QLato {
                width: parent.width
                text: _choose.descriptionText
                wrapMode: Text.WordWrap
                horizontalAlignment: Text.AlignLeft
            }
            Flickable {
                width: parent.width
                height: 420
                contentHeight: _list.height
                clip: true
                ScrollBar.vertical: QScrollBar { }
                Column {
                    id: _list
                    width: parent.width - 12
                    Repeater {
                        model: _choose.keys
                        Item {
                            width: _list.width
                            height: 64
                            Row {
                                anchors.verticalCenter: parent.verticalCenter
                                spacing: 12
                                QCircleIcon {
                                    bgSize: 48
                                    icon.iconSize: 24
                                    icon.typeStr: modelData.type
                                    icon.tag: modelData.tag
                                    anchors.verticalCenter: parent.verticalCenter
                                    color: "#F5F5F5"
                                }
                                Column {
                                    width: 420
                                    anchors.verticalCenter: parent.verticalCenter
                                    spacing: 4
                                    QLato {
                                        width: parent.width
                                        text: modelData.name
                                        elide: Text.ElideRight
                                        horizontalAlignment: Text.AlignLeft
                                    }
                                    Rectangle {
                                        width: _typeText.width + 8
                                        height: 16
                                        radius: 20
                                        color: "#EAEAEA"
                                        QText {
                                            id: _typeText
                                            anchors.centerIn: parent
                                            text: GlobalData.signers(modelData.signer_type)
                                            font.family: "Lato"
                                            font.weight: Font.Bold
                                            font.pixelSize: 10
                                            color: "#031F2B"
                                        }
                                    }
                                    QLato {
                                        text: "XFP: " + modelData.xfp
                                        font.pixelSize: 12
                                        font.capitalization: Font.AllUppercase
                                        color: "#595959"
                                        horizontalAlignment: Text.AlignLeft
                                    }
                                }
                            }
                            QIcon {
                                iconSize: 24
                                anchors {
                                    right: parent.right
                                    rightMargin: 12
                                    verticalCenter: parent.verticalCenter
                                }
                                source: _choose.selectedXfp === modelData.xfp ? "qrc:/Images/Images/radio-selected-dark.svg" : "qrc:/Images/Images/radio-dark.svg"
                            }
                            MouseArea {
                                anchors.fill: parent
                                cursorShape: Qt.PointingHandCursor
                                onClicked: _choose.selectedXfp = modelData.xfp
                            }
                        }
                    }
                }
            }
        }
    }

    bottomLeft: QTextButton {
        width: 80
        height: 48
        label.text: STR.STR_QML_059
        label.font.pixelSize: 16
        type: eTypeF
        onButtonClicked: _choose.prevClicked()
    }
    bottomRight: Row {
        spacing: 12
        QIconTextButton {
            width: rowObj.implicitWidth + 32
            height: 48
            label: _choose.addNewLabel
            icons: ["add-dark.svg", "add-dark.svg", "add-dark.svg", "add-dark.svg"]
            fontPixelSize: 16
            iconSize: _choose.addNewIcon ? 16 : 0
            type: eTypeB
            onButtonClicked: _choose.addNewKeyClicked()
        }
        QTextButton {
            width: label.paintedWidth + 2*20
            height: 48
            label.text: STR.STR_QML_2571
            label.font.pixelSize: 16
            type: eTypeE
            enabled: _choose.selectedXfp !== ""
            onButtonClicked: {
                var key = _choose.selectedKey()
                if (key) _choose.keySelected(key)
            }
        }
    }
}
