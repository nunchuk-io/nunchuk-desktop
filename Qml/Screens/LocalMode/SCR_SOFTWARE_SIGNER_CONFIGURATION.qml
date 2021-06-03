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
    property string signerNameInputted: ""
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

        Loader {
            id: mainloader
            anchors.fill: parent
            sourceComponent: nameConfiguration
        }

        Component {
            id: nameConfiguration
            Item {
                QCloseButton {
                    anchors {
                        right: parent.right
                        rightMargin: 16
                        top: parent.top
                        topMargin: 16
                    }
                    onClicked: {
                        if(NUNCHUCKTYPE.FLOW_ADD_WALLET === QMLHandle.currentFlow){
                            QMLHandle.sendEvent(EVT.EVT_SOFTWARE_SIGNER_CONFIGURATION_BACK_TO_WALLET_SIGNER_CONFIGURATION)
                        }
                        else{
                            QMLHandle.sendEvent(EVT.EVT_ONS_CLOSE_REQUEST, EVT.STATE_ID_SCR_SOFTWARE_SIGNER_CONFIGURATION)
                        }
                    }
                }

                QText {
                    width: 163
                    height: 27
                    anchors {
                        left: parent.left
                        leftMargin: 40
                        top: parent.top
                        topMargin: 24
                    }
                    text: "Give it a name"
                    color: "#031F2B"
                    font.family: "Montserrat"
                    font.weight: Font.ExtraBold
                    font.pixelSize: 24
                }

                QTextInputBoxTypeB {
                    id: signername
                    label: "Name of software signer"
                    boxWidth: 540
                    boxHeight: 48
                    maxLength: 20
                    enableLengthLimit: true
                    anchors {
                        left: parent.left
                        leftMargin: 40
                        top: parent.top
                        topMargin: 130
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
                        QMLHandle.sendEvent(EVT.EVT_ONS_CLOSE_REQUEST, EVT.STATE_ID_SCR_SOFTWARE_SIGNER_CONFIGURATION)
                    }
                }

                QTextButton {
                    width: 200
                    height: 48
                    anchors {
                        right: parent.right
                        rightMargin: 40
                        bottom: parent.bottom
                        bottomMargin: 32
                    }
                    label.text: "Continue"
                    label.font.pixelSize: 16
                    type: eTypeA
                    enabled: signername.textInputted !== ""
                    onButtonClicked: {
                        rootAddsignerToWallet.signerNameInputted = signername.textInputted
                        mainloader.sourceComponent = passPhraseConfiguration
                    }
                }

            }
        }

        Component {
            id: passPhraseConfiguration
            Item {
                QCloseButton {
                    anchors {
                        right: parent.right
                        rightMargin: 16
                        top: parent.top
                        topMargin: 16
                    }
                    onClicked: {
                        if(NUNCHUCKTYPE.FLOW_ADD_WALLET === QMLHandle.currentFlow){
                            QMLHandle.sendEvent(EVT.EVT_SOFTWARE_SIGNER_CONFIGURATION_BACK_TO_WALLET_SIGNER_CONFIGURATION)
                        }
                        else{
                            QMLHandle.sendEvent(EVT.EVT_ONS_CLOSE_REQUEST, EVT.STATE_ID_SCR_SOFTWARE_SIGNER_CONFIGURATION)
                        }
                    }
                }

                QText {
                    width: 163
                    height: 27
                    anchors {
                        left: parent.left
                        leftMargin: 40
                        top: parent.top
                        topMargin: 24
                    }
                    text: "Set a passphrase"
                    color: "#031F2B"
                    font.family: "Montserrat"
                    font.weight: Font.ExtraBold
                    font.pixelSize: 24
                }

                QText {
                    width: 540
                    height: 56
                    text: "Passphrase can be anything you want, and add extra security to your signer."
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

                Column {
                    spacing: 16
                    anchors {
                        left: parent.left
                        leftMargin: 40
                        top: parent.top
                        topMargin: 130
                    }

                    QTextInputBoxTypeB {
                        id: passphrase
                        label: "<b>Passphrase</b> (Optional)"
                        boxWidth: 540
                        boxHeight: 48
                        textweight: Font.Normal
                        echoMode: TextInput.Password
                        onTypingFinished: {
                            confirmpassphrases.isValid = true
                            confirmpassphrases.errorText = ""
                        }
                    }

                    QTextInputBoxTypeB {
                        id: confirmpassphrases
                        label: "Confirm Passphrase"
                        boxWidth: 540
                        boxHeight: 48
                        echoMode: TextInput.Password
                        onTypingFinished: {
                            confirmpassphrases.isValid = true
                            confirmpassphrases.errorText = ""
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
                        mainloader.sourceComponent = nameConfiguration
                    }
                }

                Row {
                    spacing: 12
                    anchors {
                        right: parent.right
                        rightMargin: 40
                        bottom: parent.bottom
                        bottomMargin: 32
                    }

                    QTextButton {
                        width: 200
                        height: 48
                        label.text: "Set Passphrase"
                        label.font.pixelSize: 16
                        type: eTypeB
                        enabled: (passphrase.textInputted !== "") && (confirmpassphrases.textInputted !== "")
                        onButtonClicked: {
                            if(confirmpassphrases.textInputted !== passphrase.textInputted){
                                confirmpassphrases.isValid = false
                                confirmpassphrases.errorText = "The confirm passphrase doesn't match"
                            }
                            else{
                                var signerObj = { "signername"    : rootAddsignerToWallet.signerNameInputted,
                                                  "passphrase"    : passphrase.textInputted };
                                QMLHandle.sendEvent(EVT.EVT_SOFTWARE_SIGNER_REQUEST_CREATE, signerObj)
                                stateAddSigner.visible = true
                            }
                        }
                    }

                    QTextButton {
                        width: 230
                        height: 48
                        label.text: "I don't need a passphrase"
                        label.font.pixelSize: 16
                        type: eTypeA
                        enabled: rootAddsignerToWallet.signerNameInputted !== ""
                        onButtonClicked: {
                            var signerObj = { "signername"    : rootAddsignerToWallet.signerNameInputted,
                                              "passphrase"    : ""};
                            QMLHandle.sendEvent(EVT.EVT_SOFTWARE_SIGNER_REQUEST_CREATE, signerObj)
                            stateAddSigner.visible = true
                        }
                    }
                }

            }
        }

        Rectangle {
            id: stateAddSigner
            visible: false
            anchors.fill: parent
            color: Qt.rgba(0, 0, 0, 0.9)
            anchors.bottom: parent.bottom
            radius: 8
            MouseArea {
                anchors.fill: parent
                onClicked: {}
            }
            Loader {
                id: busyIndi
                anchors.centerIn: parent
                sourceComponent: stateAddSigner.visible ? progressAddSigner : null
            }
        }

        Component {
            id: progressAddSigner
            Column {
                spacing: 8
                QProgressbar {
                    id: progresBar
                    anchors.horizontalCenter: parent.horizontalCenter
                    percentage: AppModel.addSignerPercentage
                }
                QText {
                    width: 186
                    height: 36
                    anchors.horizontalCenter: parent.horizontalCenter
                    horizontalAlignment: Text.AlignHCenter
                    color: "#F6D65D"
                    font.pixelSize: 24
                    font.weight: Font.DemiBold
                    font.family: "Montserrat"
                    text: "Processing..."
                }
                QText {
                    width: 328
                    height: 42
                    anchors.horizontalCenter: parent.horizontalCenter
                    horizontalAlignment: Text.AlignHCenter
                    wrapMode: Text.WordWrap
                    color: "#F6D65D"
                    font.pixelSize: 16
                    font.family: "Lato"
                    text: "The signer is being added. Once complete, you can assign this signer to new wallets."
                }
            }
        }
        Connections {
            target: AppModel
            onAddSignerPercentageChanged : {
                if(AppModel.addSignerPercentage === 100){
                    QMLHandle.sendEvent(EVT.EVT_ADD_SOFTWARE_SIGNER_RESULT)
                }
            }
        }
    }
}
