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
    
    SetUpBufferPeriodViewModel {
        id: vm
    }
    
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_862
    extraHeader: Item {}
    onCloseClicked: vm.close()

    content: Item {
        width: 728
        height: 500
        // Description text            
        Column {
            width: 728
            spacing: 24
            Column {
                width: 728
                spacing: 20
                QLato {
                    width: 728
                    height: 56
                    text: STR.STR_QML_2222
                    font.pixelSize: 16
                    lineHeight: 20
                    lineHeightMode: Text.FixedHeight
                    wrapMode: Text.WordWrap
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }
                QLato {
                    width: 728
                    height: 20
                    text: STR.STR_QML_2223
                    font.pixelSize: 16
                    lineHeight: 20
                    lineHeightMode: Text.FixedHeight
                    wrapMode: Text.WordWrap
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }
            }
            
            // Buffer period options grid
            Item {
                width: 728
                height: 212                
                Grid {
                    width: parent.width
                    height: parent.height
                    columns: 2
                    rowSpacing: 12
                    columnSpacing: 12
                    
                    Repeater {
                        model: vm.periods
                        
                        QRadioButtonTypeE {
                            id: btn
                            width: 356
                            height: 60
                            label: modelData.display_name
                            textWidth: modelData.textWidth
                            fontPixelSize: 16
                            fontWeight: btn.selected ? Font.ExtraBold : Font.DemiBold
                            selected: vm.buffer_period_id === modelData.id
                            textBadge: modelData.is_recommended ? STR.STR_QML_879 : ""
                            onButtonClicked: {
                                vm.buffer_period_id = modelData.id
                            }
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
            width: label.paintedWidth + 32
            height: 48
            label.text: STR.STR_QML_245
            label.font.pixelSize: 14
            type: eTypeF
            onButtonClicked: vm.close()
        }
        
        QTextButton {
            width: label.paintedWidth + 32
            height: 48
            label.text: STR.STR_QML_265
            label.font.pixelSize: 14
            type: eTypeE
            onButtonClicked: vm.onContinueClicked()
        }
    }
}
