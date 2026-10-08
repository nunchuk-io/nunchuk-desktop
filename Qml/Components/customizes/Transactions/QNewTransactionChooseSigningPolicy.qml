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
import "../../../Components/customizes/Signers"
import "../../../Components/customizes/Wallets"
import "../../../Components/customizes/Popups"
import "../../../Components/customizes/Wallets/miniscript"
import "../../../Components/customizes/Transactions/miniscript"
import "../../../../localization/STR_QML.js" as STR

QOnScreenContentTypeA {
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_1865
    isShowLine: true
    property int selectPathIndex: 0
    content: Flickable {
        width: 720 + 8
        height: walletInfo.walletAddressType === NUNCHUCKTYPE.TAPROOT ? (504 - 113 - 24) : 504
        flickableDirection: Flickable.VerticalFlick
        ScrollBar.vertical: ScrollBar { active: true }
        contentHeight: columnMiniscript.childrenRect.height
        contentWidth: 504
        clip: true                
        Column {
            id: columnMiniscript
            width: 500
            Item {
                width: 500
                height: 40
                QLato {
                    width: 500
                    text: STR.STR_QML_1864
                    font.weight: Font.Bold
                    lineHeightMode: Text.FixedHeight
                    lineHeight: 28
                    wrapMode: Text.WordWrap
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
            Item {
                width: 500
                height: 16
            }
            Item {
                width: 500
                height: 24
                Row {
                    anchors.fill: parent
                    spacing: 8
                    QIcon {
                        iconSize: 24
                        source: "qrc:/Images/Images/policies_dark.svg"
                    }
                    QLato {
                        width: paintedWidth
                        text: STR.STR_QML_1844
                        font.weight: Font.Bold
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
            }
            Item {
                width: 500
                height: 12
            }
            Column {
                width: 500
                spacing: 16
                Repeater {
                    model: transactionInfo.scriptPaths
                    delegate: QScriptPathTransactionDelegate {
                        hasTag: false
                        width: 500
                        treeMiniscript: modelData.scriptTree
                        estimatedFee: modelData.estimatedFee
                        currencyEstimatedFee: modelData.currencyEstimatedFee
                        isFocused: selectPathIndex === index
                        onSelectedClicked: {
                            selectPathIndex = index
                            transactionInfo.setSigningPathSelected(modelData.signingPath)
                        }                   
                    }
                }
            }            
        }
    }
}
