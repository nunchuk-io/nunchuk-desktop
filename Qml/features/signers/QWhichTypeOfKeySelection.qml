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
            anchors.fill: parent
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

            Column {
                spacing: 0

                Repeater {
                    model: vm.supportedList
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
            width: 528
            visible: vm.description !== ""
            height: 108
            icon: "qrc:/Images/Images/info-60px.svg"
            txt.text: vm.description
            anchors.bottom: parent.bottom
        }

        QWarningBg {
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
