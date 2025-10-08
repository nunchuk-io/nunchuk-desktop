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
import "../../../../Components/customizes/Transactions"
import "../../../../../localization/STR_QML.js" as STR

Item {
    property var    coinsGroup: miniscript.coinsGroup
    Row {
        anchors.fill: parent
        spacing: 4
        Loader {
            id: iconLoader
            sourceComponent: normalIcon
            anchors {
                top: parent.top
                topMargin: 8
            }
            visible: miniscript.type === ScriptNodeHelper.Type.HASH160 ||
                        miniscript.type ===  ScriptNodeHelper.Type.HASH256 ||
                        miniscript.type ===  ScriptNodeHelper.Type.RIPEMD160 ||
                        miniscript.type ===  ScriptNodeHelper.Type.SHA256
        }
        Component {
            id: normalIcon
            QIcon {
                iconSize: 20
                source: "qrc:/Images/Images/Article.svg"
            }
        }

        Column {
            id: maxColumn
            anchors{ 
                top: parent.top
                topMargin: 8
            }
            width: parent.width - iconLoader.width - 4
            spacing: 2
            Item {
                width: parent.width
                height: 20
                Row {
                    width: parent.width
                    height: 20
                    spacing: 4
                    
                    QLato {
                        width: paintedWidth
                        height: 20
                        text: miniscript.firstLine
                        font.weight: Font.Normal
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                    }
                    QLato {
                        width: paintedWidth
                        height: 20
                        text: getTitle()
                        font.weight: Font.Normal
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                    }
                    Rectangle {
                        width: 70
                        height: 20
                        color: "#FFFFFF"
                        border.color: "#DEDEDE"
                        radius: 20
                        visible: index == 0 && miniscript.type ===  ScriptNodeHelper.Type.OR_TAPROOT
                        QLato {
                            width: paintedWidth
                            height: 20
                            text: "Tapscripts"
                            font.weight: Font.Bold
                            font.pixelSize: 10
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            anchors.centerIn: parent
                        }
                    }
                    QIconText {
                        icon: "qrc:/Images/Images/PendingSignatures.png"
                        visible: miniscript.txStatus == NUNCHUCKTYPE.PENDING_NONCE || miniscript.txStatus == NUNCHUCKTYPE.PENDING_SIGNATURES
                        text.text: miniscript.txStatus == NUNCHUCKTYPE.PENDING_NONCE ? STR.str_QML_1884(miniscript.pendingNonce) : STR.str_QML_220(miniscript.pendingSignature)
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }

                QRoundBadgeStatus {
                    anchors {
                        right: parent.right
                        rightMargin: 0
                        verticalCenter: parent.verticalCenter
                    }
                    hasSigned: miniscript.m > miniscript.pendingNonce
                    visible: miniscript.txStatus == NUNCHUCKTYPE.PENDING_NONCE || 
                                miniscript.txStatus == NUNCHUCKTYPE.PENDING_SIGNATURES ||
                                miniscript.txStatus == NUNCHUCKTYPE.READY_TO_BROADCAST
                    txStatus: miniscript.txStatus
                }

            }
            
            QLato {
                id: description
                width: parent.width
                height: description.visible ? paintedHeight : 0
                text: getDescription()
                font.weight: Font.Normal
                font.pixelSize: 12
                color: "#757575"
                lineHeight: 16
                lineHeightMode: Text.FixedHeight
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
                visible: text !== ""
            }
            Rectangle {
                id: _coinsGroup
                width: parent.width
                height: _coinsGroup.visible ? (_group.height + 4*2) : 0
                visible: coinsGroup !== undefined && (_sign.visible || _active.visible)
                color: "#FFEAF9"
                radius: 8
                Column {
                    id: _group
                    width: parent.width
                    anchors.verticalCenter: parent.verticalCenter
                    spacing: 4
                    Item {
                        id: _sign
                        width: parent.width
                        height: 16
                        visible: coinsGroup !== undefined && coinsGroup.signXCount > 0 && miniscript.timelockType === ScriptNodeHelper.TimelockType.LOCKTYPE_RELATIVE
                        Row {
                            height: parent.height
                            spacing: 4
                            anchors {
                                verticalCenter: parent.verticalCenter
                                left: parent.left
                                leftMargin: 4
                            }
                            QIcon {
                                iconSize: 16
                                source: "qrc:/Images/Images/CurrencyBtc.svg"
                                anchors.verticalCenter: parent.verticalCenter
                            }
                            QLato {
                                width: parent.width - 20
                                height: 16
                                text: coinsGroup !== undefined ? STR.STR_QML_1870.arg(coinsGroup.signXCount).arg(coinsGroup.signXCount > 1 ? "coins" : "coin").arg(coinsGroup.amountDisplay).arg(RoomWalletData.unitValue) : ""
                                font.weight: Font.Normal
                                font.pixelSize: 12
                                horizontalAlignment: Text.AlignLeft
                                verticalAlignment: Text.AlignVCenter
                                anchors.verticalCenter: parent.verticalCenter
                            }
                        }
                    }
                    Item {
                        id: _active
                        width: parent.width
                        height: 16
                        visible: coinsGroup !== undefined && coinsGroup.hasActiveAfter
                        Row {
                            height: parent.height
                            spacing: 4
                            anchors {
                                verticalCenter: parent.verticalCenter
                                left: parent.left
                                leftMargin: 4
                            }
                            QIcon {
                                iconSize: 16
                                source: "qrc:/Images/Images/Timer.svg"
                                anchors.verticalCenter: parent.verticalCenter
                            }
                            QLato {
                                width: parent.width - 20
                                height: 16
                                text: coinsGroup !== undefined ? STR.STR_QML_1871.arg(coinsGroup.activeAfter) : ""
                                font.weight: Font.Normal
                                font.pixelSize: 12
                                horizontalAlignment: Text.AlignLeft
                                verticalAlignment: Text.AlignVCenter
                                anchors.verticalCenter: parent.verticalCenter
                            }
                        }
                    }
                }
            }
        }
    }
    
}
