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
    id: rootWalletConfirmation
    property string masterSignerName: AppModel.masterSignerInfo.masterSignername
    property string masterSignerSpec: AppModel.masterSignerInfo.masterSignerDevice.deviceMasterFingerPrint
    property int masterSignerHealth: AppModel.masterSignerInfo.masterSignerHealth

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
            anchors {
                left: parent.left
                leftMargin: 40
                top: parent.top
                topMargin: 24
            }
            text: "Add New Wallet"
            color: "#031F2B"
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
                QMLHandle.sendEvent(EVT.EVT_ONS_CLOSE_REQUEST, EVT.STATE_ID_SCR_ADD_WALLET_CONFIRMATION)
            }
        }

        QNotification {
            id: notification
            anchors {
                horizontalCenter: parent.horizontalCenter
                top: parent.top
                topMargin: 60
            }
            visible: AppModel.newWalletInfo.warningMessage.type === EWARNING.EXCEPTION_MSG
            label: AppModel.newWalletInfo.warningMessage.contentDisplay
            currentStatus: AppModel.newWalletInfo.warningMessage.type
            onCloseRequest: AppModel.newWalletInfo.warningMessage.type = EWARNING.NONE_MSG
        }

        Row {
            id: step
            width: 644
            height: 31
            spacing: 8
            anchors {
                left: parent.left
                leftMargin: 40
                top: notification.bottom
                topMargin: 8
            }
            QAddStep {
                step: 1
                stepName: "Wallet Configuration"
                currentStep: 3
                anchors.verticalCenter: parent.verticalCenter
            }
            Rectangle {
                width: 30
                height: 2
                radius: 4
                color: "#839096"
                anchors.verticalCenter: parent.verticalCenter
            }
            QAddStep {
                step: 2
                stepName: "Signer Configuration"
                currentStep: 3
                anchors.verticalCenter: parent.verticalCenter
            }
            Rectangle {
                width: 30
                height: 2
                radius: 4
                color: "#839096"
                anchors.verticalCenter: parent.verticalCenter
            }
            QAddStep {
                step: 3
                stepName: "Confirmation"
                currentStep: 3
                anchors.verticalCenter: parent.verticalCenter
            }
        }

        QText {
            id: confirmtit
            anchors {
                left: parent.left
                leftMargin: 40
                top: step.bottom
                topMargin: notification.visible ? 16 : 32
            }
            color: "#031F2B"
            font.pixelSize: 16
            font.weight: Font.DemiBold
            text: "Wallet & Signer Configuration"
        }

        Column {
            spacing: 24
            anchors {
                left: parent.left
                leftMargin: 40
                top: confirmtit.bottom
                topMargin: notification.visible ? 8 : 16
            }

            QTextInputBox {
                id: signerName
                width: 720
                heightMin: 54
                color: "Transparent"
                mode: eREADONLY_MODE
                placeholder.text: "Wallet Name"
                textOutput: AppModel.newWalletInfo.walletName
            }

            QTextInputBox {
                id: mandn
                width: 269
                heightMin: 54
                color: "Transparent"
                mode: eREADONLY_MODE
                placeholder.text: "Wallet Type"
                textOutput: AppModel.newWalletInfo.walletEscrow ? "One-Time Escrow Wallet" : "Standard Wallet"
            }

            QTextInputBox {
                id: addresstype
                readonly property var addressType_Value: ["Random", "Legacy", "Nested Segwit", "Native Segwit"]
                width: 269
                heightMin: 54
                color: "Transparent"
                mode: eREADONLY_MODE
                placeholder.text: "ADDRESS TYPE"
                textOutput: addresstype.addressType_Value[AppModel.newWalletInfo.walletAddressType]
            }

            QTextInputBox {
                id: createdate
                width: 269
                heightMin: 54
                color: "Transparent"
                mode: eREADONLY_MODE
                placeholder.text: "CREATED"
                textOutput: AppModel.newWalletInfo.walletCreateDate
            }
        }

        QTextInputBox {
            id: signerConfiguration
            anchors {
                left: parent.left
                leftMargin: 350
                top: parent.top
                topMargin: 265
            }
            width: 269
            heightMin: 54
            color: "Transparent"
            mode: eREADONLY_MODE
            placeholder.text: "SIGNER CONFIGURATION"
            textOutput: AppModel.newWalletInfo.walletN > 1 ? AppModel.newWalletInfo.walletM + " Of " +AppModel.newWalletInfo.walletN + " Multisig" :
                                                             "1 Of 1 Singlesig"
        }

        QText {
            anchors {
                left: parent.left
                leftMargin: 357
                top: parent.top
                topMargin: 346
            }
            color: "#323E4A"
            font.pixelSize: 10
            font.weight: Font.Bold
            text: "SIGNERS"
        }

        QListView {
            id: signerAssigned
            width: 343
            height: 197
            spacing: 8
            clip: true
            anchors {
                left: parent.left
                leftMargin: 357
                top: parent.top
                topMargin: 364
            }
            ScrollBar.vertical: ScrollBar { active: true }
            model: AppModel.newWalletInfo.walletSingleSignerAssigned
            delegate: Item {
                id: signerAssigneddlg
                property bool isNeedGetXpubs:  (!single_signer_isRemote && !single_signer_signed_isValid)
                signal updateSelectedState()
                width: 343
                height: single_signer_isRemote ? 73 : 53
                Rectangle {
                    id: rect
                    anchors.horizontalCenter: parent.horizontalCenter
                    width: 341
                    height: single_signer_isRemote ? 70 : 50
                    color: Qt.rgba(255, 255, 255)
                    border.color: signerAssigneddlg.isNeedGetXpubs ? "#FF7A00" : "transparent"
                }
                DropShadow {
                    anchors.fill: rect
                    verticalOffset: 2
                    cached: true
                    radius: 8
                    samples: 16
                    color: Qt.rgba(0, 0, 0, 0.15)
                    source: rect
                }

                Rectangle {
                    id: indicatiorValid
                    width: 4
                    height: parent.height - 18
                    radius: 2
                    color: single_signer_signed_isValid ? "#C9DEF1" : "#FF7A00"
                    anchors {
                        left: parent.left
                        leftMargin: 16
                        verticalCenter: rect.verticalCenter
                    }
                }

                Column {
                    id: txt
                    width: 300
                    height: single_signer_isRemote ? 53 : 37
                    anchors {
                        left: parent.left
                        leftMargin: 28
                        verticalCenter: indicatiorValid.verticalCenter
                    }
                    Item {
                        width: parent.width
                        height: 21
                        QText {
                            width: parent.width - 63
                            height: 21
                            verticalAlignment: Text.AlignVCenter
                            text: singleSigner_name
                            font.pixelSize: 14
                            font.weight: Font.DemiBold
                            font.family: "Montserrat"
                            color: "#031F2B"
                            elide: Text.ElideRight
                            anchors.verticalCenter: parent.verticalCenter
                        }
                        Rectangle {
                            width: 63
                            height: 21
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.right: parent.right
                            color: "#C9DEF1"
                            visible: single_signer_isRemote
                            radius: 4
                            QText {
                                text: "AIR-GAPPED"
                                font.family: "Lato"
                                font.weight: Font.Bold
                                font.pixelSize: 10
                                anchors.centerIn: parent
                                color: "#031F2B"
                            }
                        }
                    }

                    QText {
                        width: parent.width
                        height: 16
                        font.pixelSize: 12
                        color: "#031F2B"
                        font.family: "Lato"
                        text: "XFP: " + singleSigner_masterFingerPrint
                        font.capitalization: Font.AllUppercase
                    }

                    QText {
                        width: parent.width
                        height: single_signer_isRemote ? 16 : 0
                        visible: single_signer_isRemote
                        font.pixelSize: 10
                        color: "#839096"
                        font.family: "Lato"
                        text: "BIP32 Path: " + singleSigner_derivationPath
                    }
                }

                QTextButton {
                    width: 100
                    height: 24
                    label.text: "Get XPUBs"
                    label.font.pixelSize: 10
                    type: eTypeD
                    visible: signerAssigneddlg.isNeedGetXpubs
                    border.width: 1
                    enabled: visible
                    anchors {
                        right: parent.right
                        rightMargin: 8
                        verticalCenter: rect.verticalCenter
                    }
                    onButtonClicked: {
                        startCacheXpubs(index, singleSigner_name)
                        QMLHandle.sendEvent(EVT.EVT_ADD_WALLET_TOP_UP_XPUBS_REQUEST, index)
                    }
                }
            }
        }

        QText {
            visible: !AppModel.newWalletInfo.capableCreate
            width: 344
            height: 48
            font.pixelSize: 12
            color: "#323E4A"
            font.family: "Lato"
            wrapMode: Text.WordWrap
            anchors {
                horizontalCenter: signerAssigned.horizontalCenter
                top: signerAssigned.bottom
                topMargin: 4
            }
            text: "To create new wallets, additional XPUBs from the Signer are needed. Please plug in your Signer device, unlock, and press \"Get XPUBs\""
        }

        QButtonTextLink {
            width: 203
            height: 24
            label: "BACK TO PREVIOUS"
            anchors {
                left: parent.left
                leftMargin: 40
                bottom: parent.bottom
                bottomMargin: 43
            }
            onButtonClicked: {
                QMLHandle.sendEvent(EVT.EVT_ADD_WALLET_CONFIRMATION_BACK_REQUEST)
            }
        }

        QTextButton {
            width: 120
            height: 48
            label.text: "Cancel"
            label.font.pixelSize: 16
            type: eTypeB
            anchors {
                left: parent.left
                leftMargin: 424
                bottom: parent.bottom
                bottomMargin: 32
            }
            onButtonClicked: {
                QMLHandle.sendEvent(EVT.EVT_ONS_CLOSE_REQUEST, EVT.STATE_ID_SCR_ADD_WALLET_CONFIRMATION)
            }
        }

        QTextButton {
            width: 200
            height: 48
            label.text: "Create Wallet"
            label.font.pixelSize: 16
            type: eTypeA
            enabled: AppModel.newWalletInfo.capableCreate
            anchors {
                right: parent.right
                rightMargin: 40
                bottom: parent.bottom
                bottomMargin: 32
            }
            onButtonClicked: {
                AppModel.newWalletInfo.warningMessage.type = EWARNING.NONE_MSG
                startDownloadDescriptor()
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

            QCloseButton {
                icon: "qrc:/Images/Images/close_24px.png"
                anchors {
                    right: parent.right
                    rightMargin: 16
                    top: parent.top
                    topMargin: 16
                }
                onClicked: {
                    busyOverlay.visible = false
                    busyIndi.sourceComponent = null
                }
            }

            Loader {
                id: busyIndi
                anchors.centerIn: parent
            }
        }

        Component {
            id: cacheXpubsComp
            Column {
                spacing: 8
                QProgressbar {
                    id: progresBar
                    anchors.horizontalCenter: parent.horizontalCenter
                    percentage: AppModel.cacheXpubsPercentage
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
                    text: "Get XPUBs for Signer " + nameSignerNeedGetXpub
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
                    text: "Nunchuk is processing your signer information"
                }
            }
        }

        Component {
            id: downloadDescriptor
            Column {
                spacing: 8
                Item {
                    width: 52
                    height: 52
                    visible: false // disable spiner
                    QBusyIndicator {
                        width: 44
                        height: 44
                        anchors.centerIn: parent
                    }
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                QText {
                    width: 214
                    height: 36
                    anchors.horizontalCenter: parent.horizontalCenter
                    horizontalAlignment: Text.AlignHCenter
                    color: "#F6D65D"
                    font.pixelSize: 24
                    font.weight: Font.DemiBold
                    font.family: "Montserrat"
                    text: "Creating New Wallet"
                }
                QText {
                    width: 450
                    anchors.horizontalCenter: parent.horizontalCenter
                    horizontalAlignment: Text.AlignHCenter
                    color: "#F6D65D"
                    font.pixelSize: 14
                    wrapMode: Text.WordWrap
                    font.family: "Lato"
                    text: "Please save the wallet configuration file (.bsms). You can use it to recover your wallet later using any BSMS-compatible software. BSMS is defined in BIP-0129: Bitcoin Secure Multisig Setup."
                }

                Item {
                    width: 382
                    height: 16
                }

                QTextButton {
                    width: 265
                    height: 48
                    anchors.horizontalCenter: parent.horizontalCenter
                    label.text: "Download Wallet BSMS File"
                    label.font.pixelSize: 16
                    type: eTypeA
                    onButtonClicked: {
                        savefileDialog.currentFile = StandardPaths.writableLocation(StandardPaths.DocumentsLocation) + "/" + AppModel.newWalletInfo.walletName + "-Descriptors.bsms"
                        savefileDialog.open()
                    }
                }

                QTextButton {
                    width: 265
                    height: 48
                    anchors.horizontalCenter: parent.horizontalCenter
                    label.text: "Skip Backup"
                    label.font.pixelSize: 16
                    type: eTypeC
                    onButtonClicked: {
                        startCreateWalletWithoutBackupDescriptor()
                    }
                }

                QText {
                    width: 328
                    anchors.horizontalCenter: parent.horizontalCenter
                    horizontalAlignment: Text.AlignHCenter
                    wrapMode: Text.WordWrap
                    color: "#F6D65D"
                    font.pixelSize: 12
                    font.family: "Lato"
                    text: "*Note that this backup strictly contains information about the wallet. It does not include information on seeds or private keys."
                }
            }
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
                    width: 214
                    height: 36
                    anchors.horizontalCenter: parent.horizontalCenter
                    horizontalAlignment: Text.AlignHCenter
                    color: "#F6D65D"
                    font.pixelSize: 24
                    font.weight: Font.DemiBold
                    font.family: "Montserrat"
                    text: "Your Wallet is Almost There"
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
                    text: "Wrapping up..."
                }
            }
        }
    }

    FileDialog {
        id: savefileDialog
        fileMode: FileDialog.SaveFile
        onAccepted: {
            QMLHandle.sendEvent(EVT.EVT_ADD_WALLET_CONFIRM_CREATE)
            QMLHandle.sendEvent(EVT.EVT_ADD_WALLET_DOWNLOAD_DESCRIPTOR, savefileDialog.currentFile)
            startCreateWallet()
        }
        onRejected: {
            busyOverlay.visible = false
            busyIndi.sourceComponent = null
        }
    }

    property int indexSignerNeedGetXpub: -1
    property string nameSignerNeedGetXpub: ""

    function startCreateWalletWithoutBackupDescriptor(){
        QMLHandle.sendEvent(EVT.EVT_ADD_WALLET_CONFIRM_CREATE)
        startCreateWallet()
    }

    function startDownloadDescriptor(){
        busyIndi.sourceComponent = downloadDescriptor
        busyOverlay.visible = true
    }

    function startCreateWallet(){
        busyIndi.sourceComponent = createBusy
        busyOverlay.visible = true
        notifyFinalStep.start()
    }

    Timer {
        id: notifyFinalStep
        interval: 2000
        onTriggered: {
            if(AppModel.newWalletInfo.warningMessage.type !== EWARNING.EXCEPTION_MSG){
                QMLHandle.sendEvent(EVT.EVT_ADD_WALLET_SUCCESSFULLY)
            }
            else{
                busyIndi.sourceComponent = null
                busyOverlay.visible = false
                notifyFinalStep.stop()
            }
        }
    }

    Connections {
        target: AppModel
        onCacheXpubsPercentageChanged: {
            if(indexSignerNeedGetXpub !== -1 && AppModel.cacheXpubsPercentage === 100){
                busyOverlay.visible = false
                QMLHandle.sendEvent(EVT.EVT_ADD_WALLET_GENERATE_SIGNER,  indexSignerNeedGetXpub)
                indexSignerNeedGetXpub = -1
                nameSignerNeedGetXpub = ""
                busyIndi.sourceComponent = null
            }
        }
    }

    function startCacheXpubs(num, name) {
        busyIndi.sourceComponent = cacheXpubsComp
        busyOverlay.visible = true
        indexSignerNeedGetXpub = num
        nameSignerNeedGetXpub = name
    }
}
