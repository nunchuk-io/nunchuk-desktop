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
import QtQuick.Controls 2.0
import NUNCHUCKTYPE 1.0
import Features.Claiming.ViewModels 1.0
import "./../../../origins"
import "./../../../customizes"
import "./../../../customizes/Chats"
import "./../../../customizes/Texts"
import "./../../../customizes/Buttons"
import "../../../../features/inheritance/components"

QOnScreenContentTypeA {
    id: root
    
    ReleaseScheduleViewModel {
        id: vm
    }
    
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_2151
    extraHeader: Item {}
    
    property string totalAllocated: "100%"
    
    onCloseClicked: vm.close()
    
    content: Item {
        width: 661
        height: 450
        
        Column {
            anchors.fill: parent
            spacing: 24
            
            // Description text
            Item {
                width: parent.width
                height: descText.paintedHeight
                
                QLato {
                    id: descText
                    text: STR.STR_QML_2241
                    font.pixelSize: 16
                    wrapMode: Text.WordWrap
                    lineHeight: 20
                    lineHeightMode: Text.FixedHeight
                }
            }
            
            // Main content area with two columns
            Row {
                height: 428
                spacing: 12
                
                Rectangle {
                    width: 370
                    height: 428
                    border.color: "#DEDEDE"
                    border.width: 1
                    color: "#FFFFFF"
                    radius: 12
                    Flickable {
                        width: parent.width
                        height: parent.height
                        contentHeight: stagesColumn.height
                        clip: true
                        ScrollBar.vertical: ScrollBar { active: true }
                        
                        Column {
                            id: stagesColumn
                            width: parent.width
                            spacing: 0
                            
                            Repeater {
                                model: vm.stagesData                                
                                QStageDetail {
                                    width: parent.width
                                    stageName: modelData.name
                                    stagePercentage: modelData.percentage
                                    stageColor: modelData.color
                                    firstWithdrawalDate: modelData.firstWithdrawalDate
                                    releaseInfo: modelData.releaseInfo
                                    isExpanded: modelData.isExpanded
                                    installments: modelData.displayInstallments
                                    isFirst: index === 0
                                    isLast: index === vm.stagesData.length - 1
                                    hasEdit: false
                                    enabled: index <= vm.current_stage_index
                                    opacity: enabled ? 1.0 : 0.4
                                    isLastEnabled: index === vm.current_stage_index
                                    current_installment_index: vm.current_installment_index
                                    
                                    onExpandClicked: {
                                        vm.onExpandClicked(index)
                                    }
                                }
                            }
                        }
                    }
                }
                
                
                // Right column: Overview
                Column {
                    width: 346
                    spacing: 10                   
                    Rectangle {
                        width: 346
                        height: 48
                        color: "#F5F5F5"
                        radius: 12
                        Row {
                            width: parent.width
                            height: 16
                            spacing: 6
                            anchors {
                                left: parent.left
                                leftMargin: 12
                                verticalCenter: parent.verticalCenter
                            }

                            QIcon {
                                anchors.verticalCenter: parent.verticalCenter
                                source: "qrc:/Images/Images/period.svg"
                                width: 16
                                height: 16
                            }

                            QLato {
                                text: STR.STR_QML_2242 + vm.availablePercentage + "% (" + vm.availableBalanceDisplay + (AppSetting.unit === NUNCHUCKTYPE.BTC ? " BTC)" : " sats)")
                                font.pixelSize: 12
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignLeft
                                font.weight: Font.Bold
                            }
                        }
                    }
                }
            }
        }
    }
    bottomLeft: Item {}
    bottomRight: Row {
        spacing: 12
        QTextButton {
            width: label.paintedWidth + 32
            height: 48
            label.text: STR.STR_QML_777
            label.font.pixelSize: 14
            type: eTypeR
            onButtonClicked: {
                vm.close()
            }
        }
        
        QTextButton {
            width: label.paintedWidth + 32
            height: 48
            label.text: STR.STR_QML_1735
            label.font.pixelSize: 14
            type: eTypeE
            enabled: vm.availableBalanceSats > 0
            onButtonClicked: {
                vm.withdrawBitcoinClicked()
            }
        }
    }
}