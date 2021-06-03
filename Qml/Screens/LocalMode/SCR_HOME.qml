import QtQuick 2.12
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0
import HMIEVENTS 1.0
import NUNCHUCKTYPE 1.0
import QRCodeItem 1.0
import "../../Components/customizes"
import "../../Components/origins"

QScreen {
    id: homeroot
    Component {
        id: step1
        QHomeInitialStep1 {
            anchors.fill: parent
        }
    }
    Component {
        id: step2
        QHomeInitialStep2 {
            anchors.fill: parent
        }
    }
    Component {
        id: step3
        Item {
            anchors.fill: parent
            Image {
                id: area_wldetail
                width: parent.width - 64
                height: parent.height * 0.42
                source: "qrc:/Images/Images/Rectangle 2944.png"
                anchors {
                    horizontalCenter: parent.horizontalCenter
                    top: parent.top
                    topMargin: 32
                }
                visible: false
            }

            Rectangle {
                id: mask
                anchors.fill: area_wldetail
                radius: 8
                visible: false
            }

            OpacityMask {
                id: realBalanceCard
                anchors.fill: area_wldetail
                source: area_wldetail
                maskSource: mask
                Item {
                    height: parent.height
                    width: parent.width * 0.6

                    QText {
                        id: displayWalletname
                        width: 460
                        height: 36
                        text: AppModel.walletInfo.walletName
                        font.weight: Font.DemiBold
                        font.family: "Montserrat"
                        font.pixelSize: 24
                        color: "#F1FAFE"
                        elide: Text.ElideRight
                        anchors {
                            left: buttongHandles.left
                            top: parent.top
                            topMargin: 37
                        }
                    }

                    QText {
                        id: displayDescription
                        text: AppModel.walletInfo.walletDescription
                        width: displayWalletname.width
                        elide: Text.ElideRight
                        height: displayDescription.text !== "" ? 18 : 0
                        font.pixelSize: 12
                        font.family: "Lato"
                        color: "#F1FAFE"
                        anchors {
                            left: displayWalletname.left
                            top: displayWalletname.bottom
                        }
                    }

                    QButtonIcon {
                        width: 150
                        height: 30
                        label: "WALLET DETAILS"
                        fontPixelSize: 10
                        type: eFOURD
                        iconSize: 16
                        icons: ["wallet_F6D65D.png","wallet_9CAEB8.png","wallet_F1FAFE.png","wallet_031F2B.png"]
                        enabled: walletmanagerlst.count > 0
                        anchors {
                            left: displayWalletname.left
                            top: displayWalletname.bottom
                            topMargin: 16 + displayDescription.height
                        }
                        onButtonClicked: {
                            QMLHandle.sendEvent(EVT.EVT_HOME_WALLET_INFO_REQUEST)
                        }
                    }

                    QText {
                        id: wlBalance
                        width: parent.width * 0.75
                        color: "#F1FAFE"
                        font.pixelSize: 40*width/550
                        font.family: "Lato"
                        font.weight: Font.Bold
                        horizontalAlignment: Text.AlignRight
                        text: AppModel.walletInfo.walletBalance
                        anchors {
                            right: wlBtc.left
                            rightMargin: 5
                            top: parent.top
                            topMargin: 162
                        }
                        clip: true
                    }

                    QText {
                        id: wlBtc
                        font.pixelSize: 24
                        font.family: "Lato"
                        font.weight: Font.Bold
                        color: "#F1FAFE";
                        text: (AppSetting.unit === 1) ? "sat" : "BTC"
                        anchors {
                            right: buttongHandles.right
                            verticalCenter: wlBalance.verticalCenter
                            verticalCenterOffset: wlBalance.height/2 - height/2 - 2
                        }
                    }

                    Row {
                        id: buttongHandles
                        spacing: parent.width * 0.3
                        anchors {
                            horizontalCenter: parent.horizontalCenter
                            top: parent.top
                            topMargin: 294
                        }
                        QTextButton {
                            label.text: "Send"
                            type: eTypeA
                            enabled: walletmanagerlst.count > 0
                            onButtonClicked: { QMLHandle.sendEvent(EVT.EVT_HOME_SEND_REQUEST) }
                        }

                        QTextButton {
                            label.text: "Receive"
                            type: eTypeA
                            enabled: walletmanagerlst.count > 0
                            onButtonClicked: { QMLHandle.sendEvent(EVT.EVT_HOME_RECEIVE_REQUEST) }
                        }
                    }
                }

                Rectangle {
                    id: area_quickrecevied
                    width: parent.width * 0.4
                    height: parent.height
                    anchors {
                        right: parent.right
                    }
                    gradient: Gradient {
                        orientation: Gradient.Horizontal
                        GradientStop { position: 0.0; color: Qt.rgba(247, 247, 249, 0.08) }
                        GradientStop { position: 1.0067; color: Qt.rgba(247, 247, 249, 0) }
                    }

                    Column {
                        width: parent.width
                        anchors.centerIn: parent
                        spacing: 16

                        Row {
                            spacing: 16
                            anchors.horizontalCenter: parent.horizontalCenter
                            Rectangle {
                                width: 40
                                height: 1
                                color: "#C9DEF1"
                                anchors.verticalCenter: quickRecv.verticalCenter
                            }

                            QText {
                                id: quickRecv
                                font.weight: Font.DemiBold
                                font.pixelSize: 12
                                color: "#C9DEF1";
                                text: "QUICK RECEIVE"
                            }

                            Rectangle {
                                width: 40
                                height: 1
                                color: "#C9DEF1"
                                anchors.verticalCenter: quickRecv.verticalCenter
                            }

                        }

                        QRCodeItem {
                            id: qrCode
                            width: parent.height*0.3 + 50
                            height: width
                            anchors.horizontalCenter: parent.horizontalCenter
                            borderWitdh: 6
                            textInput: AppModel.walletInfo.walletAddress
                        }

                        Rectangle {
                            width: parent.width * 0.7
                            height: 40
                            color: Qt.rgba(255, 255, 255, 0.2)
                            anchors.horizontalCenter: parent.horizontalCenter
                            radius: 4

                            QText {
                                anchors.fill: parent
                                anchors.leftMargin: 5
                                anchors.topMargin: 5
                                anchors.rightMargin: 5
                                anchors.bottomMargin: 5
                                horizontalAlignment: Text.AlignHCenter
                                wrapMode: Text.WrapAnywhere
                                font.pixelSize: 12
                                color: "#F1FAFE";
                                text: qrCode.textInput
                                anchors.centerIn: parent
                            }
                        }

                        Column {
                            spacing: 16
                            anchors.horizontalCenter: parent.horizontalCenter
                            QButtonIcon {
                                width: 150
                                height: 30
                                label: "COPY ADDRESS"
                                icons: ["Copy_F1FAFE.png", "Copy_9CAEB8.png", "Copy_031F2B.png","Copy_031F2B.png"]
                                anchors.horizontalCenter: parent.horizontalCenter
                                fontPixelSize: 10
                                iconSize: 16
                                type: eTHIRD
                                enabled: walletmanagerlst.count > 0
                                onButtonClicked: {
                                    QMLHandle.sendEvent(EVT.EVT_HOME_WALLET_COPY_ADDRESS, qrCode.textInput)
                                }
                            }

                            QButtonIcon {
                                width: 150
                                height: 30
                                label: "VERIFY ON DEVICE"
                                icons: ["visibility_F1FAFE.png","visibility_9CAEB8.png","visibility_031F2B.png","visibility_031F2B.png"]
                                fontPixelSize: 10
                                iconSize: 16
                                type: eTHIRD
                                anchors.horizontalCenter: parent.horizontalCenter
                                onButtonClicked: {QMLHandle.sendEvent(EVT.EVT_HOME_DISPLAY_ADDRESS, qrCode.textInput) }
                                enabled: AppModel.walletInfo.containsHWSigner
                                QTooltip {
                                    anchors.left: parent.right
                                    anchors.leftMargin: 8
                                    anchors.verticalCenter: parent.verticalCenter
                                    toolTip: "Check the correctness of this address on your device. Make sure your device is connected and unlocked."
                                    rightOfParent: true
                                }
                            }
                        }
                    }
                }

            }

            DropShadow {
                anchors.fill: realBalanceCard
                horizontalOffset: 3
                verticalOffset: 3
                radius: 8.0
                samples: 17
                color: "#80000000"
                source: realBalanceCard
            }

            Item {
                width: parent.width - 64
                height: parent.height * 0.45
                anchors {
                    horizontalCenter: parent.horizontalCenter
                    bottom: parent.bottom
                    bottomMargin: 50
                }

                Item {
                    width: parent.width*0.25
                    height: parent.height
                    anchors.left: parent.left

                    QText {
                        id: asigne_signer_lbl
                        height: 24
                        font.weight: Font.DemiBold
                        font.pixelSize: 14
                        color: "#323E4A";
                        font.family: "Montserrat"
                        text: "Member Signers (" + AppModel.walletInfo.walletM + " of " + AppModel.walletInfo.walletN + ")"
                    }

                    QListView {
                        id: assigned_signer_lst
                        width: parent.width
                        height: 336
                        spacing: 16
                        model: AppModel.walletInfo.walletSingleSignerAssigned
                        anchors {
                            left: parent.left
                            top: asigne_signer_lbl.bottom
                            topMargin: 24
                        }
                        clip: true
                        delegate: QAssignedSignerDelegate {
                            width: assigned_signer_lst.width
                            height: 68
                            signerName: singleSigner_name
                            lastHealthCheck: singleSigner_lastHealthCheck
                            isRemote: single_signer_isRemote
                            signerXFP: singleSigner_masterFingerPrint
                        }
                        ScrollBar.vertical: ScrollBar { active: true }
                    }
                }

                Item {
                    width: parent.width*0.75
                    height: parent.height
                    anchors.right: parent.right
                    QText {
                        id: trans_lbl
                        height: 24
                        font.weight: Font.DemiBold
                        font.pixelSize: 14
                        color: "#323E4A";
                        font.family: "Montserrat"
                        text: "Recent Transactions (Total: " + transaction_lst.count + ")"
                    }

                    QListView {
                        id: transaction_lst
                        width: parent.width
                        height: 327
                        model: AppModel.transactionHistoryShort
                        anchors {
                            left: parent.left
                            top: trans_lbl.bottom
                            topMargin: 24
                        }
                        clip: true
                        delegate: QTransactionDelegate {
                            width: transaction_lst.width
                            height: 40
                            transactionstatus: transaction_status
                            transactionMemo: transaction_memo
                            transactionAmount: (transaction_isReceiveTx ? "" : "- ") + (transaction_isReceiveTx ? transaction_subtotal : transaction_total)
                            confirmation: Math.max(0, (AppModel.chainTip - transaction_height)+1)
                            transactionDate: transaction_blocktime

                            statusWidth: transaction_lst.width*0.25
                            timeWidth: transaction_lst.width*0.15
                            memoWidth: transaction_lst.width*0.30
                            amountWidth: transaction_lst.width*0.30

                            onButtonClicked: {
                                QMLHandle.sendEvent(EVT.EVT_HOME_TRANSACTION_INFO_REQUEST, transaction_txid)
                            }
                        }
                        ScrollBar.vertical: ScrollBar { active: true }
                    }

                    QButtonTextLink {
                        width: 203
                        height: 24
                        label: "VIEW ALL TRANSACTIONS"
                        direction: eRIGHT
                        enabled: transaction_lst.count > 0
                        anchors {
                            right: parent.right
                            top: transaction_lst.bottom
                            topMargin: 20
                        }
                        onButtonClicked: {
                            QMLHandle.sendEvent(EVT.EVT_HOME_TRANSACTION_HISTORY_REQUEST)
                        }
                    }
                }
            }
        }
    }

    Row {
        Item {
            id: pannel_left
            width: 304
            height: Math.max(910,QAPP_DEVICE_HEIGHT/SCREEN_SCALE)
            transformOrigin: Item.Center
            Rectangle {
                id: bgColor
                anchors.fill: parent
                color: "#031F2B"
                visible: false
            }
            DropShadow {
                id: dropshodow1
                anchors.fill: bgColor
                radius: 24
                samples: 16
                color: Qt.rgba(0, 0, 0, 0.14)
                source: bgColor
            }

            Row {
                spacing: 8
                width: 142
                height: 24
                anchors {
                    left: parent.left
                    leftMargin: 148
                    top: parent.top
                    topMargin: 20
                }
                QConnection {
                    width: 100
                    height: 24
                    label.text: (AppSetting.primaryServer === 0) ? "MAINNET" : "TESTNET"
                    anchors.verticalCenter: parent.verticalCenter
                }
                QImage {
                    width: 24
                    height: 24
                    source: "qrc:/Images/Images/appsettings.png"
                    anchors.verticalCenter: parent.verticalCenter
                    scale: settingmouse.containsMouse ? 1.1 : 1
                    transformOrigin: Item.Center
                    MouseArea {
                        id: settingmouse
                        anchors.fill: parent
                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor
                        onClicked:  QMLHandle.sendEvent(EVT.EVT_APP_SETTING_REQUEST)
                    }
                }
            }

            QImage {
                id: logo
                width: 32
                height: 32
                anchors {
                    left: parent.left
                    leftMargin: 16
                    top: parent.top
                    topMargin: 16
                }
                source: "qrc:/Images/Images/Logo_Palcehoder.png"
            }

            QText {
                width: 126
                height: 12
                anchors {
                    left: parent.left
                    leftMargin: 20
                    top: parent.top
                    topMargin: 90
                }
                text: "WALLET MANAGER"
                color: "#F6D65D"
                font.weight: Font.Bold
                font.pixelSize: 12
                font.letterSpacing: 0.05
                lineHeight: 12
            }

            QWalletManagerDelegate {
                width: 304
                height: 64
                enabled: false
                anchors {
                    left: parent.left
                    top: parent.top
                    topMargin: 118
                }
                visible: walletmanagerlst.count == 0
                isEscrow: false
                walletName : "Sample Wallet"
                walletBalance: "0"
                walletM: "0"
                walletN: "0"
                Rectangle {
                    anchors.fill: parent
                    color: "#031F2B"
                    opacity: 0.5
                }
            }

            QListView {
                id: walletmanagerlst
                width: 304
                height: 320
                model: AppModel.walletList
                clip: true
                currentIndex: AppModel.walletListCurrentIndex
                anchors {
                    left: parent.left
                    top: parent.top
                    topMargin: 118
                }
                delegate: QWalletManagerDelegate {
                    width: 304
                    height: 64
                    isCurrentIndex: index == walletmanagerlst.currentIndex
                    isEscrow: wallet_Escrow
                    walletName : wallet_name
                    walletBalance: wallet_Balance
                    walletM: wallet_M
                    walletN: wallet_N

                    onButtonClicked: {
                        if(index != walletmanagerlst.currentIndex){
                            QMLHandle.sendEvent(EVT.EVT_HOME_WALLET_SELECTED, index)
                        }
                    }
                }
                ScrollBar.vertical: ScrollBar { active: true }
            }

            QButtonMedium {
                width: 185
                height: 32
                label: "Add New Wallet"
                type: eOUTLINE_DARK
                //            visible: walletmanagerlst.count > 0
                enabled: visible
                anchors {
                    left: parent.left
                    leftMargin: 60
                    top: parent.top
                    topMargin: 462
                }
                onButtonClicked : {
                    QMLHandle.sendEvent(EVT.EVT_HOME_ADD_WALLET_REQUEST)
                }
            }

            QText {
                width: 126
                height: 12
                anchors {
                    left: parent.left
                    leftMargin: 20
                    top: parent.top
                    topMargin: 550
                }
                text: "SIGNER MANAGER"
                color: "#F6D65D"
                font.weight: Font.Bold
                font.pixelSize: 12
                font.letterSpacing: 0.05
                lineHeight: 12
            }

            QMasterSignerDelegate {
                width: 304
                height: 54
                enabled: false
                anchors {
                    left: parent.left
                    top: parent.top
                    topMargin: 574
                }
                visible: !flickerSignerList.signerReady
                signername : "Sample Signer"
                signerXFP: "XFP: DEADBEEF"
                Rectangle {
                    anchors.fill: parent
                    color: "#031F2B"
                    opacity: 0.5
                }
            }

            Flickable {
                id: flickerSignerList
                width: 304
                height: 240
                property bool signerReady: (mastersignerlist.count > 0) || (remoteSignerlist.count > 0)
                visible: signerReady
                clip: true
                flickableDirection: Flickable.VerticalFlick
                interactive: true;//contentHeight > contentDisplay.height
                contentHeight: contentDisplay.height
                ScrollBar.vertical: ScrollBar { active: true }
                anchors {
                    left: parent.left
                    top: parent.top
                    topMargin: 574
                }
                Column {
                    id: contentDisplay
                    QListView {
                        id: mastersignerlist
                        width: 304
                        height: 48*mastersignerlist.count
                        model: AppModel.masterSignerList
                        interactive: false
                        delegate: QMasterSignerDelegate {
                            width: 304
                            height: 48
                            signername : master_signer_name
                            signerXFP: "XFP: " + master_signer_fingerPrint
                            devicetype: master_signer_deviceType

                            Rectangle {
                                width: 63
                                height: 21
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.right: parent.right
                                anchors.rightMargin: 10
                                color: "#C9DEF1"
                                visible: master_signer_isSoftware
                                radius: 4
                                QText {
                                    text: "SOFTWARE"
                                    font.family: "Lato"
                                    font.weight: Font.Bold
                                    font.pixelSize: 10
                                    anchors.centerIn: parent
                                    color: "#031F2B"
                                }
                            }
                            onButtonClicked: {
                                QMLHandle.sendEvent(EVT.EVT_HOME_MASTER_SIGNER_INFO_REQUEST, index)
                            }
                        }
                    }
                    QListView {
                        id: remoteSignerlist
                        width: 304
                        height: 48*remoteSignerlist.count
                        model: AppModel.remoteSignerList
                        interactive: false
                        delegate: QMasterSignerDelegate {
                            width: 304
                            height: 48
                            nameWidth: 170
                            signername : singleSigner_name
                            signerXFP: "XFP: " + singleSigner_masterFingerPrint

                            Rectangle {
                                width: 63
                                height: 21
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.right: parent.right
                                anchors.rightMargin: 10
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
                            onButtonClicked: {
                                QMLHandle.sendEvent(EVT.EVT_HOME_REMOTE_SIGNER_INFO_REQUEST, index)
                            }
                        }
                    }
                }
            }

            QButtonMedium {
                width: 185
                height: 32
                label: "Add New Signer"
                type: eOUTLINE_DARK
                enabled: visible
                anchors {
                    left: parent.left
                    leftMargin: 60
                    top: parent.top
                    topMargin: 834
                }
                onButtonClicked: {
                    QMLHandle.sendEvent(EVT.EVT_HOME_ADD_NEW_SIGNER_REQUEST)
                }
            }
        }

        Rectangle {
            width: Math.max(QAPP_DEVICE_WIDTH/SCREEN_SCALE - pannel_left.width, 976)
            height: Math.max(910,QAPP_DEVICE_HEIGHT/SCREEN_SCALE)
            color: "#F1FAFE"
            Loader {
                width: parent.width
                height: Math.max(910,QAPP_DEVICE_HEIGHT/SCREEN_SCALE)
                anchors.centerIn: parent
                sourceComponent: {
                    if(walletmanagerlst.count > 0){ return step3;}
                    else{
                        if(mastersignerlist.count > 0){ return step2;}
                        else{ return step1 ;}
                    }
                }
            }
        }
    }

    Rectangle {
        id: displayAddressBusybox
        anchors.fill: parent
        color: Qt.rgba(255, 255, 255, 0.7)
        radius: 8
        MouseArea {anchors.fill: parent; onClicked: {}}
        visible: false

        Rectangle {
            id: busymask
            width: 500
            height: 250
            radius: 8
            visible: false
        }

        Rectangle {
            id: busyboxbg
            width: 500
            height: 250
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
            height: 250
            radius: 8
            anchors.centerIn: parent
        }

        OpacityMask {
            id: busyboxmask
            anchors.fill: busyboxbg
            source: busyboxbg
            maskSource: busymask

            QCloseButton {
                anchors {
                    right: parent.right
                    rightMargin: 16
                    top: parent.top
                    topMargin: 16
                }
                onClicked: { displayAddressBusybox.visible = false }
            }

            Column {
                spacing: 16
                anchors.centerIn: parent
                QBusyIndicator {
                    width: 70
                    height: 70
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                QText {
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.family: "Lato"
                    font.pixelSize: 14
                    font.weight: Font.Bold
                    text: "Please check the address on your device"
                }

                Rectangle {
                    width: 450
                    height: 60
                    color: Qt.rgba(0, 0, 0, 0.1)
                    anchors.horizontalCenter: parent.horizontalCenter
                    QText {
                        anchors.fill: parent
                        anchors.leftMargin: 5
                        anchors.rightMargin: 5
                        anchors.topMargin: 5
                        anchors.bottomMargin: 5
                        wrapMode: Text.WrapAnywhere
                        horizontalAlignment: Text.AlignHCenter
                        text: AppModel.walletInfo.walletAddress
                        anchors.centerIn: parent
                        font.pixelSize: 18
                        color: "#323E4A"
                    }
                }
            }
        }

        DropShadow {
            anchors.fill: busyboxmask
            horizontalOffset: 3
            verticalOffset: 3
            radius: 8.0
            samples: 17
            color: "#80000000"
            source: busyboxmask
        }
    }

    Connections {
        target: AppModel
        onPreCheckAddressOnDevice: {
            displayAddressBusybox.visible = isOnTop ? valid : false
        }
//            onFinishedDisplayAddress: {
//                displayAddressBusybox.visible = false
//            }
    }
}
