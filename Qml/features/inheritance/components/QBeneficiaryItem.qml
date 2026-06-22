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
import QtQuick.Controls 2.3
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../Components/origins"

Rectangle {
    color: "#F9F9F9"
    border.color: "#F5F5F5"
    border.width: 1
    radius: 8
    height: 160
    
    // Properties
    property int beneficiaryIndex: 0
    property string beneficiaryEmail: ""
    property int beneficiaryPercentage: 0
    
    // Signals
    signal deleteBeneficiaryClicked(int index)
    signal emailChanged(int index, string email)
    signal percentageChanged(int index, int percent)
    
    Column {
        id: contentColumn
        anchors.fill: parent
        anchors.margins: 12
        spacing: 20

        QTextInputBoxTypeB {
            id: _input_email
            label: STR.STR_QML_2177 + " " + (beneficiaryIndex + 1)
            labelComponent.textFormat: Text.RichText
            labelComponent.font.pixelSize: 12
            boxWidth: parent.width
            boxHeight: 48
            textInputted: beneficiaryEmail
            input.placeholderText: ""
            onTextInputtedChanged: {
                if(!_input_email.isValid){
                    _input_email.isValid = true
                    _input_email.errorText = ""
                }
                _input_email.showError = false;
                emailChanged(beneficiaryIndex, _input_email.textInputted)
            }
        }
        Row {
            width: parent.width
            height: 48
            spacing: 8
            Slider {
                id: percentageSlider
                width: 252
                from: 0
                to: 100
                value: beneficiaryPercentage
                stepSize: 1
                anchors.verticalCenter: parent.verticalCenter
                
                onValueChanged: {
                    percentageChanged(beneficiaryIndex, value)
                }
                
                background: Rectangle {
                    x: percentageSlider.leftPadding
                    y: percentageSlider.topPadding + percentageSlider.availableHeight / 2 - height / 2
                    implicitWidth: 200
                    implicitHeight: 6
                    width: percentageSlider.availableWidth
                    height: implicitHeight
                    radius: 3
                    color: "#EAEAEA"
                    
                    Rectangle {
                        width: percentageSlider.visualPosition * parent.width
                        height: parent.height
                        color: "#031F2B"
                        radius: 3
                    }
                }
                
                handle: Rectangle {
                    x: percentageSlider.leftPadding + percentageSlider.visualPosition * (percentageSlider.availableWidth - width)
                    y: percentageSlider.topPadding + percentageSlider.availableHeight / 2 - height / 2
                    implicitWidth: 18
                    implicitHeight: 18
                    radius: 9
                    color: "#031F2B"
                    border.color: "#FFFFFF"
                    border.width: 2
                }
            }
            Rectangle {
                width: 58
                height: 48
                color: "#FFFFFF"
                border.color: "#DEDEDE"
                border.width: 1
                radius: 8
                
                QLato {
                    anchors.centerIn: parent
                    text: beneficiaryPercentage + "%"
                    font.pixelSize: 16
                    font.weight: Font.Normal
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                }
            }
        }
    }

     QIconButton{
        iconWidth: 24
        iconSize: 24
        anchors {
            right: parent.right
            rightMargin: -6
            top: parent.top
            topMargin: -9
        }
        icon:"qrc:/Images/Images/delete-benefi-dark.svg"
        onClicked: {
            deleteBeneficiaryClicked(beneficiaryIndex)
        }
    }
}

