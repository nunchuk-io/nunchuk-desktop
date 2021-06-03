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
                QMLHandle.sendEvent(EVT.EVT_ONS_CLOSE_REQUEST, EVT.STATE_ID_SCR_ADD_WALLET)
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
                currentStep: 1
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
                currentStep: 1
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
                currentStep: 1
                anchors.verticalCenter: parent.verticalCenter
            }
        }

        QText {
            text: "Wallet Configuration"
            font.weight: Font.Bold
            font.pixelSize: 16
            color: "#031F2B"
            anchors {
                left: parent.left
                leftMargin: 40
                top: step.bottom
                topMargin: notification.visible ? 16 : 32
            }
        }

        Row {
            anchors {
                left: parent.left
                leftMargin: 40
                top: parent.top
                topMargin: 195
            }
            spacing: 32
            QTextInputBox {
                id: signerName
                width: 269
                heightMin: 56
                mode: textOutput === "" ? eEDIT_MODE : ePREVIEW_MODE
                placeholder.text: "Wallet Name"
                maximumLength: 42
                textOutput: AppModel.newWalletInfo.walletName
                color: Qt. rgba(255, 255, 255, 0.5)
                rightPading: 50
                border.color: "#C9DEF1"
                radius: 4
                anchors.verticalCenter: parent.verticalCenter
            }

            QTextInputBox {
                id: signerDescription
                width: 419
                heightMin: 56
                mode: textOutput === "" ? eEDIT_MODE : ePREVIEW_MODE
                placeholder.text: "Wallet Description"
                textOutput: AppModel.newWalletInfo.walletDescription
                color: Qt. rgba(255, 255, 255, 0.5)
                border.color: "#C9DEF1"
                rightPading: 50
                maximumLength: 76
                radius: 4
                anchors.verticalCenter: parent.verticalCenter
            }
        }

        QText {
            text: "Wallet Type"
            font.weight: Font.Bold
            font.pixelSize: 14
            color: "#031F2B"
            anchors {
                left: parent.left
                leftMargin: 40
                top: parent.top
                topMargin: 283
            }
        }

        FocusScope {
            anchors {
                left: parent.left
                leftMargin: 40
                top: parent.top
                topMargin: 312
            }
            Item {
                id: standardWallet
                focus: !AppModel.newWalletInfo.walletEscrow
                width: 130
                height: 24
                Image {
                    id:icostandard
                    source: standardWallet.focus ? "qrc:/Images/Images/RadioEnabled.png" : "qrc:/Images/Images/RadioDeselected.png"
                }
                QText {
                    text: "Standard Wallet"
                    font.pixelSize: 14
                    color: "#031F2B"
                    anchors {
                        left: icostandard.right
                        leftMargin: 8
                        verticalCenter: icostandard.verticalCenter
                    }
                }
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor
                    onClicked: standardWallet.focus = true
                }
            }

            Item {
                id: escrowWallet
                focus: AppModel.newWalletInfo.walletEscrow
                height: 24
                anchors {
                    left: standardWallet.right
                    leftMargin: 42
                    verticalCenter: standardWallet.verticalCenter
                }

                Row {
                    Item {
                        width: 212
                        height: 24
                        Image {
                            id:icoescrow
                            source: escrowWallet.focus ? "qrc:/Images/Images/RadioEnabled.png" : "qrc:/Images/Images/RadioDeselected.png"
                        }
                        QText {
                            id: textEscrow
                            text: "One-Time Escrow Wallet"
                            font.pixelSize: 14
                            color: "#031F2B"
                            anchors {
                                left: icoescrow.right
                                leftMargin: 8
                                verticalCenter: icoescrow.verticalCenter
                            }
                        }
                        MouseArea {
                            anchors.fill: parent
                            hoverEnabled: true
                            cursorShape: Qt.PointingHandCursor
                            onClicked: escrowWallet.focus = true
                        }
                    }
                    QTooltip {
                        toolTip: "An Escrow Wallet is a special single-use wallet, designed to hold funds temporarily."
                    }
                }
            }
        }

        Rectangle {
            width: 720
            height: 1
            color: "#C9DEF1"
            anchors {
                horizontalCenter: parent.horizontalCenter
                top: parent.top
                topMargin: 368
            }
        }

        Item {
            id: expandOption
            property bool expanded: false
            width: 492
            height: 24
            anchors {
                left: parent.left
                leftMargin: 40
                top: parent.top
                topMargin: 394
            }
            Row {
                Image {
                    id: expandIcon
                    source: expandOption.expanded ? "qrc:/Images/Images/expand_less_24px.png" : "qrc:/Images/Images/expand_more_24px.png"
                }
                QText {
                    text: "Advanced: Address Type (Default: Nested Segwit)"
                    font.weight: Font.Bold
                    font.pixelSize: 14
                    color: "#031F2B"
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor
                onClicked: expandOption.expanded = !expandOption.expanded
            }
        }

        Column {
            visible: expandOption.expanded
            focus: true
            spacing: 8
            anchors {
                left: parent.left
                leftMargin: 68
                top: parent.top
                topMargin: 425
            }
            Repeater {
                id: addressTypeSelection

                property int typeSeleted: AppModel.newWalletInfo.walletAddressType
                readonly property var typeValue: [NUNCHUCKTYPE.NATIVE_SEGWIT, NUNCHUCKTYPE.NESTED_SEGWIT, NUNCHUCKTYPE.LEGACY]
                readonly property var typeText: ["Native Segwit (highest fee saving)", "Nested Segwit (medium fee saving)", "Legacy (no fee saving)"]
                model: 3
                Item {
                    width: 576
                    height: 24
                    Row {
                        id: walletType
                        anchors.fill: parent
                        spacing: 8
                        Image {
                            id:icowalletType
                            source: (addressTypeSelection.typeSeleted === addressTypeSelection.typeValue[index]) ? "qrc:/Images/Images/RadioEnabled.png" :
                                                                                                                   "qrc:/Images/Images/RadioDeselected.png"     // Default   :
                        }
                        QText {
                            text: addressTypeSelection.typeText[index]
                            font.pixelSize: 14
                            color: "#031F2B"
                            anchors.verticalCenter: icowalletType.verticalCenter
                        }
                    }
                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor
                        onClicked: { addressTypeSelection.typeSeleted = addressTypeSelection.typeValue[index] }
                    }
                }
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
                leftMargin: 32
                bottom: parent.bottom
                bottomMargin: 32
            }
            onButtonClicked: {
                QMLHandle.sendEvent(EVT.EVT_ONS_CLOSE_REQUEST, EVT.STATE_ID_SCR_ADD_WALLET)
            }
        }

        QButtonLargeTail {
            id: importwallet
            width: 209
            height: 48
            anchors {
                left: parent.left
                leftMargin: 335
                bottom: parent.bottom
                bottomMargin: 32
            }
            label: "Import Wallet"
            type: eSECONDARY
            optionVisible: popimportwallet.visible
            onButtonClicked: {
                popimportwallet.visible = true
            }
        }

        QTextButton {
            width: 200
            height: 48
            label.text: "NEXT: Signer Setup"
            label.font.pixelSize: 16
            type: eTypeA
            enabled: (signerName.textOutput !== "")
            anchors {
                right: parent.right
                rightMargin: 40
                bottom: parent.bottom
                bottomMargin: 32
            }
            onButtonClicked: {
                AppModel.newWalletInfo.warningMessage.type = EWARNING.NONE_MSG
                var newSignerObj = { "walletNameInputted"    : signerName.textOutput,
                                     "walletDescription"     : signerDescription.textOutput ,
                                     "walletEscrow"          : escrowWallet.focus ,
                                     "addressType"           : addressTypeSelection.typeSeleted };
                QMLHandle.sendEvent(EVT.EVT_ADD_WALLET_SIGNER_CONFIGURATION_REQUEST, newSignerObj)
            }
        }

        Item {
            id: popimportwallet
            visible: false
            anchors.fill: parent
            MouseArea {
                anchors.fill: parent
                onClicked: { popimportwallet.visible = false; }
            }

            Rectangle {
                id: pop
                width: 264
                height: importmenu.height + 32
                anchors {
                    right: parent.right
                    rightMargin: 264
                    bottom: parent.bottom
                    bottomMargin: 48
                }
                radius: 4
                color: "#FFFFFF"
                visible: false
            }

            DropShadow {
                anchors.fill: pop
                verticalOffset: 3
                cached: true
                radius: 16
                samples: 24
                color: Qt.rgba(0, 0, 0, 0.14)
                source: pop
                Column {
                    id: importmenu
                    width: pop.width
                    anchors.centerIn: parent

                    Rectangle {
                        width: parent.width
                        height: 32
                        anchors.horizontalCenter: parent.horizontalCenter
                        color: importwalletdescriptor.containsMouse ? "#C9DEF1" : "transparent"
                        Image {
                            id: ico3
                            width: 24
                            height: 24
                            anchors.left: parent.left
                            anchors.leftMargin: 16
                            anchors.verticalCenter: parent.verticalCenter
                            source: "qrc:/Images/Images/import.png"
                        }
                        QText {
                            anchors.verticalCenter: ico3.verticalCenter
                            anchors.left: ico3.right
                            anchors.leftMargin: 8
                            text: "Import Via Descriptors"
                            color: Qt.rgba(0, 0, 0, 0.87)
                            font.family: "Lato"
                            font.pixelSize: 16
                        }
                        MouseArea {
                            id: importwalletdescriptor
                            hoverEnabled: true
                            cursorShape: Qt.PointingHandCursor
                            anchors.fill: parent
                            onClicked: {
                                popimportwallet.visible = false
                                fileDialog.iType = fileDialog.eWALLET_DESCRIPTOR
                                fileDialog.open()
                            }
                        }
                    }

                    Rectangle {
                        width: parent.width
                        height: 32
                        anchors.horizontalCenter: parent.horizontalCenter
                        color: importwalletdbmouse.containsMouse ? "#C9DEF1" : "transparent"
                        QImage {
                            id: ico1
                            width: 24
                            height: 24
                            anchors.left: parent.left
                            anchors.leftMargin: 16
                            anchors.verticalCenter: parent.verticalCenter
                            source: "qrc:/Images/Images/download.png"
                        }
                        QText {
                            anchors.verticalCenter: ico1.verticalCenter
                            anchors.left: ico1.right
                            anchors.leftMargin: 8
                            text: "Import Wallet Database"
                            color: Qt.rgba(0, 0, 0, 0.87)
                            font.family: "Lato"
                            font.pixelSize: 16
                        }
                        MouseArea {
                            id: importwalletdbmouse
                            hoverEnabled: true
                            cursorShape: Qt.PointingHandCursor
                            anchors.fill: parent
                            onClicked: {
                                AppModel.newWalletInfo.warningMessage.type = EWARNING.NONE_MSG
                                popimportwallet.visible = false
                                fileDialog.iType = fileDialog.eWALLET_DB
                                fileDialog.open()
                            }
                        }
                    }

                    Rectangle {
                        width: parent.width
                        height: 32
                        anchors.horizontalCenter: parent.horizontalCenter
                        color: importwalletQRmouse.containsMouse ? "#C9DEF1" : "transparent"
                        Image {
                            id: icoqrimport
                            width: 24
                            height: 24
                            anchors.left: parent.left
                            anchors.leftMargin: 16
                            anchors.verticalCenter: parent.verticalCenter
                            source: "qrc:/Images/Images/QrIco.png"
                        }
                        QText {
                            anchors.verticalCenter: icoqrimport.verticalCenter
                            anchors.left: icoqrimport.right
                            anchors.leftMargin: 8
                            text: "Import Via QR code"
                            color: Qt.rgba(0, 0, 0, 0.87)
                            font.family: "Lato"
                            font.pixelSize: 16
                        }
                        MouseArea {
                            id: importwalletQRmouse
                            hoverEnabled: true
                            cursorShape: Qt.PointingHandCursor
                            anchors.fill: parent
                            onClicked: {
                                AppModel.newWalletInfo.warningMessage.type = EWARNING.NONE_MSG
                                popimportwallet.visible = false
                                qrscaner.visible = true
                            }
                        }
                    }

                    Rectangle {
                        width: parent.width
                        height: 32
                        anchors.horizontalCenter: parent.horizontalCenter
                        color: importwalletConfiguremouse.containsMouse ? "#C9DEF1" : "transparent"
                        Image {
                            id: icoConfigImport
                            width: 24
                            height: 24
                            anchors.left: parent.left
                            anchors.leftMargin: 16
                            anchors.verticalCenter: parent.verticalCenter
                            source: "qrc:/Images/Images/import.png"
                        }
                        QText {
                            anchors.verticalCenter: icoConfigImport.verticalCenter
                            anchors.left: icoConfigImport.right
                            anchors.leftMargin: 8
                            text: "Import Via configure file"
                            color: Qt.rgba(0, 0, 0, 0.87)
                            font.family: "Lato"
                            font.pixelSize: 16
                        }
                        MouseArea {
                            id: importwalletConfiguremouse
                            hoverEnabled: true
                            cursorShape: Qt.PointingHandCursor
                            anchors.fill: parent
                            onClicked: {
                                AppModel.newWalletInfo.warningMessage.type = EWARNING.NONE_MSG
                                popimportwallet.visible = false
                                fileDialog.iType = fileDialog.eWALLET_CONFIGFILE
                                fileDialog.open()
                            }
                        }
                    }
                }
            }
        }

        Rectangle {
            id: qrscaner
            anchors.fill: parent
            color: Qt.rgba(255, 255, 255, 0.7)
            radius: 8
            visible: false

            Rectangle {
                id: qrmask
                width: 500
                height: 500
                radius: 8
                visible: false
            }

            Rectangle {
                id: qrbg
                width: 500
                height: 500
                color: "#F1FAFE"
                radius: 8
                anchors.centerIn: parent
                visible: false
                Rectangle {
                    height: 4
                    width: parent.width
                    color: "#F6D65D"
                }
            }

            Rectangle {
                width: 500
                height: 500
                anchors.centerIn: parent
            }

            OpacityMask {
                id: opamask
                anchors.fill: qrbg
                source: qrbg
                maskSource: qrmask

                QCloseButton {
                    anchors {
                        right: parent.right
                        rightMargin: 16
                        top: parent.top
                        topMargin: 16
                    }
                    onClicked: qrscaner.visible = false
                }

                QText {
                    anchors.top: parent.top
                    anchors.topMargin: 50
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.family: "Lato"
                    font.pixelSize: 30
                    font.weight: Font.ExtraBold
                    text: "Scanning for QR code"
                }

                Loader {
                    id: qrcameraLoader
                    sourceComponent: qrscaner.visible ? qrcameraComp : null
                    anchors.centerIn: parent
                    anchors.verticalCenterOffset: 30
                }

                Connections {
                    target: qrcameraLoader.item
                    onScanFinished: {
                        if(count > 0){
                            var importData = {  "qrTags"        : result,
                                                "importType"    : fileDialog.eWALLET_QRCODE,
                                                "walletname"    : signerName.textOutput,
                                                "walletdescription"    : signerDescription.textOutput};
                            QMLHandle.sendEvent(EVT.EVT_ADD_WALLET_IMPORT, importData)

                        }
                        qrscaner.visible = false
                    }
                }
            }

            DropShadow {
                anchors.fill: opamask
                horizontalOffset: 3
                verticalOffset: 3
                radius: 8.0
                samples: 17
                color: "#80000000"
                source: opamask
            }
        }

        Component {
            id: qrcameraComp
            QQrScanner {
                width: 300
                height: 300
            }
        }
    }

    FileDialog {
        id: fileDialog
        property int iType: eWALLET_DB
        readonly property int eWALLET_DB: 0
        readonly property int eWALLET_DESCRIPTOR: 1
        readonly property int eWALLET_CONFIGFILE: 2
        readonly property int eWALLET_QRCODE: 3
        fileMode: FileDialog.OpenFile
        onAccepted: {
            var importData = {  "filePath"      : fileDialog.file,
                                "importType"    : fileDialog.iType,
                                "walletname"    : signerName.textOutput,
                                "walletdescription"    : signerDescription.textOutput};
            QMLHandle.sendEvent(EVT.EVT_ADD_WALLET_IMPORT, importData)
        }
    }
}
