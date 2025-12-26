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
import HMIEVENTS 1.0
import NUNCHUCKTYPE 1.0
import "../../../Components/origins"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../../localization/STR_QML.js" as STR

Rectangle {
    width: 352
    height: 480
    color: "#F5F5F5"
    radius: 12
    property bool   walletIsReplaced        : walletInfo.isReplaced
    property bool   walletIsAssisted        : walletInfo.isAssistedWallet
    property bool   walletIsShared          : walletInfo.isSharedWallet
    property bool   walletIsSandboxWallet   : walletInfo.isGlobalGroupWallet
    readonly property var types: [
        {type: NUNCHUCKTYPE.NATIVE_SEGWIT,   displayName: STR.STR_QML_062 },
        {type: NUNCHUCKTYPE.TAPROOT,         displayName: STR.STR_QML_553 },
        {type: NUNCHUCKTYPE.NESTED_SEGWIT,   displayName: STR.STR_QML_063 },
        {type: NUNCHUCKTYPE.LEGACY,          displayName: STR.STR_QML_064 },
    ]
    function editFinished() {
        wlname.typingFinish()
    }
    function getWalletTypeDes() {
        if (walletInfo.walletType === NUNCHUCKTYPE.MINISCRIPT) {
            return STR.STR_QML_1801
        } else {
            return (walletInfo.walletN === 1) ? STR.STR_QML_070 : 
                            qsTr("%1/%2 %3").arg(walletInfo.walletM).arg(walletInfo.walletN).arg(STR.STR_QML_069)
        }
    }
    Column {
        anchors {
            fill: parent
            topMargin: 18
            leftMargin: 18
            bottomMargin: 18
            rightMargin: 6
        }
        spacing: 8
        Rectangle {
            width: 326
            height: walletCol.childrenRect.height
            color: "transparent"
            Column {
                id: walletCol
                width: parent.width
                spacing: 4
                QTextInputBoxTypeA {
                    id: wlname
                    width: 326
                    placeholderText: ""
                    text: walletInfo.walletName
                    backgroundColor: "Transparent"
                    borderColor: "Transparent"
                    color: "#000000"
                    font.family: "Lato"
                    font.pixelSize: 20
                    font.weight: Font.Bold
                    anchors.horizontalCenter: parent.horizontalCenter
                    showEdit: wlname.enabled
                    enabled: {
                        if (walletInfo.isGroupWallet) {
                            return walletInfo.myRole === "MASTER"
                        }
                        else {
                            return true
                        }
                    }
                    onTypingFinished: {
                        if(currentText !== walletInfo.walletName) {
                            var infoObj = {
                                "name"          : currentText,
                                "description"   : ""
                            };
                            QMLHandle.sendEvent(EVT.EVT_WALLET_INFO_EDIT_NAME, infoObj)
                        }
                    }
                }
                QBadge {
                    text: (myRole === "FACILITATOR_ADMIN") ? "••••••" : getWalletTypeDes()
                    color: "#EAEAEA"
                    font.weight: Font.Medium
                    font.pixelSize: 12
                }
                QTypeWallet {
                    height: label.text == "" ? 0 : 16
                    icon.iconSize: 16
                    icon.source: {
                        if (walletIsShared) return "qrc:/Images/Images/collab-wallet-dark.svg"
                        else if (walletIsSandboxWallet) return "qrc:/Images/Images/sandboxGroup.svg"
                        else if (walletIsAssisted && !walletIsReplaced) return "qrc:/Images/Images/collab-wallet-dark.svg"
                        else return ""
                    }
                    label.text: {
                        if (walletIsShared) return STR.STR_QML_438
                        else if (walletIsSandboxWallet) return STR.STR_QML_1675
                        else if (walletIsAssisted && !walletIsReplaced) return (walletInfo.myRole === "FACILITATOR_ADMIN") ? "••••••" : STR.STR_QML_679
                        else return ""
                    }
                    label.font.weight: Font.Medium
                    label.font.pixelSize: 12
                    color: "transparent"
                }
            }
        }

        Column {
            width: 310
            height: 300
            spacing: 24
            Item {
                width: parent.width
                height: 48
                visible: walletInfo.walletAliasName === "" && isAlias
                Row {
                    anchors{
                        left: parent.left
                        leftMargin: 0
                        verticalCenter: parent.verticalCenter
                    }
                    spacing: 6
                    QLato {
                        text: STR.STR_QML_1185
                        font.weight: Font.Bold
                    }
                    QIconButton {
                        iconSize: 24
                        icon: "qrc:/Images/Images/right-arrow-dark.svg"
                        onButtonClicked: {
                            eFlow = eWALLET_SET_ALIAS
                        }
                        bgColor: "transparent"
                    }
                }
            }
            Item {
                width: parent.width
                height: 48
                visible: walletInfo.walletAliasName !== "" && isAlias
                Row {
                    anchors{
                        left: parent.left
                        leftMargin: 0
                        verticalCenter: parent.verticalCenter
                    }
                    spacing: 6
                    QLato {
                        text: STR.STR_QML_1190
                        font.weight: Font.Bold
                    }
                    QIconButton {
                        iconSize: 24
                        icon: "qrc:/Images/Images/right-arrow-dark.svg"
                        onButtonClicked: {
                            eFlow = eWALLET_CHANGE_ALIAS
                        }
                        bgColor: "transparent"
                    }
                }
            }

            Item {
                id: addresstype
                width: parent.width
                height: 28
                QLato {
                    anchors{
                        left: parent.left
                        leftMargin: 0
                        verticalCenter: parent.verticalCenter
                    }
                    text: STR.STR_QML_066
                    font.weight: Font.Normal
                }
                QLato {
                    anchors{
                        right: parent.right
                        rightMargin: 0
                        verticalCenter: parent.verticalCenter
                    }
                    text: types.find(function(e) {return e.type === walletInfo.walletAddressType }).displayName
                    font.weight: Font.Bold
                }
            }
        }
    }
}
