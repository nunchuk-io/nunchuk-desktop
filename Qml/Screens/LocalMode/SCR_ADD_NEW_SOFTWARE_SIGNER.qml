import QtQuick 2.4
import QtQuick.Controls 1.4
import QtQuick.Controls 2.3
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.12
import HMIEVENTS 1.0
import EWARNING 1.0
import NUNCHUCKTYPE 1.0
import "../../Components/customizes"
import "../../Components/origins"
QScreen {
    id: rootAddsignerToWallet
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

        QText {
            width: 163
            height: 27
            anchors {
                left: parent.left
                leftMargin: 40
                top: parent.top
                topMargin: 24
            }
            text: "New signer mnemonic"
            color: "#031F2B"
            font.family: "Montserrat"
            font.weight: Font.ExtraBold
            font.pixelSize: 24
        }

        QText {
            width: 540
            height: 56
            text: "Please take a moment to write down this mnemonic phrase. <b>It is your signer's backup</b>. You can use it to recover the signer later."
            anchors {
                left: parent.left
                leftMargin: 40
                top: parent.top
                topMargin: 58
            }
            verticalAlignment: Text.AlignVCenter
            color: "#031F2B"
            font.family: "Lato"
            font.pixelSize: 16
            wrapMode: Text.WordWrap
            lineHeightMode: Text.FixedHeight
            lineHeight: 28
        }

        QCloseButton {
            anchors {
                right: parent.right
                rightMargin: 16
                top: parent.top
                topMargin: 16
            }
            onClicked: {
                if(NUNCHUCKTYPE.FLOW_ADD_WALLET === QMLHandle.currentFlow){
                    QMLHandle.sendEvent(EVT.EVT_NEW_SOFTWARE_SIGNER_BACK_TO_WALLET_SIGNER_CONFIGURATION)
                }
                else {
                    QMLHandle.sendEvent(EVT.EVT_ONS_CLOSE_REQUEST, EVT.STATE_ID_SCR_ADD_NEW_SOFTWARE_SIGNER)
                }
            }
        }

        Grid {
            id: gridmmonic
            width: 730
            height: 500
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: 150
            columns: 6
            spacing: 12
            Repeater {
                model: 24
                Item {
                    width: (gridmmonic.width - gridmmonic.spacing*(gridmmonic.columns-1)) / gridmmonic.columns
                    height: 48
                    y: 100
                    Rectangle {
                        id: itembg
                        height: 48
                        width: parent.width - 4
                        radius: 8
                        color: "#FFFFFF"
                        visible: false
                    }
                    DropShadow {
                        anchors.fill: itembg
                        horizontalOffset: 3
                        verticalOffset: 3
                        radius: 8.0
                        samples: 17
                        color: "#80000000"
                        source: itembg
                        QText {
                            id: nemonictext
                            anchors {
                                fill: parent
                                topMargin: 5
                                bottomMargin: 5
                                leftMargin: 10
                                rightMargin: 5
                            }
                            verticalAlignment: Text.AlignVCenter
                            text: ((index+1) > 9 ? (index+1) : "0"+(index+1)) + ". " + textMnemonic.split(' ')[index];
                        }
                    }
                }
            }
        }

        QButtonTextLink {
            width: 203
            height: 24
            label: "BACK TO PREVIOUS"
            anchors {
                left: parent.left
                leftMargin: 32
                bottom: parent.bottom
                bottomMargin: 40
            }
            onButtonClicked: {
                if(NUNCHUCKTYPE.FLOW_ADD_WALLET === QMLHandle.currentFlow){
                    QMLHandle.sendEvent(EVT.EVT_ADD_NEW_SOFTWARE_SIGNER_TO_WALLET_BACK)
                }
                else {
                    QMLHandle.sendEvent(EVT.EVT_ADD_NEW_SOFTWARE_SIGNER_BACK)
                }
            }
        }

        QTextButton {
            width: 200
            height: 48
            label.text: "Continue"
            label.font.pixelSize: 16
            type: eTypeA
            enabled: (textMnemonic !== "")
            anchors {
                right: parent.right
                rightMargin: 40
                bottom: parent.bottom
                bottomMargin: 32
            }
            onButtonClicked: {
                QMLHandle.sendEvent(EVT.EVT_CREATE_NEW_SEED)
            }
        }
    }
    property string textMnemonic: AppModel.mnemonic
}
