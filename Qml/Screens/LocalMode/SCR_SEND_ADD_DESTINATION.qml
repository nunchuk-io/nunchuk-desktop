import QtQuick 2.4
import QtQuick.Controls 2.3
import QtGraphicalEffects 1.12
import HMIEVENTS 1.0
import EWARNING 1.0
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

        QCloseButton {
            anchors {
                right: parent.right
                rightMargin: 16
                top: parent.top
                topMargin: 16
            }
            onClicked: {
                QMLHandle.sendEvent(EVT.EVT_ONS_CLOSE_REQUEST, EVT.STATE_ID_SCR_SEND)
            }
        }

        Row {
            anchors {
                left: parent.left
                leftMargin: 40
                top: parent.top
                topMargin: 24
            }
            spacing: 8
            QText {
                id: sendfund
                text: "Send Funds"
                color: "#031F2B"
                font.weight: Font.ExtraBold
                font.family: "Montserrat"
                font.pixelSize: 24
                anchors.verticalCenter: parent.verticalCenter
            }

            Rectangle {
                width: visible ? 84 : 0
                height: visible ? 24 : 0
                color: "#031F2B"
                radius: 4
                visible: AppModel.walletInfo.walletEscrow
                anchors.verticalCenter: parent.verticalCenter
                Row {
                    anchors.centerIn: parent
                    spacing: 3
                    QImage {
                        source: "qrc:/Images/Images/Escrow Wallet.png"
                        width: 16
                        height: 16
                    }
                    QText {
                        text: "ESCROW"
                        color: "#F8D418"
                        width: 49
                        height: 10
                        font.weight: Font.Bold
                        font.family: "Lato"
                        font.pixelSize: 10
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
            }

            QText {
                text: "(" + AppModel.walletInfo.walletName + ")"
                color: "#031F2B"
                font.weight: Font.DemiBold
                font.family: "Montserrat"
                font.pixelSize: 14
                anchors.verticalCenter: parent.verticalCenter
            }
        }

        QNotification {
            id: notification
            anchors {
                horizontalCenter: parent.horizontalCenter
                top: parent.top
                topMargin: 81
            }
            visible: AppModel.transactionInfo.warningMessage.type !== EWARNING.NONE_MSG
            label: AppModel.transactionInfo.warningMessage.contentDisplay
            currentStatus: AppModel.transactionInfo.warningMessage.type
            onCloseRequest: AppModel.transactionInfo.warningMessage.type = EWARNING.NONE_MSG
        }

        QText {
            id: createtxlabel
            anchors {
                left: parent.left
                leftMargin: 43
                top: parent.top
                topMargin: notification.visible ? 146 : 92
            }
            text: "Create Transaction"
            color: "#031F2B"
            font.weight: Font.Bold
            font.pixelSize: 16
        }

        Image {
            id: checkboxMultiDes
            property bool checked: false
            width: 24
            height: 24
            anchors {
                left: parent.left
                leftMargin: 40
                top: parent.top
                topMargin: notification.visible ? 186 : 132
            }
            source: checkboxMultiDes.checked ? "qrc:/Images/Images/Checked_n.png" : "qrc:/Images/Images/UnChecked_n.png"
            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor
                onClicked: {
                    checkboxMultiDes.checked = !checkboxMultiDes.checked
                    if(checkboxMultiDes.checked){ requestAddMultiDestinations(eINCREASE) }
                    else{ requestAddMultiDestinations(eDECREASE) }
                }
            }
        }

        QText {
            anchors {
                left: checkboxMultiDes.right
                leftMargin: 8
                verticalCenter: checkboxMultiDes.verticalCenter
            }
            text: "Advanced: Multiple Destinations"
            color: "#031F2B"
            font.pixelSize: 14
        }

        Row {
            id: balanceinfo
            spacing: 10
            height: 24
            width: 260
            anchors {
                right: parent.right
                rightMargin: 40
                verticalCenter: checkboxMultiDes.verticalCenter
            }

            QText {
                height: 21
                width: 110
                text: "Wallet Balance:"
                color: "#000000"
                font.family: "Montserrat"
                font.weight: Font.DemiBold
                font.pixelSize: 14
                anchors.verticalCenter: parent.verticalCenter
            }

            QText {
                height: 24
                width: 140
                text: AppModel.walletInfo.walletBalance + ((AppSetting.unit === 1) ? " sat" : " BTC")
                color: "#000000"
                font.family: "Lato"
                font.pixelSize: 16
                horizontalAlignment: Text.AlignRight
                anchors.verticalCenter: parent.verticalCenter
            }
        }

        Flickable {
            width: 720
            height: 350
            flickableDirection: Flickable.VerticalFlick
            clip: true
            contentHeight: destColumn.height
            ScrollBar.vertical: ScrollBar { active: true }
            anchors {
                horizontalCenter: parent.horizontalCenter
                top: parent.top
                topMargin: notification.visible ? 225 : 171
            }
            Column {
                id: destColumn
                spacing: checkboxMultiDes.checked ? 16 : 36

                Repeater {
                    id: destination
                    property int numberDestination: 1
                    readonly property int destinationMAX: 10
                    property bool fullfill: false
                    model: 1
                    Row {
                        id: destinationRow
                        property alias toAddress: toAddr.textOutput
                        property alias toAmount: amount.textOutput

                        onToAddressChanged: {
                            destination.fullfill = destinationRow.toAddress !== "" && destinationRow.toAmount !== ""
                        }

                        onToAmountChanged: {
                            destination.fullfill = destinationRow.toAddress !== "" && destinationRow.toAmount !== ""
                        }

                        spacing: 32
                        Item {
                            width: 419
                            height: toAddr.height
                            QTextInputBox {
                                id: toAddr
                                width: 419 - qricoimport.width
                                heightMin: 56
                                mode: eEDIT_MODE
                                placeholder.text: checkboxMultiDes.checked ? "To Address " + (index + 1) : "To Address"
                            }
                            Rectangle {
                                width: 56
                                height: toAddr.height
                                anchors.left: toAddr.right
                                anchors.verticalCenter: toAddr.verticalCenter
                                color: "#C9DEF1"
                                radius: 8
                                QImage {
                                    id: qricoimport
                                    source: "qrc:/Images/Images/QrIco.png"
                                    anchors.centerIn: parent
                                    transformOrigin: Item.Center
                                    scale: qrMouse.pressed ? 0.95 : 1
                                    width: 56
                                    height: 56
                                    MouseArea {
                                        id: qrMouse
                                        hoverEnabled: true
                                        cursorShape: Qt.PointingHandCursor
                                        anchors.fill: parent
                                        onClicked: {
                                            qrscaner.addressRequestIndex = index
                                            qrscaner.visible = true
                                        }
                                    }
                                }
                            }
                        }
                        QTextInputBox {
                            id: amount
                            width: 269
                            heightMin: 56
                            mode: !checkboxSendAll.checked ? eEDIT_MODE : eREADONLY_MODE
                            placeholder.text: "Amount"
                            rightPading: 40
                            validator: (AppSetting.unit === 1) ? intvalidator : doubleValidator
                            QText {
                                color: "#839096"
                                width: 23
                                height: 16
                                anchors {
                                    top: parent.top
                                    topMargin: (amount.textActiveFocus) || (amount.textOutput !== "") ? 28: 20
                                    right: parent.right
                                    rightMargin: 16
                                }
                                text: (AppSetting.unit === 1) ? "sat" : "BTC"
                                font.pixelSize: 16
                                font.family: "Lato"
                            }
                            Row {
                                id: sendAll
                                width: 90
                                height: 24
                                spacing: 8
                                visible: !checkboxMultiDes.checked
                                enabled: visible
                                anchors {
                                    right: parent.right
                                    top: parent.bottom
                                    topMargin: 6
                                }

                                Image {
                                    id: checkboxSendAll
                                    property bool checked: false
                                    anchors.verticalCenter: parent.verticalCenter
                                    width: 24
                                    height: 24
                                    source: checkboxSendAll.checked && visible ? "qrc:/Images/Images/Checked_n.png" : "qrc:/Images/Images/UnChecked_n.png"
                                    MouseArea {
                                        anchors.fill: parent
                                        hoverEnabled: true
                                        cursorShape: Qt.PointingHandCursor
                                        onClicked: {
                                            checkboxSendAll.checked = !checkboxSendAll.checked
                                            if(checkboxSendAll.checked){ destination.itemAt(0).toAmount = AppModel.walletInfo.walletBalance }
                                            else{ destination.itemAt(0).toAmount = "" }
                                        }
                                    }
                                    onVisibleChanged: {
                                        if(visible && index === 0 && checkboxSendAll.checked && destination.count === 1) { destination.itemAt(0).toAmount = AppModel.walletInfo.walletBalance }
                                        else{ destination.itemAt(index).toAmount = "" }
                                    }
                                }

                                QText {
                                    anchors.verticalCenter: checkboxSendAll.verticalCenter
                                    text: "Send All"
                                    color: "#031F2B"
                                    font.pixelSize: 14
                                }
                            }
                        }
                    }
                }

                QButtonMedium {
                    width: 185
                    height: 40
                    radius: 20
                    type: eOUTLINE_NORMAL
                    label: "Add Recipient"
                    visible: checkboxMultiDes.checked
                    enabled: checkboxMultiDes.checked
                    onButtonClicked: {
                        requestAddMultiDestinations(eINCREASE)
                    }
                }

                QTextInputBox {
                    id: memo
                    width: 720
                    heightMin: 96
                    mode: eEDIT_MODE
                    placeholder.text: "Memo"
                    texOutputValignment: memo.heightMin === 56 ? Text.AlignVCenter : Text.AlignTop
                }
            }
        }

        Row {
            spacing: 16
            anchors {
                right: parent.right
                rightMargin: 40
                bottom: parent.bottom
                bottomMargin: 44
            }
            QTextButton {
                width: 120
                height: 48
                label.text: "CANCEL"
                label.font.pixelSize: 16
                type: eTypeB
                onButtonClicked: {
                    QMLHandle.sendEvent(EVT.EVT_SEND_BACK_REQUEST)
                }
            }

            QTextButton {
                width: 260
                height: 48
                enabled: destination.fullfill
                label.text: "NEXT: Review Transaction"
                label.font.pixelSize: 16
                type: eTypeA
                onButtonClicked: {
                    var saved = [];
                    for(var i = 0; i < destination.model; i++){
                        if(destination.itemAt(i).toAddress !== "" && destination.itemAt(i).toAmount !== ""){
                            var savedObj = { "toAddress": destination.itemAt(i).toAddress,
                                             "toAmount" : destination.itemAt(i).toAmount};
                            saved[i] = savedObj
                        }
                    }

                    var destinationObj = { "destinationList"    : saved,
                                           "destinationMemo"    : memo.textOutput};
                    QMLHandle.sendEvent(EVT.EVT_SEND_CREATE_TRANSACTION_REQUEST, destinationObj)
                }
            }
        }

        Rectangle {
            id: qrscaner
            property int addressRequestIndex: -1
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
                    text: "Put your QR code into camera"
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
                        if(qrscaner.addressRequestIndex !== -1 && qrscaner.addressRequestIndex < destination.model){
                            if(result.length > 0){
                                destination.itemAt(qrscaner.addressRequestIndex).toAddress = result[0]
                            }
                            qrscaner.addressRequestIndex = -1
                            qrscaner.visible = false
                        }
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
    }

    Component {
        id: qrcameraComp
        QQrScanner {
            width: 300
            height: 300
        }
    }

    RegExpValidator { id: intvalidator;      regExp: /^[1-9][0-9]*$/ }
    RegExpValidator { id: doubleValidator;   regExp: /^(?:0|[1-9][0-9]*)(\.\d{1,8})?$/ }

    readonly property int eINCREASE: 1
    readonly property int eDECREASE: -1
    function requestAddMultiDestinations(direction){
        // Get data already
        var saved = []
        var oldCount = (direction === eINCREASE) ? destination.model : 1
        for(var i = 0; i < destination.model; i++){
            var savedObj = { "toAddress": destination.itemAt(i).toAddress,
                             "toAmount" : destination.itemAt(i).toAmount};
            saved[i] = savedObj
        }

        // Reload
        if(direction === eINCREASE) {  if(destination.model < destination.destinationMAX) {destination.model++} }
        else{ destination.model = 1 }

        for(var j = 0; j < oldCount; j++){
            var reaload = saved[j]
            destination.itemAt(j).toAddress = reaload["toAddress"]
            destination.itemAt(j).toAmount = reaload["toAmount"]
        }

        if(direction === eINCREASE) {destination.fullfill = false}
        else{ destination.fullfill = (destination.itemAt(0).toAddress !== "") && (destination.itemAt(0).toAmount !== "") }
    }
}
