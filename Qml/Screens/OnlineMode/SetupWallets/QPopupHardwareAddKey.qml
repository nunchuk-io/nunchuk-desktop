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
    property int key_index: -1
    property string hardware: ""
    property bool isKeyHolderLimited: false
    property bool isMiniscript: false
    property bool isInheritance: false
    signal nextClicked()
    property string titleText: STR.STR_QML_942
    property string subtitleText: isKeyHolderLimited ? STR.STR_QML_1282 : ""
    property bool   supportWarning: true
    onOpened: {
        GroupWallet.addHardwareFromConfig(-1, "", -1)
        hardware = ""
    }
    QSupportedKeys {
        id: supportedKeys
        isInheritance: _popup.isInheritance
        isKeyHolderLimited: _popup.isKeyHolderLimited
        isMiniscript: _popup.isMiniscript
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
                id: _header
                anchors { left: parent.left; right: parent.right; top: parent.top }
                spacing: 0
                QLato {
                    id: titleLabel
                    width: parent.width
                    height: titleLabel.lineCount == 1 ? 32 : 60
                    text: titleText
                    font.weight: Font.Normal
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignTop
                    wrapMode: Text.WordWrap
                    lineHeightMode: Text.FixedHeight
                    lineHeight: 16
                }
                QLato {
                    width: parent.width
                    height: subtitleText != "" ? 32 : 0
                    text: subtitleText
                    font.weight: Font.Normal
                    font.pixelSize: 12
                    color: "#595959"
                    wrapMode: Text.WordWrap
                    lineHeightMode: Text.FixedHeight
                    lineHeight: 16
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    visible: subtitleText != ""
                }
            }
            // BUGFIX: the key list used to just stack under the header with no bottom bound, so the
            // warning box (anchored to parent.bottom) overlapped/hid the last item(s) whenever the list
            // was long enough. Now a fixed area between the header and the warning box, scrollable
            // (Flickable) if the list doesn't fit, with an explicit 12px gap above the warning box.
            Flickable {
                id: _listArea
                anchors {
                    top: _header.bottom
                    left: parent.left
                    right: parent.right
                    bottom: _warningLoader.active ? _warningLoader.top : parent.bottom
                    bottomMargin: _warningLoader.active ? 12 : 0
                }
                clip: true
                contentWidth: width
                contentHeight: _list.height
                // BUGFIX: QScrollBar's actual interactive hit-region is wider than its visual 8px
                // track and overlapped the row's rightmost pixels, blocking the radio icon's
                // click/hover there. No ScrollBar shown now - the list still scrolls via drag/wheel.
                Column {
                    id: _list
                    // BUGFIX: rows used to sit exactly flush against _listArea's clip edges (x:0 and
                    // x:width) on both sides - hover/click confirmed dead right at those flush edges
                    // even though the point is inside both hitArea's own bounds and _listArea's
                    // viewport. Inset 6px on each side so there's real (non-flush, non-clipped) margin
                    // for QRadioSelect's -12 hitArea extension to land in.
                    x: 6
                    width: parent.width - 12
                    spacing: 0
                    Repeater {
                        model: supportedKeys.listSupportedKeys()
                        QRadioButtonTypeA {
                            id: btn
                            width: parent.width
                            height: 48
                            label: modelData.name
                            layoutDirection: Qt.LeftToRight
                            fontFamily: "Lato"
                            fontPixelSize: 16
                            fontWeight: Font.Normal
                            enabled: !(modelData.type === NUNCHUCKTYPE.ADD_TAPSIGNER)
                            selected: GroupWallet.qAddHardware === modelData.type
                            onButtonClicked: {
                                if (GroupWallet.dashboardInfo) {
                                    GroupWallet.addHardwareFromConfig(modelData.type, GroupWallet.dashboardInfo.groupId, key_index, isInheritance)
                                } else {
                                    GroupWallet.addHardwareFromConfig(modelData.type, "", key_index, isInheritance)
                                }
                                hardware = modelData.tag
                            }
                        }
                    }
                }
            }
            // BUGFIX: single Loader instead of 2 mutually-exclusive Rectangles with independent
            // visible/height, so _listArea's bottom anchor above has one stable reference regardless of
            // which variant (isInheritance or not) is showing.
            Loader {
                id: _warningLoader
                anchors.bottom: parent.bottom
                width: 528
                active: supportWarning && !supportedKeys.isMiniscript
                sourceComponent: isInheritance ? _warningMultiComp : _warningSimpleComp
            }
            Component {
                id: _warningMultiComp
                QWarningBgMulti {
                    width: 528
                    icon: "qrc:/Images/Images/info-60px.svg"
                    txt.text: STR.STR_QML_1603
                }
            }
            Component {
                id: _warningSimpleComp
                QWarningBg {
                    width: 528
                    icon: "qrc:/Images/Images/info-60px.svg"
                    txt.text: STR.STR_QML_943
                }
            }
        }
        nextEnable: GroupWallet.qAddHardware === NUNCHUCKTYPE.ADD_COLDCARD ||
                    GroupWallet.qAddHardware === NUNCHUCKTYPE.ADD_LEDGER ||
                    GroupWallet.qAddHardware === NUNCHUCKTYPE.ADD_TREZOR ||
                    GroupWallet.qAddHardware === NUNCHUCKTYPE.ADD_BITBOX ||
                    GroupWallet.qAddHardware === NUNCHUCKTYPE.ADD_JADE ||
                    // KEEPKEY: wired flow, same group as the others.
                    GroupWallet.qAddHardware === NUNCHUCKTYPE.ADD_KEEPKEY
        onPrevClicked:{ closeClicked() }  
        onNextClicked:{ _popup.nextClicked() }            
    }
}
