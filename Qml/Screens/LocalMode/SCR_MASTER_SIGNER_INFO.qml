import QtQuick 2.4
import QtQuick.Controls 2.3
import QtGraphicalEffects 1.12
import HMIEVENTS 1.0
import EWARNING 1.0
import NUNCHUCKTYPE 1.0
import "../../Components/customizes"
import "../../Components/origins"
QScreen {
    property string masterSignerName: AppModel.masterSignerInfo.masterSignername
    property string masterSignerSpec: AppModel.masterSignerInfo.fingerPrint
    property int    masterSignerHealth: -1
    property bool   masterSignerConnected:    AppModel.masterSignerInfo.masterSignerDevice.deviceConnected
    property bool   deviceNeedsPinSent:       AppModel.masterSignerInfo.needPinSent
    property bool   deviceNeedsPassphraseSent:AppModel.masterSignerInfo.needPassphraseSent

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
            text: "Signer Info"
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
                if(QMLHandle.onsRequester() === EVT.STATE_ID_SCR_WALLET_INFO){
                    QMLHandle.sendEvent(EVT.EVT_MASTER_SIGNER_INFO_BACK_WALLET_INFO)
                }
                else {
                    QMLHandle.sendEvent(EVT.EVT_MASTER_SIGNER_INFO_BACK_REQUEST)
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
            visible: AppModel.masterSignerInfo.warningMessage.type === EWARNING.EXCEPTION_MSG
            label: AppModel.masterSignerInfo.warningMessage.contentDisplay
            currentStatus: AppModel.masterSignerInfo.warningMessage.type
            onCloseRequest: AppModel.masterSignerInfo.warningMessage.type = EWARNING.NONE_MSG
        }

        Column {
            id: columnInfo
            spacing: 16
            anchors {
                left: parent.left
                leftMargin: 41
                top: notification.bottom
                topMargin: (QMLHandle.onsRequester() === EVT.STATE_ID_SCR_WALLET_INFO) ? 0 : 16
            }

            QTextInputBox {
                id: signerName
                width: 343
                heightMin: 54
                mode: ePREVIEW_MODE
                color: "Transparent"
                placeholder.text: "Signer Name"
                textOutput: masterSignerName
                rightPading: 50
                maximumLength: 50
                onTypingFinished: {
                    if(currentText !== masterSignerName && currentText !== ""){
                        QMLHandle.sendEvent(EVT.EVT_MASTER_SIGNER_INFO_EDIT_NAME, currentText)
                    }
                }
            }

            QTextInputBox {
                id: signerSpec
                width: 343
                heightMin: 54
                mode: eREADONLY_MODE
                color: "Transparent"
                placeholder.text: "SIGNER SPEC (MASTER KEY FINGERPRINT)"
                textOutput: masterSignerSpec
                textOutputCapitalization: Font.AllUppercase
            }

            QTextInputBox {
                id: xpubs
                visible: (QMLHandle.onsRequester() === EVT.STATE_ID_SCR_WALLET_INFO)
                width: 343
                heightMin: 54
                mode: eREADONLY_MODE
                color: "Transparent"
                placeholder.text: (QMLHandle.onsRequester() === EVT.STATE_ID_SCR_WALLET_INFO) ? "XPUB:" : ""
                textOutput: (QMLHandle.onsRequester() === EVT.STATE_ID_SCR_WALLET_INFO) ? AppModel.singleSignerInfo.signerXpub : ""
            }

            QTextInputBox {
                id: derpath
                visible: (QMLHandle.onsRequester() === EVT.STATE_ID_SCR_WALLET_INFO)
                width: 343
                heightMin: 54
                mode: eREADONLY_MODE
                color: "Transparent"
                placeholder.text: (QMLHandle.onsRequester() === EVT.STATE_ID_SCR_WALLET_INFO) ? "BIP32 Path:" : ""
                textOutput: (QMLHandle.onsRequester() === EVT.STATE_ID_SCR_WALLET_INFO) ? AppModel.singleSignerInfo.signerDerivationPath : ""
            }
        }

        QText {
            width: 500
            anchors {
                left: parent.left
                leftMargin: 56
                top: columnInfo.bottom
                topMargin: 24
            }
            text: "USED IN WALLETS"
            color: "#323E4A"
            font.weight: Font.Bold
            font.family: "Lato"
            font.pixelSize: 10
            visible: inWallets.count
        }

        QListView {
            id: inWallets
            width: 344
            height: 160
            clip: true
            anchors {
                left: parent.left
                leftMargin: 40
                top: columnInfo.bottom
                topMargin: 42
            }
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
            source: 0 === masterSignerHealth ? "qrc:/Images/Images/check_circle_outline_24px.png" :
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
            contentHeight: (switchCutomizeMessage.checked ?  custommessage.height : 0) + onOffCutomizeMessage.height + healthStatustext.height + 48
            ScrollBar.vertical: ScrollBar { active: true }
            anchors {
                right: parent.right
                rightMargin: 40
                top: healthStatus.bottom
                topMargin: 20
            }

            Row {
                id: onOffCutomizeMessage
                height: 24
                spacing: 56
                QText {
                    width: 203
                    height: 24
                    text: "Custom Test Message"
                    color: "#323E4A"
                    font.family: "Montserrat"
                    font.weight: Font.DemiBold
                    font.pixelSize: 14
                    anchors.verticalCenter: parent.verticalCenter
                }

                QSwitch {
                    id: switchCutomizeMessage
                    width: 34
                    height: 20
                    checked: false
                    anchors.verticalCenter: parent.verticalCenter
                }
            }

            Column {
                id: custommessage
                spacing: 16
                width: 344
                visible: switchCutomizeMessage.checked
                anchors {
                    top: onOffCutomizeMessage.bottom
                    topMargin: 16
                }

                QTextInputBox {
                    id: messageInput
                    width: 344
                    heightMin: custommessage.visible ? 56 : 0
                    placeholder.text: "Message to sign"
                    textOutput: AppModel.masterSignerInfo.message
                    mode: ePREVIEW_MODE
                    rightPading: 50
                    onTypingFinished: { AppModel.masterSignerInfo.message = currentText}
                }

                QTextInputBox {
                    id: signatureInput
                    width: 344
                    heightMin: custommessage.visible ? 80 : 0
                    placeholder.text: "Signature"
                    textOutput: AppModel.masterSignerInfo.signature
                    mode: eREADONLY_MODE
                }
            }

            Rectangle {
                id: healthStatustext
                width: 343
                visible: false
                height: visible ? (0 === masterSignerHealth) || (-1 === masterSignerHealth) ? 94 : 110 : 0
                anchors {
                    top: switchCutomizeMessage.checked ? custommessage.bottom : onOffCutomizeMessage.bottom
                    topMargin: 16
                }
                radius: 4
                border.width: 2
                border.color: (0 === masterSignerHealth )? "#35ABEE" : (-1 === masterSignerHealth) ? "Transparent" :"#E02247"

                QText {
                    width: 314
                    height: (-1 === masterSignerHealth)  ? 48 : 72
                    anchors {
                        top: parent.top
                        topMargin: 8
                        horizontalCenter: parent.horizontalCenter
                    }
                    wrapMode: Text.WordWrap
                    text: (-1 === masterSignerHealth) ? "Health Check Result Details..." :
                                                        (0 === masterSignerHealth) ? "Your signer device is healthy." :
                                                                                     "No signature generated. Please check your connection and make sure you have unlocked the device."
                    color: (-1 === masterSignerHealth) ? "#9CAEB8" : "#031F2B"
                    font.pixelSize: 16
                    font.family: "Lato"
                }
            }
        }

        QText {
            anchors {
                horizontalCenter: btnhealthCheck.horizontalCenter
                bottom: btnhealthCheck.top
                bottomMargin: 5
            }
            text: "Plug in your signer to perform a health check."
            color: "#031F2B"
            font.family: "Lato"
            font.pixelSize: 14
        }

        QButtonIcon {
            id: btnhealthCheck
            width: 280
            height: 48
            anchors {
                right: parent.right
                rightMargin: 72
                top: healthCheckInput.bottom
                topMargin: 32
            }
            type: eFIRST
            label: "Run Health Check"
            icons: ["health_031F2B.png", "health_C9DEF1.png", "health_C9DEF1.png", "health_031F2B.png"]
            fontPixelSize: 16
            onButtonClicked: {
                masterSignerHealth = -1
                healthStatustext.visible = false
                AppModel.masterSignerInfo.warningMessage.type = EWARNING.NONE_MSG
                if(AppModel.masterSignerInfo.isSoftwareSigner){
                    if(deviceNeedsPassphraseSent){
                        passphraseModel.visible = true
                    }
                    else{
                        busyOverlay.visible = true
                        QMLHandle.sendEvent(EVT.EVT_MASTER_SIGNER_INFO_HEALTH_CHECK, AppModel.masterSignerInfo.masterSignerId)
                    }
                }
                else{
                    QMLHandle.sendEvent(EVT.EVT_MASTER_SIGNER_INFO_PROMT_PIN)
                    // Check property cua obj de show pin model
                    if(deviceNeedsPinSent){ pinModel.visible = true }
                    else{
                        busyOverlay.visible = true
                        QMLHandle.sendEvent(EVT.EVT_MASTER_SIGNER_INFO_HEALTH_CHECK, AppModel.masterSignerInfo.masterSignerId)
                    }
                }
            }
        }

        QTextButton {
            width: 280
            height: 48
            label.text: "Remove Signer From Manager"
            label.font.pixelSize: 16
            type: eTypeB
            anchors {
                right: parent.right
                rightMargin: 72
                top: btnhealthCheck.bottom
                topMargin: 16
            }
            onButtonClicked: {
                modelConfirmDelete.visible = true
            }
        }

        Rectangle {
            id: modelConfirmDelete
            anchors.fill: parent
            visible: false
            MouseArea {anchors.fill: parent; onClicked: {}}
            color: Qt.rgba(255, 255, 255, 0.7)
            radius: 8
            Rectangle {
                id: modelConfirmDeleteContent
                width: 488
                height: 317
                color: "#F1FAFE"
                anchors.centerIn: parent
                radius: 8
                Rectangle {
                    width: parent.width
                    height: 2
                    color: "Red"
                    radius: 8
                }

                QText {
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.top: parent.top
                    anchors.topMargin: 51
                    horizontalAlignment: Text.AlignHCenter
                    text: "Confirm Deleting Signer"
                    color: "#031F2B"
                    font.family: "Montserrat"
                    font.weight: Font.DemiBold
                    font.pixelSize: 24
                }

                QText {
                    width: 355
                    height: 63
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.top: parent.top
                    anchors.topMargin: 91
                    wrapMode: Text.WordWrap
                    horizontalAlignment: Text.AlignHCenter
                    text: "Are you sure you want to delete it? "
                    font.family: "Lato"
                    font.pixelSize: 14
                }

                QTextInputBox {
                    id: inputDelete
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.top: parent.top
                    anchors.topMargin: 150
                    maximumLength: 10
                    placeholder.text: "Type DELETE to confirm deletion."
                }

                Row {
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 50
                    spacing: 16
                    QTextButton {
                        width: 100
                        height: 32
                        label.text: "CANCEL"
                        label.font.pixelSize: 10
                        type: eTypeB
                        radius: 12
                        onButtonClicked: {modelConfirmDelete.visible = false}
                    }

                    QTextButton {
                        width: 100
                        height: 32
                        label.text: "DELETE"
                        radius: 12
                        label.font.pixelSize: 10
                        type: eTypeA
                        enabled: (inputDelete.textOutput === "DELETE")
                        onButtonClicked: {
                            if(inputDelete.textOutput === "DELETE"){
                                QMLHandle.sendEvent(EVT.EVT_MASTER_SIGNER_INFO_REMOVE_REQUEST, AppModel.masterSignerInfo.masterSignerId)
                            }
                        }
                    }
                }
            }

            DropShadow {
                anchors.fill: modelConfirmDeleteContent
                horizontalOffset: 3
                verticalOffset: 3
                radius: 8.0
                samples: 17
                color: "#80000000"
                source: modelConfirmDeleteContent
            }
        }

        Rectangle {
            id: busyOverlay
            visible: false
            anchors.fill: parent
            color: Qt.rgba(0, 0, 0, 0.9)
            anchors.bottom: parent.bottom
            radius: 8
            MouseArea {
                anchors.fill: parent
                onClicked: {}
            }
            Connections {
                target: AppModel
                onFinishedHealthCheckMasterSigner : {
                    busyOverlay.visible = false
                    if(AppModel.masterSignerInfo.warningMessage.type === EWARNING.NONE_MSG){
                        masterSignerHealth = AppModel.masterSignerInfo.masterSignerHealth
                        healthStatustext.visible = true
                    }
                    else{
                        masterSignerHealth = AppModel.masterSignerInfo.masterSignerHealth
                        healthStatustext.visible = false
                    }
                }
            }

            Loader {
                id: busyIndi
                anchors.centerIn: parent
                sourceComponent: createBusy
            }

            Component {
                id: createBusy
                Column {
                    spacing: 8
                    Item {
                        width: 52
                        height: 52
                        QBusyIndicator {
                            width: 44
                            height: 44
                            anchors.centerIn: parent
                        }
                        anchors.horizontalCenter: parent.horizontalCenter
                    }

                    QText {
                        width: 700
                        anchors.horizontalCenter: parent.horizontalCenter
                        horizontalAlignment: Text.AlignHCenter
                        color: "#F6D65D"
                        font.pixelSize: 24
                        font.weight: Font.DemiBold
                        font.family: "Montserrat"
                        wrapMode: Text.WrapAnywhere
                        text: "Health check signer: " + masterSignerName
                    }
                    QText {
                        width: 328
                        height: 42
                        anchors.horizontalCenter: parent.horizontalCenter
                        horizontalAlignment: Text.AlignHCenter
                        color: "#F6D65D"
                        font.pixelSize: 14
                        wrapMode: Text.WordWrap
                        font.family: "Lato"
                        text: "Please Confirm on the Signer Device"
                    }
                }
            }
        }

        QNumPad {
            id: pinModel
            anchors.fill: parent
            visible: false
            onSenPINClicked : { QMLHandle.sendEvent(EVT.EVT_MASTER_SIGNER_INFO_SEND_PIN, pinModel.pinInputted)}
        }

        QPassphraseInput {
            id: passphraseModel
            color: Qt.rgba(255, 255, 255, 0.7)
            anchors.fill: parent
            visible: false
            onCloseClicked: {
                passphraseModel.visible = false
            }
            onSendPassphraseClicked: {
                var signerObj = { "passphraseInput"    : passphrase,
                                  "mastersignerId"     : AppModel.masterSignerInfo.masterSignerId};
                QMLHandle.sendEvent(EVT.EVT_MASTER_SIGNER_INFO_SEND_PASSPHRASE, signerObj)
                if(AppModel.masterSignerInfo.warningMessage.type === EWARNING.NONE_MSG){
                    busyOverlay.visible = true
                    QMLHandle.sendEvent(EVT.EVT_MASTER_SIGNER_INFO_HEALTH_CHECK, AppModel.masterSignerInfo.masterSignerId)
                }
                passphraseModel.visible = false
                passphraseModel.textInputted = ""
            }
        }

        Connections {
            target: AppModel
            onSentPINToDeviceResult : {
                console.log("onSentPINToDeviceResult ", result)
                if(result === EWARNING.NONE_MSG){
                    pinModel.visible = false
                    busyOverlay.visible = true
                }
                else{
                    // Warning message
                    pinModel.warning = "The PIN you entered is incorrect. Please try again."
                    pinModel.validInput = false
                }
                pinModel.pinInputted = ""
            }
        }
    }
}
