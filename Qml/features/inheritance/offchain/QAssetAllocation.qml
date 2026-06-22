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
import "../../../Components/customizes"
import "../../../Components/customizes/Buttons"
import "../../../Components/customizes/Chats"
import "../../../Components/customizes/Texts"
import "../../../Components/origins"
import "../components"
import DataPool 1.0
import EWARNING 1.0
import Features.Inheritance.OffChain.ViewModels 1.0
import HMIEVENTS 1.0
import NUNCHUCKTYPE 1.0
import QtGraphicalEffects 1.12
import QtQuick 2.12
import QtQuick.Controls 2.3

QOnScreenContentTypeA {
    id: root
    
    AssetAllocationViewModel {
        id: vm
    }
    
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_2181
    extraHeader: Item {}
    onCloseClicked: vm.close()
    
    content: Item {
        width: 850
        height: 550
        
        Column {
            anchors.fill: parent
            anchors.margins: 0
            spacing: 24
            width: parent.width
            
            // Description text
            QLato {
                width: parent.width
                height: implicitHeight
                text: STR.STR_QML_2227
                font.pixelSize: 16
                wrapMode: Text.WordWrap
                lineHeight: 20
                lineHeightMode: Text.FixedHeight
            }
            
            // Main content: beneficiaries list and donut chart
            Row {
                width: parent.width
                height: parent.height - 110
                spacing: 24
                
                Rectangle {
                    width: 378
                    height: 428
                    color: "#FFFFFF"
                    border {
                        color: "#DEDEDE"
                        width: 1
                    }
                    radius: 8
                    
                    Flickable {
                        anchors.fill: parent
                        anchors.margins: 12
                        contentHeight: beneficiariesColumn.implicitHeight
                        clip: true
                        ScrollBar.vertical: ScrollBar { active: true }
                        
                        Column {
                            id: beneficiariesColumn
                            width: parent.width - 2
                            anchors.top: parent.top
                            anchors.topMargin: 9
                            spacing: 16
                            
                            Repeater {
                                model: vm.assetAllocation.length
                                 
                                QBeneficiaryItem {
                                    width: 343
                                    beneficiaryIndex: index
                                    beneficiaryEmail: vm.assetAllocation[index].email
                                    beneficiaryPercentage: vm.assetAllocation[index].asset_percentage
                                    
                                    onDeleteBeneficiaryClicked: vm.deleteBeneficiary(beneficiaryIndex)
                                    onEmailChanged: vm.updateBeneficiaryEmail(beneficiaryIndex, email)
                                    onPercentageChanged: vm.updateBeneficiaryPercentage(beneficiaryIndex, percent)
                                }
                            }
                        }
                    }
                }
            
                // Right column: Donut chart
                Rectangle {
                    width: 343
                    height: legendColumn.childrenRect.height + 18*2
                    color: "#F5F5F5"
                    radius: 8
                    
                    Row {
                        anchors.fill: parent
                        anchors.margins: 12
                        spacing: 16
                        width: parent.width
                        
                        DonutChart {
                            id: donutChart
                            anchors.verticalCenter: parent.verticalCenter
                            width: 60
                            height: 60                                
                            chartData: vm.assetAllocation
                        }
                        
                        // Legend
                        Column {
                            id: legendColumn
                            anchors.verticalCenter: parent.verticalCenter
                            width: 172
                            spacing: 4
                            
                            Repeater {
                                model: vm.assetAllocation
                                
                                Row {
                                    width: parent.width
                                    height: 16
                                    spacing: 8
                                    
                                    Rectangle {
                                        width: 12
                                        height: 12
                                        color: modelData.color
                                        radius: 2
                                        anchors.verticalCenter: parent.verticalCenter
                                    }
                                    
                                    QLato {
                                        text: STR.STR_QML_2177 + " " + (index + 1) + ": " + modelData.asset_percentage + "%"
                                        font.pixelSize: 12
                                        anchors.verticalCenter: parent.verticalCenter
                                    }
                                }
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
            width: label.implicitWidth + 32
            height: 48
            label.text: STR.STR_QML_245
            label.font.pixelSize: 14
            type: eTypeF
            onButtonClicked: vm.close()
        }
        
        QTextButton {
            width: label.implicitWidth + 32
            height: 48
            label.text: "+  " + STR.STR_QML_2180
            label.font.pixelSize: 14
            type: eTypeR
            onButtonClicked: vm.addBeneficiary()
        }
        
        QTextButton {
            width: label.implicitWidth + 32
            height: 48
            label.text: STR.STR_QML_835
            label.font.pixelSize: 14
            type: eTypeE
            onButtonClicked: {
                // Save beneficiary allocations
                vm.saveBeneficiaries()
            }
        }
    }
}

