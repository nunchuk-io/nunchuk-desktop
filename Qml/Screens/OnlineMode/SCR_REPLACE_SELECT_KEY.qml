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
import QtQuick.Controls 1.4
import QtQuick.Controls 2.3
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.12
import HMIEVENTS 1.0
import EWARNING 1.0
import NUNCHUCKTYPE 1.0
import "../../Components/customizes"
import "../../Components/origins"
import "../../Components/customizes/Texts"
import "../../Components/customizes/Buttons"
import "../../../localization/STR_QML.js" as STR

QScreen {
    property var dashInfo: GroupWallet.dashboardInfo
    property int key_index: -1
    property bool isKeyHolderLimited: dashInfo.myRole === "KEYHOLDER_LIMITED"
    property string hardware: ""
    QOnScreenContentTypeB {
        width: 600
        height: 516
        anchors.centerIn: parent
        label.text: STR.STR_QML_106
        extraHeader: Item {}
        onCloseClicked: closeTo(NUNCHUCKTYPE.WALLET_TAB)
        content: Item {
            Column {
                anchors {
                    top: parent.top
                    topMargin: -8
                }
                QLato {
                    width: parent.width
                    height: 56
                    text: STR.STR_QML_942
                    font.weight: Font.Bold
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }
                QLato {
                    width: parent.width
                    height: 32
                    text: STR.STR_QML_1282
                    font.weight: Font.Normal
                    font.pixelSize: 12
                    color: "#595959"
                    wrapMode: Text.WordWrap
                    lineHeightMode: Text.FixedHeight
                    lineHeight: 16
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    visible: isKeyHolderLimited
                }
                Column {
                    spacing: 0
                    Repeater {
                        model: {
                            var ls = []
                            if (!isKeyHolderLimited) {
                                ls.push({add_type: NUNCHUCKTYPE.ADD_BITBOX,   txt: "BitBox"   , type: "bitbox02", tag: "BITBOX"  })
                                ls.push({add_type: NUNCHUCKTYPE.ADD_COLDCARD, txt: "COLDCARD" , type: "coldcard", tag: "COLDCARD"})
                            }
                            ls.push({add_type: NUNCHUCKTYPE.ADD_LEDGER,   txt: "Ledger"   , type: "ledger"  , tag: "LEDGER"  })
                            ls.push({add_type: NUNCHUCKTYPE.ADD_TREZOR,   txt: "Trezor"   , type: "trezor"  , tag: "TREZOR"  })
                            return ls
                        }
                        QRadioButtonTypeA {
                            id: btn
                            width: 528
                            height: 48
                            label: modelData.txt
                            layoutDirection: Qt.LeftToRight
                            fontFamily: "Lato"
                            fontPixelSize: 16
                            fontWeight: Font.Normal
                            selected: GroupWallet.qAddHardware === modelData.add_type && hardware === modelData.tag
                            onButtonClicked: {
                                GroupWallet.addHardwareFromConfig(modelData.add_type, GroupWallet.dashboardInfo.groupId, key_index)
                                hardware = modelData.tag
                            }
                        }
                    }
                }
            }
            QWarningBg {
                icon: "qrc:/Images/Images/info-60px.png"
                txt.text: STR.STR_QML_943
                anchors.bottom: parent.bottom
            }
        }
        nextEnable: GroupWallet.qAddHardware === NUNCHUCKTYPE.ADD_COLDCARD ||
                    GroupWallet.qAddHardware === NUNCHUCKTYPE.ADD_LEDGER ||
                    GroupWallet.qAddHardware === NUNCHUCKTYPE.ADD_TREZOR ||
                    GroupWallet.qAddHardware === NUNCHUCKTYPE.ADD_BITBOX;
        onPrevClicked: closeTo(NUNCHUCKTYPE.WALLET_TAB)
        onNextClicked: {
            dashInfo.requestStartKeyReplacement(hardware)
        }
    }
}
