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
import "../../../../Components/origins"
import "../../../../Components/customizes"
import "../../../../Components/customizes/Chats"
import "../../../../Components/customizes/Texts"
import "../../../../Components/customizes/Buttons"
import "../../../../../localization/STR_QML.js" as STR

Flickable {
    id: root
    readonly property int widthHalf: 352
    width: widthHalf
    height: 480
    flickableDirection: Flickable.VerticalFlick
    ScrollBar.vertical: ScrollBar { active: true }
    contentHeight: columnMiniscript.childrenRect.height
    contentWidth: widthHalf
    clip: true
    
    Column {
        id: columnMiniscript
        width: widthHalf
        spacing: 4
        Item {
            width: widthHalf
            height: 24
            Row {
                anchors.fill: parent
                spacing: 8
                QIcon {
                    iconSize: 24
                    source: "qrc:/Images/Images/policies_dark.svg"
                }
                QLato {
                    width: paintedWidth
                    text: STR.STR_QML_1844
                    font.weight: Font.Bold
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
        }
        Item {
            width: widthHalf
            height: 24
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
        Column {
            visible: newWalletInfo.walletAddressType === NUNCHUCKTYPE.TAPROOT && newWalletInfo.keypaths.length > 0
            width: 450
            Repeater {
                model: newWalletInfo.keypaths
                delegate: newWalletInfo.keypaths.length === 1 ? keyPathDelegate : muSigDelegate
            }
            Component {
                id: keyPathDelegate
                QKeyPathReview {
                    width: widthHalf
                    miniscript: modelData
                }
            }
            Component {
                id: muSigDelegate
                QMiniscriptPolicesDraftReviewDlg {
                    width: widthHalf
                    miniscript: modelData
                }
            }
        }
        QLine {
            width: widthHalf
            height: 1
            color: "#DEDEDE"
            visible: newWalletInfo.walletAddressType === NUNCHUCKTYPE.TAPROOT && newWalletInfo.keypaths.length > 0
        }
        Item {
            width: widthHalf
            height: 28
            QBadge {
                width: 74
                height: 16
                text: STR.STR_QML_1857
                fontColor: "#FFFFFF"
                color: "#031F2B"
                font.pixelSize: 10
                font.weight: Font.Black
                anchors.bottom: parent.bottom
            }
            visible: newWalletInfo.walletAddressType === NUNCHUCKTYPE.TAPROOT
        }
        Column {
            width: widthHalf
            Repeater {
                model: newWalletInfo.treeMiniscript
                delegate: QMiniscriptPolicesDraftReviewDlg {
                    width: widthHalf
                    miniscript: modelData
                }
            }
        }
    }
}
