/**************************************************************************
 * This file is part of the Nunchuk software (https://nunchuk.io/)        *
 * Copyright (C) 2020-2022 Enigmo						          *
 * Copyright (C) 2022 Nunchuk						              *
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
import Features.Inheritance.OffChain.ViewModels 1.0

QOnScreenContentTypeA {
    id: root
    
    SetUpBufferPeriodWithdrawalViewModel {
        id: vm
    }
    
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_2160
    extraHeader: Item {}
    onCloseClicked: vm.close()
    property var bufferApplyOns: [
        { id: "FIRST_WITHDRAWAL", labelTop: STR.STR_QML_2186, labelBottom: STR.STR_QML_2217 },
        { id: "EVERY_WITHDRAWAL", labelTop: STR.STR_QML_2187, labelBottom: STR.STR_QML_2218 }
    ]
    
    
    content: Item {
        width: 728
        height: 500
        // Description text            
        Column {
            width: 728
            spacing: 24
            QLato {
                width: 728
                height: 20
                text: STR.STR_QML_2216
                font.pixelSize: 16
                lineHeight: 20
                lineHeightMode: Text.FixedHeight
                wrapMode: Text.WordWrap
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
            }
            
            // Buffer period options grid
            Item {
                width: 728
                height: 212                
                Row {
                    width: parent.width
                    height: parent.height
                    spacing: 12
                    
                    Repeater {
                        model: bufferApplyOns
                        QRadioButtonTypeF {
                            width: 356
                            height: 100
                            labelMaxWidth: 284
                            labelBottomLineHeight: 20
                            selected: vm.buffer_apply_on === modelData.id
                            labelTop: modelData.labelTop
                            labelBottom: modelData.labelBottom
                            onButtonClicked: { vm.buffer_apply_on = modelData.id }
                        }
                    }
                }
            }
        }
    }

    onPrevClicked: vm.back()
    
    bottomRight: Row {
        spacing: 12
        
        QTextButton {
            width: 100
            height: 48
            label.text: STR.STR_QML_245
            label.font.pixelSize: 14
            type: eTypeF
            onButtonClicked: vm.close()
        }
        
        QTextButton {
            width: 100
            height: 48
            label.text: STR.STR_QML_265
            label.font.pixelSize: 14
            type: eTypeE
            onButtonClicked: vm.onContinueClicked()
        }
    }
}
