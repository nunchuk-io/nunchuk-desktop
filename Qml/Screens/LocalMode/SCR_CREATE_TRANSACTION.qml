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
    readonly property var iconChecked: ["qrc:/Images/Images/Checked_n.png", "qrc:/Images/Images/Checked_d.png"]
    readonly property var iconUnChecked: ["qrc:/Images/Images/UnChecked_n.png", "qrc:/Images/Images/UnChecked_d.png"]
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
                QMLHandle.sendEvent(EVT.EVT_ONS_CLOSE_REQUEST, EVT.STATE_ID_SCR_CREATE_TRANSACTION)
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
                text: (QMLHandle.onsRequester() === EVT.STATE_ID_SCR_TRANSACTION_INFO) ? "Replace-by-Fee" :
                                                                                         (QMLHandle.onsRequester() === EVT.STATE_ID_SCR_CONSOLIDATE_OUTPUT) ? "Consolidate Outputs"
                                                                                                                                                            :"Send Funds"
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

        Rectangle {
            id: bgcontentOverview
            width: 344
            height: 460
            anchors {
                right: parent.right
                rightMargin: 40
                top: notification.bottom
                topMargin: 17
            }
            color: Qt.rgba(255, 255, 255, 0.5)
            border.color: "#9CAEB8"
            border.width: 1
            radius: 8
            Column {
                width: parent.width
                Item {
                    id:overview
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
                    height: bgcontentOverview.height - overview.height
                    flickableDirection: Flickable.VerticalFlick
                    clip: true
                    interactive: contentHeight > flickcontent.height
                    contentHeight: contentDisp.height
                    ScrollBar.vertical: ScrollBar { active: true }
                    Column {
                        id: contentDisp
                        width: parent.width-2
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
                                    leftMargin: 16
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
                                    rightMargin: 16
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
                                    width: 200
                                    text: destination_address
                                    font.pixelSize: 12
                                    wrapMode: Text.WrapAnywhere
                                    color: "#031F2B"
                                    font.family: "Lato"
                                    anchors {
                                        left: parent.left
                                        leftMargin: 16
                                        verticalCenter: parent.verticalCenter
                                    }
                                }
                                QText {
                                    width: 90
                                    height: 16
                                    text: destination_amount + ((AppSetting.unit === 1) ? " sat" : " BTC")
                                    horizontalAlignment: Text.AlignRight
                                    font.pixelSize: 12
                                    color: "#031F2B"
                                    font.family: "Lato"
                                    anchors {
                                        right: parent.right
                                        rightMargin: 16
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
                                    leftMargin: 120
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
                                    rightMargin: 16
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
                                    leftMargin: 120
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
                                    rightMargin: 16
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
                                    leftMargin: 100
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
                                    rightMargin: 16
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
                                    leftMargin: 16
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
                                    rightMargin: 16
                                    verticalCenter: parent.verticalCenter
                                }
                            }
                        }

                        Item {
                            visible: changeinfo.visible
                            width: parent.width
                            height: changeinfo.visible ? 32 : 0
                            QText {
                                width: 200
                                wrapMode: Text.WrapAnywhere
                                text: AppModel.transactionInfo.change.address
                                font.pixelSize: 12
                                color: "#031F2B"
                                font.family: "Lato"
                                anchors {
                                    left: parent.left
                                    leftMargin: 16
                                    verticalCenter: parent.verticalCenter
                                }
                            }
                            QText {
                                width: 90
                                height: 16
                                text: AppModel.transactionInfo.change.amount + ((AppSetting.unit === 1) ? " sat" : " BTC")
                                horizontalAlignment: Text.AlignRight
                                font.pixelSize: 12
                                color: "#031F2B"
                                font.family: "Lato"
                                anchors {
                                    right: parent.right
                                    rightMargin: 16
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
                            clip: true
                            QText {
                                text: "MEMO"
                                font.family: "Lato"
                                font.weight: Font.Bold
                                font.pixelSize: 10
                                color: "#323E4A"
                                anchors {
                                    left: parent.left
                                    leftMargin: 16
                                    top: parent.top
                                    topMargin: 16
                                }
                            }
                            QText {
                                text: AppModel.transactionInfo.memo
                                font.pixelSize: 12
                                color: "#323E4A"
                                anchors {
                                    left: parent.left
                                    leftMargin: 24
                                    top: parent.top
                                    topMargin: 26
                                }
                                width: 346
                                height: 40
                                wrapMode: Text.WrapAnywhere
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
                                color: "#323E4A"
                                font.family: "Lato"
                                font.weight: Font.Bold
                                anchors {
                                    left: parent.left
                                    leftMargin: 16
                                    top: parent.top
                                    topMargin: 16
                                }
                            }
                            QText {
                                id: numbersignaturepending
                                text: "0 / " + AppModel.transactionInfo.m
                                font.pixelSize: 16
                                color: "#031F2B"
                                font.family: "Lato"
                                font.weight: Font.Bold
                                anchors {
                                    left: parent.left
                                    leftMargin: 16
                                    top: parent.top
                                    topMargin: 36
                                }
                                width: 346
                                height: 32
                                wrapMode: Text.WordWrap
                            }

                            Row {
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
                                    color: "#E02247"
                                    anchors.verticalCenter: parent.verticalCenter
                                }

                                QText {
                                    text: "Pending " + AppModel.transactionInfo.m + " Signatures"
                                    font.pixelSize: 12
                                    font.family: "Lato"
                                    color: indicatorStatus.color
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
            height: 24
            spacing: 56
            enabled: !AppModel.walletInfo.walletEscrow && (QMLHandle.onsRequester() !== EVT.STATE_ID_SCR_TRANSACTION_INFO)
            anchors {
                left: parent.left
                leftMargin: 43
                top: notification.bottom
                topMargin: 16
            }

            QText {
                width: 203
                height: 24
                text: "Enable Manual Coin Selection"
                color: "#031F2B"
                font.weight: Font.Bold
                font.pixelSize: 16
                anchors.verticalCenter: parent.verticalCenter
            }

            QSwitch {
                id: manualOutput
                width: 34
                height: 20
                checked: false
                enabled: (QMLHandle.onsRequester() !== EVT.STATE_ID_SCR_TRANSACTION_INFO) && !AppModel.walletInfo.walletEscrow
                anchors.verticalCenter: parent.verticalCenter
            }
        }

        Row {
            id: tabheader
            visible: manualOutput.checked
            property int currentTabIndex: 2
            readonly property int utxo_txid_role    : 0
            readonly property int utxo_vout_role    : 1
            readonly property int utxo_address_role : 2
            readonly property int utxo_amount_role  : 3
            readonly property int utxo_height_role  : 4

            anchors {
                left: parent.left
                leftMargin: 43
                top: notification.bottom
                topMargin: visible ? 50 : 0
            }
            QTableElement {
                id: addresstab
                width: 160
                height: 32
                label: "OUTPUT ADDRESS"
                fontPixelSize: 10
                horizontalAlignment: Text.AlignLeft
                enabled: manualOutput.checked
                isCurrentTab: tabheader.currentTabIndex === 0
                onTabClicked: {
                    tabheader.currentTabIndex = 0
                    var sortData = { "sortRole"    : tabheader.utxo_address_role,
                                     "sortOrder"   : sortOrder}
                    QMLHandle.sendEvent(EVT.EVT_CREATE_TRANSACTION_UTXO_SORT_REQUEST, sortData)
                }
            }
            QTableElement {
                id: conftab
                width: 71
                height: 32
                label: "CONF."
                fontPixelSize: 10
                horizontalAlignment: Text.AlignRight
                enabled: manualOutput.checked
                isCurrentTab: tabheader.currentTabIndex === 1
                iconPading: 0
                onTabClicked: {
                    tabheader.currentTabIndex = 1
                    var sortData = { "sortRole"    : tabheader.utxo_height_role,
                                     "sortOrder"   : sortOrder}
                    QMLHandle.sendEvent(EVT.EVT_CREATE_TRANSACTION_UTXO_SORT_REQUEST, sortData)
                }
            }
            QTableElement {
                id: amnttab
                width: 113
                height: 32
                label: "AMT"
                enabled: manualOutput.checked
                fontPixelSize: 10
                horizontalAlignment: Text.AlignRight
                isCurrentTab: tabheader.currentTabIndex === 2
                onTabClicked: {
                    tabheader.currentTabIndex = 2
                    var sortData = { "sortRole"    : tabheader.utxo_amount_role,
                                     "sortOrder"   : sortOrder}
                    QMLHandle.sendEvent(EVT.EVT_CREATE_TRANSACTION_UTXO_SORT_REQUEST, sortData)
                }
            }
        }

        QListView {
            id: utxos
            width: 344
            spacing: 8
            property int rowHeight: 44
            property int maxHeight: Math.min(148,(rowHeight*2) + 8)
            property int realHeight: utxos.count*rowHeight + (utxos.count-1)*utxos.spacing
            height: visible ? Math.min(maxHeight, realHeight) : 0
            enabled: manualOutput.checked
            visible: manualOutput.checked
            clip: true
            anchors {
                left: parent.left
                leftMargin: 43
                top: tabheader.bottom
                topMargin: visible ? 8 : 0
            }
            model: AppModel.utxoList
            interactive: realHeight > maxHeight
            ScrollBar.vertical: ScrollBar { active: utxos.enabled }
            delegate: Row {
                id: utdl
                width: 344
                height: utxoaddress.height + 5
                spacing: 8

                Component.onCompleted: {if(utdl.height > utxos.rowHeight)  utxos.rowHeight = utdl.height }

                Image {
                    id: checkboxutxo
                    width: 24
                    height: 24
                    source: utxo_selected ? (utxos.enabled ? iconChecked[0] : iconChecked[1]) : (utxos.enabled ? iconUnChecked[0] : iconUnChecked[1])
                    anchors.verticalCenter: parent.verticalCenter
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            utxo_selected = !utxo_selected
                            requestDraftTransaction()
                        }
                    }
                }

                QText {
                    id: utxoaddress
                    width: addresstab.width - 32
                    anchors.verticalCenter: parent.verticalCenter
                    font.family: "Lato"
                    font.pixelSize: 14
                    color: "#031F2B"
                    horizontalAlignment: Text.AlignLeft
                    text: utxo_address
                    wrapMode: Text.WrapAnywhere
                }

                Item {
                    width: conftab.width - 8
                    height: 24
                    anchors.top: utxoaddress.top
                    QText {
                        width: parent.width - 8
                        font.family: "Lato"
                        font.pixelSize: 14
                        color: "#031F2B"
                        horizontalAlignment: Text.AlignRight
                        text: Math.max(0, (AppModel.chainTip - utxo_height)+1)
                    }
                }

                QText {
                    width: amnttab.width - 8
                    anchors.top: utxoaddress.top
                    font.family: "Lato"
                    font.pixelSize: 14
                    color: "#031F2B"
                    horizontalAlignment: Text.AlignRight
                    text: utxo_amount + ((AppSetting.unit === 1) ? " sat" : " BTC")
                }
            }
        }

        Rectangle {
            id: linespace
            width: 344
            height: 1
            color: "#9CAEB8"
            anchors {
                left: parent.left
                leftMargin: 43
                top: tabheader.bottom
                topMargin: 8 + utxos.height + 8
            }
        }

        Column {
            id: optionSelection
            spacing: notification.visible ? 8 : 16
            anchors {
                left: parent.left
                leftMargin: 43
                top: linespace.bottom
                topMargin: notification.visible ? 8 : 16
            }

            QText {
                id: autofeeText
                width: 141
                height: 24
                font.family: "Montserrat"
                font.weight: Font.Bold
                font.pixelSize: 16
                color: "#031F2B"
                text: "Fee Settings"
            }

            Item {
                id: subtract
                width: 300
                height: 24
                property bool selected: ((QMLHandle.onsRequester() === EVT.STATE_ID_SCR_CONSOLIDATE_OUTPUT) || AppModel.walletInfo.walletEscrow) ?
                                            true : AppModel.transactionInfo.subtractFromFeeAmount
                enabled: (QMLHandle.onsRequester() !== EVT.STATE_ID_SCR_TRANSACTION_INFO) &&
                         (QMLHandle.onsRequester() !== EVT.STATE_ID_SCR_CONSOLIDATE_OUTPUT) &&
                         (!AppModel.walletInfo.walletEscrow)
                Image {
                    id: icosubtract
                    source: subtract.selected ? (subtract.enabled ? iconChecked[0]: iconChecked[1]) : (subtract.enabled ? iconUnChecked[0]: iconUnChecked[1])
                    anchors.verticalCenter: parent.verticalCenter
                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor
                        onClicked: {
                            subtract.focus = true
                            subtract.selected = !subtract.selected
                            requestDraftTransaction()
                        }
                    }
                }
                QText {
                    font.family: "Montserrat"
                    font.pixelSize: 14
                    font.weight: Font.DemiBold
                    color: "#031F2B"
                    anchors {
                        left: parent.left
                        leftMargin: 8 + icosubtract.width
                        verticalCenter: parent.verticalCenter
                    }
                    text: "Subtract Fee from Send Amount"
                }

                QTooltip {
                    anchors.right: parent.right
                    toolTip: "The fee will be deducted from the amount being sent. The recipient will receive less bitcoins than you enter in the amount field."
                }
            }

            Item {
                id: manualfee
                width: 284
                height: 24
                property bool selected: (QMLHandle.onsRequester() === EVT.STATE_ID_SCR_TRANSACTION_INFO) ? true : false
                Image {
                    id: manualfeeico
                    source: manualfee.selected ? iconChecked[0] : iconUnChecked[0]
                    anchors.verticalCenter: parent.verticalCenter
                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor
                        enabled: (QMLHandle.onsRequester() === EVT.STATE_ID_SCR_TRANSACTION_INFO) ? false : true
                        onClicked: {
                            manualfee.focus = true
                            manualfee.selected = !manualfee.selected
                            if(!manualfee.selected) {
                                if(feeinput.textOutput !== ""){
                                    requestDraftTransaction()
                                    feeinput.textOutput = "";
                                }
                            }
                            else{
                                feeinput.textOutput = AppModel.transactionInfo.feeRate
                            }
                        }
                    }
                }
                QText {
                    id: manualtit
                    font.family: "Montserrat"
                    font.pixelSize: 14
                    font.weight: Font.DemiBold
                    color: "#031F2B"
                    anchors {
                        left: parent.left
                        leftMargin: 8 + manualfeeico.width
                        verticalCenter: parent.verticalCenter
                    }
                    text: "Use Manual Fee Rate"
                }
                QText {
                    width: 312
                    height: 16
                    anchors {
                        left: parent.left
                        leftMargin: 8 + manualfeeico.width
                        top: manualtit.bottom
                    }
                    text: "You may bump the fee rate later using RBF."
                    font.pixelSize: 12
                    font.family: "Lato"
                    color: "#323E4A"
                }
            }
        }

        QTextInputBox {
            id: feeinput
            width: 344
            heightMin: 56
            anchors {
                left: parent.left
                leftMargin: 40
                top: optionSelection.bottom
                topMargin: 32
            }
            visible: manualfee.selected
            color: Qt.rgba(255, 255, 255, 0.5)
            border.color: "#C9DEF1"
            mode: eEDIT_MODE
            placeholder.text: "FEE RATE"
            errorText.text: "The new fee rate needs to be higher than the original."
            validInput: {
                if(feeinput.textOutput === "") return true
                else if(QMLHandle.onsRequester() !== EVT.STATE_ID_SCR_TRANSACTION_INFO){ return (feeinput.textOutput >= 0.001) }
                else if(feeinput.textOutput >= AppModel.transactionInfo.feeRate) {return true}
                else {return false}
            }

            onTypingFinished: {
                if((feeinput.textOutput !== "")){
                    if(feeinput.validInput) requestDraftTransaction()
                }
            }

            QText {
                color: "#839096"
                width: 52
                height: 16
                anchors {
                    top: parent.top
                    topMargin: (feeinput.textActiveFocus) || (feeinput.textOutput !== "") ? 28: 20
                    right: parent.right
                    rightMargin: 16
                }
                text: "Sat/Byte"
                font.pixelSize: 12
                font.family: "#839096"
            }
        }

        Column {
            id: suggestedFeeRate
            visible: feeinput.visible
            spacing: 8
            anchors {
                left: parent.left
                leftMargin: 40
                top: feeinput.bottom
                topMargin: feeinput.validInput ? 8 : 16
            }
            QText {
                text: "SUGGESTED FEE RATES:"
                font.pixelSize: 10
                font.family: "Lato"
            }

            Row {
                width: 330
                height: 32
                Column {
                    width: 110
                    height: 32
                    QText {
                        width: parent.width
                        height: parent.height/2
                        text: "Priority Rate:"
                        color: "#839096"
                        font.pixelSize: 10
                        font.family: "Lato"
                    }
                    QText {
                        width: parent.width
                        height: parent.height/2
                        text: AppModel.estimateFeePriority + " Sat/Byte"
                        color: "#323E4A"
                        font.pixelSize: 10
                        font.family: "Lato"
                    }
                }
                Column {
                    width: 110
                    height: 32
                    QText {
                        width: parent.width
                        height: parent.height/2
                        text: "Standard Rate:"
                        color: "#839096"
                        font.pixelSize: 10
                        font.family: "Lato"
                    }
                    QText {
                        width: parent.width
                        height: parent.height/2
                        text: AppModel.estimateFeeStandard + " Sat/Byte"
                        color: "#323E4A"
                        font.pixelSize: 10
                        font.family: "Lato"
                    }
                }
                Column {
                    width: 110
                    height: 32
                    QText {
                        width: parent.width
                        height: parent.height/2
                        text: "Economical Rate:"
                        color: "#839096"
                        font.pixelSize: 10
                        font.family: "Lato"
                    }
                    QText {
                        width: parent.width
                        height: parent.height/2
                        text: AppModel.estimateFeeEconomical + " Sat/Byte"
                        color: "#323E4A"
                        font.pixelSize: 10
                        font.family: "Lato"
                    }
                }
            }
        }

        Row {
            spacing: 16
            anchors {
                right: parent.right
                rightMargin: 40
                bottom: parent.bottom
                bottomMargin: 40
            }
            QTextButton {
                width: 179
                height: 48
                label.text: "Save Transaction"
                label.font.pixelSize: 16
                type: eTypeB
                onButtonClicked: {
                    savefileDialog.currentFile = StandardPaths.writableLocation(StandardPaths.DocumentsLocation) + "/" + AppModel.transactionInfo.txid + ".psbt"
                    savefileDialog.open()
                }
            }

            QTextButton {
                width: 227
                height: 48
                label.text: "Create Transaction"
                label.font.pixelSize: 16
                type: eTypeA
                enabled:{
                    if(manualfee.selected){ return ((feeinput.textOutput !== "") && (feeinput.validInput)) }
                    else{ return true }
                }
                onButtonClicked: { requestCreateTransaction() }
            }
        }

        QButtonTextLink {
            width: 203
            height: 24
            label: "BACK TO PREVIOUS"
            anchors {
                left: parent.left
                leftMargin: 40
                bottom: parent.bottom
                bottomMargin: 40
            }
            visible: (QMLHandle.onsRequester() !== EVT.STATE_ID_SCR_TRANSACTION_INFO)
            onButtonClicked: {
                if(QMLHandle.onsRequester() === EVT.STATE_ID_SCR_CONSOLIDATE_OUTPUT){
                    QMLHandle.sendEvent(EVT.EVR_CREATE_TRANSACTION_BACK_UTXO_CONSILIDATE)
                }
                else {
                    QMLHandle.sendEvent(EVT.EVT_CREATE_TRANSACTION_BACK_REQUEST)
                }
            }
        }
    }

    FileDialog {
        id: savefileDialog
        fileMode: FileDialog.SaveFile
        onAccepted: {
            QMLHandle.sendEvent(EVT.EVT_CREATE_TRANSACTION_SAVE_REQUEST, savefileDialog.currentFile)
        }
    }

    function requestDraftTransaction(){
        var msg = { "subtractFromFeeAmout"   : subtract.selected,
                    "feeRate"                : feeinput.textOutput,
                    "manualFee"              : manualfee.selected,
                    "manualOutput"           : manualOutput.checked};
        QMLHandle.sendEvent(EVT.EVT_CREATE_TRANSACTION_MAKE_DRAFT_TX, msg)
    }

    function requestCreateTransaction(){
        var msg = { "subtractFromFeeAmout"   : subtract.selected,
                    "feeRate"                : feeinput.textOutput,
                    "manualFee"              : manualfee.selected,
                    "manualOutput"           : manualOutput.checked};
        QMLHandle.sendEvent(EVT.EVT_CREATE_TRANSACTION_SIGN_REQUEST, msg)
    }
}
