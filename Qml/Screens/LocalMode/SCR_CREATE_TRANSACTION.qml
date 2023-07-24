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
import Qt.labs.platform 1.1
import HMIEVENTS 1.0
import EWARNING 1.0
import NUNCHUCKTYPE 1.0
import QRCodeItem 1.0
import DataPool 1.0
import "../../Components/origins"
import "../../Components/customizes"
import "../../Components/customizes/Chats"
import "../../Components/customizes/Texts"
import "../../Components/customizes/Buttons"
import "../../../localization/STR_QML.js" as STR

QScreen {
    readonly property var iconChecked: ["qrc:/Images/Images/Checked_n.png", "qrc:/Images/Images/Checked_d.png"]
    readonly property var iconUnChecked: ["qrc:/Images/Images/UnChecked_n.png", "qrc:/Images/Images/UnChecked_d.png"]

    QOnScreenContent {
        id: contenCenter
        width: popupWidth
        height: popupHeight
        anchors.centerIn: parent
        label.text: (QMLHandle.onsRequester() === EVT.STATE_ID_SCR_TRANSACTION_INFO)
                    ? STR.STR_QML_209 : (QMLHandle.onsRequester() === EVT.STATE_ID_SCR_CONSOLIDATE_OUTPUT) ? STR.STR_QML_210 : STR.STR_QML_211
        extraHeader:  Row {
            spacing: 8
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
                        text: STR.STR_QML_212
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
        onCloseClicked: {
            if(NUNCHUCKTYPE.CHAT_TAB === AppModel.tabIndex){
                QMLHandle.sendEvent(EVT.EVT_ONLINE_ONS_CLOSE_REQUEST, EVT.STATE_ID_SCR_CREATE_TRANSACTION)
            }
            else{
                QMLHandle.sendEvent(EVT.EVT_ONS_CLOSE_REQUEST, EVT.STATE_ID_SCR_CREATE_TRANSACTION)
            }
        }
        Row {
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: 100
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
                                text: qsTr("%1 (%2)").arg(STR.STR_QML_213).arg(lstDestination.count)
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
                            id: lstDestination
                            model: AppModel.transactionInfo.destinationList
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
                                    text: AppModel.transactionInfo.fee + RoomWalletData.unitValue
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
                                    .arg(AppModel.transactionInfo.feeCurrency)
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
                                    text: AppModel.transactionInfo.total + RoomWalletData.unitValue
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
                                    .arg(AppModel.transactionInfo.totalCurrency)
                                    .arg(AppSetting.currency)
                                    horizontalAlignment: Text.AlignRight
                                }
                            }
                        }
                        Rectangle {
                            id: changeinfo
                            width: parent.width
                            visible: AppModel.transactionInfo.hasChange
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
                                text: AppModel.transactionInfo.change.address
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
                                    text: AppModel.transactionInfo.change.amount + RoomWalletData.unitValue
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
                                    .arg(AppModel.transactionInfo.change.amountCurrency)
                                    .arg(AppSetting.currency)
                                    horizontalAlignment: Text.AlignRight
                                }
                            }
                        }
                        Rectangle {
                            width: parent.width
                            visible: AppModel.transactionInfo.hasChange
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
                            text: AppModel.transactionInfo.memo
                            lineHeightMode: Text.FixedHeight
                            lineHeight: 20
                            font.pixelSize: 16
                            color: "#323E4A"
                            wrapMode: Text.WrapAnywhere
                            anchors.horizontalCenter: parent.horizontalCenter
                        }
                        Rectangle {
                            width: parent.width
                            visible: AppModel.transactionInfo.hasChange
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
                                    text: STR.str_QML_220(AppModel.transactionInfo.m)
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
                            enabled: (QMLHandle.onsRequester() !== EVT.STATE_ID_SCR_TRANSACTION_INFO) && !AppModel.walletInfo.walletEscrow
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
                                            QMLHandle.sendEvent(EVT.EVT_CREATE_TRANSACTION_UTXO_SORT_REQUEST, sortData)
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
                                            QMLHandle.sendEvent(EVT.EVT_CREATE_TRANSACTION_UTXO_SORT_REQUEST, sortData)
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
                                            QMLHandle.sendEvent(EVT.EVT_CREATE_TRANSACTION_UTXO_SORT_REQUEST, sortData)
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
                                visible: manualfeesetting.switchOn && AppModel.transactionInfo.isCpfp
                            }
                        }
                        QSwitchTypeB {
                            id: manualfeesetting
                            width: 64
                            height: 36
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.right: parent.right
                            enabled: (QMLHandle.onsRequester() !== EVT.STATE_ID_SCR_TRANSACTION_INFO) && (QMLHandle.onsRequester() !== EVT.STATE_ID_SCR_CONSOLIDATE_OUTPUT)
                            switchOn: (QMLHandle.onsRequester() === EVT.STATE_ID_SCR_TRANSACTION_INFO) || (QMLHandle.onsRequester() === EVT.STATE_ID_SCR_CONSOLIDATE_OUTPUT)
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
                            property bool selected: ((QMLHandle.onsRequester() === EVT.STATE_ID_SCR_CONSOLIDATE_OUTPUT) || AppModel.walletInfo.walletEscrow) ?
                                                        true : AppModel.transactionInfo.subtractFromFeeAmount
                            enabled: (QMLHandle.onsRequester() !== EVT.STATE_ID_SCR_TRANSACTION_INFO) &&
                                     (QMLHandle.onsRequester() !== EVT.STATE_ID_SCR_CONSOLIDATE_OUTPUT) &&
                                     (!AppModel.walletInfo.walletEscrow)

                            QText {
                                z:1
                                id: textsubtractfee
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
                            property bool selected: (QMLHandle.onsRequester() === EVT.STATE_ID_SCR_TRANSACTION_INFO) ? true : false
                            QText {
                                id: textmanualfee
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
                                errorText.text: STR.STR_QML_230.arg((QMLHandle.onsRequester() === EVT.STATE_ID_SCR_TRANSACTION_INFO) ?
                                                                                                             STR.STR_QML_231.arg(AppModel.transactionInfo.feeRate):
                                                                                                             AppModel.minFee)
                                validInput: {
                                    if(feeinput.textOutput === "") {return true}
                                    else{
                                        if(QMLHandle.onsRequester() === EVT.STATE_ID_SCR_TRANSACTION_INFO){
                                            return (parseFloat(feeinput.textOutput) >= parseFloat(AppModel.transactionInfo.feeRate))
                                        }
                                        else { return (parseFloat(feeinput.textOutput) >= AppModel.minFee) }
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
                                text: STR.STR_QML_836.arg(AppModel.transactionInfo.packageFeeRate)
                                visible: AppModel.transactionInfo.isCpfp
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
        QButtonTextLink {
            height: 48
            label: STR.STR_QML_059
            fontPixelSize: 16
            anchors {
                left: parent.left
                leftMargin: 36
                bottom: parent.bottom
                bottomMargin: 36
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
        QTextButton {
            width: 265
            height: 48
            label.text: STR.STR_QML_235
            label.font.pixelSize: 16
            label.font.family: "Lato"
            type: eTypeE
            anchors {
                right: parent.right
                rightMargin: 36
                bottom: parent.bottom
                bottomMargin: 36
            }
            enabled:{
                if(manualfee.selected){ return (feeinput.textOutput !== "") }
                else{ return true }
            }
            onButtonClicked: {
                if(feeinput.validInput){
                    requestCreateTransaction()
                }
                else{
                    confirmLowFee.open()
                }
            }
        }
    }
    Popup {
        id: createTxBusyBox
        width: parent.width
        height: parent.height
        modal: true
        focus: true
        background: Item{}
        Rectangle {
            id: boxmask
            width: 300
            height: 196
            radius: 24
            color: "#FFFFFF"
            anchors.centerIn: parent
            layer.enabled: true
            layer.effect: OpacityMask {
                maskSource: Rectangle {
                    width: 300
                    height: 196
                    radius: 24
                }
            }
            Column {
                spacing: 12
                anchors.centerIn: parent
                QBusyIndicator {
                    width: 70
                    height: 70
                    anchors.horizontalCenter: parent.horizontalCenter
                }
                QText {
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.family: "Lato"
                    font.pixelSize: 16
                    font.weight: Font.Bold
                    text: STR.STR_QML_236
                }
                QText {
                    width: 252
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.family: "Lato"
                    font.pixelSize: 16
                    text: STR.STR_QML_122
                    wrapMode: Text.WordWrap
                    horizontalAlignment: Text.AlignHCenter
                }
            }
        }
        DropShadow {
            anchors.fill: boxmask
            horizontalOffset: 3
            verticalOffset: 5
            spread: 0
            radius: 8
            samples: 30
            color: "#aa000000"
            source: boxmask
        }
    }
    QConfirmYesNoPopup {
        id: confirmLowFee
        contentText: STR.STR_QML_809
        onConfirmNo: close()
        onConfirmYes: {
            close()
            requestCreateTransaction()
        }
    }
    Timer {
        id: timerCreateTx
        property var destinationData
        interval: 500
        onTriggered: {
            QMLHandle.sendEvent(EVT.EVT_CREATE_TRANSACTION_SIGN_REQUEST, timerCreateTx.destinationData)
            createTxBusyBox.close()
        }
    }
    function requestDraftTransaction(){
        var msg = { "subtractFromFeeAmout"   : subtract.selected,
                    "feeRate"                : feeinput.textOutput,
                    "manualFee"              : manualfee.selected,
                    "manualOutput"           : manualOutput.switchOn};
        QMLHandle.sendEvent(EVT.EVT_CREATE_TRANSACTION_MAKE_DRAFT_TX, msg)
    }
    function requestCreateTransaction(){
        var msg = { "subtractFromFeeAmout"   : subtract.selected,
                    "feeRate"                : feeinput.textOutput,
                    "manualFee"              : manualfee.selected,
                    "manualOutput"           : manualOutput.switchOn};
        createTxBusyBox.open()
        timerCreateTx.destinationData = msg
        timerCreateTx.restart()
    }
}
