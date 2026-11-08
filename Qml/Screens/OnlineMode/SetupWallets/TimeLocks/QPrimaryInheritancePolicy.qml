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
import NUNCHUCKTYPE 1.0
import "../../../../Components/origins"
import "../../../../Components/customizes"
import "../../../../Components/customizes/Texts"
import "../../../../Components/customizes/Buttons"
import "../../../../Components/customizes/Wallets"
import "../../../../../localization/STR_QML.js" as STR

QOnScreenContentTypeB {
    id: _policyScreen
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    property string titleText: STR.STR_QML_2391
    property string descriptionText: STR.STR_QML_2392
    label.text: titleText
    property int valueM: 2
    property int valueN: 4
    property int maxN: 15
    signal saved(int m, int n)

    function setup(m, n) {
        valueN = Math.max(1, n)
        valueM = Math.max(1, Math.min(m, valueN))
    }

    content: Item {
        Column {
            width: parent.width
            spacing: 16
            QLato {
                width: 680
                text: _policyScreen.descriptionText
                lineHeightMode: Text.FixedHeight
                lineHeight: 28
                wrapMode: Text.WordWrap
                horizontalAlignment: Text.AlignLeft
            }
            QLato {
                width: parent.width
                text: STR.STR_QML_2393
                lineHeightMode: Text.FixedHeight
                lineHeight: 28
                wrapMode: Text.WordWrap
                horizontalAlignment: Text.AlignLeft
            }
            Column {
                spacing: 24
                QNumberChanged {
                    headline: STR.STR_QML_1652
                    description: STR.STR_QML_1826
                    value: _policyScreen.valueM
                    minusEnabled: _policyScreen.valueM > 1
                    plusEnabled: _policyScreen.valueM < _policyScreen.maxN
                    onValueUpdated: (newValue) => {
                        _policyScreen.valueM = newValue
                        if (_policyScreen.valueN < _policyScreen.valueM) {
                            _policyScreen.valueN = _policyScreen.valueM
                        }
                    }
                }
                QNumberChanged {
                    headline: STR.STR_QML_1827
                    description: STR.STR_QML_1828
                    value: _policyScreen.valueN
                    minusEnabled: _policyScreen.valueN > _policyScreen.valueM
                    plusEnabled: _policyScreen.valueN < _policyScreen.maxN
                    onValueUpdated: (newValue) => {
                        _policyScreen.valueN = newValue
                        if (_policyScreen.valueM > _policyScreen.valueN) {
                            _policyScreen.valueM = _policyScreen.valueN
                        }
                    }
                }
            }
        }
    }

    bottomLeft: QTextButton {
        width: 80
        height: 48
        label.text: STR.STR_QML_059
        label.font.pixelSize: 16
        type: eTypeF
        onButtonClicked: _policyScreen.prevClicked()
    }
    bottomRight: QTextButton {
        width: label.paintedWidth + 2*20
        height: 48
        label.text: STR.STR_QML_265
        label.font.pixelSize: 16
        type: eTypeE
        onButtonClicked: _policyScreen.saved(_policyScreen.valueM, _policyScreen.valueN)
    }
}
