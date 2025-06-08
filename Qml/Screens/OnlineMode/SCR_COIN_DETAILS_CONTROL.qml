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
import "../../Components/customizes/Signers"
import "../OnlineMode/Coins"
import "../../../localization/STR_QML.js" as STR

QScreen {
    property var walletInfo: AppModel.walletInfo
    property var transactionInfo: AppModel.transactionInfo
    property string coinFlow: walletInfo.coinFlow
    readonly property var map_screens: [
        {screen_name: "coin-details",                   screen_component: _coin_details},
        {screen_name: "coin-filter-tag",                screen_component: _coin_tag_filter},
        {screen_name: "coin-filter-collection",         screen_component: _coin_collection_filter},
        {screen_name: "view-coin-tags",                 screen_component: _coin_tags},
        {screen_name: "view-coin-collections",          screen_component: _coin_collections},
        {screen_name: "view-locked-coins",              screen_component: _view_locked_coins},
        {screen_name: "edit-view-coin-tags",            screen_component: _edit_coin_tags},
        {screen_name: "edit-view-coin-collections",     screen_component: _edit_coin_collections},
        {screen_name: "coin-move-to-another-collection",screen_component: _move_to_another_collection},
        {screen_name: "filtering-search-coins",         screen_component: _filter_Search_coins},
        {screen_name: "edit-multi-coin-tags",           screen_component: _edit_multi_coin_tags},
        {screen_name: "edit-multi-coin-collections",    screen_component: _edit_multi_coin_collections},
        {screen_name: "edit-locked-multi-coin-tags",           screen_component: _edit_locked_multi_coin_tags},
        {screen_name: "edit-locked-multi-coin-collections",    screen_component: _edit_locked_multi_coin_collections},
        {screen_name: "view-consolidate-coins",         screen_component: _view_consolidate_coins},
        {screen_name: "coin-ancestry",                  screen_component: _coin_ancestry},
        {screen_name: "spent-coin-details",             screen_component: _spent_coin_details},
        {screen_name: "select-coin-for-create-transaction",screen_component: _select_coin_for_create_transaction},
        {screen_name: "filtering-search-coins-for-create-transaction",  screen_component: _filter_Search_coins_for_tx},
        {screen_name: "create-a-new-collection",        screen_component: _create_a_new_collection},
        {screen_name: "create-a-new-collection-description", screen_component: _create_a_new_collection_description},
    ]

    Loader {
        width: popupWidth
        height: popupHeight
        anchors.centerIn: parent
        sourceComponent: map_screens.find(function(e) {if (e.screen_name === coinFlow) return true; else return false}).screen_component
    }

    Component {
        id: _coin_details
        QViewCoinsDetails {}
    }

    Component {
        id: _coin_tag_filter
        QViewFilterCoinTagsDetails {}
    }

    Component {
        id: _coin_collection_filter
        QViewFilterCoinCollectionDetails {}
    }

    Component {
        id: _coin_tags
        QViewCoinTags {
            onPrevClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
        }
    }

    Component {
        id: _coin_collections
        QViewCoinCollections {}
    }

    Component {
        id: _edit_coin_tags
        QViewCoinTags {
            isEdit: true
            onPrevClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
        }
    }

    Component {
        id: _edit_coin_collections
        QViewCoinCollections {
            isEdit: true
        }
    }

    Component {
        id: _edit_multi_coin_tags
        QViewCoinTags {
            isEdit: true
            inputType: "add-multi-coin-tags-to-wallet"
            onPrevClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
        }
    }

    Component {
        id: _edit_multi_coin_collections
        QViewCoinCollections {
            isEdit: true
            inputType: "add-multi-coin-collections-to-wallet"

        }
    }

    Component {
        id: _edit_locked_multi_coin_tags
        QViewCoinTags {
            isEdit: true
            inputType: "add-locked-multi-coin-tags-to-wallet"
            onPrevClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
        }
    }

    Component {
        id: _edit_locked_multi_coin_collections
        QViewCoinCollections {
            isEdit: true
            inputType: "add-locked-multi-coin-collections-to-wallet"
        }
    }

    Component {
        id: _move_to_another_collection
        QSelectMoveToAnotherCollection {

        }
    }

    Component {
        id: _filter_Search_coins
        QViewFilteringSearchCoins {
            filterData: walletInfo.utxoList.filter
            includeLockedCoins: true
            onCloseClicked: {
                closeTo(NUNCHUCKTYPE.CURRENT_TAB)
            }
            onApplyClicked: {
                closeTo(NUNCHUCKTYPE.CURRENT_TAB)
            }
        }
    }

    Component {
        id: _view_locked_coins
        QViewLockedCoins {

        }
    }

    Component {
        id: _view_consolidate_coins
        QViewConsolidateCoins {

        }
    }

    Component {
        id: _coin_ancestry
        QViewCoinAncestry {}
    }

    Component {
        id: _spent_coin_details
        QViewSpentCoinDetails {}
    }

    Component {
        id: _select_coin_for_create_transaction
        QSelectCoinForTransaction {}
    }

    Component {
        id: _filter_Search_coins_for_tx
        QViewFilteringSearchCoins {
            filterData: transactionInfo.manualCoins.filter
            hasBack: true
            includeLockedCoins: false
            typeApply: "apply-filter-coin-collection-for-create-transaction"
            onCloseClicked: {
                QMLHandle.sendEvent(EVT.EVT_CONSOLIDATE_COINS_MERGE_MAKE_TRANSACTION_REQUEST, {})
            }
            onApplyClicked: {
                var input = {
                    type: "select-coin-for-create-transaction"
                }
                QMLHandle.sendEvent(EVT.EVT_COIN_DETAILS_CONTROL_ENTER, input)
            }
            onBackScreen: {
                var input = {
                    type: "select-coin-for-create-transaction"
                }
                QMLHandle.sendEvent(EVT.EVT_COIN_DETAILS_CONTROL_ENTER, input)
            }
        }
    }

    Component {
        id: _create_a_new_collection
        QCreateANewCollection {}
    }

    Component {
        id: _create_a_new_collection_description
        QCreateANewCollectionDescription {}
    }
}
