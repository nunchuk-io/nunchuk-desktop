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
import NUNCHUCKTYPE 1.0
import "../../../../Components/origins"
import "../../../../Components/customizes/Chats"
import "../../../../Components/customizes/Texts"
import "../../../../Components/customizes/Buttons"
import "../../../../../localization/STR_QML.js" as STR

Rectangle {
    width: 360
    height: 192
    radius: 8
    border.color: "#DEDEDE"
    border.width: 1
    property string colorText: "#031F2B"
    property string miniscript: ""
    property bool selected: false
    signal selectMiniscriptTemplate(var templateName)
    signal enterCustomMiniscript(var option)
    signal miniscriptEdit()
    signal miniscriptDelete()
    Column {
        width: parent.width
        spacing: 12
        Rectangle {
            width: parent.width
            height: 40
            radius: 8
            color: selected ? "#FDEBD2" : "#FFFFFF"
            Rectangle {
                width: parent.width
                height: 20
                color: selected ? "#FDEBD2" : "#FFFFFF"
                anchors.bottom: parent.bottom
            }
            QLato {
                id: _title
                font.weight: Font.Normal
                font.pixelSize: 16
                text: STR.STR_QML_1498
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
                anchors {
                    left: parent.left
                    leftMargin: 12
                    verticalCenter: parent.verticalCenter
                }
            }
            QBadge {
                text: STR.STR_QML_1548
                border.width: 1
                border.color: "#DEDEDE"
                color: "#FFFFFF"
                font.weight: Font.Bold
                fontSize: 10
                anchors {
                    left: _title.right
                    leftMargin: 12
                    verticalCenter: parent.verticalCenter
                }
            }
            QIcon {
                id: icon
                iconSize: 24
                source: selected ? "qrc:/Images/Images/radio-selected-dark.svg" : "qrc:/Images/Images/radio-dark.svg"
                anchors {
                    verticalCenter: parent.verticalCenter
                    right: parent.right
                    rightMargin: 12
                }
            }
        }

        Loader {
            id: miniscriptLoader
            width: parent.width
            height: 116
            sourceComponent: miniscript === "" ? _addMiniscript : _hadMiniscript
        }

        Component {
            id: _addMiniscript
            Column {
                width: parent.width
                anchors {
                    left: parent.left
                    right: parent.right
                    top: parent.top
                    leftMargin: 12
                    rightMargin: 12
                    topMargin: 12
                }
                spacing: 16
                QLato {
                    width: parent.width - 24
                    font.weight: Font.Normal
                    font.pixelSize: 16
                    text: STR.STR_QML_1874
                    lineHeightMode: Text.FixedHeight
                    lineHeight: 20
                    wrapMode: Text.WrapAnywhere
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }
                QTextButton {
                    width: label.paintedWidth + 16*2
                    height: 48
                    label.text: STR.STR_QML_1873
                    label.font.pixelSize: 16
                    type: eTypeE
                    onButtonClicked: {
                        optionMenu.x = 20
                        optionMenu.y = 20 - optionMenu.height
                        optionMenu.open()
                    }
                    QMiniscriptMultiContextMenu {
                        id: optionMenu
                        menuWidth: 350
                        mapMenu: [
                            {
                                isHeadLine: true,
                                isSeparator: false,
                                height: 16,
                                visible: true,
                                label: STR.STR_QML_1805,
                                icon: "",
                                iconRight: "",
                                color: "#031F2B",
                                enable: true,
                                subMenu: null,
                                action: function(){}
                            },
                            {
                                isHeadLine: false,
                                isSeparator: false,
                                height: 78,
                                visible: true,
                                label: STR.STR_QML_1806,
                                description: STR.STR_QML_1807,
                                icon: "qrc:/Images/Images/expanding-multisig.svg",
                                iconRight: "",
                                color: "#031F2B",
                                colorDescription: "#757575",
                                enable: true,
                                subMenu: null,
                                action: function(){
                                    selectMiniscriptTemplate("expanding-multisig")
                                }
                            },
                            {
                                isHeadLine: false,
                                isSeparator: false,
                                height: 78,
                                visible: true,
                                label: STR.STR_QML_1808,
                                description: STR.STR_QML_1809,
                                icon: "qrc:/Images/Images/decaying-multisig.svg",
                                iconRight: "",
                                color: "#031F2B",
                                colorDescription: "#757575",
                                enable: true,
                                subMenu: null,
                                action: function(){
                                    selectMiniscriptTemplate("decaying-multisig")
                                }
                            },
                            {
                                isHeadLine: false,
                                isSeparator: false,
                                height: 62,
                                visible: true,
                                label: STR.STR_QML_1810,
                                description: STR.STR_QML_1811,
                                icon: "qrc:/Images/Images/flexible-multisig.svg",
                                iconRight: "",
                                color: "#031F2B",
                                colorDescription: "#757575",
                                enable: true,
                                subMenu: null,
                                action: function(){
                                    selectMiniscriptTemplate("flexible-multisig")
                                }
                            },
                            {
                                isHeadLine: false,
                                isSeparator: true,
                                height: 25,
                                visible: true,
                                label: STR.STR_QML_1805,
                                icon: "",
                                iconRight: "",
                                color: "#031F2B",
                                enable: true,
                                subMenu: null,
                                action: function(){}
                            },
                            {
                                isHeadLine: true,
                                isSeparator: false,
                                height: 16,
                                visible: true,
                                label: STR.STR_QML_1812,
                                icon: "",
                                iconRight: "",
                                color: "#031F2B",
                                enable: true,
                                subMenu: null,
                                action: function(){}
                            },
                            {
                                isHeadLine: false,
                                isSeparator: false,
                                height: 62,
                                visible: true,
                                label: STR.STR_QML_1813,
                                description: STR.STR_QML_1814,
                                icon: "qrc:/Images/Images/enter-miniscript.svg",
                                iconRight: "",
                                color: "#031F2B",
                                colorDescription: "#757575",
                                enable: true,
                                subMenu: null,
                                action: function(){
                                    enterCustomMiniscript("enter-miniscript")
                                }
                            },
                            {
                                isHeadLine: false,
                                isSeparator: false,
                                height: 62,
                                visible: true,
                                label: STR.STR_QML_1815,
                                description: STR.STR_QML_1816,
                                icon: "qrc:/Images/Images/importFile.svg",
                                iconRight: "",
                                color: "#031F2B",
                                colorDescription: "#757575",
                                enable: true,
                                subMenu: null,
                                action: function(){
                                    enterCustomMiniscript("import-miniscript-file")
                                }
                            },
                        ]
                    }
                }
            }
        }

        Component {
            id: _hadMiniscript
            Item {
                width: parent.width
                height: 152
                QTextArea  {
                    id: _customize
                    width: parent.width
                    height: 152
                    background: Item{}
                    color: colorText
                    text: miniscript
                    readOnly: true
                    placeholderText: "Example: \nandor(\nln:older(12900),\nthresh(2,pk(A), s:pk(B), s:pk(C)),\nthresh(2,pk(A),s:pk(D),s:pk(E))\n)"
                    verticalAlignment: Text.AlignTop
                    wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                }
                Item {
                    width: parent.width
                    height: 16
                    anchors {
                        bottom: parent.bottom
                        bottomMargin: -12
                    }
                    Row {
                        anchors {
                            verticalCenter: parent.verticalCenter
                            right: parent.right
                            rightMargin: 24
                        }
                        width: parent.width
                        layoutDirection: Qt.RightToLeft
                        height: 16
                        spacing: 16
                        QIconTextButton {
                            width: 44
                            height: 16
                            label: STR.STR_QML_1134
                            icons: ["Trash.svg", "Trash.svg", "Trash.svg", "Trash.svg"]
                            fontPixelSize: 12
                            iconSize: 16
                            type: eTypeP
                            onButtonClicked: { 
                                miniscriptDelete()
                            }
                        }
                        QIconTextButton {
                            width: 67
                            height: 16
                            label: STR.STR_QML_849
                            icons: ["edit-dark.svg", "edit-dark.svg", "edit-dark.svg","edit-dark.svg"]
                            fontPixelSize: 12
                            iconSize: 16
                            type: eTypeP
                            onButtonClicked: {
                                miniscriptEdit()
                            }
                        }                        
                    }
                }
            }
        }
    }
}
