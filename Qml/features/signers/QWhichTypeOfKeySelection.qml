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
import Features.Signers.ViewModels 1.0
import "../../Components/origins"
import "../../Components/customizes"
import "../../Components/customizes/Texts"
import "../../Components/customizes/Buttons"

QOnScreenContentTypeB {
    width: 600
    height: vm.heightOffset
    anchors.centerIn: parent
    label.text: vm.headline
    label.width: 600
    onCloseClicked: vm.close()
    onPrevClicked: vm.close()
    onNextClicked: {
        vm.onContinueClicked();
    }

    extraHeader: Item {
    }

    content: Item {
        Column {
            id: headerCol
            anchors { top: parent.top; left: parent.left; right: parent.right }
            spacing: 0

            QLato {
                id: titleLabel
                width: parent.width
                height: titleLabel.lineCount == 1 ? 32 : 60
                text: vm.title
                font.weight: Font.DemiBold
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignTop
                wrapMode: Text.WordWrap
                lineHeightMode: Text.FixedHeight
                lineHeight: 16
            }

            QLato {
                width: parent.width
                height: vm.subtitle != "" ? 32 : 0
                text: vm.subtitle
                font.weight: Font.Normal
                font.pixelSize: 12
                color: "#595959"
                wrapMode: Text.WordWrap
                lineHeightMode: Text.FixedHeight
                lineHeight: 16
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
                visible: vm.subtitle != ""
            }
        }

        // BUGFIX: list used to be a plain Column with no bounded height, so once the backend-driven
        // list grew past ~2 items (NUN-10192 filtering) it overflowed past the info box below and the
        // two visually overlapped. Bounded between the header and the info box, with its own scrollbar,
        // so the info box always stays put as the bottom edge - it never gets pushed or covered.
        Flickable {
            id: listArea
            anchors {
                top: headerCol.bottom
                left: parent.left
                right: parent.right
                bottom: infoBoxMulti.visible ? infoBoxMulti.top : (infoBoxOneLine.visible ? infoBoxOneLine.top : parent.bottom)
                // 12px gap so the last list row doesn't sit flush against the gray info box.
                bottomMargin: (infoBoxMulti.visible || infoBoxOneLine.visible) ? 12 : 0
            }
            clip: true
            contentWidth: width
            contentHeight: listColumn.height
            boundsBehavior: Flickable.StopAtBounds
            ScrollBar.vertical: ScrollBar {}

            Column {
                id: listColumn
                width: parent.width
                spacing: 0

                Repeater {
                    model: vm.supportedList
                    // BUGFIX (design update): no per-device caption on this screen (matches
                    // QPopupHardwareAddKey.qml's inheritance list, which is plain rows only) - removed
                    // the claim_note text that didn't appear in the Figma reference.
                    QRadioButtonTypeA {
                        id: btn
                        width: 528
                        height: 48
                        label: modelData.name
                        layoutDirection: Qt.LeftToRight
                        fontFamily: "Lato"
                        fontPixelSize: 16
                        fontWeight: Font.Normal
                        enabled: modelData.is_enabled
                        selected: vm.keyType === modelData.type
                        onButtonClicked: {
                            vm.selectKeyType(modelData.type);
                        }
                    }
                }
            }
        }

        QWarningBgMulti {
            id: infoBoxMulti
            width: 528
            visible: vm.description !== ""
            // BUGFIX: explicit height here overrode QWarningBgMulti's own self-sizing (height:
            // _content.height + 2*12), defeating the 12px-margin fix - removed, matches
            // QPopupHardwareAddKey.qml's usage which leaves height unset.
            icon: "qrc:/Images/Images/info-60px.svg"
            txt.text: vm.description
            anchors.bottom: parent.bottom
        }

        QWarningBg {
            id: infoBoxOneLine
            width: 528
            visible: vm.descriptionOneLine !== ""
            height: 60
            icon: "qrc:/Images/Images/info-60px.svg"
            txt.text: vm.descriptionOneLine
            anchors.bottom: parent.bottom
        }

    }

    WhichTypeOfKeySelectionViewModel {
        id: vm
    }
}
