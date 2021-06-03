import QtQuick 2.4
import QtQuick.Controls 2.3
import QtGraphicalEffects 1.12
import HMIEVENTS 1.0
import EWARNING 1.0
import QRCodeItem 1.0
import NUNCHUCKTYPE 1.0
import "../../Components/customizes"
import "../../Components/origins"
QScreen {
    Rectangle {
        id: mask
        width: popupWidth
        height: popupHeight
        radius: 8
        visible: false
    }

    Rectangle {
        id: content
        width: popupWidth
        height: popupHeight
        color: "#F1FAFE"
        radius: 8
        anchors.centerIn: parent
        visible: false
    }

    OpacityMask {
        anchors.fill: content
        source: content
        maskSource: mask

        QCloseButton {
            anchors {
                right: parent.right
                rightMargin: 16
                top: parent.top
                topMargin: 16
            }
            onClicked: {
                QMLHandle.sendEvent(EVT.EVT_ONS_CLOSE_REQUEST, EVT.STATE_ID_SCR_CONSOLIDATE_OUTPUT)
            }
        }

        Row {
            anchors {
                left: parent.left
                leftMargin: 40
                top: parent.top
                topMargin: 24
            }
            spacing: 8
            QText {
                id: sendfund
                text: "Consolidate Outputs"
                color: "#031F2B"
                font.weight: Font.ExtraBold
                font.family: "Montserrat"
                font.pixelSize: 24
                anchors.verticalCenter: parent.verticalCenter
            }

            Rectangle {
                width: visible ? 84 : 0
                height: visible ? 24 : 0
                color: "#031F2B"
                radius: 4
                visible: AppModel.walletInfo.walletEscrow
                anchors.verticalCenter: parent.verticalCenter
                Row {
                    anchors.centerIn: parent
                    spacing: 3
                    QImage {
                        source: "qrc:/Images/Images/Escrow Wallet.png"
                        width: 16
                        height: 16
                    }
                    QText {
                        text: "ESCROW"
                        color: "#F8D418"
                        width: 49
                        height: 10
                        font.weight: Font.Bold
                        font.family: "Lato"
                        font.pixelSize: 10
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
            }

            QText {
                text: "(" + AppModel.walletInfo.walletName + ")"
                color: "#031F2B"
                font.weight: Font.DemiBold
                font.family: "Montserrat"
                font.pixelSize: 14
                anchors.verticalCenter: parent.verticalCenter
            }
        }

        QNotification {
            id: notification
            anchors {
                horizontalCenter: parent.horizontalCenter
                top: parent.top
                topMargin: 81
            }
            visible: AppModel.transactionInfo.warningMessage.type !== EWARNING.NONE_MSG
            label: AppModel.transactionInfo.warningMessage.contentDisplay
            currentStatus: AppModel.transactionInfo.warningMessage.type
            onCloseRequest: AppModel.transactionInfo.warningMessage.type = EWARNING.NONE_MSG
        }

        QText {
            anchors {
                left: parent.left
                leftMargin: 43
                top: parent.top
                topMargin: notification.visible ? 146 : 92
            }
            text: "Create Transaction"
            color: "#031F2B"
            font.weight: Font.Bold
            font.pixelSize: 16
        }

        Row {
            spacing: 10
            height: 24
            anchors {
                right: parent.right
                rightMargin: 40
                bottom: flickcontent.top
                bottomMargin: 8
            }

            QText {
                height: 21
                width: 110
                text: "Selected Total:"
                color: "#000000"
                font.family: "Montserrat"
                font.weight: Font.DemiBold
                font.pixelSize: 14
                anchors.verticalCenter: parent.verticalCenter
            }

            QText {
                height: 24
                width: 140
                text: AppModel.utxoList.totalAmountSelected + ((AppSetting.unit === 1) ? " sat" : " BTC")
                color: "#000000"
                font.family: "Lato"
                font.pixelSize: 16
                horizontalAlignment: Text.AlignRight
                anchors.verticalCenter: parent.verticalCenter
            }
        }

        Flickable {
            id: flickcontent
            width: 720
            height: 350
            flickableDirection: Flickable.VerticalFlick
            clip: true
            contentHeight: destColumn.height
            ScrollBar.vertical: ScrollBar { active: true }
            anchors {
                horizontalCenter: parent.horizontalCenter
                top: parent.top
                topMargin: notification.visible ? 225 : 171
            }
            Column {
                id: destColumn
                spacing: 16

                Row {
                    id: destinationRow
                    spacing: 32
                    QTextInputBox {
                        id: toAddr
                        width: 419
                        heightMin: 56
                        mode: eEDIT_MODE
                        placeholder.text: "To Address"
                    }

                    QTextInputBox {
                        id: amount
                        width: 269
                        heightMin: 56
                        mode: eREADONLY_MODE
                        placeholder.text: "Amount"
                        textOutput: AppModel.utxoList.totalAmountSelected
                        color: Qt.rgba(0, 0, 0, 0.1)
                        rightPading: 40
                        QText {
                            color: "#839096"
                            width: 23
                            height: 16
                            anchors {
                                top: parent.top
                                topMargin: (amount.textActiveFocus) || (amount.textOutput !== "") ? 28: 20
                                right: parent.right
                                rightMargin: 16
                            }
                            text: (AppSetting.unit === 1) ? "sat" : "BTC"
                            font.pixelSize: 16
                            font.family: "Lato"
                        }
                    }
                }

                QTextInputBox {
                    id: memo
                    width: 720
                    heightMin: 96
                    mode: eEDIT_MODE
                    placeholder.text: "Memo"
                }
            }
        }

        Row {
            spacing: 16
            anchors {
                right: parent.right
                rightMargin: 40
                bottom: parent.bottom
                bottomMargin: 40
            }

            QTextButton {
                width: 120
                height: 48
                label.text: "CANCEL"
                label.font.pixelSize: 16
                type: eTypeB
                onButtonClicked: { QMLHandle.sendEvent(EVT.EVT_CONSOLIDATE_BACK_REQUEST) }
            }

            QTextButton {
                width: 260
                height: 48
                enabled: (toAddr.textOutput !== "") && (amount.textOutput !== "")
                label.text: "NEXT: Review Transaction"
                label.font.pixelSize: 16
                type: eTypeA
                onButtonClicked: {
                    var destinationObj = { "toAddress": toAddr.textOutput,
                                           "toAmount":  amount.textOutput,
                                           "destinationMemo"    : memo.textOutput};
                    QMLHandle.sendEvent(EVT.EVT_CONSOLIDATE_MAKE_TRANSACTION, destinationObj)
                }
            }
        }
    }
}
