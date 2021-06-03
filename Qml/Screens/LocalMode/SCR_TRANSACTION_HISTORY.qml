import QtQuick 2.4
import QtQuick.Controls 2.3
import QtGraphicalEffects 1.12
import Qt.labs.platform 1.1
import HMIEVENTS 1.0
import NUNCHUCKTYPE 1.0
import QRCodeItem 1.0
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
            text: "Full Transaction History"
            color: "#031F2B"
            font.family: "Montserrat"
            font.weight: Font.ExtraBold
            font.pixelSize: 24
        }

        QText {
            height: 21
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
                QMLHandle.sendEvent(EVT.EVT_ONS_CLOSE_REQUEST, EVT.STATE_ID_SCR_TRANSACTION_HISTORY)
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
            id: contenthistory
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
                property int currentTabIndex: 1
                readonly property int transaction_txid_role : 0
                readonly property int transaction_memo_role : 1
                readonly property int transaction_status_role : 2
                readonly property int transaction_subtotal_role : 9
                readonly property int transaction_total_role : 10
                readonly property int transaction_blocktime_role : 12
                readonly property int transaction_height_role : 13
                QTableElement {
                    width: 166
                    height: 32
                    label: "STATUS"
                    fontPixelSize: 10
                    enabled: true
                    isCurrentTab: tabparent.currentTabIndex == 0
                    horizontalAlignment: Text.AlignLeft
                    onTabClicked: {
                        tabparent.currentTabIndex = 0
                        var sortData = { "sortRole"    : tabparent.transaction_status_role,
                                         "sortOrder"   : sortOrder}
                        QMLHandle.sendEvent(EVT.EVT_TRANSACTION_HISTORY_SORT_REQUEST, sortData)
                        pagecontrol.currentPage = 1
                        pagecontrol.pageRequest(pagecontrol.currentPage)
                    }
                }
                QTableElement {
                    width: 123
                    height: 32
                    label: "TIME"
                    fontPixelSize: 10
                    enabled: true
                    isCurrentTab: tabparent.currentTabIndex == 1
                    horizontalAlignment: Text.AlignLeft
                    onTabClicked: {
                        tabparent.currentTabIndex = 1
                        var sortData = { "sortRole"    : tabparent.transaction_blocktime_role,
                                         "sortOrder"   : sortOrder}
                        QMLHandle.sendEvent(EVT.EVT_TRANSACTION_HISTORY_SORT_REQUEST, sortData)
                        pagecontrol.currentPage = 1
                        pagecontrol.pageRequest(pagecontrol.currentPage)
                    }
                }
                QTableElement {
                    width: 272
                    height: 32
                    label: "TRANSACTION MEMO"
                    fontPixelSize: 10
                    enabled: true
                    isCurrentTab: tabparent.currentTabIndex == 2
                    horizontalAlignment: Text.AlignLeft
                    onTabClicked: {
                        tabparent.currentTabIndex = 2
                        var sortData = { "sortRole"    : tabparent.transaction_memo_role,
                                         "sortOrder"   : sortOrder}
                        QMLHandle.sendEvent(EVT.EVT_TRANSACTION_HISTORY_SORT_REQUEST, sortData)
                        pagecontrol.currentPage = 1
                        pagecontrol.pageRequest(pagecontrol.currentPage)
                    }
                }
                QTableElement {
                    width: 158
                    height: 32
                    label: "AMOUNT"
                    fontPixelSize: 10
                    enabled: true
                    isCurrentTab: tabparent.currentTabIndex == 3
                    horizontalAlignment: Text.AlignRight
                    onTabClicked: {
                        tabparent.currentTabIndex = 3
                        var sortData = { "sortRole"    : tabparent.transaction_total_role,
                                         "sortOrder"   : sortOrder}
                        QMLHandle.sendEvent(EVT.EVT_TRANSACTION_HISTORY_SORT_REQUEST, sortData)
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
                text: "Transaction Data " + (((currentPage-1)*10)+1) + " - " + Math.min(currentPage*10, transaction_lst.count) + " of " + transaction_lst.count
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

        OpacityMask {
            anchors.fill: contenthistory
            source: contenthistory
            maskSource: maskofcontent
            cached: true

            QListView {
                id: transaction_lst
                width: 720
                height: (pagecontrol.currentPage === pagecontrol.totalPage) ? (transaction_lst.count%10)*40 : 400
                model: AppModel.transactionHistory
                anchors {
                    top: parent.top
                    topMargin: 32
                }
                clip: true
                interactive: false
                delegate: QTransactionDelegate {
                    width: 720
                    height: 40
                    statusWidth: 166
                    timeWidth: 123
                    memoWidth: 252
                    amountWidth: 178
                    transactionstatus: transaction_status
                    transactionMemo: transaction_memo
                    transactionAmount: (transaction_isReceiveTx ? "" : "- ") + (transaction_isReceiveTx ? transaction_subtotal : transaction_total)
                    confirmation:  Math.max(0, (AppModel.chainTip - transaction_height)+1)
                    transactionDate: transaction_blocktime
                    onButtonClicked: {
                        QMLHandle.sendEvent(EVT.EVT_TRANSACTION_INFO_ITEM_SELECTED, transaction_txid)
                    }
                }
            }
        }

    }
}
