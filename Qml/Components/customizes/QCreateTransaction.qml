/**************************************************************************
 * This file is part of the Nunchuk software (https://nunchuk.io/)        *
 * Copyright (C) 2020-2022 Enigmo								          *
 * Copyright (C) 2022 Nunchuk								              *
 *                                                                        *
 * This program is free software; you can redistribute it and/or          *
 * modify it under the terms of the GNU General Public License            *
 * as published by the Free Software Foundation; either version 3         *
 * of the License, or (at your option) any later version.                 *
 *                                                                        *
 * This program is distributed in the hope that it will be useful,        *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 * GNU General Public License for more details.                           *
 *                                                                        *
 * You should have received a copy of the GNU General Public License      *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.  *
 *                                                                        *
 **************************************************************************/
import QtQuick 2.4
import QtQuick.Controls 2.3
import QtGraphicalEffects 1.12
import HMIEVENTS 1.0
import EWARNING 1.0
import NUNCHUCKTYPE 1.0
import DataPool 1.0
import "../../Components/origins"
import "../../Components/customizes"
import "../../Components/customizes/Chats"
import "../../Components/customizes/Texts"
import "../../Components/customizes/Buttons"
import "../../../localization/STR_QML.js" as STR

Item {
    readonly property var iconChecked: ["qrc:/Images/Images/Checked_n.png", "qrc:/Images/Images/Checked_d.png"]
    readonly property var iconUnChecked: ["qrc:/Images/Images/UnChecked_n.png", "qrc:/Images/Images/UnChecked_d.png"]
    property var transactionInfo
    signal requestSort(var sortData)
    signal signalDraftTransaction(var msg)
    signal signalCreateTransaction(var msg)
    property bool pendingSignatureShow: true
    property alias manualFeeValid: feeinput.validInput
    Row {
        spacing: 30
        Rectangle {
            width: 350
            height: 480
            radius: 12
            border.color: "#EAEAEA"
            color: "#FFFFFF"
            layer.enabled: true
            layer.effect: OpacityMask {
                maskSource: Rectangle {
                    width: 350
                    height: 480
                    radius: 12
                }
            }
            Flickable {
                anchors.fill: parent
                flickableDirection: Flickable.VerticalFlick
                clip: true
                interactive: contentHeight > height
                contentHeight: contentDisp.height
                ScrollBar.vertical: ScrollBar { active: true }
                Column {
                    id: contentDisp
                    width: parent.width
                    spacing: 12
                    anchors.horizontalCenter: parent.horizontalCenter
                    Rectangle {
                        width: parent.width
                        height: 32
                        color: "#F5F5F5"
                        QText {
                            text: STR.STR_QML_213
                            font.family: "Lato"
                            font.weight: Font.Bold
                            font.pixelSize: 12
                            color: "#323E4A"
                            anchors {
                                left: parent.left
                                leftMargin: 16
                                verticalCenter: parent.verticalCenter
                            }
                        }
                        QText {
                            text: STR.STR_QML_214
                            font.family: "Lato"
                            font.weight: Font.Bold
                            font.pixelSize: 12
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
                        model: transactionInfo.destinationList
                        width: parent.width
                        Row {
                            width: parent.width - 24
                            anchors.horizontalCenter: parent.horizontalCenter
                            spacing: 12
                            QText {
                                width: 192
                                lineHeightMode: Text.FixedHeight
                                lineHeight: 20
                                wrapMode: Text.WrapAnywhere
                                font.pixelSize: 16
                                font.weight: Font.Bold
                                color: "#031F2B"
                                font.family: "Lato"
                                text: destination_address
                            }
                            Column {
                                width: 122
                                spacing: 4
                                QText {
                                    width: parent.width
                                    lineHeightMode: Text.FixedHeight
                                    lineHeight: 20
                                    wrapMode: Text.WrapAnywhere
                                    font.pixelSize: 16
                                    font.weight: Font.Bold
                                    color: "#031F2B"
                                    font.family: "Lato"
                                    text: destination_amount + RoomWalletData.unitValue
                                    horizontalAlignment: Text.AlignRight
                                }
                                QText {
                                    width: parent.width
                                    lineHeightMode: Text.FixedHeight
                                    lineHeight: 16
                                    wrapMode: Text.WrapAnywhere
                                    font.pixelSize: 12
                                    color: "#031F2B"
                                    font.family: "Lato"
                                    text: qsTr("%1%2 %3")
                                    .arg(AppSetting.currencySymbol)
                                    .arg(model.destination_amount_currency)
                                    .arg(AppSetting.currency)
                                    horizontalAlignment: Text.AlignRight
                                }
                            }
                        }
                    }
                    Rectangle { width: parent.width; height: 1; color: "#F5F5F5"}
                    Row {
                        width: parent.width - 24
                        anchors.horizontalCenter: parent.horizontalCenter
                        spacing: 12
                        z:1
                        Item {
                            width: 192
                            height: 24
                            z: 1
                            Row {
                                spacing: 6
                                QLato {
                                    width: 97
                                    font.weight: Font.Bold
                                    text: STR.STR_QML_215
                                }
                                QTooltip {
                                    tipWidth: 200
                                    toolTip: STR.STR_QML_807
                                    source: "qrc:/Images/Images/OnlineMode/help_outline_24px.svg"
                                }
                            }
                        }
                        Column {
                            width: 122
                            spacing: 4
                            QText {
                                width: parent.width
                                lineHeightMode: Text.FixedHeight
                                lineHeight: 20
                                wrapMode: Text.WrapAnywhere
                                font.pixelSize: 16
                                font.weight: Font.Bold
                                color: "#031F2B"
                                font.family: "Lato"
                                text: transactionInfo.fee + RoomWalletData.unitValue
                                horizontalAlignment: Text.AlignRight
                            }
                            QText {
                                width: parent.width
                                lineHeightMode: Text.FixedHeight
                                lineHeight: 16
                                wrapMode: Text.WrapAnywhere
                                font.pixelSize: 12
                                color: "#031F2B"
                                font.family: "Lato"
                                text: qsTr("%1%2 %3")
                                .arg(AppSetting.currencySymbol)
                                .arg(transactionInfo.feeCurrency)
                                .arg(AppSetting.currency)
                                horizontalAlignment: Text.AlignRight
                            }
                        }
                    }
                    Row {
                        width: parent.width - 24
                        anchors.horizontalCenter: parent.horizontalCenter
                        spacing: 12
                        QText {
                            width: 192
                            lineHeightMode: Text.FixedHeight
                            lineHeight: 20
                            wrapMode: Text.WrapAnywhere
                            font.pixelSize: 16
                            font.weight: Font.Bold
                            color: "#031F2B"
                            font.family: "Lato"
                            text: STR.STR_QML_216
                        }
                        Column {
                            width: 122
                            spacing: 4
                            QText {
                                width: parent.width
                                lineHeightMode: Text.FixedHeight
                                lineHeight: 20
                                wrapMode: Text.WrapAnywhere
                                font.pixelSize: 16
                                font.weight: Font.Bold
                                color: "#031F2B"
                                font.family: "Lato"
                                text: transactionInfo.total + RoomWalletData.unitValue
                                horizontalAlignment: Text.AlignRight
                            }
                            QText {
                                width: parent.width
                                lineHeightMode: Text.FixedHeight
                                lineHeight: 16
                                wrapMode: Text.WrapAnywhere
                                font.pixelSize: 12
                                color: "#031F2B"
                                font.family: "Lato"
                                text: qsTr("%1%2 %3")
                                .arg(AppSetting.currencySymbol)
                                .arg(transactionInfo.totalCurrency)
                                .arg(AppSetting.currency)
                                horizontalAlignment: Text.AlignRight
                            }
                        }
                    }
                    Rectangle {
                        id: changeinfo
                        width: parent.width
                        visible: transactionInfo.hasChange
                        height: changeinfo.visible ? 32 : 0
                        color: "#F5F5F5"
                        QText {
                            text: STR.STR_QML_217
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
                            text: STR.STR_QML_214
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
                    Row {
                        visible: changeinfo.visible
                        width: parent.width - 24
                        anchors.horizontalCenter: parent.horizontalCenter
                        spacing: 12
                        QText {
                            width: 192
                            lineHeightMode: Text.FixedHeight
                            lineHeight: 20
                            wrapMode: Text.WrapAnywhere
                            font.pixelSize: 16
                            font.weight: Font.Bold
                            color: "#031F2B"
                            font.family: "Lato"
                            text: transactionInfo.change.address
                        }
                        Column {
                            width: 122
                            spacing: 4
                            QText {
                                width: parent.width
                                lineHeightMode: Text.FixedHeight
                                lineHeight: 20
                                wrapMode: Text.WrapAnywhere
                                font.pixelSize: 16
                                font.weight: Font.Bold
                                color: "#031F2B"
                                font.family: "Lato"
                                text: transactionInfo.change.amount + RoomWalletData.unitValue
                                horizontalAlignment: Text.AlignRight
                            }
                            QText {
                                width: parent.width
                                lineHeightMode: Text.FixedHeight
                                lineHeight: 16
                                wrapMode: Text.WrapAnywhere
                                font.pixelSize: 12
                                color: "#031F2B"
                                font.family: "Lato"
                                text: qsTr("%1%2 %3")
                                .arg(AppSetting.currencySymbol)
                                .arg(transactionInfo.change.amountCurrency)
                                .arg(AppSetting.currency)
                                horizontalAlignment: Text.AlignRight
                            }
                        }
                    }
                    Rectangle {
                        width: parent.width
                        visible: transactionInfo.hasChange
                        height: 32
                        color: "#F5F5F5"
                        QText {
                            text: STR.STR_QML_218
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
                    }
                    QText {
                        width: parent.width - 24
                        text: transactionInfo.memo
                        lineHeightMode: Text.FixedHeight
                        lineHeight: 20
                        font.pixelSize: 16
                        color: "#323E4A"
                        wrapMode: Text.WrapAnywhere
                        anchors.horizontalCenter: parent.horizontalCenter
                    }
                    Rectangle {
                        width: parent.width
                        visible: transactionInfo.hasChange
                        height: 32
                        color: "#F5F5F5"
                        QText {
                            text: STR.STR_QML_219
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
                    }
                    Item {
                        width: parent.width
                        height: 64
                        visible: pendingSignatureShow
                        QText {
                            id: numbersignaturepending
                            text: "0 / " + transactionInfo.m
                            font.pixelSize: 16
                            color: "#031F2B"
                            font.family: "Lato"
                            font.weight: Font.Bold
                            anchors {
                                left: parent.left
                                leftMargin: 16
                                verticalCenter: parent.verticalCenter
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
                                text: STR.str_QML_220(transactionInfo.m)
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
        Item {
            width: 350
            height: 480
            Column {
                anchors.fill: parent
                spacing: 12
                Item {
                    width: parent.width
                    height: 36
                    QText {
                        text: STR.STR_QML_221
                        color: "#031F2B"
                        font.weight: Font.Bold
                        font.pixelSize: 16
                        font.family: "Lato"
                        anchors.left: parent.left
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    QSwitchTypeB {
                        id: manualOutput
                        width: 64
                        height: 36
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.right: parent.right
                        enabled: !AppModel.walletInfo.walletEscrow
                    }
                }
                Rectangle {
                    visible: manualOutput.switchOn
                    width: parent.width
                    height: 172
                    radius: 12
                    border.color: "#EAEAEA"
                    color: "#FFFFFF"
                    layer.enabled: true
                    layer.effect: OpacityMask {
                        maskSource: Rectangle {
                            width: 350
                            height: 172
                            radius: 12
                        }
                    }

                    Flickable {
                        anchors.fill: parent
                        flickableDirection: Flickable.VerticalFlick
                        clip: true
                        interactive: contentHeight > height
                        contentHeight: utxoContentDisp.height
                        ScrollBar.vertical: ScrollBar { active: true }
                        Column {
                            id: utxoContentDisp
                            width: parent.width
                            Row {
                                id: tabheader
                                property int currentTabIndex: 2
                                readonly property int utxo_txid_role    : 0
                                readonly property int utxo_vout_role    : 1
                                readonly property int utxo_address_role : 2
                                readonly property int utxo_amount_role  : 3
                                readonly property int utxo_height_role  : 4
                                QTableElement {
                                    id: addresstab
                                    width: 160
                                    height: 32
                                    label: STR.STR_QML_222
                                    fontPixelSize: 10
                                    horizontalAlignment: Text.AlignLeft
                                    isCurrentTab: tabheader.currentTabIndex === 0
                                    onTabClicked: {
                                        tabheader.currentTabIndex = 0
                                        var sortData = { "sortRole"    : tabheader.utxo_address_role,
                                                         "sortOrder"   : sortOrder}
                                        requestSort(sortData)
                                    }
                                }
                                QTableElement {
                                    id: conftab
                                    width: 50
                                    height: 32
                                    label: STR.STR_QML_223
                                    fontPixelSize: 10
                                    horizontalAlignment: Text.AlignRight
                                    isCurrentTab: tabheader.currentTabIndex === 1
                                    iconPading: 0
                                    onTabClicked: {
                                        tabheader.currentTabIndex = 1
                                        var sortData = { "sortRole"    : tabheader.utxo_height_role,
                                                         "sortOrder"   : sortOrder}
                                        requestSort(sortData)
                                    }
                                }
                                QTableElement {
                                    id: amnttab
                                    width: 140
                                    height: 32
                                    label: STR.STR_QML_224
                                    fontPixelSize: 10
                                    horizontalAlignment: Text.AlignRight
                                    isCurrentTab: tabheader.currentTabIndex === 2
                                    enabled: true
                                    onTabClicked: {
                                        tabheader.currentTabIndex = 2
                                        var sortData = { "sortRole"    : tabheader.utxo_amount_role,
                                                         "sortOrder"   : sortOrder}
                                        requestSort(sortData)
                                    }
                                }
                            }
                            QListView {
                                id: utxos
                                width: parent.width
                                height: 132
                                spacing: 12
                                clip: true
                                model: AppModel.utxoList
                                ScrollBar.vertical: ScrollBar { active: utxos.enabled }
                                delegate: Row {
                                    spacing: 0
                                    width: utxos.width
                                    height: 28
                                    Item {
                                        width: addresstab.width
                                        height: parent.height
                                        QImage {
                                            id: checkboxutxo
                                            width: 24
                                            height: 24
                                            source: utxo_selected ? (utxos.enabled ? iconChecked[0] : iconChecked[1]) : (utxos.enabled ? iconUnChecked[0] : iconUnChecked[1])
                                            anchors.verticalCenter: parent.verticalCenter
                                            anchors.left: parent.left
                                            anchors.leftMargin: 12
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
                                            width: 112
                                            anchors.verticalCenter: parent.verticalCenter
                                            anchors.left: checkboxutxo.right
                                            anchors.leftMargin: 8
                                            font.family: "Lato"
                                            font.pixelSize: 14
                                            color: "#031F2B"
                                            horizontalAlignment: Text.AlignLeft
                                            elide: Text.ElideRight
                                            text: utxo_address
                                        }
                                    }
                                    Item {
                                        width: conftab.width
                                        height: parent.height
                                        QText {
                                            anchors.fill: parent
                                            font.family: "Lato"
                                            font.pixelSize: 14
                                            color: "#031F2B"
                                            horizontalAlignment: Text.AlignRight
                                            text: utxo_confirmed//Math.max(0, (AppModel.chainTip - utxo_height)+1)
                                        }
                                    }
                                    Item {
                                        width: amnttab.width
                                        height: parent.height
                                        QText {
                                            width: parent.width - 12
                                            height: parent.height
                                            anchors.right: parent.right
                                            anchors.rightMargin: 12
                                            font.family: "Lato"
                                            font.pixelSize: 14
                                            color: "#031F2B"
                                            horizontalAlignment: Text.AlignRight
                                            text: utxo_amount + RoomWalletData.unitValue
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                Item {
                    width: parent.width
                    height: 36
                    Column {
                        spacing: 4
                        anchors.left: parent.left
                        anchors.verticalCenter: parent.verticalCenter
                        QText {
                            text: STR.STR_QML_225
                            color: "#031F2B"
                            font.weight: Font.Bold
                            font.pixelSize: 16
                            font.family: "Lato"
                        }
                        QText {
                            text: STR.STR_QML_895
                            width: 250
                            wrapMode: Text.WordWrap
                            color: "#595959"
                            font.pixelSize: 12
                            font.family: "Lato"
                            verticalAlignment: Text.AlignVCenter
                            visible: manualfeesetting.switchOn && transactionInfo.isCpfp
                        }
                    }
                    QSwitchTypeB {
                        id: manualfeesetting
                        width: 64
                        height: 36
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.right: parent.right
                        enabled: false
                        switchOn: true
                    }
                }
                Column {
                    visible: manualfeesetting.switchOn
                    width: parent.width
                    spacing: 12
                    Item {
                        id: subtract
                        width: parent.width
                        height: 28
                        z: manualfee.z + 1
                        property bool selected: (AppModel.walletInfo.walletEscrow) ?
                                                    true : transactionInfo.subtractFromFeeAmount
                        QLato {
                            z:1
                            anchors.left: parent.left
                            anchors.verticalCenter: parent.verticalCenter
                            text: STR.STR_QML_226
                            QTooltip {
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.left: parent.right
                                anchors.leftMargin: 8
                                toolTip: STR.STR_QML_227
                                source: "qrc:/Images/Images/OnlineMode/help_outline_24px.svg"
                            }
                        }
                        QImage {
                            id: icosubtract
                            width: 24
                            height: 24
                            source: subtract.selected ? (subtract.enabled ? iconChecked[0]: iconChecked[1]) : (subtract.enabled ? iconUnChecked[0]: iconUnChecked[1])
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.right: parent.right
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
                    }
                    Item {
                        id: manualfee
                        width: parent.width
                        height: 28
                        property bool selected: false
                        QText {
                            font.family: "Lato"
                            font.pixelSize: 16
                            color: "#031F2B"
                            anchors.left: parent.left
                            anchors.verticalCenter: parent.verticalCenter
                            text: STR.STR_QML_228
                        }
                        QImage {
                            id: manualfeeico
                            width: 24
                            height: 24
                            source: manualfee.selected ? iconChecked[0] : iconUnChecked[0]
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.right: parent.right
                            MouseArea {
                                anchors.fill: parent
                                hoverEnabled: true
                                cursorShape: Qt.PointingHandCursor
                                enabled: true
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
                                        feeinput.textOutput = transactionInfo.feeRate
                                    }
                                }
                            }
                        }
                    }
                    Column {
                        visible: manualfee.selected
                        width: parent.width
                        spacing: 4
                        QTextInputBox {
                            id: feeinput
                            width: 280
                            heightMin: 56
                            visible: manualfee.selected
                            color: Qt.rgba(255, 255, 255, 0.5)
                            border.color: "#C9DEF1"
                            mode: eEDIT_MODE
                            placeholder.text: "FEE RATE"
                            errorText.text: STR.STR_QML_230.arg(STR.STR_QML_231.arg(transactionInfo.feeRate))
                            validInput: {
                                if(feeinput.textOutput === "") {return true}
                                else{
                                    return (parseFloat(feeinput.textOutput) >= parseFloat(transactionInfo.feeRate))
                                }
                            }
                            onTypingFinished: {
                                if((feeinput.textOutput !== "")){
                                    if(feeinput.validInput) requestDraftTransaction()
                                }
                            }
                            QText {
                                color: "#031F2B"
                                width: 52
                                height: 16
                                anchors {
                                    verticalCenter: parent.verticalCenter
                                    left: parent.right
                                    leftMargin: 12
                                }
                                text: "sat/vB"
                                font.pixelSize: 12
                                font.family: "Lato"
                            }
                        }
                        Item {width: parent.width; height: 12; visible: !feeinput.validInput}
                        QLato {
                            id: cpfptext
                            color: "#595959"
                            height: 28
                            text: STR.STR_QML_836.arg(transactionInfo.packageFeeRate)
                            visible: transactionInfo.isCpfp
                        }
                    }
                    Column {
                        visible: manualfee.selected
                        spacing: 4
                        QText {
                            color: "#031F2B"
                            text: "Processing speed"
                            font.pixelSize: 16
                            font.family: "Lato"
                            height: 16
                        }
                        Row {
                            spacing: 12
                            Column {
                                QText {
                                    width: 100
                                    height: 16
                                    text: STR.STR_QML_232
                                    color: "#839096"
                                    font.pixelSize: 12
                                    font.family: "Lato"
                                }
                                QText {
                                    width: 100
                                    height: 16
                                    text: AppModel.fastestFee + " sat/vB"
                                    color: "#323E4A"
                                    font.pixelSize: 12
                                    font.family: "Lato"
                                }
                            }
                            Column {
                                QText {
                                    width: 100
                                    height: 16
                                    text: STR.STR_QML_233
                                    color: "#839096"
                                    font.pixelSize: 12
                                    font.family: "Lato"
                                }
                                QText {
                                    width: 100
                                    height: 16
                                    text: AppModel.halfHourFee + " sat/vB"
                                    color: "#323E4A"
                                    font.pixelSize: 12
                                    font.family: "Lato"
                                }
                            }
                            Column {
                                QText {
                                    width: 100
                                    height: 16
                                    text: STR.STR_QML_234
                                    color: "#839096"
                                    font.pixelSize: 12
                                    font.family: "Lato"
                                }
                                QText {
                                    width: 100
                                    height: 16
                                    text: AppModel.hourFee + " sat/vB"
                                    color: "#323E4A"
                                    font.pixelSize: 12
                                    font.family: "Lato"
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    function requestDraftTransaction(){
        var msg = { "subtractFromFeeAmout"   : subtract.selected,
                    "feeRate"                : feeinput.textOutput,
                    "manualFee"              : manualfee.selected,
                    "manualOutput"           : manualOutput.switchOn};
        signalDraftTransaction(msg)
    }
    function requestCreateTransaction(){
        var msg = { "subtractFromFeeAmout"   : subtract.selected,
                    "feeRate"                : feeinput.textOutput,
                    "manualFee"              : manualfee.selected,
                    "manualOutput"           : manualOutput.switchOn};
        signalCreateTransaction(msg)
    }
}
