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
import Qt5Compat.GraphicalEffects
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

Rectangle {
    id: _sendAddressArea
    width: 350
    height: 480
    radius: 12
    border.color: "#EAEAEA"
    color: "#FFFFFF"
    // OpacityMask is applied to the Flickable (not this Rectangle) so the
    // border rendered by this Rectangle is never consumed by the layer texture.
    // The Flickable layer (content + scrollbar) is clipped to the rounded shape.
    property string myRole: ""
    property var    transactionInfo
    property bool   isDummy: false
    property int    pendingSignature: 0

    signal addrToVerify(var addr)
    signal newMemoNotify(var newMemo)
    Flickable {
        id: _flickable
        anchors.fill: parent
        flickableDirection: Flickable.VerticalFlick
        clip: true
        interactive: contentHeight > height
        contentHeight: contentDisp.height
        // Clip the Flickable layer (content + attached scrollbar) to the rounded
        // corner shape of the outer Rectangle. The outer Rectangle's border is
        // painted independently and remains fully visible.
        layer.enabled: true
        layer.effect: OpacityMask {
            maskSource: Rectangle {
                width: _sendAddressArea.width
                height: _sendAddressArea.height
                radius: _sendAddressArea.radius  // 12px — matches outer border-radius
            }
        }
        ScrollBar.vertical: QScrollBar {
            // topPadding/bottomPadding keeps the track/thumb within the straight
            // portion of the border (past the 12px corner arc) even before masking.
            topPadding: 12
            bottomPadding: 12
        }
        Column {
            id: contentDisp
            width: parent.width - 8
            spacing: 12
            anchors.horizontalCenter: parent.horizontalCenter
            QSendToAddressBlock {
                id: sendToAddress
                useMouseArea: true
                anchors.horizontalCenter: parent.horizontalCenter
                onAddressClicked: (addr) => {
                    addrToVerify(addr)
                }
            }

            QChangeAddressBlock {
                id: changeAddress
                visible: transactionInfo.hasChange && !isDummy
                editShow: {
                    var isShow = false
                    if (transactionInfo.status === NUNCHUCKTYPE.PENDING_SIGNATURES) {
                        isShow = transactionInfo.hasDraftCoinChange()
                    }
                    else {
                        isShow = transactionInfo.hasChange
                    }
                    return isShow
                }
                anchors.horizontalCenter: parent.horizontalCenter
                onEditClicked: {
                    if (transactionInfo.status === NUNCHUCKTYPE.PENDING_SIGNATURES) {
                        if (transactionInfo.hasDraftCoinChange()) {
                            changeTags.open()
                        }
                    }
                    else {
                        var input = {
                            type: "edit-change-address"
                        }
                        AppModel.walletInfo.requestSyncSelectCoinForMakeTransaction(input)
                    }
                }
            }

            QTransactionNoteBlock {
                id: transationNote
                visible: !isDummy
                onMemoNotify: (newMemo) => {
                    newMemoNotify(newMemo)
                }
            }
            QSignaturesBlock {
                id: signatures
                visible: !isDummy && (transactionInfo.status === NUNCHUCKTYPE.PENDING_SIGNATURES)
                anchors.horizontalCenter: parent.horizontalCenter
                pendingSignature: isDummy? _sendAddressArea.pendingSignature : transactionInfo.pendingSignatures
            }
            QShowInputCoinsBlock {
                id: inputCoins
                visible: !isDummy
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }
    }
}
