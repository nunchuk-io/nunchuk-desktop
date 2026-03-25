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

QOnScreenContentTypeA {
    id: _content
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_089
    onCloseClicked: vm.close()
    onPrevClicked: vm.back()
    extraHeader: Item { }
    content: Item {
        QRadioList {
            id: optionList
            width: 725
            height: optionList.count * 48
            model: vm.options
            delegateType: 0
            function customizeDelegate() {
                return _TypeA
            }
            Component {
                id: _TypeA
                Item {
                    width: 725
                    height: 48
                    QRadioButtonTypeA {
                        width: parent.width
                        height: 48
                        fontPixelSize: 16
                        fontWeight: Font.Normal
                        anchors.left: parent.left
                        anchors.leftMargin: 0
                        layoutDirection: Qt.LeftToRight
                        label: modelData.label
                        selected: vm.option === modelData.id
                        onButtonClicked: {
                            vm.selectedOption(modelData.id)
                        }
                    }
                }
            }
        }        
    }

    onNextClicked: {
        vm.onContinueClicked()
    }

    AddSoftwareKeyViewModel {
        id: vm
    }
}
