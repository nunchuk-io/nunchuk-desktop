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
import QtGraphicalEffects 1.0
import Qt.labs.platform 1.1
import HMIEVENTS 1.0
import NUNCHUCKTYPE 1.0
import QRCodeItem 1.0
import DataPool 1.0
import DRACO_CODE 1.0
import EWARNING 1.0
import "./../../origins"
import "./../../customizes"
import "./../../customizes/Chats"
import "./../../customizes/Texts"
import "./../../customizes/Buttons"
import "../../../../localization/STR_QML.js" as STR

Item {
    id: feeSettingItem
    anchors.fill: parent

    property bool anyChanged: defaultFeeRate.anyChanged || automaticFeeSelection.anyChanged || antiFeeSniping.anyChanged

    Flickable {
        anchors.fill: parent
        contentWidth: parent.width
        contentHeight: _feesetting.childrenRect.height
        interactive: contentHeight > height
        clip: true
        flickableDirection: Flickable.VerticalFlick
        ScrollBar.vertical: ScrollBar { active: true }

        Column{
            id: _feesetting
            anchors{
                top:parent.top
                topMargin: 24
                left: parent.left
                leftMargin: 24
            }
            spacing: 24
            QText {
                font.family: "Lato"
                font.pixelSize: 28
                font.weight: Font.Bold
                text: STR.STR_QML_1715
            }

            Column {
                spacing: 16
                Column {
                    id: defaultFeeRate
                    spacing: 0
                    Column {
                        QLato {
                            font.pixelSize: 16
                            text: STR.STR_QML_1752
                            color: "#000000"
                        }
                        QLato {
                            font.pixelSize: 12
                            text: STR.STR_QML_1753
                            color: "#757575"
                            lineHeight: 16
                            lineHeightMode: Text.FixedHeight
                            verticalAlignment: Text.AlignVCenter
                        }
                    }
                    Item {
                        width: 2
                        height: 8
                    }
                    Item {
                        id: economyitem
                        property bool selected: defaultFeeRate.feeValue === NUNCHUCKTYPE.ECONOMY
                        width: 450
                        height: 62
                        Column {
                            width: parent.width
                            anchors.verticalCenter: parent.verticalCenter
                            Row {
                                spacing: 8
                                height: 24
                                QLato {
                                    text: "Economy"
                                    font.pixelSize: 16
                                    font.weight: Font.Normal
                                    anchors.verticalCenter: parent.verticalCenter
                                }
                                Rectangle {
                                    width: 91
                                    height: 16
                                    radius: 20
                                    color: "#EAEAEA"
                                    anchors.verticalCenter: parent.verticalCenter
                                    QText {
                                        text: "Recommended"
                                        font.pixelSize: 10
                                        font.weight: Font.Bold
                                        anchors.centerIn: parent
                                    }
                                }
                            }
                            QLato {
                                text: STR.STR_QML_1717
                                font.pixelSize: 12
                                font.weight: Font.Normal
                                color: "#757575"
                                height: 16
                                verticalAlignment: Text.AlignVCenter
                            }
                        }
                        ColorOverlay {
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.right: parent.right
                            width: 24
                            height: 24
                            source: QIcon {
                                iconSize: 24
                                source: economyitem.selected ? "qrc:/Images/Images/radio-selected-dark.svg" : "qrc:/Images/Images/radio-dark.svg"
                            }
                            color: economyitem.enabled ? "#031F2B" : "#666666"
                        }
                        MouseArea {
                            anchors.fill: parent
                            hoverEnabled: true
                            cursorShape: Qt.PointingHandCursor
                            onClicked: {
                                defaultFeeRate.feeValue = NUNCHUCKTYPE.ECONOMY
                            }
                        }
                    }
                    Item {
                        id: standarditem
                        property bool selected: defaultFeeRate.feeValue === NUNCHUCKTYPE.STANDARD
                        width: 450
                        height: 62
                        Column {
                            width: parent.width
                            anchors.verticalCenter: parent.verticalCenter
                            QLato {
                                height: 24
                                verticalAlignment: Text.AlignVCenter
                                text: "Standard"
                                font.pixelSize: 16
                                font.weight: Font.Normal
                            }
                            QLato {
                                text: STR.STR_QML_1718
                                font.pixelSize: 12
                                font.weight: Font.Normal
                                color: "#757575"
                                height: 16
                                verticalAlignment: Text.AlignVCenter
                            }
                        }
                        ColorOverlay {
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.right: parent.right
                            width: 24
                            height: 24
                            source: QIcon {
                                iconSize: 24
                                source: standarditem.selected ? "qrc:/Images/Images/radio-selected-dark.svg" : "qrc:/Images/Images/radio-dark.svg"
                            }
                            color: standarditem.enabled ? "#031F2B" : "#666666"
                        }
                        MouseArea {
                            anchors.fill: parent
                            hoverEnabled: true
                            cursorShape: Qt.PointingHandCursor
                            onClicked: {
                                defaultFeeRate.feeValue = NUNCHUCKTYPE.STANDARD
                            }
                        }
                    }
                    Item {
                        id: priorityitem
                        property bool selected: defaultFeeRate.feeValue === NUNCHUCKTYPE.PRIORITY
                        width: 450
                        height: 62
                        Column {
                            width: parent.width
                            anchors.verticalCenter: parent.verticalCenter
                            QLato {
                                height: 24
                                verticalAlignment: Text.AlignVCenter
                                text: "Priority"
                                font.pixelSize: 16
                                font.weight: Font.Normal
                            }
                            QLato {
                                text: STR.STR_QML_1719
                                font.pixelSize: 12
                                font.weight: Font.Normal
                                color: "#757575"
                                height: 16
                                verticalAlignment: Text.AlignVCenter
                            }
                        }
                        ColorOverlay {
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.right: parent.right
                            width: 24
                            height: 24
                            source: QIcon {
                                iconSize: 24
                                source: priorityitem.selected ? "qrc:/Images/Images/radio-selected-dark.svg" : "qrc:/Images/Images/radio-dark.svg"
                            }
                            color: priorityitem.enabled ? "#031F2B" : "#666666"
                        }
                        MouseArea {
                            anchors.fill: parent
                            hoverEnabled: true
                            cursorShape: Qt.PointingHandCursor
                            onClicked: {
                                defaultFeeRate.feeValue = NUNCHUCKTYPE.PRIORITY
                            }
                        }
                    }

                    property int  feeValue: AppSetting.feeSetting
                    property bool anyChanged: AppSetting.feeSetting !== feeValue
                    function applySettings(){
                        AppSetting.feeSetting = defaultFeeRate.feeValue
                    }
                }
                Column {
                    id: automaticFeeSelection
                    visible: AppSetting.enableAutoFeeFunction
                    spacing: 16
                    Row {
                        spacing: 0
                        Column {
                            spacing: 4
                            QLato {
                                font.pixelSize: 16
                                text: STR.STR_QML_1754
                                color: "#000000"
                            }
                            QLato {
                                width: 350
                                font.pixelSize: 12
                                text: STR.STR_QML_1755
                                color: "#757575"
                                wrapMode: Text.WordWrap
                                lineHeight: 16
                                lineHeightMode: Text.FixedHeight
                                verticalAlignment: Text.AlignVCenter
                            }
                        }
                        QSwitchTypeB {
                            id: autoFeeSwitcher
                            width: 84
                            height: 48
                            switchOn: AppSetting.enableAutoFeeSelection
                            anchors.verticalCenter: parent.verticalCenter
                        }
                    }
                    Column {
                        visible: autoFeeSwitcher.switchOn
                        spacing: 16
                        QTextInputBoxTypeB {
                            id: thresholdPercent
                            label: STR.STR_QML_1758
                            boxWidth: 450
                            boxHeight: 48
                            titleFontSize: 12
                            textweight: Font.Bold
                            isValid: true
                            validator: RegExpValidator { regExp: /[0-9]*/ }
                            textInputted: AppSetting.thresholdPercentDisplay
                        }
                        QTextInputBoxTypeB {
                            id: thresholdAmount
                            label: STR.STR_QML_1759
                            boxWidth: 450
                            boxHeight: 48
                            titleFontSize: 12
                            textweight: Font.Bold
                            isValid: true
                            validator: DoubleValidator {
                                bottom: 0.00
                                top: 1.00
                                decimals: 2
                            }
                            textInputted: AppSetting.thresholdAmountDisplay
                        }
                    }
                    property bool anyChanged: !autoFeeSwitcher.switchOn ? (autoFeeSwitcher.switchOn !== AppSetting.enableAutoFeeSelection) : (autoFeeSwitcher.switchOn !== AppSetting.enableAutoFeeSelection)
                                                                                                                                              || (thresholdPercent.textInputted !== AppSetting.thresholdPercentDisplay)
                                                                                                                                              || (thresholdAmount.textInputted !== AppSetting.thresholdAmountDisplay)
                    function applySettings(){
                        if(autoFeeSwitcher.switchOn !== AppSetting.enableAutoFeeSelection) {
                            AppSetting.enableAutoFeeSelection = autoFeeSwitcher.switchOn
                        }
                        if(thresholdPercent.textInputted !== AppSetting.thresholdPercentDisplay) {
                            AppSetting.thresholdPercentDisplay = thresholdPercent.textInputted
                        }
                        if(thresholdAmount.textInputted !== AppSetting.thresholdAmountDisplay) {
                            AppSetting.thresholdAmountDisplay = thresholdAmount.textInputted
                        }
                        var valid_percent = isGreaterThanZero(thresholdPercent.textInputted);
                        var valid_amount = isGreaterThanZero(thresholdAmount.textInputted);
                        if(!valid_percent || !valid_amount) {
                            AppModel.showToast(0, STR.STR_QML_1763, EWARNING.ERROR_MSG)
                        }
                    }
                    function isGreaterThanZero(input) {
                        var cleaned = input.toString().replace(/["%\s]/g, "");
                        var number = Number(cleaned);
                        return (number > 0);
                    }
                }
                Rectangle {
                    width: 450
                    height: 1
                    color: "#DEDEDE"
                }
                Column {
                    id: antiFeeSniping
                    spacing: 16
                    Row {
                        spacing: 0
                        Column {
                            spacing: 4
                            QLato {
                                font.pixelSize: 16
                                text: STR.STR_QML_1756
                                color: "#000000"
                            }
                            QLato {
                                width: 350
                                font.pixelSize: 12
                                text: STR.STR_QML_1757
                                color: "#757575"
                                wrapMode: Text.WordWrap
                                lineHeight: 16
                                lineHeightMode: Text.FixedHeight
                                verticalAlignment: Text.AlignVCenter
                            }
                        }
                        QSwitchTypeB {
                            id: antiFeeSwitcher
                            width: 84
                            height: 48
                            switchOn: AppSetting.enableAntiFeeSniping
                            anchors.verticalCenter: parent.verticalCenter
                        }
                    }
                    Column {
                        visible: false
                        spacing: 16
                    }

                    property bool anyChanged: (antiFeeSwitcher.switchOn !== AppSetting.enableAntiFeeSniping)
                    function applySettings(){
                        AppSetting.enableAntiFeeSniping = antiFeeSwitcher.switchOn
                    }
                }
            }
        }
    }

    QTextButton {
        width: 189
        height: 48
        label.text: STR.STR_QML_1726
        label.font.pixelSize: 16
        label.font.family: "Lato"
        type: eTypeE
        anchors{
            right: parent.right
            rightMargin: 24
            bottom: parent.bottom
            bottomMargin: 16
        }
        enabled: feeSettingItem.anyChanged
        onButtonClicked: {
            feeSettingItem.saveFeeSettings()
        }
    }
    function saveFeeSettings() {
        if(feeSettingItem.anyChanged) {
            if(defaultFeeRate.anyChanged) {
                defaultFeeRate.applySettings();
            }
            if(automaticFeeSelection.anyChanged) {
                automaticFeeSelection.applySettings();
            }
            if(antiFeeSniping.anyChanged) {
                antiFeeSniping.applySettings();
            }

            var threadHold = true;
            if(thresholdPercent.textInputted !== AppSetting.thresholdPercentDisplay) {
                threadHold = false;
            }
            if(thresholdAmount.textInputted !== AppSetting.thresholdAmountDisplay) {
                threadHold = false;
            }

            if(threadHold){
                AppModel.showToast(0, "Fee settings updated", EWARNING.SUCCESS_MSG);
            }
        }
    }
}
