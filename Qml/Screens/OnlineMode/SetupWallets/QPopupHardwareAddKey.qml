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

QPopupEmpty {
    id: _popup
    property string hardware: ""
    property bool isInheritance: false
    onOpened: {
        GroupWallet.addHardwareFromConfig(-1, "", -1)
        hardware = ""
    }
    content: QOnScreenContentTypeB {
        width: 600
        height: 546
        anchors.centerIn: parent
        label.text: isInheritance ? STR.STR_QML_1601 : STR.STR_QML_1602
        label.width: 600
        extraHeader: Item {}
        onCloseClicked: { _popup.close() }
        content: Item {
            Column {
                anchors.fill: parent
                spacing: 0
                QLato {
                    width: parent.width
                    height: 20 + 12
                    text: STR.STR_QML_942
                    font.weight: Font.Bold
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignTop
                }
                QLato {
                    width: parent.width
                    height: isKeyHolderLimited ? 32 : 0
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
                                if (!isInheritance) {
                                    ls.push({add_type: NUNCHUCKTYPE.ADD_BITBOX,   txt: "BitBox"   , type: "bitbox02", tag: "BITBOX"  })
                                }
                                ls.push({add_type: NUNCHUCKTYPE.ADD_COLDCARD, txt: "COLDCARD" , type: "coldcard", tag: "COLDCARD"})
                            }
                            if (!isInheritance) {
                                ls.push({add_type: NUNCHUCKTYPE.ADD_LEDGER,   txt: "Ledger"   , type: "ledger"  , tag: "LEDGER"  })
                                ls.push({add_type: NUNCHUCKTYPE.ADD_TREZOR,   txt: "Trezor"   , type: "trezor"  , tag: "TREZOR"  })
                            }
                            if (isInheritance) {
                                ls.push({add_type: NUNCHUCKTYPE.ADD_TAPSIGNER,   txt: "TAPSIGNER"   , type: "TAPSIGNER"  , tag: "INHERITANCE"  })
                            }
                            if (!isInheritance) {
                                ls.push({add_type: NUNCHUCKTYPE.ADD_JADE,   txt: "Blockstream Jade"   , type: "JADE"  , tag: "JADE"  })
                            }
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
                            enabled: !(modelData.add_type === NUNCHUCKTYPE.ADD_TAPSIGNER)
                            selected: GroupWallet.qAddHardware === modelData.add_type
                            onButtonClicked: {
                                GroupWallet.addHardwareFromConfig(modelData.add_type, GroupWallet.dashboardInfo.groupId, key_index, isInheritance)
                                hardware = modelData.tag
                            }
                        }
                    }
                }
            }
            QWarningBgMulti {
                width: 528
                visible: isInheritance
                height: 108
                icon: "qrc:/Images/Images/info-60px.svg"
                txt.text: STR.STR_QML_1603
                anchors.bottom: parent.bottom
            }

            QWarningBg {
                width: 528
                visible: !isInheritance
                height: 60
                icon: "qrc:/Images/Images/info-60px.svg"
                txt.text: STR.STR_QML_943
                anchors.bottom: parent.bottom
            }
        }
        nextEnable: GroupWallet.qAddHardware === NUNCHUCKTYPE.ADD_COLDCARD ||
                    GroupWallet.qAddHardware === NUNCHUCKTYPE.ADD_LEDGER ||
                    GroupWallet.qAddHardware === NUNCHUCKTYPE.ADD_TREZOR ||
                    GroupWallet.qAddHardware === NUNCHUCKTYPE.ADD_BITBOX ||
                    GroupWallet.qAddHardware === NUNCHUCKTYPE.ADD_JADE
        onPrevClicked:{ closeClicked() }
        onNextClicked: {
            if (ServiceSetting.existHardware(hardware)) {
                QMLHandle.sendEvent(EVT.EVT_ADD_HARDWARE_KEY_EXIST_REQ)
            } else {
                QMLHandle.sendEvent(EVT.EVT_ADD_HARDWARE_KEY_REQUEST)
            }
        }
    }
}
