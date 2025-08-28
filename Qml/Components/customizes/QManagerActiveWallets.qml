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
import QtQuick 2.12
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0
import HMIEVENTS 1.0
import NUNCHUCKTYPE 1.0
import QRCodeItem 1.0
import Qt.labs.platform 1.1
import QtQml.Models 2.1
import DataPool 1.0
import "../../Components/customizes/Wallets"
import "../../Components/origins"
import "../../Components/customizes/Texts"
import "../../Components/customizes/Buttons"
import "../../Components/customizes/Chats"
import "../../../localization/STR_QML.js" as STR
Item {
    id: root
    property int count: AppModel.walletList.count
    width: 300
    height: walletList.contentHeight
    Component {
        id: dragDelegate
        MouseArea {
            id: dragArea
            property bool held: false
            property string oldWalletId: ""
            anchors {
                left: parent.left
                right: parent.right
            }
            height: content.height
            drag.target: held ? content : undefined
            drag.axis: Drag.YAxis
            onPressAndHold: {
                held = true
                oldWalletId = model.wallet_id;
            }
            onReleased: held = false
            onClicked: walletListdelegate.buttonClicked()
            Item {
                id: content
                anchors {
                    horizontalCenter: parent.horizontalCenter
                    verticalCenter: parent.verticalCenter
                }
                width: dragArea.width
                height: walletListdelegate.height
                opacity: dragArea.held ? 0.7 : 1.0
                Drag.active: dragArea.held
                Drag.source: dragArea
                Drag.hotSpot.x: width / 2
                Drag.hotSpot.y: height / 2
                states: State {
                    when: dragArea.held
                    ParentChange {
                        target: content
                        parent: root
                    }
                    AnchorChanges {
                        target: content
                        anchors {
                            horizontalCenter: undefined
                            verticalCenter: undefined
                        }
                    }
                }
                QWalletManagerDelegate {
                    id: walletListdelegate
                    width: walletList.width
                    isCurrentIndex: (!pendingList.visible) ? (walletList.visible) && (index === walletList.currentIndex) :
                                                             (walletList.visible) && (index === walletList.currentIndex)
                                                             && (GlobalData.listFocusing === _FOCUS_WALLET)
                    walletCurrency: model.wallet_Balance_Currency
                    walletName :model.wallet_name
                    walletBalance: model.wallet_Balance
                    walletM: model.wallet_M
                    walletN: model.wallet_N
                    walletRole: model.wallet_role
                    hasOwner: model.wallet_hasOwner
                    primaryOwner: model.wallet_primaryOwner
                    isHotWallet: model.wallet_isHotWallet
                    isDashboard: model.wallet_slug !== ""
                    isLocked: model.wallet_dashboard ? (model.wallet_dashboard.isLocked || model.wallet_isLocked || model.wallet_isReplaced) : false
                    isReplaced: model.wallet_isReplaced
                    isEscrow: model.wallet_Escrow
                    isShared: model.wallet_isSharedWallet
                    isAssisted: model.wallet_isAssistedWallet
                    isSandboxWallet: model.wallet_isSanboxWallet
                    isArchived: model.wallet_isArchived
                    walletType: model.wallet_walletType
                    mouseActive: false
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
                    layer.enabled: true
                    layer.effect: OpacityMask {
                        maskSource: Rectangle {
                            width: walletListdelegate.width
                            height: walletListdelegate.height
                            radius: 8
                        }
                    }
                }
            }
            DropArea {
                anchors {
                    fill: parent
                    margins: 10
                }
                onEntered: (drag) => {
                    visualModel.items.move(
                            drag.source.DelegateModel.itemsIndex,
                            dragArea.DelegateModel.itemsIndex)
                            
                    AppModel.walletList.swapWallets(drag.source.DelegateModel.itemsIndex,
                                                    dragArea.DelegateModel.itemsIndex)
                    AppModel.walletList.slotsMoveFinished(oldWalletId)
                }
            }
        }
    }
    DelegateModel {
        id: visualModel
        model: AppModel.walletList
        delegate: dragDelegate
    }
    QListView {
        id: walletList
        anchors.fill: parent
        interactive: false
        currentIndex: AppModel.walletListCurrentIndex
        model: visualModel
        spacing: 4
        cacheBuffer: 50
    }
}