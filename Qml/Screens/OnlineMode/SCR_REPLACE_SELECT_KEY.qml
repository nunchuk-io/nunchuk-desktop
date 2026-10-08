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
    property var alert: dashInfo.alert
    property bool isKeyHolderLimited: dashInfo.myRole === "KEYHOLDER_LIMITED"
    property string hardware: ""
    QSupportedKeys {
        id: supportedKeys
        isInheritance: alert.payload.is_inheritance
        isKeyHolderLimited: isKeyHolderLimited
        isMiniscript: dashInfo.walletType === "MINISCRIPT"
    }
    QOnScreenContentTypeB {
        width: 600
        height: 516
        anchors.centerIn: parent
        label.text: alert.payload.is_inheritance ? STR.STR_QML_1601 : STR.STR_QML_1602
        extraHeader: Item {}
        onCloseClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
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
                Flickable {
                    width: 528 + 8
                    height: 4 * 48
                    contentHeight: keyList.count * 48
                    clip: true
                    ScrollBar.vertical: ScrollBar {
                        active: true
                        policy: ScrollBar.AlwaysOn
                    }
                    Column {
                        spacing: 0
                        Repeater {
                            id: keyList
                            model: supportedKeys.listSupportedKeys()
                            QRadioButtonTypeA {
                                id: btn
                                width: 528
                                height: 48
                                label: modelData.name
                                layoutDirection: Qt.LeftToRight
                                fontFamily: "Lato"
                                fontPixelSize: 16
                                fontWeight: Font.Normal
                                enabled: !(modelData.type === NUNCHUCKTYPE.ADD_TAPSIGNER)
                                selected: GroupWallet.qAddHardware === modelData.type && hardware === modelData.tag
                                onButtonClicked: {
                                    var key_index = alert.payload.key_index
                                    GroupWallet.addHardwareFromConfig(modelData.type, GroupWallet.dashboardInfo.groupId, key_index)
                                    hardware = modelData.tag
                                }
                            }
                        }
                    }
                }
            }
            QWarningBgMulti {
                width: 528
                visible: alert.payload.is_inheritance && !supportedKeys.isMiniscript
                height: 108
                icon: "qrc:/Images/Images/info-60px.svg"
                txt.text: STR.STR_QML_1603
                anchors.bottom: parent.bottom
            }

            QWarningBg {
                width: 528
                visible: !alert.payload.is_inheritance && !supportedKeys.isMiniscript
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
        onPrevClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
        onNextClicked: {
            dashInfo.requestStartKeyReplacement(hardware)
        }
    }
}
