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
import QtQuick.Controls 2.1
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.0
import "../../origins"
import "../../../Components/customizes/Buttons"

QListView {
    id: listView
    property int widthDelegate: listView.width
    readonly property int _RadioTypeA: 1
    readonly property int _RadioTypeB: 2
    property int delegateType: _RadioTypeA
    property int delegateHeight: 48
    delegate: switch(delegateType) {
              case _RadioTypeA: return _TypeA
              case _RadioTypeB: return _TypeB
              default: return _TypeA
              }

    function radioSelect() {
        return false
    }

    function setRadioSelect(id, data) {

    }

    Component {
        id: _TypeA
        Rectangle {
            border.width: 2
            border.color: btn.selected ? "#031F2B" : "#DEDEDE"
            width: widthDelegate
            height: delegateHeight
            radius: 12
            QRadioButtonTypeA {
                width: widthDelegate - 18*2
                height: delegateHeight
                fontPixelSize: 16
                fontWeight: Font.Bold
                anchors.horizontalCenter: parent.horizontalCenter
                layoutDirection: Qt.LeftToRight
                label: modelData.label
                selected: radioSelect() === modelData.id
                onButtonClicked: {
                    setRadioSelect(modelData.id, modelData)
                }
            }
        }
    }
    Component {
        id: _TypeB
        Rectangle {
            border.width: 2
            border.color: btn.selected ? "#031F2B" : "#DEDEDE"
            width: widthDelegate
            height: delegateHeight
            radius: 12
            QRadioButtonTypeA {
                id: btn
                width: widthDelegate - 18*2
                height: delegateHeight
                fontPixelSize: 16
                fontWeight: Font.Bold
                anchors.horizontalCenter: parent.horizontalCenter
                layoutDirection: Qt.RightToLeft
                label: modelData.label
                selected: radioSelect() === modelData.id
                onButtonClicked: {
                    setRadioSelect(modelData.id, modelData)
                }
            }
        }
    }
}
