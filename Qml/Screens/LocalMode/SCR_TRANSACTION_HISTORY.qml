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
import NUNCHUCKTYPE 1.0
import QRCodeItem 1.0
import "../../Components/origins"
import "../../Components/customizes"
import "../../Components/customizes/Texts"
import "../../Components/customizes/Buttons"
import "../../../localization/STR_QML.js" as STR

QScreen {

    property string myRole: AppModel.walletInfo.myRole
    QOnScreenContent {
        id: contenCenter
        width: popupWidth
        height: popupHeight
        anchors.centerIn: parent
        label.text: STR.STR_QML_278
        extraHeader: QText {
            height: 21
            width: 550
            elide: Text.ElideRight
            text: "(" + AppModel.walletInfo.walletName + ")"
            color: "#031F2B"
            font.family: "Montserrat"
            font.weight: Font.DemiBold
            font.pixelSize: 14
        }
        onCloseClicked: {
            QMLHandle.sendEvent(EVT.EVT_ONS_CLOSE_REQUEST, EVT.STATE_ID_SCR_TRANSACTION_HISTORY)
        }
        Rectangle {
            id: contenthistory
            width: 720
            height: 497
            radius: 4
            color: "#FFFFFF"
            anchors.top: parent.top
            anchors.topMargin: 92
            anchors.horizontalCenter: parent.horizontalCenter
            layer.enabled: true
            layer.effect: OpacityMask {
                maskSource: Rectangle {
                    width: 720
                    height: 497
                    radius: 4
                }
            }
            Row {
                id: tabparent
                anchors.top: parent.top
                width: parent.width
                property int currentTabIndex: 1
                readonly property int transaction_txid_role : 0
                readonly property int transaction_memo_role : 1
                readonly property int transaction_status_role : 2
                readonly property int transaction_subtotal_role : 9
                readonly property int transaction_total_role : 10
                readonly property int transaction_blocktime_role : 13
                readonly property int transaction_height_role : 14
                QTableElement {
                    width: transaction_lst.width*0.15
                    height: 32
                    label: "ADDRESS"
                    fontPixelSize: 10
                    enabled: true
                    isCurrentTab: tabparent.currentTabIndex == 0
                    horizontalAlignment: Text.AlignLeft
                    onTabClicked: {
                        tabparent.currentTabIndex = 0
                        var sortData = { "sortRole"    : tabparent.transaction_txid_role,
                                         "sortOrder"   : sortOrder}
                        QMLHandle.sendEvent(EVT.EVT_TRANSACTION_HISTORY_SORT_REQUEST, sortData)
                        pagecontrol.currentPage = 1
                        pagecontrol.pageRequest(pagecontrol.currentPage)
                    }
                }
                QTableElement {
                    width: transaction_lst.width*0.25
                    height: 32
                    label: STR.STR_QML_279
                    fontPixelSize: 10
                    enabled: true
                    isCurrentTab: tabparent.currentTabIndex == 1
                    horizontalAlignment: Text.AlignLeft
                    onTabClicked: {
                        tabparent.currentTabIndex = 1
                        var sortData = { "sortRole"    : tabparent.transaction_status_role,
                                         "sortOrder"   : sortOrder}
                        QMLHandle.sendEvent(EVT.EVT_TRANSACTION_HISTORY_SORT_REQUEST, sortData)
                        pagecontrol.currentPage = 1
                        pagecontrol.pageRequest(pagecontrol.currentPage)
                    }
                }
                QTableElement {
                    width: transaction_lst.width*0.15
                    height: 32
                    label: "TIME"
                    fontPixelSize: 10
                    enabled: true
                    isCurrentTab: tabparent.currentTabIndex == 2
                    horizontalAlignment: Text.AlignLeft
                    onTabClicked: {
                        tabparent.currentTabIndex = 2
                        var sortData = { "sortRole"    : tabparent.transaction_blocktime_role,
                                         "sortOrder"   : sortOrder}
                        QMLHandle.sendEvent(EVT.EVT_TRANSACTION_HISTORY_SORT_REQUEST, sortData)
                        pagecontrol.currentPage = 1
                        pagecontrol.pageRequest(pagecontrol.currentPage)
                    }
                }
                QTableElement {
                    width: transaction_lst.width*0.20
                    height: 32
                    label: STR.STR_QML_280
                    fontPixelSize: 10
                    enabled: true
                    isCurrentTab: tabparent.currentTabIndex == 3
                    horizontalAlignment: Text.AlignLeft
                    onTabClicked: {
                        tabparent.currentTabIndex = 3
                        var sortData = { "sortRole"    : tabparent.transaction_memo_role,
                                         "sortOrder"   : sortOrder}
                        QMLHandle.sendEvent(EVT.EVT_TRANSACTION_HISTORY_SORT_REQUEST, sortData)
                        pagecontrol.currentPage = 1
                        pagecontrol.pageRequest(pagecontrol.currentPage)
                    }
                }
                QTableElement {
                    width: transaction_lst.width*0.25
                    height: 32
                    label: STR.STR_QML_214
                    fontPixelSize: 10
                    enabled: true
                    isCurrentTab: tabparent.currentTabIndex == 4
                    horizontalAlignment: Text.AlignRight
                    onTabClicked: {
                        tabparent.currentTabIndex = 4
                        var sortData = { "sortRole"    : tabparent.transaction_total_role,
                                         "sortOrder"   : sortOrder}
                        QMLHandle.sendEvent(EVT.EVT_TRANSACTION_HISTORY_SORT_REQUEST, sortData)
                        pagecontrol.currentPage = 1
                        pagecontrol.pageRequest(pagecontrol.currentPage)
                    }
                }
            }

            QListView {
                id: transaction_lst
                width: 720
                height: (pagecontrol.currentPage !== pagecontrol.totalPage) ? 400 : (transaction_lst.count % 10 == 0 ? 400 : ((transaction_lst.count % 10) * 40))
                model: AppModel.walletInfo.transactionHistory
                anchors {
                    top: parent.top
                    topMargin: 32
                }
                clip: true
                interactive: false
                delegate: QTransactionDelegate {
                    width: transaction_lst.width
                    height: 40
                    parentList: transaction_lst
                    addressWidth: width*0.15
                    statusWidth: width*0.25
                    timeWidth: width*0.15
                    memoWidth: width*0.20
                    amountWidth: width*0.25
                    transactionisReceiveTx: transaction_isReceiveTx
                    transactiontxid: transaction_txid
                    transactiondestinationList: transaction_destinationDisp_role
                    transactionstatus: transaction_status
                    transactionMemo:   transaction_memo
                    transactionAmount: (transaction_isReceiveTx ? transaction_subtotal : transaction_total)
                    transactiontotalCurrency: (transaction_isReceiveTx ? transaction_subtotalCurrency : transaction_totalCurrency)
                    confirmation:  Math.max(0, (AppModel.chainTip - transaction_height)+1)
                    transactionDate: transaction_blocktime
                    isFacilitatorAdmin: (myRole === "FACILITATOR_ADMIN")
                    onButtonClicked: {
                        QMLHandle.sendEvent(EVT.EVT_TRANSACTION_INFO_ITEM_SELECTED, transaction_txid)
                    }
                }
            }
            QText {
                id: pageinfo
                property int currentPage: 1
                anchors {
                    right: pagecontrol.left
                    rightMargin: 16
                    verticalCenter: pagecontrol.verticalCenter
                }
                height: 16
                font.family: "Lato"
                font.pixelSize: 12
                color: "#323E4A"
                text: qsTr("%1 %2 - %3 %4 %5")
                .arg(STR.STR_QML_281)
                .arg(((currentPage-1)*10)+1)
                .arg(Math.min(currentPage*10, transaction_lst.count))
                .arg(STR.STR_QML_057)
                .arg(transaction_lst.count)
            }
            QPageControl {
                id: pagecontrol
                totalPage: Math.floor(transaction_lst.count/10) + (transaction_lst.count%10 == 0 ? 0 : 1)
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 16
                anchors.right: parent.right
                anchors.rightMargin: 16
                onPageRequest: {
                    pageinfo.currentPage = pageNumber
                    var nextindex = (pageNumber-1)*10
                    transaction_lst.positionViewAtIndex(nextindex, ListView.Beginning)
                }
            }
        }
    }
}
