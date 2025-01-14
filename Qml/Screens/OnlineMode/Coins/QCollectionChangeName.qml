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
import DataPool 1.0
import "../../../Components/origins"
import "../../../Components/customizes"
import "../../../Components/customizes/Chats"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../Components/customizes/QRCodes"
import "../../../Components/customizes/Popups"
import "../../../Components/customizes/services"
import "../../../../localization/STR_QML.js" as STR


QPopupEmpty {
    id: _collect_name
    closePolicy:Popup.NoAutoClose
    property bool isCreate: false
    property var collect: walletInfo.coinCollections.collection
    content: QOnScreenContentTypeB {
        id: _content
        width: 600
        height: 600
        anchors.centerIn: parent
        label.text: ""
        extraHeader: Item {}
        extraTop: Item {
            width: 600
            height: 112
            Item {
                width: 60
                height: 48
                anchors {
                    verticalCenter: parent.verticalCenter
                    left: parent.left
                    leftMargin: 36
                }

                Rectangle {
                    width: 48
                    height: 48
                    radius: 24
                    color: "#FDD95C"
                    QLato {
                        text: getFistCharterName(collect.coin_collection_name)
                        anchors.centerIn: parent
                        font.weight: Font.Bold
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    anchors.verticalCenter: parent.verticalCenter
                }
                Rectangle {
                    width: 24
                    height: 24
                    radius: 24
                    color: "#FFFFFF"
                    QIcon {
                        iconSize: 12
                        anchors.centerIn: parent
                        source: "qrc:/Images/Images/camera-dark.svg"
                    }
                    anchors {
                        bottom: parent.bottom
                        right: parent.right
                    }
                }
            }
        }

        onCloseClicked: { _collect_name.close() }
        content: Item {
            anchors {
                top: parent.top
                topMargin: 74
            }
            Connections {
                target: walletInfo
                onUpdateCollectionNameChanged: {
                    if (isError) {
                        collection_name.isValid = false
                        collection_name.showError = true
                        collection_name.errorText = STR.STR_QML_1473
                    } else {
                        _collect_name.close()
                    }
                }
            }

            Column {
                width: 528
                height: 287
                spacing: 24
                QTextInputBoxTypeB {
                    id: collection_name
                    label: STR.STR_QML_1435
                    boxWidth: 528
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
                QLine {
                    width: 528
                }
                Column {
                    width: 528
                    spacing: 0
                    Item {
                        width: parent.width
                        height: 36
                        QLato {
                            text: STR.STR_QML_1436
                            font.pixelSize: 16
                            font.weight: Font.Bold
                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter
                        }
                    }
                    QSwitchTypeLabel {
                        id: move_all_coins
                        width: 527
                        height: 65
                        label: STR.STR_QML_1437
                        value: false
                    }
                    QSwitchTypeLabel {
                        id: auto_lock
                        width: 527
                        height: 65
                        label: STR.STR_QML_1438
                        value: false
                    }
                }
            }
            function update() {
                var input = {
                    type: "coin-update-collection",
                    coin_id: collect.coin_collection_id,
                    coinName: collection_name.textInputted,
                    isMoveAll: move_all_coins.value,
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
            Connections {
                target: _collect_name
                onClearPopup: {
                    clear()
                }
            }
        }
        isShowLine: true
        onPrevClicked: { _collect_name.close() }
        bottomRight: QTextButton {
            width: 219
            height: 48
            label.text: isCreate ? STR.STR_QML_1450 : STR.STR_QML_1434
            label.font.pixelSize: 16
            type: eTypeE
            onButtonClicked: {
                if (isCreate) {
                    _content.contentItem.create()
                } else {
                    _content.contentItem.update()
                }

            }
        }
    }
    signal clearPopup()
}
