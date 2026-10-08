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
    // Height grows to fit content (including error row when visible)
    height: contentColumn.implicitHeight + 24

    // Properties
    property int beneficiaryIndex: 0
    property string beneficiaryEmail: ""
    property int beneficiaryPercentage: 0
    property string emailError: ""  // set by VM on Save failure; cleared by VM when user edits

    // When VM clamps and pushes back a new percentage (e.g. after drag exceeds capacity),
    // force-reset the slider handle. The _updating guard prevents the reset from
    // triggering onValueChanged → percentageChanged → VM → infinite loop.
    onBeneficiaryPercentageChanged: {
        if (percentageSlider.value !== beneficiaryPercentage) {
            percentageSlider._updating = true
            percentageSlider.value = beneficiaryPercentage
            percentageSlider._updating = false
        }
    }

    // Signals
    signal deleteBeneficiaryClicked(int index)
    signal emailChanged(int index, string email)
    signal percentageChanged(int index, int percent)

    // Drive input error state whenever emailError changes from VM.
    // Only the red border is shown (isValid = false); error text is suppressed
    // because the toast from markEmailErrors() is sufficient feedback.
    onEmailErrorChanged: {
        _input_email.isValid   = (emailError === "")
        _input_email.showError = false
        _input_email.errorText = ""
    }

    Column {
        id: contentColumn
        anchors {
            left:  parent.left
            right: parent.right
            top:   parent.top
            margins: 12
        }
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
                // Immediately clear any error state when the user edits the field.
                // VM will also clear emailError via updateBeneficiaryEmail → setassetAllocation,
                // which triggers onEmailErrorChanged for a clean final reset.
                _input_email.isValid   = true
                _input_email.showError = false
                _input_email.errorText = ""
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
                stepSize: 1
                anchors.verticalCenter: parent.verticalCenter

                // No declarative binding on `value` — binding is broken by drag gestures.
                // Initial value set via Component.onCompleted; subsequent updates via
                // onPressedChanged (snap after drag ends) and onBeneficiaryPercentageChanged
                // (snap when VM pushes a new value outside of a drag).
                property bool _updating: false
                Component.onCompleted: value = beneficiaryPercentage

                onValueChanged: {
                    if (!_updating) {
                        percentageChanged(beneficiaryIndex, value)
                    }
                }

                // Qt recalculates slider value from mouse position every frame during drag,
                // and may fire one extra onValueChanged after mouse release.
                // Qt.callLater defers the snap to after all pending slider events are processed,
                // ensuring the clamped value wins.
                onPressedChanged: {
                    if (!pressed) {
                        Qt.callLater(function() {
                            percentageSlider._updating = true
                            percentageSlider.value = beneficiaryPercentage
                            percentageSlider._updating = false
                        })
                    }
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
                border.color: beneficiaryPercentage === 0 ? "#CF4018" : "#DEDEDE"
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

