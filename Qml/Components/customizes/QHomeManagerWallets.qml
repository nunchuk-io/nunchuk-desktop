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
import Features.Wallets.ViewModels 1.0
import "../../Components/customizes/Wallets"
import "../../Components/origins"
import "../../Components/customizes/Texts"
import "../../Components/customizes/Buttons"
import "../../Components/customizes/Chats"
import "../../../localization/STR_QML.js" as STR

Rectangle {
    id: walletsRoot
    color: "transparent"
    signal askDeny(var deny)
    property int  count: walletList.count + pendingList.count + pendingGroupList.count
    property bool isPending: pendingList.count > 0 ? true : false
    // Controls whether the archived wallets panel is shown instead of the active list.
    property bool showingArchived: false
    property string _focusedPendingGroupId: ""
    property string _focusedPendingWalletId: ""
    property string _lastWalletFocusTrace: ""
    // QAbstractItemModel::modelReset can replace/reorder rows without changing
    // count. Keep an explicit dependency so the selected wallet ID is rescanned.
    property int _walletModelRevision: 0
    // Tracks the last-known archivedCount so the Connections handler can detect
    // when a wallet was unarchived (count decreased) vs archived (count increased).
    property int _prevArchivedCount: 0
    readonly property int _FOCUS_NONE: -1
    readonly property int _FOCUS_PENDING_GROUP_WALLET: 0
    readonly property int _FOCUS_PENDING_WALLET: 1
    readonly property int _FOCUS_WALLET: 2
    readonly property string _selectedWalletId: AppModel.walletInfo ? AppModel.walletInfo.walletId : ""
    readonly property int _selectedWalletModelIndex: {
        var _modelResetRevision = _walletModelRevision
        if (_selectedWalletId === "") {
            return -1
        }
        for (var i = 0; i < AppModel.walletList.count; ++i) {
            var item = AppModel.walletList.get(i)
            if (item && item.wallet_id === _selectedWalletId) {
                return i
            }
        }
        return -1
    }
    readonly property bool _hasValidWalletSelection: _selectedWalletModelIndex >= 0

    function pendingWalletIndexById(groupId) {
        if (!groupId) {
            return -1
        }
        var dashboards = GroupWallet.dashboards
        for (var i = 0; i < dashboards.length; ++i) {
            if (dashboards[i] && dashboards[i].groupId === groupId) {
                return i
            }
        }
        return -1
    }

    function pendingGroupIndexById(groupId) {
        var model = SharedWallet.sandboxList
        return groupId !== "" && model ? model.indexOf(groupId) : -1
    }

    function hasActivePendingWalletDashboard() {
        var dashboard = GroupWallet.dashboardInfo
        return dashboard
                && dashboard.isShowDashBoard
                && !dashboard.hasWallet
                && pendingWalletIndexById(dashboard.groupId) >= 0
    }

    function traceWalletFocus() {
        var pendingWalletCount = GroupWallet.dashboards ? GroupWallet.dashboards.length : 0
        var pendingGroupCount = SharedWallet.sandboxList ? SharedWallet.sandboxList.count : 0
        var signature = [GlobalData.listFocusing,
                         _selectedWalletModelIndex,
                         AppModel.walletList.count,
                         pendingWalletCount,
                         pendingGroupCount].join("|")
        if (_lastWalletFocusTrace === signature) {
            return
        }
        _lastWalletFocusTrace = signature
        console.info("[WALLET_FOCUS_TRACE:v2]",
                     "focus:", GlobalData.listFocusing,
                     "selectedIndex:", _selectedWalletModelIndex,
                     "wallets:", AppModel.walletList.count,
                     "pending:", pendingWalletCount,
                     "pendingGroups:", pendingGroupCount)
    }

    function syncWalletFocus() {
        var pendingGroupIndex = pendingGroupIndexById(_focusedPendingGroupId)
        if (GlobalData.listFocusing === _FOCUS_PENDING_GROUP_WALLET
                && pendingGroupIndex >= 0) {
            if (SharedWallet.currentIndex !== pendingGroupIndex) {
                SharedWallet.currentIndex = pendingGroupIndex
            }
            traceWalletFocus()
            return
        }
        if (hasActivePendingWalletDashboard()) {
            var pendingIndex = pendingWalletIndexById(GroupWallet.dashboardInfo.groupId)
            _focusedPendingGroupId = ""
            _focusedPendingWalletId = GroupWallet.dashboardInfo.groupId
            if (GroupWallet.currentIndex !== pendingIndex) {
                GroupWallet.currentIndex = pendingIndex
            }
            GlobalData.listFocusing = _FOCUS_PENDING_WALLET
        } else if (_hasValidWalletSelection) {
            _focusedPendingGroupId = ""
            _focusedPendingWalletId = ""
            if (AppModel.walletListCurrentIndex !== _selectedWalletModelIndex) {
                AppModel.walletListCurrentIndex = _selectedWalletModelIndex
            }
            GlobalData.listFocusing = _FOCUS_WALLET
        } else {
            _focusedPendingGroupId = ""
            _focusedPendingWalletId = ""
            GlobalData.listFocusing = _FOCUS_NONE
        }
        traceWalletFocus()
    }

    // True when the wallet shown in the detail pane is archived.
    // Used to show a yellow left accent on the Archived wallets button in the active view.
    // archivedCount is read to create a QML binding dependency on walletCountChanged,
    // so this property re-evaluates whenever any wallet is archived or unarchived.
    readonly property bool _archivedIsSelected: {
        var _dep = AppModel.walletList.archivedCount  // reactive anchor
        return GlobalData.listFocusing === _FOCUS_WALLET
                && _hasValidWalletSelection
                && AppModel.walletInfo
                && AppModel.walletInfo.isArchived
    }
    width: parent.width
    // height được quản lý bởi ColumnLayout (Layout.fillHeight: true) ở SCR_HOME.qml.
    // Không khai báo height cứng ở đây để tránh conflict.
    clip: true
    Column {
        width: parent.width
        spacing: 12
        Item {
            width: parent.width
            height: 24
            QText {
                id: walletsTextTitle
                anchors {
                    left: parent.left
                    leftMargin: 16
                    verticalCenter: parent.verticalCenter
                }
                text: STR.STR_QML_000
                font.family: "Lato"
                font.pixelSize: 16
                color: "#FFFFFF"
                font.weight: Font.Bold
            }
            QConnection {
                width: label.paintedWidth + 12 + 20 + 6
                height: 24
                radius: 20
                label.text: {
                    var mode = "";
                    switch(AppSetting.connectionState){
                    case NUNCHUCKTYPE.OFFLINE: mode = STR.STR_QML_612; break;
                    case NUNCHUCKTYPE.SYNCING: mode = STR.STR_QML_613; break;
                    case NUNCHUCKTYPE.ONLINE: mode = STR.STR_QML_611; break;
                    default: mode = ""
                    }
                    var server = ""
                    switch(AppSetting.primaryServer){
                    case NUNCHUCKTYPE.MAIN: server = ""; break;
                    case NUNCHUCKTYPE.TESTNET: server = "(" + STR.STR_QML_013 + ")"; break;
                    case NUNCHUCKTYPE.SIGNET: server = "(" + STR.STR_QML_584 + ")"; break;
                    default: server = ""
                    }
                    return mode + server
                }
                anchors.left: walletsTextTitle.right
                anchors.leftMargin: 24
                anchors.verticalCenter: parent.verticalCenter
            }
            QIconButton{
                width: 24
                height: 24
                anchors.right: parent.right
                anchors.rightMargin: 10
                anchors.verticalCenter: parent.verticalCenter
                icon:"qrc:/Images/Images/add-light.svg"
                onClicked: {
                    OnBoarding.screenFlow = "addAWallet"
                    QMLHandle.sendEvent(EVT.EVT_ONBOARDING_REQUEST)
                }
            }
        }
        Item {
            width: parent.width
            height: walletsRoot.height - (40)

            // --- ACTIVE WALLETS VIEW ---
            // Visible when showingArchived = false.
            Item {
                id: activeView
                visible: !walletsRoot.showingArchived
                anchors.fill: parent

                Loader {
                    visible: (walletList.count + pendingList.count) === 0
                    anchors.fill: parent
                    sourceComponent: (walletList.count + pendingList.count) >= 0 ? null : loadingWalletBusy
                }

                Flickable {
                    id: flickerWalletList
                    anchors {
                        top: parent.top
                        left: parent.left
                        right: parent.right
                        // Leave room for archived button at bottom when it is visible.
                        bottom: archivedBtnContainer.top
                    }
                    clip: true
                    flickableDirection: Flickable.VerticalFlick
                    interactive: true
                    contentHeight: contentDisplay.height
                    ScrollBar.vertical: QScrollBar { }
                    Column {
                        id: contentDisplay
                        width: parent.width
                        spacing: 4
                        QListView {
                            id: pendingGroupList
                            height: contentHeight
                            width: parent.width - 32
                            anchors.horizontalCenter: parent.horizontalCenter
                            visible: pendingGroupList.count > 0
                            snapMode: ListView.NoSnap
                            model: SharedWallet.sandboxList
                            interactive: false
                            currentIndex: SharedWallet.currentIndex
                            onCountChanged: {
                                if (count === 0
                                        && GlobalData.listFocusing === _FOCUS_PENDING_GROUP_WALLET) {
                                    walletsRoot._focusedPendingGroupId = ""
                                    GlobalData.listFocusing = _FOCUS_NONE
                                    SharedWallet.currentIndex = -1
                                }
                                Qt.callLater(walletsRoot.syncWalletFocus)
                            }
                            spacing: 4
                            delegate: QPendingGroupWallet {
                                width: pendingGroupList.width
                                name_group: model.group_name
                                inviter_email: model.group_InviterEmail
                                isInviter: model.group_isInviter
                                isCurrentIndex: GlobalData.listFocusing === _FOCUS_PENDING_GROUP_WALLET
                                                && pendingGroupList.visible
                                                && model.group_id === walletsRoot._focusedPendingGroupId

                                onDeny: {
                                    GlobalData.listFocusing = _FOCUS_PENDING_GROUP_WALLET
                                    walletsRoot._focusedPendingGroupId = model.group_id
                                    SharedWallet.currentIndex = index
                                    var obj = {
                                        type: "denySandbox",
                                        group_id: model.group_id
                                    }
                                    askDeny(obj)
                                }
                                onAccept: {
                                    GlobalData.listFocusing = _FOCUS_PENDING_GROUP_WALLET
                                    walletsRoot._focusedPendingGroupId = model.group_id
                                    SharedWallet.currentIndex = index
                                    var obj = {
                                        type: "acceptSandbox",
                                        group_id: model.group_id
                                    }
                                    QMLHandle.sendEvent(EVT.EVT_HOME_WALLET_SELECTED, obj)
                                }
                                onButtonClicked: {
                                    GlobalData.listFocusing = _FOCUS_PENDING_GROUP_WALLET
                                    walletsRoot._focusedPendingGroupId = model.group_id
                                    SharedWallet.currentIndex = index
                                    var obj = {
                                        type: "setup-group-wallet",
                                        group_id: model.group_id
                                    }
                                    QMLHandle.sendEvent(EVT.EVT_HOME_WALLET_SELECTED, obj)
                                }
                            }
                        }
                        QListView {
                            id: pendingList
                            height: contentHeight
                            width: parent.width - 32
                            anchors.horizontalCenter: parent.horizontalCenter
                            visible: pendingList.count > 0
                            snapMode: ListView.NoSnap
                            model: GroupWallet.dashboards
                            interactive: false
                            currentIndex: GroupWallet.currentIndex
                            onCountChanged: {
                                if (count === 0
                                        && GlobalData.listFocusing === _FOCUS_PENDING_WALLET) {
                                    GroupWallet.currentIndex = -1
                                    walletsRoot._focusedPendingWalletId = ""
                                    GlobalData.listFocusing = _FOCUS_NONE
                                }
                                Qt.callLater(walletsRoot.syncWalletFocus)
                            }
                            spacing: 4
                            delegate: QPendingWallet {
                                id: pendingdelegate
                                width: pendingList.width
                                name_person: modelData.userName
                                email_person: modelData.userEmail
                                user_accepted: modelData.accepted
                                isCurrentIndex: GlobalData.listFocusing === _FOCUS_PENDING_WALLET
                                                && pendingList.visible
                                                && modelData.groupId === walletsRoot._focusedPendingWalletId
                                onDeny: {
                                    GlobalData.listFocusing = _FOCUS_PENDING_WALLET
                                    walletsRoot._focusedPendingWalletId = modelData.groupId
                                    GroupWallet.currentIndex = index
                                    var obj = {
                                        type: "deny",
                                        group_id: modelData.groupId
                                    }
                                    askDeny(obj)
                                }
                                onAccept: {
                                    GlobalData.listFocusing = _FOCUS_PENDING_WALLET
                                    walletsRoot._focusedPendingWalletId = modelData.groupId
                                    GroupWallet.currentIndex = index
                                    var obj = {
                                        type: "accept",
                                        group_id: modelData.groupId
                                    }
                                    QMLHandle.sendEvent(EVT.EVT_HOME_WALLET_SELECTED, obj)
                                }
                                onDashboard: {
                                    GlobalData.listFocusing = _FOCUS_PENDING_WALLET
                                    walletsRoot._focusedPendingWalletId = modelData.groupId
                                    GroupWallet.currentIndex = index
                                    var obj = {
                                        type: "dashboard",
                                        group_id: modelData.groupId
                                    }
                                    QMLHandle.sendEvent(EVT.EVT_HOME_WALLET_SELECTED, obj)
                                }
                                onButtonClicked: {
                                    GlobalData.listFocusing = _FOCUS_PENDING_WALLET
                                    walletsRoot._focusedPendingWalletId = modelData.groupId
                                    GroupWallet.currentIndex = index
                                    var obj = {
                                        type: "dashboard",
                                        group_id: modelData.groupId
                                    }
                                    QMLHandle.sendEvent(EVT.EVT_HOME_WALLET_SELECTED, obj)
                                }

                                layer.enabled: true
                                layer.effect: OpacityMask {
                                    maskSource: Rectangle {
                                        width: pendingdelegate.width
                                        height: pendingdelegate.height
                                        radius: 8
                                    }
                                }
                            }
                        }
                        QManagerActiveWallets {
                            id: walletList
                            width: parent.width - 32
                            anchors.horizontalCenter: parent.horizontalCenter
                            selectionActive: GlobalData.listFocusing === _FOCUS_WALLET
                            selectedWalletId: walletsRoot._selectedWalletId
                        }
                    }
                }

                // "Archived wallets (N)" button pinned at the bottom of the active view.
                // Height collapses to 0 when there are no archived wallets so the
                // Flickable above expands to fill the full area.
                Item {
                    id: archivedBtnContainer
                    width: parent.width
                    // 8px top padding + 40px button height = 48px total when visible.
                    height: AppModel.walletList.archivedCount > 0 ? 48 : 0
                    anchors.bottom: parent.bottom

                    MouseArea {
                        visible: AppModel.walletList.archivedCount > 0
                        cursorShape: Qt.PointingHandCursor
                        width: parent.width - 32
                        height: 40
                        anchors {
                            bottom: parent.bottom
                            horizontalCenter: parent.horizontalCenter
                        }
                        onClicked: walletsRoot.showingArchived = true

                        Rectangle {
                            anchors.fill: parent
                            color: Qt.rgba(1, 1, 1, 0.08)
                            radius: 8

                            // Yellow left accent mirrors the wallet card isCurrentIndex indicator.
                            // Visible when the currently selected wallet lives in the archived list.
                            Rectangle {
                                width: 8
                                height: parent.height
                                color: walletsRoot._archivedIsSelected ? "#F6D65D" : "transparent"
                                radius: 4
                            }

                            Row {
                                anchors.centerIn: parent
                                spacing: 6

                                QText {
                                    anchors.verticalCenter: parent.verticalCenter
                                    text: qsTr("Archived wallets (%1)").arg(AppModel.walletList.archivedCount)
                                    font.family: "Lato"
                                    font.pixelSize: 14
                                    color: "#FFFFFF"
                                }

                                Image {
                                    anchors.verticalCenter: parent.verticalCenter
                                    width: 16
                                    height: 16
                                    source: "qrc:/Images/Images/right-arrow-dark.svg"
                                    // Tint to white so it is visible on the dark panel background.
                                    layer.enabled: true
                                    layer.effect: ColorOverlay {
                                        color: "#FFFFFF"
                                    }
                                }
                            }
                        }
                    }
                }
            }

            // --- ARCHIVED WALLETS VIEW ---
            // Visible when showingArchived = true.
            Item {
                id: archivedView
                visible: walletsRoot.showingArchived
                anchors.fill: parent

                // Back header: clicking anywhere on the header returns to the active list.
                Item {
                    id: archivedHeader
                    width: parent.width
                    height: 36
                    anchors {
                        top: parent.top
                        left: parent.left
                        right: parent.right
                    }

                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: walletsRoot.showingArchived = false

                        Row {
                            anchors {
                                left: parent.left
                                leftMargin: 16
                                verticalCenter: parent.verticalCenter
                            }
                            spacing: 8

                            Image {
                                width: 16
                                height: 16
                                anchors.verticalCenter: parent.verticalCenter
                                source: "qrc:/Images/Images/left-arrow-dark.svg"
                                // Tint to white so it is visible on the dark panel background.
                                layer.enabled: true
                                layer.effect: ColorOverlay {
                                    color: "#FFFFFF"
                                }
                            }

                            QText {
                                anchors.verticalCenter: parent.verticalCenter
                                text: qsTr("Archived wallets")
                                font.family: "Lato"
                                font.pixelSize: 14
                                color: "#FFFFFF"
                                font.weight: Font.Bold
                            }
                        }
                    }
                }

                // Scrollable list of archived wallets (non-draggable).
                // Uses height=0 + visible=false per item so model indices stay aligned
                // with AppModel.walletList, keeping isCurrentIndex / EVT_HOME_WALLET_SELECTED correct.
                Flickable {
                    id: archivedFlickable
                    anchors {
                        top: archivedHeader.bottom
                        left: parent.left
                        right: parent.right
                        bottom: parent.bottom
                        topMargin: 4
                    }
                    clip: true
                    flickableDirection: Flickable.VerticalFlick
                    interactive: true
                    contentHeight: archivedContent.height
                    ScrollBar.vertical: QScrollBar { }

                    Column {
                        id: archivedContent
                        width: parent.width - 32
                        anchors.horizontalCenter: parent.horizontalCenter
                        spacing: 4

                        Repeater {
                            model: AppModel.walletList

                            Item {
                                width: archivedContent.width
                                // Preserve model index alignment: non-archived items collapse to 0.
                                // height=0 hides the item without calling setVisible(), avoiding the
                                // removeGrabber crash during app teardown.
                                height: model.wallet_isArchived ? archivedDelegate.height : 0
                                clip: true

                                QWalletManagerDelegate {
                                    id: archivedDelegate
                                    width: parent.width
                                    // Allow the built-in mouse handler so onButtonClicked fires.
                                    mouseActive: true
                                    // Highlight follows the wallet shown in the detail pane, not a transient row index.
                                    isCurrentIndex: GlobalData.listFocusing === _FOCUS_WALLET
                                                    && model.wallet_isArchived
                                                    && model.wallet_id === walletsRoot._selectedWalletId
                                    walletCurrency: model.wallet_Balance_Currency
                                    walletName: model.wallet_name
                                    walletBalance: model.wallet_Balance
                                    walletM: model.wallet_M
                                    walletN: model.wallet_N
                                    walletRole: model.wallet_role
                                    hasOwner: model.wallet_hasOwner
                                    primaryOwner: model.wallet_primaryOwner
                                    isHotWallet: model.wallet_isHotWallet
                                    isDashboard: model.wallet_slug !== "" || model.wallet_isSanboxWallet
                                    isLocked: model.wallet_dashboard ? (model.wallet_dashboard.isLocked || model.wallet_isLocked || model.wallet_isReplaced) : false
                                    isReplaced: model.wallet_isReplaced
                                    isEscrow: model.wallet_Escrow
                                    isShared: model.wallet_isSharedWallet
                                    isAssisted: model.wallet_isAssistedWallet
                                    isSandboxWallet: model.wallet_isSanboxWallet
                                    isArchived: model.wallet_isArchived
                                    walletType: model.wallet_walletType
                                    alertCount: model.wallet_alertCount

                                    layer.enabled: true
                                    layer.effect: OpacityMask {
                                        maskSource: Rectangle {
                                            width: archivedDelegate.width
                                            height: archivedDelegate.height
                                            radius: 8
                                        }
                                    }

                                    onButtonClicked: {
                                        GlobalData.listFocusing = _FOCUS_WALLET
                                        var obj = {
                                            type: "selected",
                                            data: index,
                                            group_id: model.wallet_group_id,
                                            wallet_id: model.wallet_id
                                        }
                                        QMLHandle.sendEvent(EVT.EVT_HOME_WALLET_SELECTED, obj)
                                    }
                                    onDashboard: {
                                        GlobalData.listFocusing = _FOCUS_WALLET
                                        var obj = {
                                            type: "wallet_dashboard",
                                            data: index,
                                            group_id: model.wallet_group_id,
                                            wallet_id: model.wallet_id
                                        }
                                        QMLHandle.sendEvent(EVT.EVT_HOME_WALLET_SELECTED, obj)
                                    }
                                    onAlertCountClicked: {
                                        GlobalData.listFocusing = _FOCUS_WALLET
                                        var obj = {
                                            type: "wallet_dashboard",
                                            data: index,
                                            group_id: model.wallet_group_id,
                                            wallet_id: model.wallet_id
                                        }
                                        QMLHandle.sendEvent(EVT.EVT_HOME_WALLET_SELECTED, obj)
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    readonly property int _LIMIT_WAITING_BUSY: 5000
    Component {
        id: loadingWalletBusy
        Item {
            anchors.fill: parent
            QWalletManagerDelegate {
                visible: !busyIdct.running
                width: parent.width
                height: 64
                enabled: false
                isEscrow: false
                walletName : STR.STR_QML_014
                walletBalance: "0"
                walletM: 0
                walletN: 0
                Rectangle {
                    anchors.fill: parent
                    color: "#031F2B"
                    opacity: 0.5
                }
            }
            BusyIndicator {
                id: busyIdct
                anchors.centerIn: parent
                running: true
            }
            Timer {
                interval: _LIMIT_WAITING_BUSY
                running: true
                onTriggered: busyIdct.running = false
            }
        }
    }
    // Reset to active view automatically when no archived wallets remain,
    // or when a wallet is unarchived while the archived panel is visible,
    // so the user is never stranded and can see the wallet in the active list.
    Connections {
        target: AppModel.walletList
        function onModelReset() {
            walletsRoot._walletModelRevision += 1
            Qt.callLater(walletsRoot.syncWalletFocus)
        }
        function onWalletCountChanged() {
            var curr = AppModel.walletList.archivedCount
            if (curr === 0 || (walletsRoot.showingArchived && curr < walletsRoot._prevArchivedCount)) {
                walletsRoot.showingArchived = false
            }
            walletsRoot._prevArchivedCount = curr
        }
    }
    Connections {
        target: AppModel
        function onWalletInfoChanged() {
            Qt.callLater(walletsRoot.syncWalletFocus)
        }
    }
    Connections {
        target: GroupWallet
        function onDashboardInfoChanged() {
            Qt.callLater(walletsRoot.syncWalletFocus)
        }
        function onDashboardListChanged() {
            Qt.callLater(walletsRoot.syncWalletFocus)
        }
    }
    Connections {
        target: GroupWallet.dashboardInfo
        function onShowDashBoardChanged() {
            Qt.callLater(walletsRoot.syncWalletFocus)
        }
    }
    Connections {
        target: SharedWallet.sandboxList
        function onModelReset() {
            Qt.callLater(walletsRoot.syncWalletFocus)
        }
    }
    Component.onCompleted: {
        _prevArchivedCount = AppModel.walletList.archivedCount
        walletsRoot.syncWalletFocus()
        Qt.callLater(walletsRoot.syncWalletFocus)
    }
    WalletListViewModel {
        id: vm
    }
}
