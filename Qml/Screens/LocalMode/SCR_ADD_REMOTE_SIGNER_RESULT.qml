import QtQuick 2.4
import QtQuick.Controls 2.3
import QtGraphicalEffects 1.12
import Qt.labs.platform 1.1
import HMIEVENTS 1.0
import EWARNING 1.0
import NUNCHUCKTYPE 1.0
import "../../Components/customizes"
import "../../Components/origins"
QScreen {
    property string remoteSignerName: AppModel.singleSignerInfo.signerName
    property string remoteSignerXpub: AppModel.singleSignerInfo.signerXpub
    property string remoteSignerPublickey: AppModel.singleSignerInfo.signerPublickey
    property string remoteSignerbip32: AppModel.singleSignerInfo.signerDerivationPath
    property string remoteSignerfingerprint: AppModel.singleSignerInfo.signerMasterFingerPrint
    property int remoteSignerHealthStatus: -1

    property bool walletEscrow: (QMLHandle.onsRequester() === EVT.STATE_ID_SCR_WALLET_INFO) ? AppModel.walletInfo.walletEscrow :
                                (QMLHandle.onsRequester() === EVT.STATE_ID_SCR_ADD_SIGNER_TO_WALLET) ? AppModel.newWalletInfo.walletEscrow : false
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
                leftMargin: 41
                top: parent.top
                topMargin: 24
            }
            text: "Signer Info (Air-Gapped)"
            color: "#031F2B"
            font.family: "Montserrat"
            font.weight: Font.ExtraBold
            font.pixelSize: 24
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
                    QMLHandle.sendEvent(EVT.EVT_REMOTE_SIGNER_RESULT_CONFIRM_ADD_TO_WALLET_SIGNER_CONFIG)
                }
                else {
                    QMLHandle.sendEvent(EVT.EVT_REMOTE_SIGNER_RESULT_CONFIRM_ADD_TO_WALLET_SIGNER_CONFIG)
                }
            }
        }

        QNotification {
            id: notification
            anchors {
                horizontalCenter: parent.horizontalCenter
                top: parent.top
                topMargin: 81
            }
            visible: false
            label: "The Signer has been added successfully!"
//            currentStatus: eSUCCESS
        }

        QTextInputBox {
            id: signerName
            width: 343
            heightMin: 54
            mode: eREADONLY_MODE
            placeholder.text: "Signer Name"
            textOutput: remoteSignerName
            leftPading: 0
            rightPading: 50
            maximumLength: 100
            color: "transparent"
            fontFamily: "Lato"
            anchors {
                left: parent.left
                leftMargin: 41
                top: notification.bottom
                topMargin: 17
            }
        }

        QText {
            id: spectit
            anchors {
                left: parent.left
                leftMargin: 41
                top: signerName.bottom
                topMargin: 24
            }
            text: "AIR-GAPPED SIGNER SPEC"
            font.family: "Lato"
            font.pixelSize: 10
            font.weight: Font.Bold
        }

        Flickable {
            id: specinfo
            width: 328
            height: Math.min(250, contentHeight)
            flickableDirection: Flickable.VerticalFlick
            clip: true
            interactive: specinfo.height < specinfo.contentHeight
            contentHeight: specinfoContent.height
            ScrollBar.vertical: ScrollBar { active: true }
            anchors {
                left: parent.left
                leftMargin: 41
                top: spectit.bottom
                topMargin: 14
            }
            Column {
                id: specinfoContent
                width: 328
                height: xpubdisplay.height + bip32display.height + fingerPdisplay.height

                Item {
                    id: xpubdisplay
                    width: 328
                    height: 40 + (xpubValue.lineCount-1)*24
                    Row {
                        width: 328
                        height: parent.height
                        anchors.verticalCenter: parent.verticalCenter
                        spacing: 24
                        QText {
                            width: 152
                            height: 21
                            color: "#839096"
                            font.family: "Lato"
                            font.pixelSize: 14
                            anchors.verticalCenter: parent.verticalCenter
                            text: walletEscrow ? "Public Key:" : "XPUB:"
                        }
                        QText {
                            id: xpubValue
                            width: 133
                            height: lineCount*24
                            clip: true
                            color: "#031F2B"
                            font.family: "Lato"
                            font.pixelSize: 16
                            anchors.verticalCenter: parent.verticalCenter
                            wrapMode: Text.WrapAnywhere
                            text: walletEscrow ? remoteSignerPublickey : remoteSignerXpub
                        }
                    }
                    Rectangle {
                        width: 328
                        height: 1
                        color: "#C9DEF1"
                        anchors.bottom: parent.bottom
                    }
                }

                Item {
                    id: bip32display
                    width: 328
                    height: 40 + (bip32Value.lineCount-1)*24
                    Row {
                        width: 328
                        height: parent.height
                        spacing: 24
                        QText {
                            width: 152
                            height: 21
                            color: "#839096"
                            font.family: "Lato"
                            font.pixelSize: 14
                            anchors.verticalCenter: parent.verticalCenter
                            text: "BIP32 Path:"
                        }
                        QText {
                            id: bip32Value
                            width: 133
                            height: lineCount*24
                            clip: true
                            color: "#031F2B"
                            font.family: "Lato"
                            font.pixelSize: 16
                            anchors.verticalCenter: parent.verticalCenter
                            wrapMode: Text.WrapAnywhere
                            text: remoteSignerbip32
                        }
                    }
                    Rectangle {
                        width: 328
                        height: 1
                        color: "#C9DEF1"
                        anchors.bottom: parent.bottom
                    }
                }

                Item {
                    id: fingerPdisplay
                    width: 328
                    height: 40 + (fingerprintValue.lineCount-1)*24
                    Row {
                        width: 328
                        height: parent.height
                        spacing: 24
                        QText {
                            width: 152
                            height: 21
                            clip: true
                            color: "#839096"
                            font.family: "Lato"
                            text: "Master Key Fingerprint:"
                            font.pixelSize: 14
                            anchors.verticalCenter: parent.verticalCenter
                        }
                        QText {
                            id: fingerprintValue
                            width: 133
                            height: lineCount*24
                            color: "#031F2B"
                            font.family: "Lato"
                            font.pixelSize: 16
                            wrapMode: Text.WrapAnywhere
                            anchors.verticalCenter: parent.verticalCenter
                            text: remoteSignerfingerprint
                        }
                    }
                    Rectangle {
                        width: 328
                        height: 1
                        color: "#C9DEF1"
                        anchors.bottom: parent.bottom
                    }
                }
            }
        }
        QText {
            text: "USED IN WALLETS"
            font.pixelSize: 10
            font.family: "Lato"
            color: "#323E4A"
            font.weight: Font.Bold
            anchors.top: specinfo.bottom
            anchors.topMargin: 24
            anchors.left: parent.left
            anchors.leftMargin: 56
            visible: usedInWallets.count
        }

        QListView {
            id: usedInWallets
            width: 328
            height: 200
            anchors.top: specinfo.bottom
            anchors.topMargin: 42
            anchors.left: parent.left
            anchors.leftMargin: 40
            clip: true
            model: AppModel.walletsUsingSigner
            delegate: Item {
                width: 344
                height: 40
                QImage {
                    source: "qrc:/Images/Images/wallet_031F2B.png"
                    width: 24
                    height: 24
                    anchors {
                        verticalCenter: parent.verticalCenter
                        left: parent.left
                        leftMargin: 18
                    }
                }
                QText {
                    text: modelData
                    anchors {
                        verticalCenter: parent.verticalCenter
                        left: parent.left
                        leftMargin: 48
                    }
                    font.pixelSize: 16
                    color: "#031F2B"
                    font.family: "Lato"
                }
                Rectangle {
                    width: 468
                    height: 1
                    color: "#C9DEF1"
                    anchors {
                        horizontalCenter: parent.horizontalCenter
                        bottom: parent.bottom
                    }
                }
            }
        }

        QText {
            id: healthStatus
            anchors {
                left: parent.left
                leftMargin: 418
                top: notification.bottom
                topMargin: 17
            }
            text: "HEALTH CHECK"
            color: "#323E4A"
            font.family: "Montserrat"
            font.weight: Font.DemiBold
            font.pixelSize: 14
        }

        QImage {
            id: healthIndicator
            width: 24
            height: 24
            source: 0 === remoteSignerHealthStatus ? "qrc:/Images/Images/check_circle_outline_24px.png" :
                                                     "qrc:/Images/Images/error_outline_24px.png"
            anchors {
                left: healthStatus.right
                leftMargin: 10
                verticalCenter: healthStatus.verticalCenter
            }
            visible: healthStatustext.visible
        }

        Flickable {
            id: healthCheckInput
            width: 344
            height: Math.min(notification.visible ? 300 : 350, contentHeight)
            flickableDirection: Flickable.VerticalFlick
            clip: true
            interactive: healthCheckInput.height < healthCheckInput.contentHeight
            contentHeight: messageInput.height + signatureInput.height + healthStatustext.height + 48
            ScrollBar.vertical: ScrollBar { active: true }
            anchors {
                right: parent.right
                rightMargin: 40
                top: healthStatus.bottom
                topMargin: 20
            }

            QTextInputBox {
                id: messageInput
                width: 344
                heightMin: 56
                placeholder.text: "Message to sign"
                textOutput: AppModel.singleSignerInfo.signerMessage
                maximumLength: 280
                rightPading: 135
                onTypingFinished: {AppModel.singleSignerInfo.signerMessage = currentText}

                QTextButton {
                    width: 127
                    height: 24
                    label.text: "EXPORT MESSAGE"
                    label.font.pixelSize: 10
                    type: eTypeB
                    anchors {
                        right: parent.right
                        rightMargin: 8
                        verticalCenter: parent.verticalCenter
                    }
                    onButtonClicked: {
                        exportMessage.currentFile = StandardPaths.writableLocation(StandardPaths.DocumentsLocation) + "/" + AppModel.singleSignerInfo.signerMessage + "-message_to_sign.txt"
                        exportMessage.open()
                    }
                }
            }

            QTextInputBox {
                id: signatureInput
                width: 344
                heightMin: 80
                placeholder.text: "Signature"
                rightPading: 145
                anchors {
                    top: messageInput.bottom
                    topMargin: 16
                }

                QTextButton {
                    width: 135
                    height: 24
                    label.text: "IMPORT SIGNATURE"
                    label.font.pixelSize: 10
                    type: eTypeB
                    anchors {
                        right: parent.right
                        rightMargin: 8
                        verticalCenter: parent.verticalCenter
                    }
                    onButtonClicked: {
                        importSignature.open()
                    }
                }
            }

            Rectangle {
                id: healthStatustext
                width: 343
                visible: false
                height: visible ? (0 === remoteSignerHealthStatus) || (-1 === remoteSignerHealthStatus) ? 94 : 110 : 0
                anchors {
                    top: signatureInput.bottom
                    topMargin: 16
                }
                radius: 4
                border.color: (0 === remoteSignerHealthStatus )? "#35ABEE" : (-1 === remoteSignerHealthStatus) ? "Transparent" :"#E02247"

                QText {
                    width: 314
                    height: (-1 === remoteSignerHealthStatus)  ? 48 : 72
                    anchors {
                        top: parent.top
                        topMargin: 8
                        horizontalCenter: parent.horizontalCenter
                    }
                    wrapMode: Text.WordWrap
                    text: (-1 === remoteSignerHealthStatus) ? "Health Check Result" :
                                                              (0 === remoteSignerHealthStatus) ? "Your signer device is healthy." :
                                                                                                 "That doesn't seem right. Please double check the signature."
                    color: (-1 === remoteSignerHealthStatus) ? "#9CAEB8" : "#031F2B"
                    font.pixelSize: 16
                    font.family: "Lato"
                }
            }
        }

        QButtonIcon {
            id: btnhealthCheck
            width: 280
            height: 48
            anchors {
                right: parent.right
                rightMargin: 72
                top: healthCheckInput.bottom
                topMargin: 24
            }
            type: eFIRST
            label: "Run Health Check"
            icons: ["health_031F2B.png", "health_C9DEF1.png", "health_C9DEF1.png", "health_031F2B.png"]
            fontPixelSize: 16
            onButtonClicked: {
                QMLHandle.sendEvent(EVT.EVT_REMOTE_SIGNER_RESULT_HEALTH_CHECK)
                if(AppModel.masterSignerInfo.warningMessage.type === EWARNING.NONE_MSG){
                    remoteSignerHealthStatus = AppModel.singleSignerInfo.signerHealth
                    healthStatustext.visible = true
                }
                else{
                    remoteSignerHealthStatus = AppModel.singleSignerInfo.signerHealth
                    healthStatustext.visible = false
                }
            }
        }

        QTextButton {
            width: 222
            height: 48
            label.text: (QMLHandle.onsRequester() === EVT.STATE_ID_SCR_ADD_HARDWARE_SIGNER) ? "Close" : "Confirm & Close"
            label.font.pixelSize: 16
            type: eTypeB
            anchors {
                horizontalCenter: parent.horizontalCenter
                bottom: parent.bottom
                bottomMargin: 93
            }
            onButtonClicked: {
                if(NUNCHUCKTYPE.FLOW_ADD_WALLET === QMLHandle.currentFlow){
                    QMLHandle.sendEvent(EVT.EVT_REMOTE_SIGNER_RESULT_CONFIRM_ADD_TO_WALLET_SIGNER_CONFIG)
                }
                else {
                    QMLHandle.sendEvent(EVT.EVT_REMOTE_SIGNER_RESULT_CONFIRM_ADD_TO_WALLET_SIGNER_CONFIG)
                }
            }
        }
    }

    FileDialog {
        id: exportMessage
        fileMode: FileDialog.SaveFile
        onAccepted: {
            QMLHandle.sendEvent(EVT.EVT_REMOTE_SIGNER_RESULT_EXPORT_MESSAGE, exportMessage.currentFile)
        }
    }

    FileDialog {
        id: importSignature
        fileMode: FileDialog.OpenFile
        onAccepted: {
            QMLHandle.sendEvent(EVT.EVT_REMOTE_SIGNER_RESULT_IMPORT_SIGNATURE, importSignature.file)
        }
    }
}
