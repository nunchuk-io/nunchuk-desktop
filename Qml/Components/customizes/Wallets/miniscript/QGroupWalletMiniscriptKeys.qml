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
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0
import HMIEVENTS 1.0
import NUNCHUCKTYPE 1.0
import QRCodeItem 1.0
import Qt.labs.platform 1.1
import DataPool 1.0
import "../../../../Components/origins"
import "../../../../Components/customizes"
import "../../../../Components/customizes/Signers"
import "../../../../Components/customizes/Texts"
import "../../../../../localization/STR_QML.js" as STR

Column {
    width: 352
    readonly property var tmpColors: [
        {colorStr: "#9EC063",           url: "qrc:/Images/Images/User-dark.svg" },
        {colorStr: "#2F466C",           url: "qrc:/Images/Images/User-dark.svg" },
        {colorStr: "#FAA077",           url: "qrc:/Images/Images/User-dark.svg" },
        {colorStr: "#1C652D",           url: "qrc:/Images/Images/User-dark.svg" },
        {colorStr: "#B4DCFF",           url: "qrc:/Images/Images/User-dark.svg" },
        {colorStr: "#7E519B",           url: "qrc:/Images/Images/User-dark.svg" },
        {colorStr: "#FDD95C",           url: "qrc:/Images/Images/User-dark.svg" },
        {colorStr: "#595959",           url: "qrc:/Images/Images/User-dark.svg" },
        {colorStr: "#D38FFF",           url: "qrc:/Images/Images/User-dark.svg" },
        {colorStr: "#CF4018",           url: "qrc:/Images/Images/User-dark.svg" },
        {colorStr: "#A66800",           url: "qrc:/Images/Images/User-dark.svg" },
    ]

    Item {
        height: 32
        width: parent.width
        Row {
            height: 32
            spacing: 8
            QIcon {
                iconSize: 24
                source: "qrc:/Images/Images/policies_dark.svg"
                anchors.verticalCenter: parent.verticalCenter
            }
            QLato {
                width: 44
                height: 20
                anchors.verticalCenter: parent.verticalCenter
                text: STR.STR_QML_1844
                font.weight: Font.Bold
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft
            }
        }
        Row {
            height: 32
            spacing: -4
            anchors {
                right: parent.right
                rightMargin: 9
            }
            visible: sandbox.url !== ""
            Repeater {
                model: Math.min(3, sandbox.userCount)
                Item {
                    width: 32
                    height: 32
                    property var colorData: tmpColors[index]
                    Rectangle {
                        border.width: 1
                        border.color: "#DEDEDE"
                        color: colorData.colorStr
                        radius: 16
                        width: 32
                        height: 32
                        visible: sandbox.userCount === 3
                        QIcon {
                            iconSize: 18
                            anchors.centerIn: parent
                            source: colorData.url
                        }
                    }
                    Rectangle {
                        border.width: 1
                        border.color: "#DEDEDE"
                        color: index === 2 ? "#031F2B" : colorData.colorStr
                        radius: 16
                        width: 32
                        height: 32
                        visible: sandbox.userCount !== 3
                        QIcon {
                            iconSize: 18
                            anchors.centerIn: parent
                            source: colorData.url
                            visible: index != 2
                        }
                        QLato {
                            width: 24
                            height: 18
                            anchors.centerIn: parent
                            text: qsTr("%1").arg(sandbox.userCount - 2)
                            font.weight: Font.Bold
                            color: "#FFFFFF"
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                            visible: index === 2
                        }
                    }
                }
            }
        }
    }
    Item {
        id: taprootItem
        width: 352
        height: taprootItem.visible ? (16 + 12) : 0
        QBadge {
            width: 63
            height: 16
            text: STR.STR_QML_1846
            fontColor: "#FFFFFF"
            color: "#031F2B"
            font.pixelSize: 10
            font.weight: Font.Black
            anchors.bottom: parent.bottom
            visible: newWalletInfo.keypaths.length > 0
        }
        QBadge {
            width: 108
            height: 16
            text: STR.STR_QML_1858
            fontColor: "#FFFFFF"
            color: "#031F2B"
            opacity: 0.4
            font.pixelSize: 10
            font.weight: Font.Black
            anchors.bottom: parent.bottom
            visible: newWalletInfo.keypaths.length == 0
        }
        visible: newWalletInfo.walletAddressType === NUNCHUCKTYPE.TAPROOT
    }
    Loader {
        visible: newWalletInfo.walletAddressType === NUNCHUCKTYPE.TAPROOT
        sourceComponent: newWalletInfo.keypaths.length > 0 ? keypathList : notHaveKeyPath
    }
    Component {
        id: keypathList
        Column {
            width: 352
            Column {
                width: 352
                Repeater {
                    model: newWalletInfo.keypaths
                    delegate: QKeyPath {
                        width: 352
                        miniscript: modelData
                    }
                }
            }
            QLine {
                width: 352
                height: 1
                color: "#DEDEDE"
            }
            Item {
                width: 352
                height: 8 + 16 + 12
                QBadge {
                    width: 74
                    height: 16
                    text: STR.STR_QML_1857
                    fontColor: "#FFFFFF"
                    color: "#031F2B"
                    font.pixelSize: 10
                    font.weight: Font.Black
                    anchors {
                        bottom: parent.bottom
                        bottomMargin: 8
                    }
                }
            }
        }
    }
    Component {
        id: notHaveKeyPath
        Item {
            width: 352
            height: 8 + 16 + 12
            QBadge {
                width: 74
                height: 16
                text: STR.STR_QML_1857
                fontColor: "#FFFFFF"
                color: "#031F2B"
                font.pixelSize: 10
                font.weight: Font.Black
                anchors {
                    bottom: parent.bottom
                    bottomMargin: 8
                }
            }
        }
    }
    
    Column {
        width: 352
        Repeater {
            model: newWalletInfo.treeMiniscript
            delegate: QMiniscriptPolicesAddDelegate {
                width: 352
                miniscript: modelData
            }
        }
    }
}
