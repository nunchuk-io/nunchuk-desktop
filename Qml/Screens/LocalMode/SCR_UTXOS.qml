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
import QRCodeItem 1.0
import NUNCHUCKTYPE 1.0
import DataPool 1.0
import "../../Components/origins"
import "../../Components/customizes"
import "../../Components/customizes/Texts"
import "../../Components/customizes/Buttons"
import "../../../localization/STR_QML.js" as STR
QScreen {
    property int checkedCount: 0
    QOnScreenContent {
        width: popupWidth
        height: popupHeight
        anchors.centerIn: parent
        label.text: STR.STR_QML_312
        extraHeader: QText {
            height: 21
            text: "(" + AppModel.walletInfo.walletName + ")"
            color: "#031F2B"
            font.weight: Font.DemiBold
            font.family: "Montserrat"
            font.pixelSize: 14
            anchors.verticalCenter: parent.verticalCenter
        }
        onCloseClicked: {
            if(NUNCHUCKTYPE.CHAT_TAB === AppModel.tabIndex){
                QMLHandle.sendEvent(EVT.EVT_ONLINE_ONS_CLOSE_REQUEST, EVT.STATE_ID_SCR_UTXOS)
            }
            else{
                QMLHandle.sendEvent(EVT.EVT_ONS_CLOSE_REQUEST, EVT.STATE_ID_SCR_UTXOS)
            }
        }
        QButtonTextLink {
            height: 24
            label: STR.STR_QML_059
            anchors {
                left: parent.left
                leftMargin: 40
                bottom: parent.bottom
                bottomMargin: 40
            }
            onButtonClicked: {
                if(NUNCHUCKTYPE.CHAT_TAB === AppModel.tabIndex){
                    QMLHandle.sendEvent(EVT.EVT_UTXO_OUTPUT_BACK_SHARED_WALLET)
                }
                else{
                    QMLHandle.sendEvent(EVT.EVT_UTXOS_BACK_REQUEST)
                }
            }
        }
        Rectangle {
            id: maskofcontent
            width: 720
            height: 497
            radius: 4
            visible: false
        }
        Rectangle {
            id: contentDisplay
            width: 720
            height: 497
            radius: 4
            color: "#FFFFFF"
            anchors.top: parent.top
            anchors.topMargin: 92
            anchors.horizontalCenter: parent.horizontalCenter
            opacity: 0
            Row {
                id: tabparent
                anchors.top: parent.top
                width: parent.width
                property int currentTabIndex: 2
                readonly property int utxo_txid_role    : 0
                readonly property int utxo_vout_role    : 1
                readonly property int utxo_address_role : 2
                readonly property int utxo_amount_role  : 3
                readonly property int utxo_height_role  : 4
                readonly property int utxo_memo_role    : 6

                QTableElement {
                    id: outputtab
                    width: 274
                    height: 32
                    label: STR.STR_QML_313
                    enabled: true
                    isCurrentTab: tabparent.currentTabIndex === 0
                    fontPixelSize: 10
                    horizontalAlignment: Text.AlignLeft
                    onTabClicked: {
                        tabparent.currentTabIndex = 0
                        var sortData = { "sortRole"    : tabparent.utxo_address_role,
                                         "sortOrder"   : sortOrder}
                        QMLHandle.sendEvent(EVT.EVT_UTXOS_SORT_REQUEST, sortData)
                        pagecontrol.currentPage = 1
                        pagecontrol.pageRequest(pagecontrol.currentPage)
                    }
                }

                QTableElement {
                    id: memotab
                    width: 135
                    height: 32
                    label: STR.STR_QML_218
                    enabled: true
                    isCurrentTab: tabparent.currentTabIndex === 1
                    fontPixelSize: 10
                    horizontalAlignment: Text.AlignLeft
                    onTabClicked: {
                        tabparent.currentTabIndex = 1
                        var sortData = { "sortRole"    : tabparent.utxo_memo_role,
                                         "sortOrder"   : sortOrder}
                        QMLHandle.sendEvent(EVT.EVT_UTXOS_SORT_REQUEST, sortData)
                        pagecontrol.currentPage = 1
                        pagecontrol.pageRequest(pagecontrol.currentPage)
                    }
                }

                QTableElement {
                    id: confirmationtab
                    width: 132
                    height: 32
                    label: STR.STR_QML_314
                    enabled: true
                    isCurrentTab: tabparent.currentTabIndex === 2
                    fontPixelSize: 10
                    horizontalAlignment: Text.AlignLeft
                    onTabClicked: {
                        tabparent.currentTabIndex = 2
                        var sortData = { "sortRole"    : tabparent.utxo_height_role,
                                         "sortOrder"   : sortOrder}
                        QMLHandle.sendEvent(EVT.EVT_UTXOS_SORT_REQUEST, sortData)
                        pagecontrol.currentPage = 1
                        pagecontrol.pageRequest(pagecontrol.currentPage)
                    }
                }

                QTableElement {
                    id: amounttab
                    width: 180
                    height: 32
                    label: STR.STR_QML_214
                    enabled: true
                    isCurrentTab: tabparent.currentTabIndex === 3
                    fontPixelSize: 10
                    horizontalAlignment: Text.AlignRight
                    onTabClicked: {
                        tabparent.currentTabIndex = 3
                        var sortData = { "sortRole"    : tabparent.utxo_amount_role,
                                         "sortOrder"   : sortOrder}
                        QMLHandle.sendEvent(EVT.EVT_UTXOS_SORT_REQUEST, sortData)
                        pagecontrol.currentPage = 1
                        pagecontrol.pageRequest(pagecontrol.currentPage)
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
                .arg(STR.STR_QML_315)
                .arg((((currentPage-1) * utxo_lst.rowsOnPage)+1))
                .arg(Math.min(currentPage * utxo_lst.rowsOnPage, utxo_lst.count))
                .arg(STR.STR_QML_057)
                .arg(utxo_lst.count)
            }
            QPageControl {
                id: pagecontrol
                totalPage: Math.floor(utxo_lst.count/utxo_lst.rowsOnPage) + (utxo_lst.count % utxo_lst.rowsOnPage == 0 ? 0 : 1)
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 16
                anchors.right: parent.right
                anchors.rightMargin: 16
                onPageRequest: {
                    pageinfo.currentPage = pageNumber
                    var nextindex = (pageNumber-1) * utxo_lst.rowsOnPage
                    utxo_lst.positionViewAtIndex(nextindex, ListView.Beginning)
                }
            }
        }
        OpacityMask {
            anchors.fill: contentDisplay
            source: contentDisplay
            maskSource: maskofcontent
            cached: true
            QListView {
                id: utxo_lst
                width: 720
                height: (pagecontrol.currentPage !== pagecontrol.totalPage) ? utxo_lst.pageHeight :
                                                                              utxo_lst.count % utxo_lst.rowsOnPage == 0 ? utxo_lst.pageHeight :((utxo_lst.count % utxo_lst.rowsOnPage) * utxo_lst.rowHeight)
                model: AppModel.walletInfo.utxoList
                anchors {
                    top: parent.top
                    topMargin: 32
                }
                clip: true
                interactive: false
                readonly property int pageHeight: 400
                readonly property int rowsOnPage: 5
                readonly property int rowHeight: utxo_lst.pageHeight/utxo_lst.rowsOnPage
                delegate: QUnspentOutputDelegate {
                    width: 720
                    height: utxo_lst.rowHeight
                    unspendWidth: outputtab.width
                    memoWidth: memotab.width
                    confirmedWidth: confirmationtab.width
                    amountWidth: amounttab.width
                    unspentoutput_address: utxo_address
                    unspentoutput_memo: utxo_memo
                    unspentoutput_confirmation: utxo_confirmed//Math.max(0, (AppModel.blockHeight - utxo_height)+1)
                    unspentoutput_amount: utxo_amount
                    utxoSelected: utxo_selected
                    onButtonClicked: {QMLHandle.sendEvent(EVT.EVT_UTXOS_ITEM_SELECTED, index) }
                    onCheckboxClicked : {
                        utxo_selected = !utxo_selected
                        if(utxo_selected) checkedCount++;
                        else checkedCount--;
                    }
                    Component.onCompleted: { if(utxoSelected) {checkedCount++} }
                }
            }
        }
        QTextButton {
            width: 210
            height: 48
            anchors {
                right: parent.right
                rightMargin: 40
                bottom: parent.bottom
                bottomMargin: 40
            }
            label.text: STR.STR_QML_304
            label.font.pixelSize: 16
            type: eTypeE
            enabled: checkedCount > 0
            onButtonClicked: QMLHandle.sendEvent(EVT.EVT_UTXOS_CONSOLIDATE_REQUEST)
        }
    }
}
