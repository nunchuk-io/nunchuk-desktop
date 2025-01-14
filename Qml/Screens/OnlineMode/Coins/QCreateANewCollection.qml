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
import "../../../Components/origins"
import "../../../Components/customizes"
import "../../../Components/customizes/Chats"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../Components/customizes/QRCodes"
import "../../../Components/customizes/Transactions"
import "../../../Components/customizes/Popups"
import "../../../../localization/STR_QML.js" as STR

QOnScreenContentTypeB {
    id: _content
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_1449
    onCloseClicked: closeTo(NUNCHUCKTYPE.WALLET_TAB)
    property var collect: walletInfo.coinCollections.collection
    property var coinTags: walletInfo.coinTagsFilter
    property string strOption: ""
    extraHeader: Item {}
    content: Flickable {
        id: _item
        clip: true
        contentWidth: _item.width
        contentHeight: _column.childrenRect.height
        interactive: true
        anchors {
            top: parent.top
            topMargin: 0
        }
        Connections {
            target: walletInfo
            onUpdateCollectionNameChanged: {
                if (isError) {
                    collection_name.isValid = false
                    collection_name.showError = true
                    collection_name.errorText = STR.STR_QML_1473
                }
            }
        }

        Column {
            id: _column
            width: 728
            spacing: 24
            QTextInputBoxTypeB {
                id: collection_name
                label: STR.STR_QML_1435
                boxWidth: 728
                boxHeight: 48
                isValid: true
                textInputted: collect.coin_collection_name
                maxLength: 80
                onTextInputtedChanged: {
                    if(!collection_name.isValid){
                        collection_name.isValid = true
                        collection_name.errorText = ""
                    }
                    collection_name.showError = false
                }
            }
            Column {
                width: parent.width
                spacing: 12
                Column {
                    width: parent.width
                    spacing: 8
                    QLato {
                        text: STR.STR_QML_1522
                        font.pixelSize: 16
                        font.weight: Font.Bold
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                    }
                    QCheckBoxTypeA {
                        id: _coin_without_tags
                        width: parent.width
                        label: STR.STR_QML_1523
                        description: STR.STR_QML_1524
                        border.color: strOption === label ? "#000000" : "#DEDEDE"
                        onButtonClicked: {
                            strOption = label
                        }
                    }
                }
                QCheckBoxTypeA {
                    id: _coins_with_tags
                    width: parent.width
                    label: STR.STR_QML_1525
                    description: STR.STR_QML_1526
                    border.color: strOption === label ? "#000000" : "#DEDEDE"
                    onButtonClicked: {
                        strOption = label
                    }
                }
                Item {
                    visible: _coins_with_tags.checked
                    width: parent.width
                    height: 24
                    Row {
                        spacing: 8
                        QIcon {
                            iconSize: 24
                            source: "qrc:/Images/Images/coin-tag-dark.svg"
                            anchors.verticalCenter: parent.verticalCenter
                        }
                        QLato {
                            text: STR.STR_QML_1417
                            font.pixelSize: 16
                            font.weight: Font.Bold
                            anchors.verticalCenter: parent.verticalCenter
                        }
                        QLato {
                            visible: coinTags.count > 0
                            text: qsTr("(%1)").arg(coinTags.count)
                            color: "#595959"
                            font.pixelSize: 12
                            font.weight: Font.Normal
                            anchors.verticalCenter: parent.verticalCenter
                        }
                    }
                    QTextLink {
                        id: _edit
                        width: _edit.paintedWidth
                        height: 20
                        text: STR.STR_QML_849
                        anchors {
                            verticalCenter: parent.verticalCenter
                            right: parent.right
                        }
                        onTextClicked: {
                            editTags.open()
                        }
                    }
                }
                Flow {
                    id: _flow
                    visible: _coins_with_tags.checked
                    spacing: 8
                    width: parent.width
                    Repeater {
                        model: coinTags
                        QCoinType {
                            height: 36
                            fontSize: 16
                            coinColor: coin_tag_color
                            coinText: coin_tag_name
                        }
                    }
                }

                QSwitchTypeLabel {
                    id: also_apply_filter_to_existing_coins
                    width: parent.width
                    height: 65
                    label: STR.STR_QML_1527
                    value: false
                }
            }

            Column {
                width: parent.width
                spacing: 16
                QLato {
                    width: parent.width
                    text: STR.STR_QML_1528
                    font.pixelSize: 16
                    font.weight: Font.Bold
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }
                QSwitchTypeLabel {
                    id: auto_lock
                    width: parent.width
                    height: 65
                    label: STR.STR_QML_1529
                    value: false
                }
            }
            Item {
                id: name
                width: parent.width
                height: _avatar.childrenRect.height
                Column {
                    id: _avatar
                    width: parent.width
                    spacing: 8
                    Item {
                        width: parent.width
                        height: 24
                        QLato {
                            text: STR.STR_QML_1530
                            font.pixelSize: 16
                            font.weight: Font.Bold
                            anchors.verticalCenter: parent.verticalCenter
                        }
                        QTextLink {
                            width: paintedWidth
                            height: 20
                            text: STR.STR_QML_849
                            anchors {
                                verticalCenter: parent.verticalCenter
                                right: parent.right
                            }
                            onTextClicked: {

                            }
                        }
                    }
                    Rectangle {
                        width: 48
                        height: 48
                        radius: 24
                        color: "#FDD95C"
                        visible: collection_name.textInputted != ""
                        QLato {
                            text: getFistCharterName(collection_name.textInputted)
                            anchors.centerIn: parent
                            font.weight: Font.Bold
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                    }
                }
            }
        }
        property int maxChar: 2
        function getFistCharterName(label) {
            if(label !== ""){
                if (maxChar == 1) {
                    var ret = label.charAt(0)
                    if(label.split(' ').length > 1){ ret += label.split(' ')[(label.split(' ').length - 1)].charAt(0)}
                    return ret;
                } else {
                    return label.substr(0,maxChar)
                }
            }
            return ""
        }
        function create() {
            var input = {
                type: "coin-create-collection",
                coinName: collection_name.textInputted,
                isCheckedCoinWithoutTags: _coin_without_tags.checked,
                isCheckedCoinWithTags: _coins_with_tags.checked,
                isApplyExistingCoins: also_apply_filter_to_existing_coins.value,
                isAutoLock: auto_lock.value,
            }
            QMLHandle.sendEvent(EVT.EVT_COIN_DETAILS_CONTROL_ENTER, input)
        }
        function clear() {
            collection_name.textInputted = ""
            collection_name.isValid = true
            collection_name.showError = false
            collection_name.errorText = ""
        }
    }
    onPrevClicked: {
        var input = {
            type: "create-a-new-collection-description"
        }
        QMLHandle.sendEvent(EVT.EVT_COIN_DETAILS_CONTROL_ENTER, input)
    }

    bottomRight: Row {
        spacing: 12
        QTextButton {
            width: 155
            height: 48
            label.text: STR.STR_QML_1450
            label.font.pixelSize: 16
            type: eTypeE
            onButtonClicked: {
                _content.contentItem.create()
            }
        }
    }

    QPopupEmpty {
        id: editTags
        property var questions
        property var action
        content: QViewCoinTags {
            isEdit: true
            inputType: "add-multi-coin-tags-for-collection"
            onPrevClicked: {
                editTags.close()
            }
            onSaveClicked: {
                editTags.close()
            }
        }
    }
}
