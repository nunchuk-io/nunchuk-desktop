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

QOnScreenContentTypeA {
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_1801

    signal selectMiniscriptTemplate(var templateName)
    signal enterCustomMiniscript(var option)

    content: Item {
        Row {
            spacing: 36
            Rectangle {
                width: 346
                height: 512
                radius: 24
                color: "#D0E2FF"
                QPicture {
                    width: 346
                    height: 512
                    anchors.centerIn: parent
                    source: "qrc:/Images/Images/miniscript-introduction.svg"
                }
            }
            Item {
                width: 346
                height: 512
                Column {
                    width: parent.width
                    spacing: 24
                    QLato {
                        width: parent.width
                        text: STR.STR_QML_1803
                        lineHeightMode: Text.FixedHeight
                        lineHeight: 28
                        wrapMode: Text.WordWrap
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                    }
                }
            }
        }

    }

    bottomRight: QTextButton {
        width: label.paintedWidth + 32
        height: 48
        label.text: STR.STR_QML_265
        label.font.pixelSize: 16
        type: eTypeB
        onButtonClicked: {
            optionMenu.x = 20
            optionMenu.y = 20 - optionMenu.height
            optionMenu.open()
        }
        QMiniscriptMultiContextMenu {
            id: optionMenu
            menuWidth: 330
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
                    height: 78,
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
                    isSeparator: false,
                    height: 78,
                    visible: true,
                    label: STR.STR_QML_1885,
                    description: STR.STR_QML_1886,
                    icon: "qrc:/Images/Images/zen-hodl.svg",
                    iconRight: "",
                    color: "#031F2B",
                    colorDescription: "#757575",
                    enable: true,
                    subMenu: null,
                    action: function(){
                        selectMiniscriptTemplate("zen-hodl")
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
