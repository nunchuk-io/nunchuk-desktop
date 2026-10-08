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
import Features.Signers.ViewModels 1.0
import "../../Components/origins"
import "../../Components/customizes"
import "../../Components/customizes/Chats"
import "../../Components/customizes/Texts"
import "../../Components/customizes/Buttons"
import "../../Components/customizes/Lists"
import "./platform"

QOnScreenContentTypeA {
    id: _content
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_2121
    onCloseClicked: vm.close()
    extraHeader: Item { }
    property var dataMap: [
        { id: "global-policy", label: STR.STR_QML_2122, subLabel: STR.STR_QML_2132, iconUrl: "qrc:/Images/Images/global-key.svg" },
        { id: "per-key-policy", label: STR.STR_QML_2126, subLabel: STR.STR_QML_2133, iconUrl: "qrc:/Images/Images/per-key.svg" }
    ]
    
    content: Item {
        Column {
            width: parent.width
            spacing: 32
            QLato {
                text: STR.STR_QML_2131
                font.pixelSize: 16
                font.weight: Font.Normal
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft
            }
            Column {
                width: parent.width
                spacing: 24
                Repeater {
                    model: dataMap
                    Rectangle {
                        width: 343
                        height: 80
                        border.width: 1
                        border.color: "#DEDEDE"
                        radius: 8
                        QRadioSelectPolicy {
                            anchors {
                                left: parent.left
                                leftMargin: 12
                            }
                            width: 343 - 12*2
                            height: 80
                            label: modelData.label
                            subLabel: modelData.subLabel
                            iconUrl: modelData.iconUrl
                            selected: vm.selectedPolicyId === modelData.id
                            onButtonClicked: {
                                vm.selectedPolicyId = modelData.id
                            }
                        }
                    }
                }
            }
        }
    }

    onPrevClicked: {
        vm.back()
    }

    onNextClicked: {
        vm.onContinueClicked()
    }

    PlatformKeyPoliciesViewModel {
        id: vm
    }
}
