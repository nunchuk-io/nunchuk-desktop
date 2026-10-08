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
import Features.Transactions.ViewModels 1.0
import "../../Components/origins"
import "../../Components/customizes"
import "../../Components/customizes/Buttons"
import "../../Components/customizes/Chats"
import "../../Components/customizes/QRCodes"
import "../../Components/customizes/Texts"
import "../../Components/customizes/Transactions"

QOnScreenContentTypeA {
    id: _content
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_211
    onCloseClicked: vm.close()
    onPrevClicked: vm.back()
    QConfirmYesNoPopup {
        id: confirmLowFee
        contentText: STR.STR_QML_809
        onConfirmNo: close()
        onConfirmYes: {
            close();
            _content.contentItem.requestCreateTransaction();
        }
    }

    Popup {
        id: createTxBusyBox
        width: parent.width
        height: parent.height
        modal: true
        focus: true
        Rectangle {
            id: boxmask
            width: 300
            height: 196
            radius: 24
            color: "#FFFFFF"
            anchors.centerIn: parent
            layer.enabled: true
            Column {
                spacing: 12
                anchors.centerIn: parent
                QBusyIndicator {
                    width: 70
                    height: 70
                    anchors.horizontalCenter: parent.horizontalCenter
                }
                QLato {
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.pixelSize: 16
                    font.weight: Font.Bold
                    text: STR.STR_QML_122
                }
                QLato {
                    width: 252
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.pixelSize: 16
                    text: STR.STR_QML_1800
                    wrapMode: Text.WordWrap
                    horizontalAlignment: Text.AlignHCenter
                }
            }

            layer.effect: OpacityMask {
                maskSource: Rectangle {
                    width: 300
                    height: 196
                    radius: 24
                }

            }
        }
        DropShadow {
            anchors.fill: boxmask
            horizontalOffset: 3
            verticalOffset: 5
            spread: 0
            radius: 8
            samples: 30
            color: "#aa000000"
            source: boxmask
        }
        background: Item {
        }
    }

    Timer {
        id: timerCreateTx
        interval: 500
        onTriggered: {
            createTxBusyBox.close();
        }
    }

    content: QCreateTransactionClaimInheritance {
        transactionInfo: vm.transactionInfo
        onSignalDraftTransaction: vm.createDraftTransaction(msg)
        onSignalCreateTransaction: vm.createTransaction(msg)
        onRequestSort: {
        }
    }

    bottomRight: QTextButton {
        width: 249
        height: 48
        label.text: STR.STR_QML_784
        label.font.pixelSize: 16
        type: eTypeE
        onButtonClicked: {
            if (_content.contentItem.manualFeeValid)
                _content.contentItem.requestCreateTransaction();
            else
                confirmLowFee.open();
        }
    }

    ConfirmTransactionViewModel {
        id: vm
    }
    Connections {
        target: vm
        onStartLoading: {
            createTxBusyBox.open();
            timerCreateTx.restart();
        }
        onStopLoading: {
            createTxBusyBox.close();
        }
    }
}
