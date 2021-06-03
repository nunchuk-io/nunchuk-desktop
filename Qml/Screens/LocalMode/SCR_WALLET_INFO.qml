import QtQuick 2.4
import QtQuick.Controls 2.3
import QtGraphicalEffects 1.12
import Qt.labs.platform 1.1
import HMIEVENTS 1.0
import EWARNING 1.0
import NUNCHUCKTYPE 1.0
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

        QText {
            id: scrtitle
            height: 36
            anchors {
                left: parent.left
                leftMargin: 40
                top: parent.top
                topMargin: 24
            }
            text: "Wallet Info"
            color: "#031F2B"
            font.family: "Montserrat"
            font.weight: Font.ExtraBold
            font.pixelSize: 24
        }

        QText {
            height: 21
            width: 550
            elide: Text.ElideRight
            anchors {
                left: scrtitle.right
                leftMargin: 8
                verticalCenter: scrtitle.verticalCenter
            }
            text: "(" + AppModel.walletInfo.walletName + ")"
            color: "#031F2B"
            font.family: "Montserrat"
            font.weight: Font.DemiBold
            font.pixelSize: 14
        }

        QCloseButton {
            anchors {
                right: parent.right
                rightMargin: 16
                top: parent.top
                topMargin: 16
            }
            onClicked: {
                QMLHandle.sendEvent(EVT.EVT_WALLET_INFO_BACK_REQUEST)
            }
        }

        Column {
            id: notification
            width: parent.width
            spacing: 8
            anchors {
                horizontalCenter: parent.horizontalCenter
                top: parent.top
                topMargin: 81
            }

            QNotification {
                id: commonstatus
                width: 718
                height: 48
                anchors.horizontalCenter: parent.horizontalCenter
                visible: AppModel.walletInfo.warningMessage.type !== EWARNING.NONE_MSG
                label: AppModel.walletInfo.warningMessage.contentDisplay
                currentStatus: AppModel.walletInfo.warningMessage.type
                onCloseRequest: AppModel.walletInfo.warningMessage.type = EWARNING.NONE_MSG
            }

            QNotification {
                id: coldcardwarning
                width: 718
                height: 70
                wrapMode: Text.WordWrap
                labelWidth: 620
                anchors.horizontalCenter: parent.horizontalCenter
                visible: (QMLHandle.onsRequester() === EVT.STATE_ID_SCR_ADD_WALLET_CONFIRMATION) ? listSignerAssigned.isContainColdCard && (AppModel.walletInfo.walletN > 1): false
                label: "Coldcard requires a multisig configuration file before it can sign transactions. Please choose \"Export\" > \"Export To Coldcard\", then copy the resulting file over to Coldcard."
                currentStatus: EWARNING.WARNING_MSG
                onCloseRequest: coldcardwarning.visible = false
            }
        }

        Flickable {
            width: 344
            height: exportwallet.y - y - 10
            flickableDirection: Flickable.VerticalFlick
            clip: true
            interactive: height < contentHeight
            contentHeight: contentWallets.height
            ScrollBar.vertical: ScrollBar { active: true }
            anchors {
                left: parent.left
                leftMargin: 40
                top: notification.bottom
                topMargin: 25
            }

            Column {
                id: contentWallets
                spacing: 8

                QTextInputBox {
                    id: wlname
                    width: 344
                    heightMin: 54
                    mode: ePREVIEW_MODE
                    color: "Transparent"
                    placeholder.text: "WALLET NAME"
                    textOutput: AppModel.walletInfo.walletName
                    onTypingFinished: {
                        if(currentText !== AppModel.walletInfo.walletName && currentText!== ""){
                            QMLHandle.sendEvent(EVT.EVT_WALLET_INFO_EDIT_NAME, currentText)
                        }
                    }
                }

                QTextInputBox {
                    id: wldescription
                    width: 344
                    heightMin: 54
                    mode: ePREVIEW_MODE
                    placeholder.text: "WALLET DESCRIPTION"
                    color: textOutput === "" ? Qt.rgba(201, 222, 241, 0.5) : "Transparent"
                    textOutput: AppModel.walletInfo.walletDescription
                    isEditing: true
                    onTypingFinished: {
                        if(currentText !== AppModel.walletInfo.walletDescription){
                            QMLHandle.sendEvent(EVT.EVT_WALLET_INFO_EDIT_DESCRIPTION, currentText)
                        }
                    }
                }

                QTextInputBox {
                    id: wltype
                    width: 344
                    heightMin: 54
                    mode: eREADONLY_MODE
                    color: "Transparent"
                    placeholder.text: "WALLET TYPE"
                    textOutput: AppModel.walletInfo.walletEscrow ? "One-Time Escrow Wallet" : "Standard Wallet"
                }

                QTextInputBox {
                    id: addresstype
                    readonly property var addressType_Value: ["Random", "Legacy", "Nested Segwit", "Native Segwit"]
                    width: 344
                    heightMin: 54
                    mode: eREADONLY_MODE
                    color: "Transparent"
                    placeholder.text: "ADDRESS TYPE"
                    textOutput: addresstype.addressType_Value[AppModel.walletInfo.walletAddressType]
                }

                QTextInputBox {
                    id: createdate
                    width: 344
                    heightMin: 54
                    mode: eREADONLY_MODE
                    color: "Transparent"
                    placeholder.text: "CREATED"
                    textOutput: AppModel.walletInfo.walletCreateDate
                }
            }
        }

        Row {
            id: rightTit
            spacing: 8
            anchors {
                left: parent.left
                leftMargin: 416
                top: notification.bottom
                topMargin: 25
            }
            QText {
                text: "Signers:"
                font.family: "Montserrat"
                font.weight: Font.DemiBold
                color: "#031F2B"
                font.pixelSize: 14
                verticalAlignment: Text.AlignBottom
            }

            QText {
                text: AppModel.walletInfo.walletM
                font.family: "Lato"
                font.weight: Font.Bold
                color: "#031F2B"
                font.pixelSize: 16
                verticalAlignment: Text.AlignBottom
            }

            Rectangle {
                width: 1
                height: 16
                color: "#839096"
            }

            QText {
                text: AppModel.walletInfo.walletN
                font.family: "Lato"
                font.weight: Font.Bold
                color: "#839096"
                font.pixelSize: 16
                verticalAlignment: Text.AlignBottom
            }
        }

        QListView {
            id: listSignerAssigned
            property bool isContainColdCard: false
            width: 355
            height: 283
            model: AppModel.walletInfo.walletSingleSignerAssigned
            spacing: 8
            clip: true
            anchors {
                right: parent.right
                rightMargin: 40
                top: rightTit.bottom
                topMargin: 25
            }
            interactive: count > 3
            ScrollBar.vertical: ScrollBar { active: true }
            delegate: Item {
                width: 344
                height: 89
                Component.onCompleted: { if(single_signer_isColdcard) listSignerAssigned.isContainColdCard = true }
                Rectangle {
                    id: rect
                    anchors.horizontalCenter: parent.horizontalCenter
                    width: 340
                    height: 86
                    border.color: single_signer_isColdcard ? "#F6D65D" : "transparent"
                    color: itemMouse.containsMouse ? Qt.rgba(255, 255, 255, 0.7):  Qt.rgba(255, 255, 255)
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
                    width: 4
                    height: 72
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: 16
                    color: itemMouse.containsMouse ? "#F6D65D" : "#C9DEF1"
                }

                Column {
                    width: 280
                    height: 69
                    anchors {
                        left: parent.left
                        leftMargin: 28
                        verticalCenter: parent.verticalCenter
                    }

                    Item {
                        width: parent.width
                        height: 21
                        QText {
                            id: signername
                            width: parent.width - 63
                            height: 21
                            text: singleSigner_name
                            font.pixelSize: 14
                            font.weight: Font.DemiBold
                            font.family: "Montserrat"
                            color: "#031F2B"
                            anchors.verticalCenter: parent.verticalCenter
                            elide: Text.ElideRight
                        }

                        Rectangle {
                            width: single_signer_isColdcard ? 79 : 63
                            height: 21
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.right: parent.right
                            color: single_signer_isColdcard ? "#F6D65D" : "#C9DEF1"
                            visible: single_signer_isRemote || single_signer_isColdcard
                            radius: 4
                            QText {
                                text: single_signer_isColdcard ? "COLDCARD" : "AIR-GAPPED"
                                font.family: "Lato"
                                font.weight: Font.Bold
                                font.pixelSize: 10
                                anchors.centerIn: parent
                                color: "#031F2B"
                            }
                        }
                    }

                    QText {
                        id: signerXFP
                        width: parent.width
                        height: 16
                        text: "XFP: " + singleSigner_masterFingerPrint
                        font.capitalization: Font.AllUppercase
                        font.family: "Lato"
                        font.pixelSize: 12
                        color: "#031F2B"
                    }

                    QText {
                        id: derivation
                        width: parent.width
                        height: 16
                        text: "BIP32 Path: " + singleSigner_derivationPath
                        font.family: "Lato"
                        font.pixelSize: 12
                        color: "#839096"
                    }

                    QText {
                        id: healthCheckDate
                        width: parent.width
                        height: 16
                        text: "Last Heath Check: " + singleSigner_lastHealthCheck
                        font.family: "Lato"
                        font.pixelSize: 12
                        color: "#839096"
                    }
                }


                MouseArea {
                    id: itemMouse
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked: {
                        QMLHandle.sendEvent(EVT.EVT_WALLET_INFO_SIGNER_INFO_REQUEST, index)
                    }
                }
            }
        }

        QButtonLargeTail {
            id: exportwallet
            width: 198
            height: 48
            anchors {
                right: parent.right
                rightMargin: 36
                bottom: parent.bottom
                bottomMargin: 59
            }
            label: "Export"
            type: ePRIMARY
            optionVisible: popExportWallet.visible
            onButtonClicked: { popExportWallet.visible = true }
        }

        QButtonLargeTail {
            id: otheraction
            width: 200
            height: 48
            anchors {
                right: parent.right
                rightMargin: 250
                bottom: parent.bottom
                bottomMargin: 59
            }
            label: "Other Actions"
            type: eSECONDARY
            optionVisible: popOtherActions.visible
            onButtonClicked: { popOtherActions.visible = true }
        }

        QText {
            text: "*Export data do not include seeds or private keys."
            anchors {
                right: parent.right
                rightMargin: 40
                bottom: parent.bottom
                bottomMargin: 40
            }
            font.family: "Lato"
            font.pixelSize: 12
            color: "#323E4A"
        }

        Item {
            id: popOtherActions
            visible: false
            anchors.fill: parent
            MouseArea {
                anchors.fill: parent
                onClicked: { popOtherActions.visible = false; }
            }

            Rectangle {
                id: pop
                width: 227
                height: 112
                anchors {
                    right: parent.right
                    rightMargin: 274
                    bottom: parent.bottom
                    bottomMargin: 83
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
                    width: pop.width
                    height: 96
                    anchors.centerIn: parent
                    Rectangle {
                        width: parent.width
                        height: 32
                        anchors.horizontalCenter: parent.horizontalCenter
                        color: mouseunspentoutput.containsMouse ? "#C9DEF1" : "transparent"
                        Image {
                            id: ico1
                            width: 24
                            height: 24
                            anchors.left: parent.left
                            anchors.leftMargin: 16
                            anchors.verticalCenter: parent.verticalCenter
                            source: "qrc:/Images/Images/settings.png"
                        }
                        QText {
                            anchors.verticalCenter: ico1.verticalCenter
                            anchors.left: ico1.right
                            anchors.leftMargin: 8
                            text: "Unspent Outputs"
                            color: Qt.rgba(0, 0, 0, 0.87)
                            font.family: "Lato"
                            font.pixelSize: 16
                        }
                        MouseArea {
                            id: mouseunspentoutput
                            hoverEnabled: true
                            cursorShape: Qt.PointingHandCursor
                            anchors.fill: parent
                            onClicked: {
                                QMLHandle.sendEvent(EVT.EVT_WALLET_INFO_UTXOS_REQUEST)
                                popOtherActions.visible = false
                            }
                        }
                    }
                    Rectangle {
                        width: parent.width
                        height: 32
                        anchors.horizontalCenter: parent.horizontalCenter
                        color: mousechangeaddress.containsMouse ? "#C9DEF1" : "transparent"
                        QImage {
                            id: ico2
                            width: 24
                            height: 24
                            anchors.left: parent.left
                            anchors.leftMargin: 16
                            anchors.verticalCenter: parent.verticalCenter
                            source: "qrc:/Images/Images/download.png"
                        }
                        QText {
                            anchors.verticalCenter: ico2.verticalCenter
                            anchors.left: ico2.right
                            anchors.leftMargin: 8
                            text: "Change Addresses"
                            color: Qt.rgba(0, 0, 0, 0.87) //#35ABEE
                            font.family: "Lato"
                            font.pixelSize: 16
                        }
                        MouseArea {
                            id: mousechangeaddress
                            hoverEnabled: true
                            cursorShape: Qt.PointingHandCursor
                            anchors.fill: parent
                            onClicked: {
                                QMLHandle.sendEvent(EVT.EVT_WALLET_INFO_CHANGE_ADDRESS_REQUEST)
                                popOtherActions.visible = false
                            }
                        }
                    }
                    Rectangle {
                        width: parent.width
                        height: 32
                        anchors.horizontalCenter: parent.horizontalCenter
                        color: mousedeletewallet.containsMouse ? "#C9DEF1" : "transparent"
                        Image {
                            id: ico3
                            width: 24
                            height: 24
                            anchors.left: parent.left
                            anchors.leftMargin: 16
                            anchors.verticalCenter: parent.verticalCenter
                            source: "qrc:/Images/Images/Delete.png"
                        }
                        QText {
                            anchors.verticalCenter: ico3.verticalCenter
                            anchors.left: ico3.right
                            anchors.leftMargin: 8
                            text: "Delete Wallet"
                            color: Qt.rgba(0, 0, 0, 0.87) //#35ABEE
                            font.family: "Lato"
                            font.pixelSize: 16
                        }
                        MouseArea {
                            id: mousedeletewallet
                            hoverEnabled: true
                            cursorShape: Qt.PointingHandCursor
                            anchors.fill: parent
                            onClicked: {
                                popOtherActions.visible = false
                                modelConfirmDelete.visible = true
                            }
                        }
                    }
                }
            }
        }

        Item {
            id: popExportWallet
            visible: false
            anchors.fill: parent
            MouseArea {
                anchors.fill: parent
                onClicked: { popExportWallet.visible = false; }
            }

            Rectangle {
                id: pop2
                width: 300
                height: menuExport.height + 16
                anchors {
                    right: parent.right
                    rightMargin: 60
                    bottom: parent.bottom
                    bottomMargin: 82
                }
                radius: 4
                color: "#FFFFFF"
                visible: false
            }

            DropShadow {
                anchors.fill: pop2
                verticalOffset: 3
                cached: true
                radius: 16
                samples: 24
                color: Qt.rgba(0, 0, 0, 0.14)
                source: pop2
                Column {
                    id: menuExport
                    width: pop2.width
                    anchors.centerIn: parent

                    // Descriptor
                    Rectangle {
                        width: parent.width
                        height: 32
                        anchors.horizontalCenter: parent.horizontalCenter
                        color: mouseExportDescriptor.containsMouse ? "#C9DEF1" : "transparent"
                        Image {
                            id: ico22
                            width: 24
                            height: 24
                            anchors.left: parent.left
                            anchors.leftMargin: 16
                            anchors.verticalCenter: parent.verticalCenter
                            source: "qrc:/Images/Images/description.png"
                        }
                        QText {
                            anchors.verticalCenter: ico22.verticalCenter
                            anchors.left: ico22.right
                            anchors.leftMargin: 8
                            text: "Export Wallet BSMS File"
                            color: Qt.rgba(0, 0, 0, 0.87)
                            font.family: "Lato"
                            font.pixelSize: 16
                        }
                        MouseArea {
                            id: mouseExportDescriptor
                            hoverEnabled: true
                            cursorShape: Qt.PointingHandCursor
                            anchors.fill: parent
                            onClicked: {
                                popExportWallet.visible = false
                                exportwalletDialog.exportFormat = NUNCHUCKTYPE.DESCRIPTOR
                                exportwalletDialog.currentFile = StandardPaths.writableLocation(StandardPaths.DocumentsLocation) + "/" + AppModel.walletInfo.walletName + "-Descriptors.bsms"
                                exportwalletDialog.open()
                            }
                        }
                    }

                    //DB
                    Rectangle {
                        width: parent.width
                        height: 32
                        anchors.horizontalCenter: parent.horizontalCenter
                        color: mouseExportDB.containsMouse ? "#C9DEF1" : "transparent"
                        Image {
                            id: ico12
                            width: 24
                            height: 24
                            anchors.left: parent.left
                            anchors.leftMargin: 16
                            anchors.verticalCenter: parent.verticalCenter
                            source: "qrc:/Images/Images/backup.png"
                        }
                        QText {
                            anchors.verticalCenter: ico12.verticalCenter
                            anchors.left: ico12.right
                            anchors.leftMargin: 8
                            text: "Export Wallet Database"
                            color: Qt.rgba(0, 0, 0, 0.87) //#35ABEE
                            font.family: "Lato"
                            font.pixelSize: 16
                        }
                        MouseArea {
                            id: mouseExportDB
                            hoverEnabled: true
                            cursorShape: Qt.PointingHandCursor
                            anchors.fill: parent
                            onClicked: {
                                popExportWallet.visible = false
                                exportwalletDialog.exportFormat = NUNCHUCKTYPE.DB
                                exportwalletDialog.currentFile = StandardPaths.writableLocation(StandardPaths.DocumentsLocation) + "/" + AppModel.walletInfo.walletName + "-database.dat"
                                exportwalletDialog.open()
                            }
                        }
                    }

                    //CSV
                    Rectangle {
                        width: parent.width
                        height: 32
                        anchors.horizontalCenter: parent.horizontalCenter
                        color: mouseExportTXsCsv.containsMouse ? "#C9DEF1" : "transparent"
                        Image {
                            id: ico112
                            width: 24
                            height: 24
                            anchors.left: parent.left
                            anchors.leftMargin: 16
                            anchors.verticalCenter: parent.verticalCenter
                            source: "qrc:/Images/Images/backup.png"
                        }
                        QText {
                            anchors.verticalCenter: ico112.verticalCenter
                            anchors.left: ico112.right
                            anchors.leftMargin: 8
                            text: "Export Transaction History (CSV)"
                            color: Qt.rgba(0, 0, 0, 0.87) //#35ABEE
                            font.family: "Lato"
                            font.pixelSize: 16
                        }
                        MouseArea {
                            id: mouseExportTXsCsv
                            hoverEnabled: true
                            cursorShape: Qt.PointingHandCursor
                            anchors.fill: parent
                            onClicked: {
                                popExportWallet.visible = false
                                exportwalletDialog.exportFormat = NUNCHUCKTYPE.TRANSACTION_CSV
                                exportwalletDialog.currentFile = StandardPaths.writableLocation(StandardPaths.DocumentsLocation) + "/" + AppModel.walletInfo.walletName + "-tx-history.csv"
                                exportwalletDialog.open()
                            }
                        }
                    }

                    //CSV
                    Rectangle {
                        width: parent.width
                        height: 32
                        anchors.horizontalCenter: parent.horizontalCenter
                        color: mouseExportUtxoCsv.containsMouse ? "#C9DEF1" : "transparent"
                        Image {
                            id: ico122
                            width: 24
                            height: 24
                            anchors.left: parent.left
                            anchors.leftMargin: 16
                            anchors.verticalCenter: parent.verticalCenter
                            source: "qrc:/Images/Images/backup.png"
                        }
                        QText {
                            anchors.verticalCenter: ico122.verticalCenter
                            anchors.left: ico122.right
                            anchors.leftMargin: 8
                            text: "Export UTXOs (CSV)"
                            color: Qt.rgba(0, 0, 0, 0.87) //#35ABEE
                            font.family: "Lato"
                            font.pixelSize: 16
                        }
                        MouseArea {
                            id: mouseExportUtxoCsv
                            hoverEnabled: true
                            cursorShape: Qt.PointingHandCursor
                            anchors.fill: parent
                            onClicked: {
                                popExportWallet.visible = false
                                exportwalletDialog.exportFormat = NUNCHUCKTYPE.UTXO_CSV
                                exportwalletDialog.currentFile = StandardPaths.writableLocation(StandardPaths.DocumentsLocation) + "/" + AppModel.walletInfo.walletName + "-utxos.csv"
                                exportwalletDialog.open()
                            }
                        }
                    }

                    //Coldcard
                    Rectangle {
                        width: parent.width
                        height: 32
                        anchors.horizontalCenter: parent.horizontalCenter
                        color: mouseExportColdcard.containsMouse ? "#C9DEF1" : "transparent"
                        Image {
                            id: ico32
                            width: 24
                            height: 24
                            anchors.left: parent.left
                            anchors.leftMargin: 16
                            anchors.verticalCenter: parent.verticalCenter
                            source: "qrc:/Images/Images/fileDownload.png"
                        }
                        QText {
                            anchors.verticalCenter: ico32.verticalCenter
                            anchors.left: ico32.right
                            anchors.leftMargin: 8
                            text: "Export To Coldcard"
                            color: Qt.rgba(0, 0, 0, 0.87) //#35ABEE
                            font.family: "Lato"
                            font.pixelSize: 16
                        }
                        MouseArea {
                            id: mouseExportColdcard
                            hoverEnabled: true
                            cursorShape: Qt.PointingHandCursor
                            anchors.fill: parent
                            onClicked: {
                                popExportWallet.visible = false
                                exportwalletDialog.exportFormat = NUNCHUCKTYPE.COLDCARD
                                exportwalletDialog.currentFile = StandardPaths.writableLocation(StandardPaths.DocumentsLocation) + "/" + AppModel.walletInfo.walletName + "-Coldcard-Config.txt"
                                exportwalletDialog.open()
                            }
                        }
                    }

                    //QRcode
                    Rectangle {
                        width: parent.width
                        height: 32
                        anchors.horizontalCenter: parent.horizontalCenter
                        color: mouseExportqr.containsMouse ? "#C9DEF1" : "transparent"
                        Image {
                            id: ico41
                            width: 24
                            height: 24
                            anchors.left: parent.left
                            anchors.leftMargin: 16
                            anchors.verticalCenter: parent.verticalCenter
                            source: "qrc:/Images/Images/exportqr.png"
                        }
                        QText {
                            anchors.verticalCenter: ico41.verticalCenter
                            anchors.left: ico41.right
                            anchors.leftMargin: 8
                            text: "Export as QR Code"
                            color: Qt.rgba(0, 0, 0, 0.87) //#35ABEE
                            font.family: "Lato"
                            font.pixelSize: 16
                        }
                        MouseArea {
                            id: mouseExportqr
                            hoverEnabled: true
                            cursorShape: Qt.PointingHandCursor
                            anchors.fill: parent
                            onClicked: {
                                popExportWallet.visible = false
//                                exportwalletDialog.exportFormat = NUNCHUCKTYPE.COBO
//                                exportwalletDialog.currentFile = StandardPaths.writableLocation(StandardPaths.DocumentsLocation) + "/" + AppModel.walletInfo.walletName + "-Cobo-wallet.pspt"
//                                exportwalletDialog.open()

                                // Without save pspt file
                                QMLHandle.sendEvent(EVT.EVT_WALLET_INFO_EXPORT_QRCODE, "")
                                qrcodeExportResult.visible = true
                            }
                        }
                    }
                }
            }
        }

        QQrExportResult {
            id: qrcodeExportResult
            anchors.fill: parent
            model: AppModel.qrExported
            visible: false
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
                    text: "Confirm Deleting Wallet"
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
                    text: "Once you delete the wallet, all your data (and metadata such as transaction memos) will be gone. We recommend backing up the wallet descriptors or database, if you want to recover the wallet later."
                    font.family: "Lato"
                    font.pixelSize: 14
                }

                QTextInputBox {
                    id: inputDelete
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.top: parent.top
                    anchors.topMargin: 170
                    maximumLength: 10
                    placeholder.text: "Type DELETE to confirm deleiton."
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
                                QMLHandle.sendEvent(EVT.EVT_WALLET_INFO_REMOVE, AppModel.walletInfo.walletId)
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
    }

    FileDialog {
        id: exportwalletDialog
        property int exportFormat: -1
        fileMode: FileDialog.SaveFile
        onAccepted: {
            if(exportFormat === NUNCHUCKTYPE.DB){
                QMLHandle.sendEvent(EVT.EVT_WALLET_INFO_EXPORT_DB, exportwalletDialog.currentFile)
            }
            else if(exportFormat === NUNCHUCKTYPE.DESCRIPTOR){
                QMLHandle.sendEvent(EVT.EVT_WALLET_INFO_EXPORT_DESCRIPTOR, exportwalletDialog.currentFile)
            }
            else if(exportFormat === NUNCHUCKTYPE.COLDCARD){
                QMLHandle.sendEvent(EVT.EVT_WALLET_INFO_EXPORT_COLDCARD, exportwalletDialog.currentFile)
            }
            else if(exportFormat === NUNCHUCKTYPE.COBO){
                QMLHandle.sendEvent(EVT.EVT_WALLET_INFO_EXPORT_QRCODE, exportwalletDialog.currentFile)
                qrcodeExportResult.visible = true
            }
            else if(exportFormat === NUNCHUCKTYPE.TRANSACTION_CSV){
                var csvTxObj = { "csv_type"     : 1,
                                 "file_path"    : exportwalletDialog.currentFile};
                QMLHandle.sendEvent(EVT.EVT_WALLET_INFO_EXPORT_CSV, csvTxObj)
            }
            else if(exportFormat === NUNCHUCKTYPE.UTXO_CSV){
                var csvUtxoObj = { "csv_type"     : -1,
                                   "file_path"    : exportwalletDialog.currentFile};
                QMLHandle.sendEvent(EVT.EVT_WALLET_INFO_EXPORT_CSV, csvUtxoObj)
            }
            else{}
        }
    }
}
