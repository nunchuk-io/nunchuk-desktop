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
import QtQuick
import QtQuick.Controls
import Qt5Compat.GraphicalEffects
import HMIEVENTS 1.0
import NUNCHUCKTYPE 1.0
import QRCodeItem 1.0
import Qt.labs.platform 1.1
import DataPool 1.0
import Features.Home.ViewModels 1.0
import "../../Components/customizes"
import "../../Components/origins"
import "../../Components/customizes/Texts"
import "../../Components/customizes/Buttons"
import "../../Components/customizes/Chats"
import "../../Components/customizes/Transactions"
import "../../Components/customizes/Wallets"
import "../../Components/customizes/Popups"

Item {
    id: _item
    property var    walletInfo                  : AppModel.walletInfo
    property bool   existWallet                 : false
    property string myRole                      : walletInfo.myRole
    property bool   walletKeyNeedBackup         : walletInfo.keyNeedBackup
    property bool   walletNeedBackup            : walletInfo.needBackup
    property bool   walletRegistered            : walletInfo.needRegistered
    property bool   walletIsReplaced            : walletInfo.isReplaced
    property bool   walletIsAssisted            : walletInfo.isAssistedWallet
    property bool   walletIsShared              : walletInfo.isSharedWallet
    property bool   walletIsLocked              : walletInfo.isLocked
    property bool   walletIsSandboxWallet       : walletInfo.isSandboxWallet
    anchors.fill: parent
    anchors.margins: 24
    function getWalletTypeDes() {
        if (walletInfo.walletType === NUNCHUCKTYPE.MINISCRIPT) {
            return QSTR.STR_QML_1801
        } else {
            return (walletInfo.walletN === 1) ? QSTR.STR_QML_070 :
                            qsTr("%1/%2 %3").arg(walletInfo.walletM).arg(walletInfo.walletN).arg(QSTR.STR_QML_069)
        }
    }
    // Height consumed by the banner cluster below (capped/flickable), + 24px
    // spacing before the wallet card. Drives wallet-card height so it
    // shrinks proportionally when banners appear.
    readonly property int _bannersConsumedHeight: bannersFlickable.height + (bannersFlickable.height > 0 ? 24 : 0)

    // All conditional wallet-specific warning/notice banners for this
    // screen, stacked in priority order: timelock notice (always first --
    // it affects fund availability) -> wallet backup/registration warnings
    // -> group-replace notices. At most 3 banners' worth of height is shown
    // at once; if more are visible simultaneously the cluster becomes
    // flickable instead of pushing the rest of the page further down
    // indefinitely.
    // NOTE: the generic home reminder banner (QHomeReminderBanner, driven
    // by HomeReminderViewModel) is intentionally NOT part of this cluster.
    // It's rendered once in SCR_HOME.qml, above whichever Home state is
    // currently loaded (this wallet view, or the no-key/no-wallet welcome
    // screens), so it stays visible regardless of wallet state.
    Flickable {
        id: bannersFlickable
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }
        clip: true
        contentWidth: width
        contentHeight: topBanners.height
        boundsBehavior: Flickable.StopAtBounds
        interactive: contentHeight > height
        ScrollBar.vertical: QScrollBar { }

        // Sum of the first 3 *visible* banners' heights (in the priority
        // order declared below), including spacing between them. With 3 or
        // fewer visible banners this equals contentHeight exactly, so
        // height == contentHeight and interactive is false -- no flick
        // affordance appears unless it's actually needed.
        readonly property int _firstThreeHeight: {
            var heights = []
            var kids = topBanners.children
            for (var i = 0; i < kids.length && heights.length < 3; i++) {
                var kid = kids[i]
                if (kid.visible === true && kid.height > 0) {
                    heights.push(kid.height)
                }
            }
            var sum = 0
            for (var j = 0; j < heights.length; j++) {
                sum += heights[j]
                if (j > 0) sum += topBanners.spacing
            }
            return sum
        }

        height: Math.min(contentHeight, _firstThreeHeight)

        Column {
            id: topBanners
            width: bannersFlickable.width
            spacing: 24

            // Timelock notification -- kept first/top priority.
            Rectangle {
                id: timelockNoti
                property bool needTobeVisibleWarning: walletInfo.timelockInfo.valueNeedVisibleWarning

                width: _item.width
                height: 60
                radius: 8
                color: timelockNoti.needTobeVisibleWarning ? "#FDEBD2" : "#EAEAEA"
                visible: (walletInfo.walletBalanceSats > 0)
                         && (walletInfo.walletType === NUNCHUCKTYPE.MINISCRIPT)
                         /*&& walletInfo.timeLocked*/
                         && (walletInfo.timelockInfo.valueRemainingNumeric > 0)
                Row {
                    anchors.fill: parent
                    anchors.margins: 12
                    spacing: 8
                    QIcon {
                        iconSize: 36
                        anchors.verticalCenter: parent.verticalCenter
                        source: timelockNoti.needTobeVisibleWarning ? "qrc:/Images/Images/warning_amber-60px.png" : "qrc:/Images/Images/info-60px.svg"
                    }
                    QLato {
                        width: parent.width - 48
                        height: 36
                        anchors.verticalCenter: parent.verticalCenter
                        font.pixelSize: 16
                        wrapMode: Text.WordWrap
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignLeft
                        text: QSTR.STR_QML_2078.arg(walletInfo.timelockInfo.valueRemainingString)
                        textFormat: Text.RichText
                        onLinkActivated: {
                            if(walletInfo.utxoList.count > 0){
                                walletInfo.isViewCoinShow = true
                            }
                        }
                    }
                }
            }

            QWalletWarningInfo {
            visible: {
                var ret = !forceClose && !walletIsReplaced
                if (walletInfo.walletType === NUNCHUCKTYPE.SINGLE_SIG) {
                    return walletNeedBackup && ret && walletInfo.hasNativeSegwitDerivationPath()
                } else {
                    return (walletNeedBackup || (walletRegistered && walletInfo.hasHardwareOrAirgap())) && ret
                }
            }
            width: _item.width
            height: 80
            useDoItNow: true
            content: {
                if (walletInfo.walletType === NUNCHUCKTYPE.SINGLE_SIG) {
                    return QSTR.STR_QML_1861
                } else if (walletNeedBackup) {
                    return QSTR.STR_QML_1860
                } else if (walletRegistered) {
                    return QSTR.STR_QML_1862
                } else {
                    return ""
                }
            }
            onDoItNow: {
                if (walletInfo.walletType === NUNCHUCKTYPE.SINGLE_SIG || walletNeedBackup) {
                    QMLHandle.sendEvent(EVT.EVT_HOME_WALLET_INFO_REQUEST, "bsms-file-success")
                } else if (walletRegistered) {
                    QMLHandle.sendEvent(EVT.EVT_HOME_WALLET_INFO_REQUEST, "register-wallet-hardware")
                }
            }
        }
        QWalletWarningInfo {
            visible: walletKeyNeedBackup && !forceClose && !walletIsReplaced
            width: _item.width
            height: 60
            content: QSTR.STR_QML_1265
            onHyperlinkClicked: {
                if(walletInfo.allowBackup()) {
                    _info2.contentText = QSTR.STR_QML_1268
                    _info2.labels = [QSTR.STR_QML_097,QSTR.STR_QML_1276]
                    _info2.funcs = [
                                function() {
                                    OnBoarding.screenFlow = "seedPhrase"
                                    QMLHandle.sendEvent(EVT.EVT_ONBOARDING_REQUEST)                                
                                },
                                function() {
                                    _info2.close()
                                }
                            ]
                    _info2.open();                            
                } else {
                    _info1.title = QSTR.STR_QML_339
                    _info1.contentText = QSTR.STR_QML_1773
                    _info1.open()
                }                
            }
        }
        Repeater {
            model: walletInfo.replaceGroups
            Item {
                width: _item.width
                height: replaceId.item.height
                visible: !walletIsReplaced
                Loader {
                    id: replaceId
                    width: _item.width
                    sourceComponent: modelData.accepted ? acceptComponent: notAcceptComponent
                }
                Component {
                    id: notAcceptComponent
                    Rectangle {
                        width: _item.width
                        height: 124
                        radius: 8
                        color: "#FDEBD2"
                        Column {
                            anchors.fill: parent
                            anchors.margins: 12
                            width: parent.width
                            spacing: 12
                            Column {
                                width: parent.width
                                spacing: 4
                                QLato {
                                    width: parent.width
                                    height: 20
                                    font.weight: Font.Bold
                                    font.pixelSize: 18
                                    text: QSTR.STR_QML_1706
                                    verticalAlignment: Text.AlignVCenter
                                    horizontalAlignment: Text.AlignLeft
                                }
                                QLato {
                                    width: parent.width
                                    height: 28
                                    text: QSTR.STR_QML_1707
                                    verticalAlignment: Text.AlignVCenter
                                    horizontalAlignment: Text.AlignLeft
                                }
                            }
                            Row {
                                height: 36
                                spacing: 8
                                QTextButton {
                                    width: label.paintedWidth + 2*16
                                    height: 36
                                    type: eTypeB
                                    label.text: QSTR.STR_QML_946
                                    label.font.pixelSize: 12
                                    onButtonClicked: {
                                        walletInfo.requestDeclineReplaceGroup(modelData.group_id)
                                    }
                                }
                                QTextButton {
                                    width: label.paintedWidth + 2*16
                                    height: 36
                                    label.text: QSTR.STR_QML_468
                                    label.font.pixelSize: 12
                                    type: eTypeE
                                    onButtonClicked: {
                                        walletInfo.requestAcceptReplaceGroup(modelData.group_id)
                                    }
                                }
                            }
                        }
                    }
                }
                Component {
                    id: acceptComponent
                    Rectangle {
                        width: _item.width
                        height: 76
                        radius: 8
                        color: "#D0E2FF"
                        Column {
                            anchors.fill: parent
                            anchors.margins: 12
                            width: parent.width
                            spacing: 4
                            QLato {
                                width: parent.width
                                height: 20
                                font.weight: Font.Bold
                                font.pixelSize: 18
                                text: QSTR.STR_QML_1708
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignLeft
                            }
                            QLato {
                                width: parent.width
                                height: 28
                                text: QSTR.STR_QML_1707
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignLeft
                            }
                        }
                        QIconButton {
                            iconSize: 24
                            anchors {
                                right: parent.right
                                rightMargin: 16
                                verticalCenter: parent.verticalCenter
                            }
                            icon: "qrc:/Images/Images/right-arrow-dark.svg"
                            onButtonClicked: {
                                var obj = {
                                    type: "setup-group-wallet",
                                    group_id: modelData.group_id
                                }
                                QMLHandle.sendEvent(EVT.EVT_HOME_WALLET_SELECTED, obj)
                            }
                            bgColor: "transparent"
                        }
                    }
                }
            }
        }
        }   // end topBanners
    }   // end bannersFlickable

    // Wallet card: 49% of the height remaining after the banner cluster.
    // When no banners are visible, this equals the original _item.height * 0.49.
    // Clamped to >= 0 so a small/min-height window with several banners
    // visible at once cannot push this negative and break the layout.
    QAreaWalletDetail{
        id: _walletDes
        anchors {
            top: bannersFlickable.bottom
            topMargin: bannersFlickable.height > 0 ? 24 : 0
            left: parent.left
            right: parent.right
        }
        width: _item.width
        height: Math.max(0, (_item.height - _bannersConsumedHeight) * 0.49)
            isAssisted: walletIsAssisted
            isHotWallet: walletKeyNeedBackup
            isLocked: walletIsLocked
            isReplaced: walletIsReplaced
            isSandboxWallet: walletIsSandboxWallet

            Row{
                anchors.fill: parent
                Item {
                    height: parent.height
                    width: parent.width * (myRole === "FACILITATOR_ADMIN" ? 1 : _walletDes.ratio)
                    QLato {
                        id: displayWalletname
                        width: 380
                        height: 36
                        text: walletInfo.walletName
                        font.weight: Font.Bold
                        font.pixelSize: 28
                        color: "#F1FAFE"
                        elide: Text.ElideRight
                        anchors {
                            left: parent.left
                            leftMargin: 24
                            top: parent.top
                            topMargin: 24
                        }
                    }
                    Row {
                        spacing: 8
                        anchors{
                            right: parent.right
                            rightMargin: 36
                            top: parent.top
                            topMargin: 30*QAPP_DEVICE_HEIGHT_RATIO
                        }
                        QIconButton{
                            id: _group
                            width: 24
                            height: 24
                            visible: ((myRole !== "OBSERVER") && (myRole !== "")) || walletIsSandboxWallet
                            icon: (walletInfo.isHoneyBadger || walletInfo.isIronHand) ? "qrc:/Images/Images/health-check-light.svg" : "qrc:/Images/Images/groups-dark.svg"
                            onClicked: {
                                GroupWallet.dashboardInfo.isShowDashBoard = true
                            }
                        }
                        QIconButton{
                            id:_more
                            width: 24
                            height: 24
                            visible: (myRole !== "OBSERVER") && (myRole !== "FACILITATOR_ADMIN")
                            icon: "qrc:/Images/Images/more_horiz_24px.png"
                            onClicked: {
                                optionMenu.popup(_more,mouse.x - 250 + 24,24)
                            }
                        }
                    }

                    QText {
                        id: displayDescription
                        text: walletInfo.walletDescription
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
                    Row{
                        anchors {
                            left: parent.left
                            leftMargin: 24
                            top: displayDescription.bottom
                            topMargin: 16*QAPP_DEVICE_HEIGHT_RATIO
                        }
                        width: parent.width
                        height: 16
                        spacing: 8
                        QBadge {
                            text: (myRole === "FACILITATOR_ADMIN") ? "••••••" : getWalletTypeDes()
                            color: "#EAEAEA"
                            font.weight: Font.Bold
                            font.pixelSize: 10
                            anchors.verticalCenter: parent.verticalCenter
                        }
                        QTypeWallet {
                            icon.iconSize: 12
                            icon.source: if (walletIsShared) return "qrc:/Images/Images/collab-wallet-dark.svg"
                                         else if (walletIsSandboxWallet && !_walletDes.isReplaced) return "qrc:/Images/Images/sandboxGroup.svg"
                                         else if (_walletDes.isAssisted && !_walletDes.isReplaced) return "qrc:/Images/Images/collab-wallet-dark.svg"
                                         else return ""
                            label.text: if (walletIsShared) return QSTR.STR_QML_438
                                        else if (walletIsSandboxWallet && !_walletDes.isReplaced) return QSTR.STR_QML_1675
                                        else if (_walletDes.isAssisted && !_walletDes.isReplaced) return (myRole === "FACILITATOR_ADMIN") ? "••••••" : QSTR.STR_QML_679
                                        else if (_walletDes.isReplaced) return QSTR.STR_QML_1345
                                        else return ""
                            label.font.weight: Font.Bold
                            label.font.pixelSize: 10
                            visible: label.text !== ""
                            anchors.verticalCenter: parent.verticalCenter
                        }
                    }
                    Column {
                        id: balanceInfoCol
                        width: parent.width*0.9
                        anchors {
                            top: parent.top
                            topMargin: 143*QAPP_DEVICE_HEIGHT_RATIO
                            left: parent.left
                            leftMargin: 24
                        }
                        spacing: 4
                        QText {
                            id: wlBalance
                            width: parent.width
                            color: "#F1FAFE"
                            font.pixelSize: 28
                            font.family: "Lato"
                            font.weight: Font.Bold
                            text: (myRole === "FACILITATOR_ADMIN") ? "••••••" : qsTr("%1 %2").arg(walletInfo.walletBalance).arg(RoomWalletData.unitValue)
                        }
                        QText {
                            id: wlBalanceCurrency
                            width: parent.width
                            color: "#F1FAFE"
                            font.pixelSize: 16
                            font.family: "Lato"
                            font.weight: Font.Medium
                            text: (myRole === "FACILITATOR_ADMIN") ? "••••••" : qsTr("%1%2 %3").arg(AppSetting.currencySymbol).arg(walletInfo.walletBalanceCurrency).arg(AppSetting.currency)
                        }
                    }
                    QButtonTextLink {
                        height: 24
                        label: QSTR.STR_QML_574
                        icon: [
                            "qrc:/Images/Images/right-arrow-light.svg",
                            "qrc:/Images/Images/right-arrow-light.svg",
                            "qrc:/Images/Images/right-arrow-light.svg"
                        ]
                        textColor: [
                            "#FFFFFF",
                            "#FFFFFA",
                            "#FFFFFF"
                        ]
                        direction: eRIGHT
                        _mirror:false
                        enabled: existWallet
                        anchors {
                            left: parent.left
                            leftMargin: 24
                            top: parent.top
                            topMargin: 239*QAPP_DEVICE_HEIGHT_RATIO
                        }
                        onButtonClicked: {
                            QMLHandle.sendEvent(EVT.EVT_HOME_WALLET_INFO_REQUEST)
                        }
                    }
                    Row {
                        id: buttongHandles
                        spacing: 12
                        anchors {
                            left: parent.left
                            leftMargin: 24
                            bottom: parent.bottom
                            bottomMargin: 24*QAPP_DEVICE_HEIGHT_RATIO
                        }
                        QIconTextButton {
                            width: 132
                            height: 48
                            label: QSTR.STR_QML_002
                            icons: ["spend-dark.svg", "spend-light.svg", "spend-dark.svg","spend-light.svg"]
                            fontPixelSize: 16
                            iconSize: 24
                            type: eTypeF
                            enabled: (existWallet && (myRole !== "OBSERVER")) && !walletIsLocked
                            onButtonClicked: {
                                if(walletIsShared){
                                    QMLHandle.sendEvent(EVT.EVT_GOTO_HOME_CHAT_TAB)
                                    QMLHandle.sendEvent(EVT.EVT_HOME_SHARED_WL_SEND_REQUEST,walletInfo.walletId)
                                }
                                else{
                                    QMLHandle.sendEvent(EVT.EVT_HOME_SEND_REQUEST)
                                }
                            }
                        }

                        QIconTextButton {
                            width: 132
                            height: 48
                            label: QSTR.STR_QML_003
                            icons: ["received-dark.svg", "received-light.svg", "received-dark.svg", "received-light.svg"]
                            fontPixelSize: 16
                            iconSize: 24
                            type: eTypeF
                            onButtonClicked: { QMLHandle.sendEvent(EVT.EVT_HOME_RECEIVE_REQUEST) }
                        }

                        QIconTextButton {
                            width: 132
                            height: 48
                            label: QSTR.STR_QML_1407
                            icons: ["bitcoin-dark.svg", "bitcoin-light.svg", "bitcoin-disabled.svg", "bitcoin-light.svg"]
                            fontPixelSize: 16
                            iconSize: 24
                            type: eTypeF
                            enabled: walletInfo.utxoList.count > 0
                            onButtonClicked: {
                                walletInfo.isViewCoinShow = true
                            }
                        }
                    }

                    /* for menu */
                    QMultiContextMenu {
                        id: optionMenu
                        menuWidth: 300
                        property var exportMenu: [
                            {
                                visible: true,
                                label: QSTR.STR_QML_1713,
                                icon: "",
                                iconRight: "",
                                color: "#031F2B",
                                enable: true,
                                subMenu: null,
                                action: function() { // "Export PDF invoices"
                                    var currentDate = new Date();
                                    var monthNames = ["JAN", "FEB", "MAR", "APR", "MAY", "JUN",
                                                      "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"];
                                    var day = currentDate.getDate();
                                    var month = monthNames[currentDate.getMonth()];
                                    var year = currentDate.getFullYear();
                                    var today = day + '' + month + '' + year;
                                    exportwalletDialog.visible = false
                                    exportwalletDialog.exportFormat = "pdf"
                                    exportwalletDialog.currentFile = StandardPaths.writableLocation(StandardPaths.DocumentsLocation) + "/"
                                                                    + RoomWalletData.getValidFilename(walletInfo.walletName)
                                                                    + "_transaction_history_"
                                                                    + today
                                                                    + ".pdf"
                                    exportwalletDialog.open()
                                    optionMenu.close()
                                }
                            },
                            {
                                visible: true,
                                label: QSTR.STR_QML_1714,
                                icon: "",
                                iconRight: "",
                                color: "#031F2B",
                                enable: true,
                                subMenu: null,
                                action: function(){
                                    var currentDate = new Date();
                                    var monthNames = ["JAN", "FEB", "MAR", "APR", "MAY", "JUN",
                                                      "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"];
                                    var day = currentDate.getDate();
                                    var month = monthNames[currentDate.getMonth()];
                                    var year = currentDate.getFullYear();
                                    var today = day + '' + month + '' + year;
                                    exportwalletDialog.visible = false
                                    exportwalletDialog.exportFormat = "csv"
                                    exportwalletDialog.currentFile = StandardPaths.writableLocation(StandardPaths.DocumentsLocation) + "/"
                                                                    + RoomWalletData.getValidFilename(walletInfo.walletName)
                                                                    + "_transaction_history_"
                                                                    + today
                                                                    + ".csv"
                                    exportwalletDialog.open()
                                    optionMenu.close()
                                }
                            },
                        ]
                        mapMenu: [
                            {
                                visible: true,
                                label: QSTR.STR_QML_532,
                                icon: "qrc:/Images/Images/import_031F2B.png",
                                iconRight: "",
                                color: "#031F2B",
                                enable: true,
                                subMenu: null,
                                action: function() { // Import PSBT
                                    openfileDialog.open()
                                }
                            },
                            {
                                visible: true,
                                label: QSTR.STR_QML_347,
                                icon: "qrc:/Images/Images/Backup.png",
                                iconRight: "",
                                color: "#031F2B",
                                enable: true,
                                subMenu: null,
                                action: function() { // "Save wallet config"
                                    exportwalletDialog.visible = false
                                    exportwalletDialog.exportFormat = "bsms"
                                    exportwalletDialog.currentFile = StandardPaths.writableLocation(StandardPaths.DocumentsLocation) + "/"
                                            + RoomWalletData.getValidFilename(walletInfo.walletName)
                                            + ".bsms"
                                    exportwalletDialog.open()
                                }
                            },
                            {
                                visible: (myRole !== "FACILITATOR_ADMIN" && myRole !== "KEYHOLDER_LIMITED"),
                                label: QSTR.STR_QML_1490,
                                icon: "qrc:/Images/Images/export_invoices.png",
                                iconRight: "qrc:/Images/Images/right-arrow-dark.svg",
                                color: "#031F2B",
                                enable: (myRole !== "FACILITATOR_ADMIN" && myRole !== "KEYHOLDER_LIMITED"),
                                subMenu: exportMenu,
                                action: function() { // "Save wallet config"
                                    exportwalletDialog.visible = false
                                    exportwalletDialog.exportFormat = "bsms"
                                    exportwalletDialog.currentFile = StandardPaths.writableLocation(StandardPaths.DocumentsLocation) + "/"
                                            + RoomWalletData.getValidFilename(walletInfo.walletName)
                                            + ".bsms"
                                    exportwalletDialog.open()
                                }
                            },
                            {
                                visible: walletIsSandboxWallet,
                                label: QSTR.STR_QML_970,
                                icon: "qrc:/Images/Images/export_invoices.png",
                                iconRight: "",
                                color: "#031F2B",
                                enable: walletIsSandboxWallet,
                                subMenu: null,
                                action: function() {
                                    console.warn("Manage group chat history")
                                    var obj = {
                                        type: "show-manage-chat-history"
                                    }
                                    QMLHandle.sendEvent(EVT.EVT_HOME_WALLET_SELECTED, obj)
                                    chatHistorySandbox.open()
                                }
                            }
                        ]
                    }
                }
                Item {
                    id: area_quickrecevied
                    width: parent.width * (1.0 - _walletDes.ratio)
                    height: parent.height
                    visible: (myRole !== "FACILITATOR_ADMIN")
                    Column {
                        width: parent.width
                        anchors.centerIn: parent
                        spacing: 16*QAPP_DEVICE_HEIGHT_RATIO
                        QText {
                            id: quickRecv
                            font.weight: Font.Bold
                            font.pixelSize: 16
                            color: "#031F2B";
                            text: QSTR.STR_QML_004
                            anchors.horizontalCenter: parent.horizontalCenter
                        }
                        Rectangle {
                            width: 162
                            height: 162
                            radius: 12
                            color: "#FFFFFF"
                            anchors.horizontalCenter: parent.horizontalCenter
                            QRCodeItem {
                                id: qrCode
                                anchors.fill: parent
                                anchors.margins: 6
                                borderWitdh: 6
                                textInput: walletInfo.walletAddress
                            }
                            layer.enabled: true
                            layer.effect: DropShadow {
                                horizontalOffset: 3
                                verticalOffset: 3
                                radius: 8.0
                                samples: 17
                                color: "#80000000"
                            }
                        }
                        Item{
                            width: 329
                            height: 72*QAPP_DEVICE_HEIGHT_RATIO
                            anchors.horizontalCenter: parent.horizontalCenter
                            Rectangle {
                                id: qrHex
                                anchors.fill: parent
                                color: "#FFFFFF"
                                radius: 4
                                layer.enabled: true
                                layer.effect: DropShadow {
                                    horizontalOffset: 3
                                    verticalOffset: 3
                                    radius: 8.0
                                    samples: 17
                                    color: "#80000000"
                                }
                            }
                            QText {
                                z: 1
                                anchors.fill: parent
                                anchors.margins: 5
                                horizontalAlignment: Text.AlignHCenter
                                wrapMode: Text.WrapAnywhere
                                font.weight: Font.Normal
                                font.pixelSize: 16
                                color: "#031F2B";
                                text: qrCode.textInput
                            }
                        }
                        Row {
                            anchors.horizontalCenter: parent.horizontalCenter
                            width: 311
                            height: 48
                            spacing: 10
                            QIconTextButton {
                                width: 97
                                height: 48
                                label: QSTR.STR_QML_005
                                icons: ["copy-dark.svg", "copy-light.svg", "copy-dark.svg","copy-light.svg"]
                                fontPixelSize: 16
                                iconSize: 24
                                type: eTypeB
                                enabled: existWallet
                                onButtonClicked: {
                                    QMLHandle.sendEvent(EVT.EVT_HOME_WALLET_COPY_ADDRESS, qrCode.textInput)
                                }
                            }
                            QIconTextButton {
                                width: 175
                                height: 48
                                label: QSTR.STR_QML_006
                                icons: [
                                    "visibility-dark.svg",
                                    "visibility-light.svg",
                                    "visibility-disabled.svg",
                                    "visibility-light.svg"
                                ]
                                fontPixelSize: 16
                                iconSize: 24
                                type: eTypeB
                                onButtonClicked: {QMLHandle.sendEvent(EVT.EVT_HOME_DISPLAY_ADDRESS, qrCode.textInput) }
                                enabled: walletInfo.containsHWSigner
                            }
                            QTooltip {
                                width: 20
                                height: 20
                                anchors.verticalCenter: parent.verticalCenter
                                toolTip: QSTR.STR_QML_007
                                rightOfParent: true
                            }
                        }
                    }
                }
            }
        }
    // Transaction section: fills ALL remaining space dynamically.
    // anchors.bottom: parent.bottom means height auto-adjusts as banners appear/disappear.
    // (timelockNoti now lives inside the banner cluster above -- see bannersFlickable.)
    Item {
        id: transactionSection
        anchors {
            top: _walletDes.bottom
            topMargin: 24
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }
        width: _item.width
            Row {
                id: trans_lbl
                height: 24
                spacing: 8
                visible: AppModel.walletInfo !== null && walletInfo.transactionHistory !== null
                QText {
                    height: 24
                    font.weight: Font.DemiBold
                    font.pixelSize: 20
                    color: "#031F2B";
                    font.family: "Lato"
                    text: QSTR.STR_QML_010
                    verticalAlignment: Text.AlignVCenter
                    anchors.verticalCenter: parent.verticalCenter
                }
                QText {
                    height: 24
                    font.weight: Font.Normal
                    font.pixelSize: 12
                    color: "#757575";
                    font.family: "Lato"
                    text: QSTR.STR_QML_010_number.arg(AppModel.walletInfo && walletInfo.transactionHistory ? walletInfo.transactionHistory.count : 0)
                    verticalAlignment: Text.AlignVCenter
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
            QButtonTextLink {
                height: 24
                label: QSTR.STR_QML_011
                direction: eRIGHT
                enabled: transaction_lst.count > 0
                anchors {
                    right: parent.right
                }
                onButtonClicked: {
                    QMLHandle.sendEvent(EVT.EVT_HOME_TRANSACTION_HISTORY_REQUEST)
                }
            }
            QText {
                id: trans_not_have
                anchors {
                    top: parent.top
                    topMargin: 100
                    horizontalCenter: parent.horizontalCenter
                }
                height: 24
                font.weight: Font.Normal
                font.pixelSize: 14
                color: "#323E4A";
                font.family: "Montserrat"
                text: QSTR.STR_QML_692
                visible: transaction_lst.count == 0
            }
            QListView {
                id: transaction_lst
                width: parent.width
                model: walletInfo.transactionHistory
                ScrollBar.vertical: QScrollBar { }
                // Anchored to fill from below the header row to the bottom of transactionSection.
                // Height is fully dynamic — no fixed percentage needed.
                anchors {
                    top: trans_lbl.bottom
                    topMargin: 12
                    left: parent.left
                    right: parent.right
                    bottom: parent.bottom
                }
                clip: true
                delegate: QTransactionDelegate {
                    width: transaction_lst.width - 8  // leave room for QScrollBar (8px)
                    height: 64
                    parentList: transaction_lst
                    transactionisReceiveTx: transaction_isReceiveTx
                    transactiontxid: transaction_txid
                    transactiondestinationList: transaction_destinationDisp_role
                    transactionstatus: transaction_status
                    transactionMemo: transaction_memo
                    transactionAmount: (transaction_isReceiveTx ? transaction_subtotal : transaction_total)
                    transactiontotalCurrency: (transaction_isReceiveTx ? transaction_subtotalCurrency : transaction_totalCurrency)
                    confirmation: Math.max(0, (AppModel.blockHeight - transaction_height)+1)
                    transactionDate: transaction_blocktime
                    addressWidth: width*0.20
                    statusWidth: width*0.20
                    timeWidth: width*0.15
                    memoWidth: width*0.20
                    amountWidth: width*0.25
                    transactionIsRbf: transaction_isRbf
                    walletIsByzantineGuardian: walletInfo.isByzantineGuardian
                    isFacilitatorAdmin: (myRole === "FACILITATOR_ADMIN")
                    transactionLocked: transaction_timelockedUntil.hasLocked
                    onButtonClicked: {
                        QMLHandle.signalNotifySendEvent(EVT.EVT_HOME_TRANSACTION_INFO_REQUEST, transactiontxid)
                    }
                }
            }
            BusyIndicator {
                id: notxWaiting
                anchors.centerIn: transaction_lst
                running: transaction_lst.count === 0 && ClientController.isMatrixLoggedIn === false
            }
            Connections {
                target: AppModel
                function onFinishedGetTransactionHistory() {
                    notxWaiting.visible = false
                }
            }
        }

    Loader {
        anchors.right: parent.right
        anchors.rightMargin: 30
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 30
        sourceComponent: walletInfo.isSandboxWallet ? messageBubble : null
    }
    Component {
        id: messageBubble
        QMessageBubble { }
    }

    FileDialog {
        id: openfileDialog
        fileMode: FileDialog.OpenFile
        onAccepted: {
            QMLHandle.sendEvent(EVT.EVT_HOME_IMPORT_PSBT, openfileDialog.file)
        }
    }

    FileDialog {
        id: exportwalletDialog
        property string exportFormat: "bsms"
        fileMode: FileDialog.SaveFile
        onAccepted: {
            if(exportwalletDialog.exportFormat === "pdf") {
                invoicePopup.open()
                invoicePopup.startPrintInvoices(exportwalletDialog.currentFile)
            }
            else if(exportwalletDialog.exportFormat === "csv") {
                var exportObj = {
                    "export_type"    : "csv",
                    "export_path"    : exportwalletDialog.currentFile
                };
                QMLHandle.sendEvent(EVT.EVT_HOME_EXPORT_BSMS, exportObj)
            }
            else {
                var exportObj = {
                    "export_type"    : "bsms",
                    "export_path"    : exportwalletDialog.currentFile
                };
                QMLHandle.sendEvent(EVT.EVT_HOME_EXPORT_BSMS, exportObj)
            }
        }
    }
    enabled: !isLocked
    property bool isLocked:  walletInfo.dashboardInfo ? walletInfo.dashboardInfo.isLocked : false
    Loader {
        anchors.fill: parent
        sourceComponent: isLocked ? locked : null
    }
    Component {
        id: locked
        QHomeLockdownInProgress {
            anchors.fill: parent
        }
    }
    QExportPDF {
        id: invoicePopup
        width: parent.width
        height: parent.height
    }
}
