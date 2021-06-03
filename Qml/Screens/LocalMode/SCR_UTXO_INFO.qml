import QtQuick 2.4
import QtQuick.Controls 2.3
import QtGraphicalEffects 1.12
import HMIEVENTS 1.0
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

        Row {
            spacing: 16
            anchors {
                left: parent.left
                leftMargin: 40
                top: parent.top
                topMargin: 24
            }

            QText {
                id: title
                height: 36
                text: "Unspent Output Info"
                color: "#031F2B"
                font.weight: Font.ExtraBold
                font.family: "Montserrat"
                font.pixelSize: 24
                anchors.verticalCenter: parent.verticalCenter
            }

            QText {
                height: 21
                text: "(" + AppModel.walletInfo.walletName + ")"
                color: "#031F2B"
                font.weight: Font.DemiBold
                font.family: "Montserrat"
                font.pixelSize: 14
                anchors.verticalCenter: title.verticalCenter
            }
        }

        QCloseButton {
            anchors {
                right: parent.right
                rightMargin: 16
                top: parent.top
                topMargin: 16
            }
            onClicked: {
                QMLHandle.sendEvent(EVT.EVT_ONS_CLOSE_REQUEST, EVT.STATE_ID_SCR_UTXO_OUTPUT)
            }
        }

        QButtonTextLink {
            width: 203
            height: 24
            label: "BACK TO PREVIOUS"
            anchors {
                left: parent.left
                leftMargin: 40
                bottom: parent.bottom
                bottomMargin: 40
            }
            onButtonClicked: {
                QMLHandle.sendEvent(EVT.EVT_UTXO_INFO_BACK_REQUEST)
            }
        }

        Flickable {
            id: flickcontent
            width: 720
            height: Math.min(500, contentHeight)
            contentHeight: contentDisplay.height
            flickableDirection: Flickable.VerticalFlick
            clip: true
            ScrollBar.vertical: ScrollBar { active: true }
            interactive: height < contentHeight
            anchors {
                horizontalCenter: parent.horizontalCenter
                top: parent.top
                topMargin: 108
            }
            Column {
                id: contentDisplay
                width: 720
                spacing: 16

                Row {
                    width: parent.width
                    height: 54
                    spacing: 30
                    QTextInputBox {
                        width: 360
                        height: parent.height
                        color: "transparent"
                        placeholder.text: "AMOUNT"
                        mode: eREADONLY_MODE
                        textOutput: AppModel.utxoInfo.amount + ((AppSetting.unit === 1) ? " sat" : " BTC")
                    }
                    QTextInputBox {
                        width: 150
                        height: parent.height
                        color: "transparent"
                        placeholder.text: "CONFIRMATIONS"
                        mode: eREADONLY_MODE
                        textOutput: Math.max(0, (AppModel.chainTip - AppModel.utxoInfo.height)+1)
                    }
                    QTextInputBox {
                        width: 150
                        height: parent.height
                        color: "transparent"
                        placeholder.text: "VOUT"
                        mode: eREADONLY_MODE
                        textOutput: AppModel.utxoInfo.vout
                    }
                }

                QTextInputBox {
                    width: parent.width
                    heightMin: 54
                    color: "transparent"
                    placeholder.text: "TRANSACTION ID"
                    mode: eREADONLY_MODE
                    textOutput: AppModel.utxoInfo.txid
                }
                QTextInputBox {
                    width: parent.width
                    heightMin: 54
                    color: "transparent"
                    placeholder.text: "ADDRESS"
                    mode: eREADONLY_MODE
                    textOutput: AppModel.utxoInfo.address
                }
                QTextInputBox {
                    width: parent.width
                    heightMin: 54
                    color: "transparent"
                    placeholder.text: "SCRIPTPUBKEY"
                    mode: eREADONLY_MODE
                    textOutput: AppModel.utxoInfo.scriptPublickey
                }
            }
        }

        QButtonTextLink {
            width: 203
            height: 24
            label: "RELATED TRANSACTION"
            direction: eRIGHT
            anchors {
                right: parent.right
                rightMargin: 52
                top: flickcontent.bottom
                topMargin: 48
            }
            onButtonClicked: {
                QMLHandle.sendEvent(EVT.EVT_UTXO_INFO_CHECKOUT_TX_RELATED, AppModel.utxoInfo.txid)
            }
        }
    }
}
