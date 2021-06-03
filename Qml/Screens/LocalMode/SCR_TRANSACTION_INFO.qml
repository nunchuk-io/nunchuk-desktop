import QtQuick 2.4
import QtQuick.Controls 2.3
import QtGraphicalEffects 1.12
import Qt.labs.platform 1.1
import HMIEVENTS 1.0
import EWARNING 1.0
import NUNCHUCKTYPE 1.0
import QRCodeItem 1.0
import "../../Components/customizes"
import "../../Components/origins"
QScreen {
    property int confirmations: Math.max(0, (AppModel.chainTip - AppModel.transactionInfo.height)+1)
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
                if(QMLHandle.onsRequester() === EVT.STATE_ID_SCR_TRANSACTION_HISTORY){
                    QMLHandle.sendEvent(EVT.EVT_TRANSACTION_INFO_BACK_REQUEST)
                }
                else if(QMLHandle.onsRequester() === EVT.STATE_ID_SCR_UTXO_OUTPUT){
                    QMLHandle.sendEvent(EVT.EVT_TRANSACTION_INFO_BACK_UTXO_INFO)
                }
                else{
                    QMLHandle.sendEvent(EVT.EVT_ONS_CLOSE_REQUEST, EVT.STATE_ID_SCR_TRANSACTION_INFO)
                }
            }
        }

        Loader {
            anchors.fill: parent
            sourceComponent: AppModel.transactionInfo.isReceiveTx ? txReceived : txSend
        }

        Component {
            id: txSend
            Item {
                QText {
                    id: sendfund
                    anchors {
                        left: parent.left
                        leftMargin: 40
                        top: parent.top
                        topMargin: 24
                    }
                    text: "Transaction Info"
                    color: "#031F2B"
                    font.weight: Font.ExtraBold
                    font.pixelSize: 24
                }

                Row {
                    id: status
                    height: 24
                    spacing: 8
                    anchors {
                        left: sendfund.right
                        leftMargin: 8
                        verticalCenter: sendfund.verticalCenter
                    }

                    Rectangle {
                        width: 4
                        height: 24
                        radius: 1
                        color: indicatorStatusTop.color
                    }

                    QText {
                        id: indicatorStatusTop
                        anchors.verticalCenter: parent.verticalCenter
                        text:  {
                            if(AppModel.transactionInfo.status === NUNCHUCKTYPE.PENDING_SIGNATURES){ return "Pending Signatures" }
                            else if(AppModel.transactionInfo.status === NUNCHUCKTYPE.READY_TO_BROADCAST){ return "Ready to Broadcast" }
                            else if(AppModel.transactionInfo.status === NUNCHUCKTYPE.NETWORK_REJECTED){ return "Network Rejected" }
                            else if(AppModel.transactionInfo.status === NUNCHUCKTYPE.PENDING_CONFIRMATION){ return "Pending Confirmations" }
                            else if(AppModel.transactionInfo.status === NUNCHUCKTYPE.CONFIRMED){ return confirmations + " Confirmations" }
                            else{ return "Replaced by Fee" }
                        }
                        color:  {
                            if(AppModel.transactionInfo.status === NUNCHUCKTYPE.PENDING_SIGNATURES){ return "#E02247" }
                            else if(AppModel.transactionInfo.status === NUNCHUCKTYPE.READY_TO_BROADCAST){ return "#FF7A00" }
                            else if(AppModel.transactionInfo.status === NUNCHUCKTYPE.PENDING_CONFIRMATION){ return "#FF7A00" }
                            else if(AppModel.transactionInfo.status === NUNCHUCKTYPE.CONFIRMED){ return "#35ABEE" }
                            else{ return "#031F2B" }
                        }
                        font.weight: Font.DemiBold
                        font.pixelSize: 14
                    }
                }

                QNotification {
                    id: notification
                    anchors {
                        horizontalCenter: parent.horizontalCenter
                        top: parent.top
                        topMargin: 60
                    }
                    visible: {
                        if(AppModel.transactionInfo.status === NUNCHUCKTYPE.REPLACED || AppModel.transactionInfo.status === NUNCHUCKTYPE.NETWORK_REJECTED
                                || AppModel.transactionInfo.status === NUNCHUCKTYPE.CONFIRMED || AppModel.transactionInfo.isReceiveTx) { return false }
                        else{ return ((AppModel.transactionInfo.warningMessage.type !== EWARNING.NONE_MSG) || (AppModel.deviceList.warningMessage.type === EWARNING.EXCEPTION_MSG))}
                    }

                    label:  (AppModel.deviceList.warningMessage.type === EWARNING.EXCEPTION_MSG) ? AppModel.deviceList.warningMessage.contentDisplay :
                            (AppModel.transactionInfo.warningMessage.type !== EWARNING.NONE_MSG) ? AppModel.transactionInfo.warningMessage.contentDisplay : ""
                    currentStatus:  (AppModel.deviceList.warningMessage.type === EWARNING.EXCEPTION_MSG) ? AppModel.deviceList.warningMessage.type :
                                    (AppModel.transactionInfo.warningMessage.type !== EWARNING.NONE_MSG) ? AppModel.transactionInfo.warningMessage.type : 0
                    onCloseRequest: {
                        AppModel.transactionInfo.warningMessage.type = EWARNING.NONE_MSG
                        AppModel.deviceList.warningMessage.type = EWARNING.NONE_MSG
                    }
                }

                Rectangle {
                    width: 379
                    height: 480
                    anchors {
                        left: parent.left
                        leftMargin: 40
                        top: parent.top
                        topMargin: notification.visible ? 120 : 84
                    }
                    color: Qt.rgba(255, 255, 255, 0.5)
                    border.color: "#9CAEB8"
                    border.width: 1
                    radius: 8
                    Column {
                        width: parent.width
                        Item {
                            width: parent.width
                            height: 47
                            QText {
                                anchors.left: parent.left
                                anchors.leftMargin: 24
                                anchors.verticalCenter: parent.verticalCenter
                                text: "Transaction Overview"
                                color: "#031F2B"
                                font.weight: Font.Bold
                                font.pixelSize: 14

                                QTooltip {
                                    anchors.left: parent.right
                                    anchors.leftMargin: 8
                                    anchors.verticalCenter: parent.verticalCenter
                                    toolTip: "Click the address to verify on device"
                                    rightOfParent: true
                                }
                            }
                        }

                        Flickable {
                            id: flickcontent
                            width: parent.width
                            height: 465 - 47
                            flickableDirection: Flickable.VerticalFlick
                            clip: true
                            interactive: contentHeight > flickcontent.height
                            contentHeight: contentDisp.height
                            ScrollBar.vertical: ScrollBar { active: true }
                            Column {
                                id: contentDisp
                                width: 377
                                spacing: 5
                                anchors.horizontalCenter: parent.horizontalCenter
                                Rectangle {
                                    width: parent.width
                                    height: 32
                                    color: "#C9DEF1"
                                    QText {
                                        text: "SEND TO ADDRESS"
                                        font.family: "Lato"
                                        font.weight: Font.Bold
                                        font.pixelSize: 10
                                        color: "#323E4A"
                                        anchors {
                                            left: parent.left
                                            leftMargin: 20
                                            verticalCenter: parent.verticalCenter
                                        }
                                    }
                                    QText {
                                        text: "AMOUNT"
                                        font.family: "Lato"
                                        font.weight: Font.Bold
                                        font.pixelSize: 10
                                        color: "#323E4A"
                                        anchors {
                                            right: parent.right
                                            rightMargin: 20
                                            verticalCenter: parent.verticalCenter
                                        }
                                    }
                                }

                                // Destination infomation
                                Repeater {
                                    model: AppModel.transactionInfo.destinationList
                                    width: parent.width
                                    Item {
                                        width: parent.width
                                        height: 32
                                        Rectangle {
                                            anchors.fill: parent
                                            color: btnMouseVerify.containsMouse ?"#C9DEF1" : "transparent"
                                        }
                                        QText {
                                            width: 220
                                            text: destination_address
                                            font.pixelSize: 12
                                            wrapMode: Text.WrapAnywhere
                                            color: "#031F2B"
                                            font.family: "Lato"
                                            anchors {
                                                left: parent.left
                                                leftMargin: 20
                                                verticalCenter: parent.verticalCenter
                                            }
                                            MouseArea {
                                                id: btnMouseVerify
                                                hoverEnabled: true
                                                cursorShape: Qt.PointingHandCursor
                                                anchors.fill: parent
                                                onClicked: {
                                                    displayAddressBusybox.addrToVerify = destination_address
                                                    QMLHandle.sendEvent(EVT.EVT_TRANSACTION_VERIFY_ADDRESS, displayAddressBusybox.addrToVerify)
                                                }
                                            }
                                        }
                                        QText {
                                            width: 140
                                            height: 16
                                            text: destination_amount + ((AppSetting.unit === 1) ? " sat" : " BTC")
                                            horizontalAlignment: Text.AlignRight
                                            font.pixelSize: 12
                                            color: "#031F2B"
                                            font.family: "Lato"
                                            anchors {
                                                right: parent.right
                                                rightMargin: 20
                                            }
                                        }
                                    }
                                }

                                Rectangle {
                                    width: parent.width
                                    height: 1
                                    color: "#9CAEB8"
                                }

                                // Destination total
                                Item {
                                    width: parent.width
                                    height: 20
                                    QText {
                                        width: 93
                                        height: 16
                                        text: "Subtotal"
                                        horizontalAlignment: Text.AlignRight
                                        font.pixelSize: 12
                                        color: "#031F2B"
                                        font.family: "Lato"
                                        anchors {
                                            left: parent.left
                                            leftMargin: 150
                                            verticalCenter: parent.verticalCenter
                                        }
                                    }
                                    QText {
                                        width: 93
                                        height: 16
                                        text: AppModel.transactionInfo.subtotal + ((AppSetting.unit === 1) ? " sat" : " BTC")
                                        horizontalAlignment: Text.AlignRight
                                        font.pixelSize: 12
                                        color: "#031F2B"
                                        font.family: "Lato"
                                        anchors {
                                            right: parent.right
                                            rightMargin: 20
                                            verticalCenter: parent.verticalCenter
                                        }
                                    }
                                }

                                Item {
                                    width: parent.width
                                    height: 20
                                    QText {
                                        width: 93
                                        height: 16
                                        text: "Total Fee"
                                        horizontalAlignment: Text.AlignRight
                                        font.pixelSize: 12
                                        color: "#031F2B"
                                        font.family: "Lato"
                                        anchors {
                                            left: parent.left
                                            leftMargin: 150
                                            verticalCenter: parent.verticalCenter
                                        }
                                    }
                                    QText {
                                        width: 93
                                        height: 16
                                        text: AppModel.transactionInfo.fee + ((AppSetting.unit === 1) ? " sat" : " BTC")
                                        horizontalAlignment: Text.AlignRight
                                        font.pixelSize: 12
                                        color: "#031F2B"
                                        font.family: "Lato"
                                        anchors {
                                            right: parent.right
                                            rightMargin: 20
                                            verticalCenter: parent.verticalCenter
                                        }
                                    }
                                }

                                Rectangle {
                                    width: 162
                                    height: 1
                                    anchors.right: parent.right
                                    color: "#C9DEF1"
                                }

                                Item {
                                    width: parent.width
                                    height: 30
                                    QText {
                                        width: 93
                                        height: 16
                                        text: "Total"
                                        horizontalAlignment: Text.AlignRight
                                        font.weight: Font.Bold
                                        font.pixelSize: 16
                                        color: "#031F2B"
                                        font.family: "Lato"
                                        anchors {
                                            left: parent.left
                                            leftMargin: 120
                                            verticalCenter: parent.verticalCenter
                                        }
                                    }
                                    QText {
                                        width: 93
                                        height: 16
                                        text: AppModel.transactionInfo.total + ((AppSetting.unit === 1) ? " sat" : " BTC")
                                        horizontalAlignment: Text.AlignRight
                                        font.weight: Font.Bold
                                        font.pixelSize: 16
                                        color: "#031F2B"
                                        font.family: "Lato"
                                        anchors {
                                            right: parent.right
                                            rightMargin: 20
                                            verticalCenter: parent.verticalCenter
                                        }
                                    }
                                }

                                Rectangle {
                                    id: changeinfo
                                    width: parent.width
                                    visible: AppModel.transactionInfo.hasChange
                                    height: changeinfo.visible ? 32 : 0
                                    color: "#C9DEF1"
                                    QText {
                                        text: "CHANGE ADDRESS"
                                        font.family: "Lato"
                                        font.weight: Font.Bold
                                        font.pixelSize: 10
                                        color: "#323E4A"
                                        anchors {
                                            left: parent.left
                                            leftMargin: 20
                                            verticalCenter: parent.verticalCenter
                                        }
                                    }
                                    QText {
                                        text: "AMOUNT"
                                        font.family: "Lato"
                                        font.weight: Font.Bold
                                        font.pixelSize: 10
                                        color: "#323E4A"
                                        anchors {
                                            right: parent.right
                                            rightMargin: 20
                                            verticalCenter: parent.verticalCenter
                                        }
                                    }
                                }

                                Item {
                                    visible: changeinfo.visible
                                    width: parent.width
                                    height: changeinfo.visible ? 32 : 0
                                    Rectangle {
                                        anchors.fill: parent
                                        color: btnMouseVerifyChangeAddr.containsMouse ?"#C9DEF1" : "transparent"
                                    }
                                    QText {
                                        width: 220
                                        text: AppModel.transactionInfo.change.address
                                        wrapMode: Text.WrapAnywhere
                                        font.pixelSize: 12
                                        color: "#031F2B"
                                        font.family: "Lato"
                                        anchors {
                                            left: parent.left
                                            leftMargin: 20
                                            verticalCenter: parent.verticalCenter
                                        }

                                        MouseArea {
                                            id: btnMouseVerifyChangeAddr
                                            hoverEnabled: true
                                            cursorShape: Qt.PointingHandCursor
                                            anchors.fill: parent
                                            onClicked: {
                                                displayAddressBusybox.addrToVerify = AppModel.transactionInfo.change.address
                                                QMLHandle.sendEvent(EVT.EVT_TRANSACTION_VERIFY_ADDRESS, displayAddressBusybox.addrToVerify)
                                            }
                                        }
                                    }
                                    QText {
                                        width: 140
                                        height: 16
                                        text: AppModel.transactionInfo.change.amount + ((AppSetting.unit === 1) ? " sat" : " BTC")
                                        horizontalAlignment: Text.AlignRight
                                        font.pixelSize: 12
                                        elide: Text.ElideRight
                                        color: "#031F2B"
                                        font.family: "Lato"
                                        anchors {
                                            right: parent.right
                                            rightMargin: 20
                                        }
                                    }
                                }

                                Rectangle {
                                    width: parent.width
                                    height: 1
                                    anchors.right: parent.right
                                    color: "#C9DEF1"
                                }

                                Item {
                                    width: parent.width
                                    height: memodisplay.height
                                    clip: true
                                    QTextInputBox {
                                        id: memodisplay
                                        textOutput: AppModel.transactionInfo.memo
                                        placeholder.text: "MEMO"
                                        color: "transparent"
                                        width: parent.width
                                        heightMin: 64
                                        botomLineVisible: false
                                        mode: ePREVIEW_MODE
                                        onTypingFinished: {
                                            if(currentText !== AppModel.transactionInfo.memo && currentText !== ""){
                                                QMLHandle.sendEvent(EVT.EVT_TRANSACTION_SET_MEMO_REQUEST, currentText)
                                            }
                                        }
                                    }
                                }

                                Rectangle {
                                    width: parent.width
                                    height: 1
                                    anchors.right: parent.right
                                    color: "#C9DEF1"
                                }

                                Item {
                                    width: parent.width
                                    height: 64
                                    QText {
                                        text: "SIGNATURES"
                                        font.pixelSize: 10
                                        font.weight: Font.DemiBold
                                        color: "#323E4A"
                                        font.family: "Lato"
                                        anchors {
                                            left: parent.left
                                            leftMargin: 20
                                            top: parent.top
                                            topMargin: 16
                                        }
                                    }
                                    QText {
                                        id: numbersignaturepending
                                        text: AppModel.transactionInfo.numberSigned + " / " + AppModel.transactionInfo.m
                                        font.pixelSize: 16
                                        font.weight: Font.Bold
                                        color: "#323E4A"
                                        font.family: "Lato"
                                        anchors {
                                            left: parent.left
                                            leftMargin: 20
                                            top: parent.top
                                            topMargin: 36
                                        }
                                        width: 346
                                        height: 32
                                        wrapMode: Text.WordWrap
                                    }

                                    Row {
                                        visible: statusTransOverview.text !== ""
                                        spacing: 8
                                        height: 32
                                        anchors {
                                            left: parent.left
                                            leftMargin: 70
                                            verticalCenter: numbersignaturepending.verticalCenter
                                        }
                                        Rectangle {
                                            id: indicatorStatus
                                            width: 4
                                            height: 24
                                            radius: 1
                                            color: statusTransOverview.color
                                            anchors.verticalCenter: parent.verticalCenter
                                        }

                                        QText {
                                            id: statusTransOverview
                                            text:  {
                                                if(AppModel.transactionInfo.status === NUNCHUCKTYPE.PENDING_SIGNATURES){
                                                    return "Pending " + Math.max(0, (AppModel.transactionInfo.m - AppModel.transactionInfo.numberSigned)) + " Signatures"
                                                }
                                                else if(AppModel.transactionInfo.status === NUNCHUCKTYPE.READY_TO_BROADCAST){ return "Ready to Broadcast" }
                                                else { return "" }
                                            }
                                            font.pixelSize: 12
                                            font.family: "Lato"
                                            color: {
                                                if(AppModel.transactionInfo.status === NUNCHUCKTYPE.PENDING_SIGNATURES){ return "#E02247" }
                                                else if(AppModel.transactionInfo.status === NUNCHUCKTYPE.READY_TO_BROADCAST){ return "#FF7A00"}
                                                else { return "transparent" }
                                            }
                                            font.weight: Font.Bold
                                            width: 346
                                            height: 32
                                            anchors.verticalCenter: parent.verticalCenter
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                Row {
                    id: signerheader
                    width: 101
                    height: 22
                    spacing: 8
                    anchors {
                        left: parent.left
                        leftMargin: 462
                        top: parent.top
                        topMargin: notification.visible ? 120 : 84
                    }
                    QText {
                        width: 56
                        height: 21
                        anchors.verticalCenter: parent.verticalCenter
                        font.weight: Font.Bold
                        font.pixelSize: 14
                        color: "#031F2B"
                        text: "Signers:"
                    }
                    QText {
                        width: 10
                        height: 21
                        anchors.verticalCenter: parent.verticalCenter
                        font.weight: Font.DemiBold
                        font.pixelSize: 16
                        color: "#031F2B"
                        text: AppModel.walletInfo.walletM       // number signatures required
                    }
                    Rectangle {
                        width: 1
                        height: 16
                        color: "#839096"
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    QText {
                        width: 10
                        height: 21
                        anchors.verticalCenter: parent.verticalCenter
                        font.weight: Font.DemiBold
                        font.pixelSize: 16
                        color: "#839096"
                        text: AppModel.walletInfo.walletN       // number signers
                    }
                }

                QListView {
                    id: signedList
                    width: 301
                    height: visible ? Math.min(188, ((signedList.count*60)+(4*(signedList.count-1)))) : 0
                    model: AppModel.transactionInfo.singleSignersAssigned
                    spacing: 4
                    clip: true
                    anchors {
                        right: parent.right
                        rightMargin: 40
                        top: signerheader.bottom
                        topMargin: 6
                    }
                    ScrollBar.vertical: ScrollBar { active: true }

                    delegate: Item {
                        width: 301
                        height: 60

                        Rectangle {
                            id: rect
                            anchors.horizontalCenter: parent.horizontalCenter
                            width: 298
                            height: 57
                            color: Qt.rgba(255, 255, 255)
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

                        Image {
                            id: checked
                            anchors.top: parent.top
                            anchors.topMargin: 8
                            anchors.left: parent.left
                            anchors.leftMargin: 8
                            source: single_signer_signed_status ? "qrc:/Images/Images/check_circle_outline_24px_checked.png" :
                                                                  "qrc:/Images/Images/check_circle_outline_24px_uncheck.png"
                        }
                        Column {
                            width: 235
                            height: 53
                            anchors {
                                left: parent.left
                                leftMargin: 40
                                verticalCenter: rect.verticalCenter
                            }
                            QText {
                                width: 235
                                height: 21
                                id: signername
                                text: singleSigner_name
                                font.pixelSize: 14
                                font.weight: Font.DemiBold
                                font.family: "Montserrat"
                                color: "#031F2B"
                            }

                            QText {
                                id: xfp
                                width: 235
                                height: 16
                                text: "XFP: " + singleSigner_masterFingerPrint
                                font.capitalization: Font.AllUppercase
                                font.pixelSize: 12
                                color: "#031F2B"
                            }

                            QText {
                                id: derivation
                                width: 235
                                height: 16
                                text: "BIP32 Path: " + singleSigner_derivationPath
                                font.pixelSize: 10
                                color: "#839096"
                            }
                        }
                    }
                }

                Loader {
                    anchors {
                        fill: parent
                        leftMargin: 459
                        rightMargin: 40
                        topMargin: signedList.y + signedList.height + 10
                        bottomMargin: 0
                    }
                    sourceComponent: {
                        if(AppModel.transactionInfo.status === NUNCHUCKTYPE.PENDING_SIGNATURES){ return pendingSignatures }
                        else if(AppModel.transactionInfo.status === NUNCHUCKTYPE.READY_TO_BROADCAST){ return readytoBroadcast }
                        else if(AppModel.transactionInfo.status === NUNCHUCKTYPE.PENDING_CONFIRMATION){ return pendingconfirmation }
                        else { return confirmed }
                    }
                }

                QTextButton {
                    id: removebtn
                    width: 200
                    height: 48
                    anchors {
                        bottom: parent.bottom
                        bottomMargin: 40
                        right: parent.right
                        rightMargin: (AppModel.transactionInfo.status === NUNCHUCKTYPE.REPLACED) ||
                                     (AppModel.transactionInfo.status === NUNCHUCKTYPE.NETWORK_REJECTED) ? 40 : 236
                    }
                    enabled: visible
                    visible: (AppModel.transactionInfo.status !== NUNCHUCKTYPE.CONFIRMED) &&
                             (AppModel.transactionInfo.status !== NUNCHUCKTYPE.PENDING_CONFIRMATION)
                    label.text: "Remove Transaction"
                    label.font.pixelSize: 16
                    type: eTypeB
                    onButtonClicked: {
                        QMLHandle.sendEvent(EVT.EVT_TRANSACTION_REMOVE_REQUEST)
                    }
                }

                QTextButton {
                    id: savetransaction
                    width: 200
                    height: 48
                    anchors {
                        bottom: parent.bottom
                        bottomMargin: 40
                        right: removebtn.left
                        rightMargin: 16
                    }
                    visible: (AppModel.transactionInfo.status === NUNCHUCKTYPE.REPLACED) ||
                             (AppModel.transactionInfo.status === NUNCHUCKTYPE.NETWORK_REJECTED)
                    enabled: visible
                    label.text: "Save Transaction"
                    label.font.pixelSize: 16
                    type: eTypeB
                    onButtonClicked: {
                        imExModal.mode = imExModal.eEXPORT
                        imExModal.visible = true
                    }
                }

                Item {
                    id: popAdvanced
                    anchors.fill: parent
                    enabled: visible
                    visible: false
                    MouseArea {
                        anchors.fill: parent
                        onClicked: { popAdvanced.visible = false; }
                    }

                    Rectangle {
                        id: pop
                        width: 264
                        height: 84
                        anchors {
                            right: parent.right
                            rightMargin: 486
                            bottom: parent.bottom
                            bottomMargin: 70
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
                            width: 264
                            height: 64
                            anchors.centerIn: parent
                            Rectangle {
                                width: parent.width
                                height: 32
                                color: popdownloadMouse.containsMouse ? "#C9DEF1" : "transparent"
                                anchors.horizontalCenter: parent.horizontalCenter
                                QImage {
                                    id: downloadIco
                                    width: 24
                                    height: 24
                                    source: "qrc:/Images/Images/download_031F2B.png"
                                    anchors.left: parent.left
                                    anchors.leftMargin: 16
                                    anchors.verticalCenter: parent.verticalCenter
                                }
                                QText {
                                    width: 188
                                    height: 24
                                    anchors.verticalCenter: parent.verticalCenter
                                    anchors.left: downloadIco.right
                                    anchors.leftMargin: 8
                                    text: "Export Transaction"
                                    color: Qt.rgba(0, 0, 0, 0.87) //#35ABEE
                                    font.family: "Lato"
                                    font.pixelSize: 16
                                }
                                MouseArea {
                                    id: popdownloadMouse
                                    hoverEnabled: true
                                    anchors.fill: parent
                                    onClicked: {
                                        imExModal.mode = imExModal.eEXPORT
                                        imExModal.visible = true
                                        popAdvanced.visible = false
                                    }
                                }
                            }

                            Rectangle {
                                width: parent.width
                                height: 32
                                color: importmouse.containsMouse ? "#C9DEF1" : "transparent"
                                anchors.horizontalCenter: parent.horizontalCenter
                                QImage {
                                    id: importIco
                                    width: 24
                                    height: 24
                                    source: "qrc:/Images/Images/import_031F2B.png"
                                    anchors.left: parent.left
                                    anchors.leftMargin: 16
                                    anchors.verticalCenter: parent.verticalCenter
                                }
                                QText {
                                    width: 188
                                    height: 24
                                    anchors.verticalCenter: parent.verticalCenter
                                    anchors.left: importIco.right
                                    anchors.leftMargin: 8
                                    text: "Import Signature"
                                    color: Qt.rgba(0, 0, 0, 0.87) //#35ABEE
                                    font.family: "Lato"
                                    font.pixelSize: 16
                                }
                                MouseArea {
                                    id: importmouse
                                    anchors.fill: parent
                                    hoverEnabled: true
                                    onClicked: {
                                        imExModal.mode = imExModal.eIMPORT
                                        imExModal.visible = true
                                        popAdvanced.visible = false
                                    }
                                }
                            }
                        }
                    }
                }

                Rectangle {
                    id: imExModal
                    readonly property int eIMPORT: 0
                    readonly property int eEXPORT: 1
                    readonly property int eNONE: -1
                    property int mode: imExModal.eNONE
                    property var componentsImEx: [importSignature, exportSignature]
                    visible: false
                    anchors.fill: parent
                    radius: 8
                    color: Qt.rgba(255, 255, 255, 0.7)

                    Item {
                        width: 600
                        height: 344
                        anchors.centerIn: parent
                        Rectangle {
                            id: imExModalMask
                            anchors.fill: parent
                            radius: 8
                            visible: false
                        }
                        Rectangle {
                            id: imExModalBg
                            anchors.fill: parent
                            radius: 8
                            visible: false
                            color: "#F1FAFE"
                            Rectangle {
                                height: 4
                                width: parent.width
                                color: "#F6D65D"
                            }
                        }

                        OpacityMask {
                            id: imExModalOpacityMask
                            anchors.fill: imExModalBg
                            source: imExModalBg
                            maskSource: imExModalMask
                            QCloseButton {
                                anchors {
                                    right: parent.right
                                    rightMargin: 16
                                    top: parent.top
                                    topMargin: 16
                                }
                                onClicked: imExModal.visible = false
                            }

                            Loader {
                                anchors.centerIn: parent
                                sourceComponent: imExModal.mode < 0 ? null : imExModal.componentsImEx[imExModal.mode]
                            }
                        }

                        DropShadow {
                            anchors.fill: imExModalOpacityMask
                            horizontalOffset: 3
                            verticalOffset: 3
                            radius: 8.0
                            samples: 17
                            color: "#80000000"
                            source: imExModalOpacityMask
                        }
                    }
                }

                Component {
                    id: pendingSignatures
                    Item {
                        width: 303
                        height: 341

                        QText {
                            id: tit
                            width: 302
                            height: 42
                            text: "Plug in & unlock your Signer to sign this Transaction"
                            anchors {
                                horizontalCenter: parent.horizontalCenter
                                top: parent.top
                            }
                            font.pixelSize: 14
                            font.weight: Font.Bold
                            wrapMode: Text.WordWrap
                            color: "#031F2B"
                        }

                        Image {
                            id: nodevice
                            visible: !flickerDeviceList.visible
                            anchors {
                                top: tit.bottom
                                topMargin: 8
                            }
                            source: "qrc:/Images/Images/Signer_Level2_2.png"
                        }

                        Flickable {
                            id: flickerDeviceList
                            width: 301
                            height: 170
                            clip: true
                            flickableDirection: Flickable.VerticalFlick
                            interactive: true;
                            contentHeight: contentDisplay.height
                            ScrollBar.vertical: ScrollBar { active: true }
                            visible: devicelist.count || softwareSignerdevicelist.count
                            anchors {
                                top: tit.bottom
                                topMargin: 8
                            }
                            Column {
                                id: contentDisplay
                                spacing: 2
                                QListView {
                                    id: devicelist
                                    property int  signerReady: 0
                                    property bool needPin: false
                                    visible: devicelist.count
                                    width: 301
                                    height: visible ? devicelist.contentHeight : 0
                                    model: AppModel.deviceList
                                    spacing: devicelist.count > 1 ? 4 : 0
                                    currentIndex: (true === AppModel.deviceList.containsSignable && (devicelist.count == 1)) ? 0 : -1
                                    clip: true
                                    interactive : false
                                    delegate: Item {
                                        width: 301
                                        height: 54
                                        Rectangle {
                                            id: rect
                                            anchors.horizontalCenter: parent.horizontalCenter
                                            width: 298
                                            height: 49
                                            color: Qt.rgba(255, 255, 255)

                                            Rectangle {
                                                visible: index == devicelist.currentIndex
                                                width: 8
                                                height: parent.height
                                                color: "#F6D65D"
                                            }

                                            Image {
                                                anchors {
                                                    left: parent.left
                                                    leftMargin: 16
                                                    verticalCenter: parent.verticalCenter
                                                }
                                                source: index == devicelist.currentIndex ? "qrc:/Images/Images/RadioEnabled.png" : "qrc:/Images/Images/RadioDeselected.png"
                                            }

                                            Column {
                                                width: 240
                                                height: 37
                                                anchors {
                                                    left: parent.left
                                                    leftMargin: 48
                                                    verticalCenter: parent.verticalCenter
                                                }
                                                QText {
                                                    width: parent.width
                                                    height: 21
                                                    font.family: "Montserrat"
                                                    font.pixelSize: 14
                                                    color: "#031F2B"
                                                    font.weight: Font.DemiBold
                                                    text: device_type
                                                    elide: Text.ElideRight
                                                }
                                                QText {
                                                    width: parent.width
                                                    height: 16
                                                    font.family: "Lato"
                                                    font.pixelSize: 12
                                                    color: "#323E4A"
                                                    font.capitalization: Font.AllUppercase
                                                    text: "XFP: " + device_master_fingerprint
                                                }
                                            }
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
                                            anchors.fill: parent
                                            visible: !device_usable_to_sign
                                            color: Qt.rgba(255, 255, 255, 0.5)
                                        }

                                        MouseArea {
                                            enabled: device_usable_to_sign
                                            hoverEnabled: true
                                            cursorShape: Qt.PointingHandCursor
                                            anchors.fill: parent
                                            onClicked: {
                                                softwareSignerdevicelist.currentIndex = -1
                                                devicelist.currentIndex = index
                                                devicelist.needPin = device_needs_pin_sent
                                            }
                                        }
                                    }
                                }

                                QListView {
                                    id: softwareSignerdevicelist
                                    property int  signerReady: 0
                                    property bool needPassphrase: false
                                    property string signerId: ""
                                    visible: softwareSignerdevicelist.count
                                    width: 301
                                    height: visible ? softwareSignerdevicelist.contentHeight : 0
                                    model: AppModel.softwareSignerDeviceList
                                    currentIndex: devicelist.count === 0 ? (true === AppModel.softwareSignerDeviceList.containsSignable && (softwareSignerdevicelist.count == 1)) ? 0 : -1 : -1
                                    clip: true
                                    interactive : false
                                    delegate: Item {
                                        width: 301
                                        height: device_connected ? 54 + 4 : 0
                                        visible: device_connected
                                        property bool needPassphrase: device_needs_pass_phrase_sent
                                        property bool signerId: device_master_signer_id

                                        Rectangle {
                                            id: rect2
                                            anchors.horizontalCenter: parent.horizontalCenter
                                            width: 298
                                            height: parent.height - 9
                                            color: Qt.rgba(255, 255, 255)

                                            Rectangle {
                                                visible: index == softwareSignerdevicelist.currentIndex
                                                width: 8
                                                height: parent.height - 4
                                                color: "#F6D65D"
                                            }

                                            Image {
                                                anchors {
                                                    left: parent.left
                                                    leftMargin: 16
                                                    verticalCenter: parent.verticalCenter
                                                }
                                                source: index == softwareSignerdevicelist.currentIndex ? "qrc:/Images/Images/RadioEnabled.png" : "qrc:/Images/Images/RadioDeselected.png"
                                            }

                                            Column {
                                                width: 240
                                                height: 37
                                                anchors {
                                                    left: parent.left
                                                    leftMargin: 48
                                                    verticalCenter: parent.verticalCenter
                                                }
                                                QText {
                                                    width: parent.width
                                                    height: 21
                                                    font.family: "Montserrat"
                                                    font.pixelSize: 14
                                                    color: "#031F2B"
                                                    font.weight: Font.DemiBold
                                                    text: device_name
                                                    elide: Text.ElideRight
                                                }
                                                QText {
                                                    width: parent.width
                                                    height: 16
                                                    font.family: "Lato"
                                                    font.pixelSize: 12
                                                    color: "#323E4A"
                                                    font.capitalization: Font.AllUppercase
                                                    text: "XFP: " + device_master_fingerprint
                                                }
                                            }
                                        }

                                        DropShadow {
                                            anchors.fill: rect2
                                            verticalOffset: 2
                                            cached: true
                                            radius: 8
                                            samples: 16
                                            color: Qt.rgba(0, 0, 0, 0.15)
                                            source: rect2
                                        }

                                        Rectangle {
                                            width: 100
                                            height: (parent.height/2)-4
                                            anchors.verticalCenter: parent.verticalCenter
                                            anchors.right: parent.right
                                            anchors.rightMargin: 9
                                            color: "#C9DEF1"
                                            radius: 4
                                            QText {
                                                text: "SOFTWARE SIGNER "
                                                font.pixelSize: 10
                                                font.weight: Font.ExtraBold
                                                font.family: "Lato"
                                                color: "#323E4A"
                                                anchors.centerIn: parent
                                            }
                                        }

                                        Rectangle {
                                            anchors.fill: parent
                                            visible: !device_usable_to_sign
                                            color: Qt.rgba(255, 255, 255, 0.5)
                                        }

                                        MouseArea {
                                            enabled: device_usable_to_sign
                                            hoverEnabled: true
                                            cursorShape: Qt.PointingHandCursor
                                            anchors.fill: parent
                                            onClicked: {
                                                devicelist.currentIndex = -1
                                                softwareSignerdevicelist.currentIndex = index
                                                softwareSignerdevicelist.needPassphrase = device_needs_pass_phrase_sent
                                                softwareSignerdevicelist.signerId = device_master_signer_id
                                            }
                                        }
                                    }

                                    Component.onCompleted: {
                                        if(softwareSignerdevicelist.currentIndex === 0){
                                            softwareSignerdevicelist.needPassphrase = softwareSignerdevicelist.contentItem.children[0].needPassphrase
                                            softwareSignerdevicelist.signerId = softwareSignerdevicelist.contentItem.children[0].signerId
                                        }
                                    }
                                }
                            }
                        }

                        QRefreshButton {
                            width: 160
                            height: 32
                            label: "Refresh Devices"
                            fontPixelSize: 14
                            anchors {
                                top: tit.bottom
                                topMargin: 15 + Math.max(nodevice.height, flickerDeviceList.height)
                                left: flickerDeviceList.left
                            }
                            onButtonClicked: {
                                AppModel.transactionInfo.warningMessage.type = EWARNING.NONE_MSG
                                AppModel.deviceList.warningMessage.type = EWARNING.NONE_MSG
                                QMLHandle.sendEvent(EVT.EVT_TRANSACTION_SCAN_DEVICE_REQUEST)
                            }
                        }

                        Row {
                            spacing: 232
                            anchors {
                                right: parent.right
                                bottom: parent.bottom
                                bottomMargin: 40
                            }

                            QButtonLargeTail {
                                id: advancedBtn
                                width: 209
                                height: 48
                                type: eSECONDARY
                                label: "Import/Export"
                                optionVisible: popAdvanced.visible
                                onButtonClicked: { popAdvanced.visible = true }
                            }

                            QTextButton {
                                id: signbtn
                                width: 180
                                height: 48
                                enabled: ((devicelist.currentIndex != -1) || (softwareSignerdevicelist.currentIndex != -1)) && (AppModel.deviceList.warningMessage.type !== EWARNING.EXCEPTION_MSG)
                                label.text: "Sign Transaction"
                                label.font.pixelSize: 16
                                type: eTypeA
                                onButtonClicked: {
                                    AppModel.transactionInfo.warningMessage.type = EWARNING.NONE_MSG
                                    if(devicelist.currentIndex > -1){
                                        if(devicelist.needPin){
                                            pinModel.needshow = true
                                            pinModel.signerIndex = devicelist.currentIndex
                                            QMLHandle.sendEvent(EVT.EVT_TRANSACTION_PROMT_PIN_REQUEST, devicelist.currentIndex)
                                        }
                                        else{
                                            busyOverlay.visible = true
                                            busyOverlay.isSoftware = false
                                            var hwObj = { "isSoftware"    : false,
                                                          "deviceIndx"    : devicelist.currentIndex };
                                            QMLHandle.sendEvent(EVT.EVT_TRANSACTION_SIGN_REQUEST, hwObj)
                                        }
                                    }
                                    else if(softwareSignerdevicelist.currentIndex > -1){
                                        if(softwareSignerdevicelist.needPassphrase){
                                            passphraseModel.signerIndex = softwareSignerdevicelist.currentIndex
                                            passphraseModel.signerId = softwareSignerdevicelist.signerId
                                            passphraseModel.visible = true
                                        }
                                        else{
                                            busyOverlay.visible = true
                                            busyOverlay.isSoftware = true
                                            var swObj = { "isSoftware"    : true,
                                                          "deviceIndx"    : softwareSignerdevicelist.currentIndex };
                                            QMLHandle.sendEvent(EVT.EVT_TRANSACTION_SIGN_REQUEST, swObj)
                                        }
                                    }
                                    else{/**/}
                                }
                            }
                        }
                    }
                }

                Component {
                    id: readytoBroadcast
                    Item {
                        width: 303
                        height: 341

                        QText {
                            id: tit
                            width: 301
                            height: 42
                            text: "Are you sure you want to broadcast this transaction? Double check before broadcasting."
                            anchors.top: parent.top
                            font.pixelSize: 14
                            font.weight: Font.DemiBold
                            font.family: "Montserrat"
                            color: "#031F2B"
                            wrapMode: Text.WordWrap
                        }

                        Row {
                            spacing: 232
                            anchors {
                                right: parent.right
                                bottom: parent.bottom
                                bottomMargin: 40
                            }
                            QButtonIcon {
                                id: downloadtrans
                                width: 240
                                height: 48
                                label: "Export Transaction"
                                icons: ["download_031F2B.png","download_9CAEB8.png","download_F1FAFE.png","download_F1FAFE.png"]
                                fontPixelSize: 16
                                type: eSECOND
                                onButtonClicked: {
                                    imExModal.mode = imExModal.eEXPORT
                                    imExModal.visible = true
                                }
                            }

                            QTextButton {
                                id: startbroatcast
                                width: 180
                                height: 48
                                label.text: "Start Broadcasting"
                                label.font.pixelSize: 16
                                type: eTypeA
                                enabled: (AppModel.transactionInfo.status === NUNCHUCKTYPE.READY_TO_BROADCAST)
                                onButtonClicked: {
                                    QMLHandle.sendEvent(EVT.EVT_TRANSACTION_BROADCAST_REQUEST)
                                }
                            }
                        }
                    }
                }

                Component {
                    id: pendingconfirmation
                    Item {
                        width: 303
                        height: 341

                        Row {
                            width: 277
                            height: 24
                            spacing: 8
                            visible: (AppModel.transactionInfo.status !== NUNCHUCKTYPE.REPLACED) && (AppModel.transactionInfo.status !== NUNCHUCKTYPE.NETWORK_REJECTED)
                            QImage {
                                source: "qrc:/Images/Images/link.png"
                                anchors.verticalCenter: parent.verticalCenter
                            }

                            QText {
                                id: tit
                                width: 301
                                height: 42
                                anchors.verticalCenter: parent.verticalCenter
                                text: "View on Blockchain Explorer"
                                font.capitalization :  Font.AllUppercase
                                font.family: "Montserrat"
                                font.pixelSize: 12
                                font.weight: Font.DemiBold
                                color: mouseExplorerBlockchain.containsMouse ? "#35ABEE":"#031F2B"
                                wrapMode: Text.WordWrap
                                MouseArea {
                                    id: mouseExplorerBlockchain
                                    hoverEnabled: true
                                    cursorShape: Qt.PointingHandCursor
                                    anchors.fill: parent
                                    onClicked: {
                                        var activeLink = ""
                                        if(AppSetting.primaryServer === 0){ activeLink = BLOCKSTREAM_MAINNET + AppModel.transactionInfo.txid }
                                        else{ activeLink = BLOCKSTREAM_TESTNET + AppModel.transactionInfo.txid }
                                        Qt.openUrlExternally(activeLink)
                                    }
                                }
                            }
                        }

                        QTextButton {
                            id:downloadtrans
                            width: 210
                            height: 48
                            anchors {
                                right: parent.right
                                bottom: parent.bottom
                                bottomMargin: 40
                            }
                            label.text: "Replace-by-Fee"
                            label.font.pixelSize: 16
                            type: eTypeA
                            onButtonClicked: {
                                QMLHandle.sendEvent(EVT.EVT_TRANSACTION_REPLACE_BY_FEE_REQUEST)
                            }
                        }
                    }
                }

                Component {
                    id: confirmed
                    Item {
                        width: 303
                        height: 341
                        Row {
                            width: 277
                            height: 24
                            spacing: 8
                            visible: (AppModel.transactionInfo.status !== NUNCHUCKTYPE.REPLACED) && (AppModel.transactionInfo.status !== NUNCHUCKTYPE.NETWORK_REJECTED)
                            QImage {
                                source: "qrc:/Images/Images/link.png"
                                anchors.verticalCenter: parent.verticalCenter
                            }

                            QText {
                                id: tit
                                width: 301
                                height: 42
                                anchors.verticalCenter: parent.verticalCenter
                                text: "View on Blockchain Explorer"
                                font.capitalization :  Font.AllUppercase
                                font.family: "Montserrat"
                                font.pixelSize: 12
                                font.weight: Font.DemiBold
                                color: mouseExplorerBlockchain2.containsMouse ? "#35ABEE":"#031F2B"
                                wrapMode: Text.WordWrap
                                MouseArea {
                                    id: mouseExplorerBlockchain2
                                    hoverEnabled: true
                                    cursorShape: Qt.PointingHandCursor
                                    anchors.fill: parent
                                    onClicked: {
                                        var activeLink = ""
                                        if(AppSetting.primaryServer === 0){ activeLink = BLOCKSTREAM_MAINNET + AppModel.transactionInfo.txid }
                                        else{ activeLink = BLOCKSTREAM_TESTNET + AppModel.transactionInfo.txid }
                                        console.log("link: ", activeLink)
                                        Qt.openUrlExternally(activeLink)
                                    }
                                }
                            }
                        }
                    }
                }

                FileDialog {
                    id: openfileDialog
                    fileMode: FileDialog.OpenFile
                    onAccepted: {
                        QMLHandle.sendEvent(EVT.EVT_TRANSACTION_IMPORT_REQUEST, openfileDialog.file)
                        imExModal.mode = imExModal.eNONE
                        imExModal.visible = false
                    }
                }

                FileDialog {
                    id: savefileDialog
                    fileMode: FileDialog.SaveFile
                    onAccepted: {
                        QMLHandle.sendEvent(EVT.EVT_TRANSACTION_EXPORT_REQUEST, savefileDialog.currentFile)
                        imExModal.mode = imExModal.eNONE
                        imExModal.visible = false
                    }
                }

                Rectangle {
                    id: busyOverlay
                    property bool isSoftware: false
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
                                width: 214
                                height: 36
                                anchors.horizontalCenter: parent.horizontalCenter
                                horizontalAlignment: Text.AlignHCenter
                                color: "#F6D65D"
                                font.pixelSize: 24
                                font.weight: Font.DemiBold
                                font.family: "Montserrat"
                                text: "Signing Transaction"
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
                                text: busyOverlay.isSoftware ? "Please wait ..." : "Please Confirm to Sign on the Signer Device"
                            }
                        }
                    }
                }

                QNumPad {
                    id: pinModel
                    anchors.fill: parent
                    visible: pinModel.needshow && (AppModel.transactionInfo.status === NUNCHUCKTYPE.PENDING_SIGNATURES)
                    property bool   needshow: false
                    property int    signerIndex: -1
                    property string signerName: ""
                    onSenPINClicked : {
                        var deviceObj = {"deviceIndexSelected"   : pinModel.signerIndex,
                                                 "pinInputted"   : pinModel.pinInputted};
                        QMLHandle.sendEvent(EVT.EVT_TRANSACTION_SEND_PIN_REQUEST, deviceObj)
                    }
                }

                QPassphraseInput {
                    id: passphraseModel
                    property int    signerIndex: -1
                    property string signerId: ""
                    color: Qt.rgba(255, 255, 255, 0.7)
                    anchors.fill: parent
                    visible: false
                    onCloseClicked: {
                        passphraseModel.visible = false
                    }
                    onSendPassphraseClicked: {
                        var activeSigner = { "mastersignerId"    : passphraseModel.signerId,
                                             "passphraseInput"   : passphrase };
                        QMLHandle.sendEvent(EVT.EVT_TRANSACTION_SEND_PASSPHRASE, activeSigner)
                        if(AppModel.transactionInfo.warningMessage.type === EWARNING.NONE_MSG){
                            busyOverlay.visible = true
                            busyOverlay.isSoftware = true
                            var signObj = { "isSoftware"    : true,
                                            "deviceIndx"    : passphraseModel.signerIndex };
                            QMLHandle.sendEvent(EVT.EVT_TRANSACTION_SIGN_REQUEST, signObj)
                        }
                        passphraseModel.visible = false
                        passphraseModel.textInputted = ""
                    }
                }

                Connections {
                    target: AppModel
                    onFinishedSigningTransaction : {busyOverlay.visible = false}
                    onSentPINToDeviceResult : {
                        if(result === EWARNING.NONE_MSG){
                            pinModel.needshow = false
                            busyOverlay.visible = true
                        }
                        else{
                            // Warning message
                            pinModel.warning = "The PIN you entered is incorrect. Please try again."
                            pinModel.validInput = false
                            pinModel.pinInputted = ""
                        }
                        console.log("onSentPINToDeviceResult ", result)
                    }
                }

                Component {
                    id: exportSignature
                    Item {
                        width: 540
                        height: 270
                        QText {
                            id: exporttit
                            font.family: "Montserrat"
                            color: "#031F2B"
                            font.weight: Font.ExtraBold
                            font.pixelSize: 24
                            anchors.horizontalCenter: parent.horizontalCenter
                            text: "Export Transaction"
                        }
                        Row {
                            width: parent.width
                            height: 200
                            anchors.top: exporttit.bottom
                            anchors.topMargin: 30
                            anchors.horizontalCenter: exporttit.horizontalCenter
                            spacing: 8
                            Item {
                                width: 260
                                height: parent.height
                                QText {
                                    font.family: "Montserrat"
                                    color: "#839096"
                                    font.weight: Font.ExtraBold
                                    font.pixelSize: 12
                                    font.capitalization: Font.AllUppercase
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    text: "Export Via File"
                                }

                                QButtonIcon {
                                    id: btndownloadTx
                                    width: 236
                                    height: 48
                                    radius: 24
                                    anchors.centerIn: parent
                                    type: eFIRST
                                    label: "Download [.pspt]"
                                    icons: ["download_031F2B.png","download_C9DEF1.png","download_C9DEF1.png","download_031F2B.png"]
                                    fontPixelSize: 16
                                    onButtonClicked: {
                                        savefileDialog.currentFile = StandardPaths.writableLocation(StandardPaths.DocumentsLocation) + "/" + AppModel.transactionInfo.txid + ".psbt"
                                        savefileDialog.open()
                                    }
                                }
                            }
                            Rectangle { color: "#9CAEB8"; width: 2; height: parent.height }
                            Item {
                                width: 260
                                height: parent.height
                                QText {
                                    font.family: "Montserrat"
                                    color: "#839096"
                                    font.weight: Font.ExtraBold
                                    font.pixelSize: 12
                                    font.capitalization: Font.AllUppercase
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    text: "Export Via QR Code"
                                }

                                QQrButton {
                                    width: 100
                                    height: 100
                                    anchors.centerIn: parent
                                    onButtonClicked: {
                                        QMLHandle.sendEvent(EVT.EVT_TRANSACTION_EXPORT_QRCODE)
                                        qrcodeExportResult.visible = true
                                        imExModal.mode = imExModal.eNONE
                                        imExModal.visible = false
                                    }
                                }
                            }
                        }
                    }
                }

                Component {
                    id: importSignature
                    Item {
                        width: 540
                        height: 270
                        QText {
                            id: importtit
                            font.family: "Montserrat"
                            color: "#031F2B"
                            font.weight: Font.ExtraBold
                            font.pixelSize: 24
                            anchors.horizontalCenter: parent.horizontalCenter
                            text: "Import Transaction"
                        }
                        Row {
                            width: parent.width
                            height: 200
                            anchors.top: importtit.bottom
                            anchors.topMargin: 30
                            anchors.horizontalCenter: importtit.horizontalCenter
                            spacing: 8
                            Item {
                                width: 260
                                height: parent.height
                                QText {
                                    font.family: "Montserrat"
                                    color: "#839096"
                                    font.weight: Font.ExtraBold
                                    font.pixelSize: 12
                                    font.capitalization: Font.AllUppercase
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    text: "Import Via File"
                                }

                                QButtonIcon {
                                    id: btnuploadTx
                                    width: 236
                                    height: 48
                                    radius: 24
                                    anchors.centerIn: parent
                                    type: eFIRST
                                    label: "Upload [.pspt]"
                                    icons: ["import_031F2B.png","import_C9DEF1.png","import_C9DEF1.png","import_031F2B.png"]
                                    fontPixelSize: 16
                                    onButtonClicked: {
                                        openfileDialog.open()
                                    }
                                }
                            }
                            Rectangle { color: "#9CAEB8"; width: 2; height: parent.height }
                            Item {
                                width: 260
                                height: parent.height
                                QText {
                                    font.family: "Montserrat"
                                    color: "#839096"
                                    font.weight: Font.ExtraBold
                                    font.pixelSize: 12
                                    font.capitalization: Font.AllUppercase
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    text: "Import Via QR Code"
                                }

                                QQrButton {
                                    width: 100
                                    height: 100
                                    anchors.centerIn: parent
                                    onButtonClicked: {
                                        qrscaner.visible = true
                                        imExModal.mode = imExModal.eNONE
                                        imExModal.visible = false
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
                                    QMLHandle.sendEvent(EVT.EVT_TRANSACTION_IMPORT_QRCODE, result)
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

                QQrExportResult {
                    id: qrcodeExportResult
                    anchors.fill: parent
                    model: AppModel.qrExported
                    visible: false
                }
            }
        }

        Component {
            id: txReceived
            Item {
                QText {
                    id: receivedfund
                    anchors {
                        left: parent.left
                        leftMargin: 40
                        top: parent.top
                        topMargin: 24
                    }
                    text: "Transaction Info"
                    color: "#031F2B"
                    font.weight: Font.DemiBold
                    font.pixelSize: 24
                }

                Row {
                    id: status
                    height: 24
                    spacing: 8
                    anchors {
                        left: receivedfund.right
                        leftMargin: 8
                        verticalCenter: receivedfund.verticalCenter
                    }

                    Rectangle {
                        width: 4
                        height: 24
                        radius: 1
                        color: indicatorStatusTop.color
                    }

                    QText {
                        id: indicatorStatusTop
                        anchors.verticalCenter: parent.verticalCenter
                        text:  {
                            if(AppModel.transactionInfo.status === NUNCHUCKTYPE.PENDING_SIGNATURES){ return "Pending Signatures" }
                            else if(AppModel.transactionInfo.status === NUNCHUCKTYPE.READY_TO_BROADCAST){ return "Ready to Broadcast" }
                            else if(AppModel.transactionInfo.status === NUNCHUCKTYPE.PENDING_CONFIRMATION){ return "Pending Confirmations" }
                            else if(AppModel.transactionInfo.status === NUNCHUCKTYPE.CONFIRMED){ return confirmations + " Confirmations" }
                            else{ return "Replaced by Fee" }
                        }
                        color:  {
                            if(AppModel.transactionInfo.status === NUNCHUCKTYPE.PENDING_SIGNATURES){ return "#E02247" }
                            else if(AppModel.transactionInfo.status === NUNCHUCKTYPE.READY_TO_BROADCAST){ return "#FF7A00" }
                            else if(AppModel.transactionInfo.status === NUNCHUCKTYPE.PENDING_CONFIRMATION){ return "#FF7A00" }
                            else if(AppModel.transactionInfo.status === NUNCHUCKTYPE.CONFIRMED){ return "#35ABEE" }
                            else{  return "#031F2B" }
                        }
                        font.weight: Font.DemiBold
                        font.pixelSize: 14
                    }
                }

                Rectangle {
                    id: contentarea
                    width: 600
                    height: 382
                    anchors.top: parent.top
                    anchors.topMargin: 84
                    anchors.horizontalCenter: parent.horizontalCenter
                    radius: 8
                    color: Qt.rgba(255, 255, 255, 0.5)
                    border.color: "#9CAEB8"

                    Column {
                        anchors.fill: parent
                        Item {
                            id: overviewitem
                            width: parent.width
                            height: 47
                            QText {
                                anchors.left: parent.left
                                anchors.leftMargin: 24
                                anchors.verticalCenter: parent.verticalCenter
                                text: "Transaction Overview"
                                color: "#031F2B"
                                font.weight: Font.Bold
                                font.pixelSize: 14
                            }
                        }

                        Flickable {
                            id: flickcontent
                            width: parent.width
                            height: parent.height - overviewitem.height
                            flickableDirection: Flickable.VerticalFlick
                            clip: true
                            interactive: contentHeight > flickcontent.height
                            contentHeight: contentDisp.height
                            ScrollBar.vertical: ScrollBar { active: true }
                            Column {
                                id: contentDisp
                                width: parent.width
                                anchors.horizontalCenter: parent.horizontalCenter
                                spacing: 5
                                Rectangle {
                                    width: parent.width - 2
                                    height: 32
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    color: "#C9DEF1"
                                    QText {
                                        text: "RECEIVE ADDRESS"
                                        font.family: "Lato"
                                        font.weight: Font.Bold
                                        font.pixelSize: 10
                                        color: "#323E4A"
                                        anchors {
                                            left: parent.left
                                            leftMargin: 20
                                            verticalCenter: parent.verticalCenter
                                        }
                                    }
                                    QText {
                                        text: "AMOUNT"
                                        font.family: "Lato"
                                        font.weight: Font.Bold
                                        font.pixelSize: 10
                                        color: "#323E4A"
                                        anchors {
                                            right: parent.right
                                            rightMargin: 20
                                            verticalCenter: parent.verticalCenter
                                        }
                                    }
                                }

                                // Destination infomation
                                Repeater {
                                    model: AppModel.transactionInfo.destinationList
                                    width: parent.width
                                    Item {
                                        width: parent.width
                                        height: 32
                                        QText {
                                            width: parent.width*2/3 - 20
                                            text: destination_address
                                            wrapMode: Text.WrapAnywhere
                                            font.pixelSize: 12
                                            color: "#031F2B"
                                            font.family: "Lato"
                                            anchors {
                                                left: parent.left
                                                leftMargin: 20
                                                verticalCenter: parent.verticalCenter
                                            }
                                        }

                                        QText {
                                            width: parent.width*1/3 - 20
                                            height: 16
                                            text: destination_amount + ((AppSetting.unit === 1) ? " sat" : " BTC")
                                            horizontalAlignment: Text.AlignRight
                                            font.pixelSize: 12
                                            color: "#031F2B"
                                            font.family: "Lato"
                                            anchors {
                                                right: parent.right
                                                rightMargin: 20
                                                verticalCenter: parent.verticalCenter
                                            }
                                        }
                                    }
                                }

                                Rectangle {
                                    width: parent.width
                                    height: 1
                                    anchors.right: parent.right
                                    color: "#C9DEF1"
                                }

                                Item {
                                    width: parent.width
                                    height: memodisplay.height
                                    clip: true
                                    QTextInputBox {
                                        id: memodisplay
                                        textOutput: AppModel.transactionInfo.memo
                                        placeholder.text: "MEMO"
                                        color: "transparent"
                                        width: parent.width
                                        heightMin: 64
                                        botomLineVisible: false
                                        mode: ePREVIEW_MODE
                                        onTypingFinished: {
                                            if(currentText !== AppModel.transactionInfo.memo && currentText !== ""){
                                                QMLHandle.sendEvent(EVT.EVT_TRANSACTION_SET_MEMO_REQUEST, currentText)
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                Row {
                    width: 257
                    height: 24
                    spacing: 8
                    anchors.top: contentarea.bottom
                    anchors.topMargin: 14
                    anchors.right: contentarea.right
                    visible: (AppModel.transactionInfo.status !== NUNCHUCKTYPE.REPLACED) && (AppModel.transactionInfo.status !== NUNCHUCKTYPE.NETWORK_REJECTED)
                    QImage {
                        source: "qrc:/Images/Images/link.png"
                        anchors.verticalCenter: parent.verticalCenter
                    }

                    QText {
                        id: tit
                        width: 225
                        height: 42
                        anchors.verticalCenter: parent.verticalCenter
                        text: "View on Blockchain Explorer"
                        font.capitalization :  Font.AllUppercase
                        font.family: "Montserrat"
                        font.pixelSize: 12
                        font.weight: Font.DemiBold
                        color: mouseExplorerBlockchain3.containsMouse ? "#35ABEE":"#031F2B"
                        wrapMode: Text.WordWrap
                        MouseArea {
                            id: mouseExplorerBlockchain3
                            hoverEnabled: true
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            onClicked: {
                                var activeLink = ""
                                if(AppSetting.primaryServer === 0){ activeLink = BLOCKSTREAM_MAINNET + AppModel.transactionInfo.txid }
                                else{ activeLink = BLOCKSTREAM_TESTNET + AppModel.transactionInfo.txid }
                                console.log("link: ", activeLink)
                                Qt.openUrlExternally(activeLink)
                            }
                        }
                    }
                }

                QTextButton {
                    id: removebtn
                    width: 200
                    height: 48
                    anchors {
                        bottom: parent.bottom
                        bottomMargin: 40
                        right: parent.right
                        rightMargin: 40
                    }
                    visible: (AppModel.transactionInfo.status === NUNCHUCKTYPE.REPLACED) ||
                             (AppModel.transactionInfo.status === NUNCHUCKTYPE.NETWORK_REJECTED)
                    enabled: visible
                    label.text: "Remove Transaction"
                    label.font.pixelSize: 16
                    type: eTypeB
                    onButtonClicked: {
                        QMLHandle.sendEvent(EVT.EVT_TRANSACTION_REMOVE_REQUEST)
                    }
                }
            }
        }
    }

    Rectangle {
        id: displayAddressBusybox
        property string addrToVerify: ""
        anchors.fill: parent
        color: Qt.rgba(0, 0, 0, 0.7)
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
                onClicked: { displayAddressBusybox.visible = false ; displayAddressBusybox.addrToVerify = ""}
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
                        text: displayAddressBusybox.addrToVerify
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
    }
}
