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
import QtQuick.Controls 1.4
import QtGraphicalEffects 1.0
import QtQuick.Controls.Styles 1.4
import Qt.labs.platform 1.1
import HMIEVENTS 1.0
import NUNCHUCKTYPE 1.0
import QRCodeItem 1.0
import DataPool 1.0
import DRACO_CODE 1.0
import EWARNING 1.0
import Features.Inheritance.OffChain.ViewModels 1.0
import "../../../origins"
import "../../../customizes"
import "../../../customizes/Chats"
import "../../../customizes/Texts"
import "../../../customizes/Buttons"
import "../../../customizes/services"
import "../../../../Screens/OnlineMode/SetupWallets/TimeLocks"
import "../../../../features/inheritance/components"
import "../Common"

Item {
    id: viewHBinheritancePlanRoot
    property var inheritancePlanInfo: ServiceSetting.walletInfo.inheritancePlanInfo
    property var planInfo: inheritancePlanInfo.planInfo
    property string walletName: ServiceSetting.walletInfo.walletName
    QContextMenu {
        id: optionMenu
        menuWidth: 300
        icons: [
            "qrc:/Images/Images/close-24px.svg"
        ]
        labels: [
            STR.STR_QML_844
        ]
        colors: [
            "#CF4018"
        ]
        onItemClicked: {
            switch(index){
            case 0:
                vm.cancelInheritancePlan()
                break;
            default:
                break;
            }
        }
    }

    Column {
        anchors.fill: parent
        spacing: 0
        Rectangle {
            id: _img_bg
            width: parent.width
            height: vm.distribution_method == "CUSTOMIZE" ? 376 : 486
            color: "#D0E2FF"
            Column {
                anchors.fill: parent
                anchors.margins: 24
                spacing: 24
                Item {
                    width: parent.width
                    height: 48
                    QLato {
                        font.pixelSize: 28
                        color: "#031F2B"
                        font.weight: Font.Bold
                        text: STR.STR_QML_843
                    }
                    QIconButton{
                        width: 48
                        height: 48
                        anchors.right: parent.right
                        visible: inheritancePlanInfo.isActived
                        bgColor: "#D0E2FF"
                        icon: "qrc:/Images/Images/more-horizontal-dark.svg"
                        onClicked: {
                            optionMenu.popup()
                        }
                    }
                }
                Loader {
                    sourceComponent: headerplanOffchain
                    width: parent.width
                    height: _img_bg.height - 24 - 48 - 24*2
                }
            }
        }
        Item {
            width: parent.width
            height: parent.height - _img_bg.height - normalRect.height
            Loader {
                sourceComponent: bodyplanOffchain
                anchors.fill: parent
            }
        }
        Rectangle {
            id: normalRect
            width: parent.width
            height: 80
            color: "#FFFFFF"
            Rectangle {
                color: "#DEDEDE"
                height: 1
                width: parent.width
            }

            Row {
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 24
                spacing: 12
                layoutDirection: Qt.RightToLeft
                QTextButton {
                    id: _save
                    width: 66
                    height: 48
                    label.text: STR.STR_QML_835
                    label.font.pixelSize: 16
                    type: eTypeE
                    enabled: planInfo.edit_isChanged || vm.isDataChanged
                    anchors.verticalCenter: parent.verticalCenter
                    onButtonClicked: {
                        vm.finalizeChanges()
                    }
                }
                QTextButton {
                    width: 148
                    height: 48
                    label.text: STR.STR_QML_805
                    label.font.pixelSize: 16
                    type: eTypeF
                    enabled: planInfo.edit_isChanged || vm.isDataChanged
                    anchors.verticalCenter: parent.verticalCenter
                    onButtonClicked: {
                        vm.discardChanges()
                    }
                }
            }
        }
    }
    Component {
        id: headerplanOffchain
        Rectangle {
            radius: 24
            color: "#2F466C"
            Column {
                anchors.fill: parent
                anchors.margins: 24
                spacing: 24
                Item {
                    width: parent.width
                    height: 60
                    Row {
                        width: parent.width
                        spacing: 12
                        QIcon {
                            iconSize: 60
                            anchors.verticalCenter: parent.verticalCenter
                            source: "qrc:/Images/Images/wallet-brand-icon.svg"
                        }
                        Column {
                            width: 304
                            height: 44
                            anchors.verticalCenter: parent.verticalCenter
                            spacing: 4
                            QLato {
                                font.weight: Font.Bold
                                font.pixelSize: 16
                                color: "#FFFFFF"
                                text: STR.STR_QML_845
                            }
                            QLato {
                                color: "#FFFFFF"
                                text: walletName
                            }
                        }
                    }
                    Row {
                        width: parent.width
                        layoutDirection: Qt.RightToLeft
                        spacing: 12
                        QTextButton {
                            width: 162
                            height: 48
                            label.text: STR.STR_QML_847
                            label.font.pixelSize: 16
                            anchors.verticalCenter: parent.verticalCenter
                            type: eTypeC
                            onButtonClicked: {
                                QMLHandle.sendEvent(EVT.EVT_SHARE_YOUR_SECRET_REQUEST)
                            }
                        }
                        QTextLink {
                            width: 220
                            height: 48
                            font.weight: Font.Bold
                            font.pixelSize: 16
                            color: "#FFFFFF"
                            text: STR.STR_QML_846
                            anchors.verticalCenter: parent.verticalCenter
                            font.underline: false
                            onTextClicked: {
                                Qt.openUrlExternally("https://nunchuk.io/howtoclaim")
                            }
                        }
                    }
                }
                Column {
                    width: parent.width
                    spacing: 12
                    QLato {
                        font.pixelSize: 16
                        color: "#FFFFFF"
                        text: STR.STR_QML_1983
                    }
                    Row {
                        id: row_key
                        spacing: 12
                        Row {
                            spacing: 12
                            QInheritancePlanMagicPhrases {
                                magicPhrases: vm.assetAllocation
                                magic: planInfo.magic
                            }
                            Rectangle {
                                width: 393
                                height: 92
                                color: "#FFFFFF"
                                radius: 12
                                Row {
                                    spacing: 12
                                    anchors.fill: parent
                                    anchors.margins: 12
                                    QIcon {
                                        iconSize: 24
                                        source: "qrc:/Images/Images/change-password-dark.svg"
                                    }
                                    Column {
                                        spacing: 12
                                        QLato {
                                            width: 333
                                            font.pixelSize: 16
                                            color: "#1C1C1C"
                                            text:  STR.STR_QML_727
                                            font.weight: Font.Bold
                                            QLato {
                                                anchors.right: parent.right
                                                font.pixelSize: 16
                                                color: "#1C1C1C"
                                                text:  "Info"
                                                font.underline: true
                                                font.weight: Font.Bold
                                                MouseArea {
                                                    anchors.fill: parent
                                                    hoverEnabled: true
                                                    cursorShape: Qt.PointingHandCursor
                                                    onClicked: {
                                                        _BackupPassword.open()
                                                    }
                                                }
                                            }
                                        }
                                        QLato {
                                            font.pixelSize: 16
                                            color: "#1C1C1C"
                                            text: STR.STR_QML_917
                                            width: 333
                                            wrapMode: Text.WordWrap
                                        }
                                    }
                                }
                            }
                        }
                    }
                    Column {
                        visible: vm.distribution_method === "LUMP_SUM"
                        spacing: 12
                        QLato {
                            font.pixelSize: 16
                            color: "#FFFFFF"
                            text: STR.STR_QML_2101
                        }
                        Rectangle {
                            width: 393
                            height: 62
                            color: "#2A3F61"
                            radius: 12
                            Row {
                                spacing: 12
                                anchors.fill: parent
                                anchors.margins: 12
                                QImage {
                                    width: 24
                                    height: 24
                                    source: "qrc:/Images/Images/calendar-dark.png"
                                }
                                Column {
                                    width: 292
                                    spacing: 2
                                    QLato {
                                        font.pixelSize: 16
                                        color: "#FFFFFF"
                                        text: vm.valueDate
                                    }
                                    QLato {
                                        font.pixelSize: 12
                                        color: "#FFFFFF"
                                        text: vm.valueTimezone
                                    }
                                }
                                QLato {
                                    width:  29
                                    color: "#FFFFFF"
                                    text:  "Edit"
                                    font.underline: true
                                    font.weight: Font.Bold
                                    MouseArea {
                                        anchors.fill: parent
                                        hoverEnabled: true

                                        cursorShape: Qt.PointingHandCursor
                                        onClicked: {
                                            vm.timeLockEditClicked()
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
   
    Component {
        id: bodyplanOffchain
        Item {
            anchors.fill: parent
            Flickable {
                anchors.top: parent.top
                anchors.topMargin: vm.distribution_method == "CUSTOMIZE" ? 0 : 24
                clip: true
                width: parent.width
                height: parent.height
                contentWidth: width
                contentHeight: _colum.childrenRect.height + 100
                ScrollBar.vertical: ScrollBar { active: true }
                Column {
                    id: _colum
                    anchors.top: parent.top
                    width: parent.width
                    spacing: 24
                    QInheritanceOverview {
                        visible: vm.distribution_method == "CUSTOMIZE"
                        onAssetAllocationEditClicked: vm.onAssetAllocationEditClicked()
                        onReleaseMethodEditClicked: vm.onReleaseMethodEditClicked()
                        onReleaseScheduleEditClicked: vm.onReleaseScheduleEditClicked()
                        onTimezoneEditClicked: vm.onTimezoneEditClicked()
                        onFallbackSettingsEditClicked: vm.onFallbackSettingsEditClicked()
                        onBeneficiarySchedulesEditClicked: vm.onBeneficiarySchedulesEditClicked()
                    }
                    QTextAreaBoxTypeB {
                        anchors{
                            left: parent.left
                            leftMargin: 24
                        }
                        width: 651
                        height: 128
                        label.text: STR.STR_QML_850
                        input.text: planInfo.note
                        input.backgroundColor: "#F5F5F5"
                        input.verticalAlignment: Text.AlignTop
                        input.height: 96
                        input.readOnly: true
                        textColor: "#031F2B"
                        onTextEditClicked: {
                            QMLHandle.sendEvent(EVT.EVT_EDIT_YOUR_INHERITANCE_PLAN_REQUEST, ServiceType.IE_LEAVE_MESSAGE)
                        }
                    }
                    Rectangle {
                        anchors{
                            left: parent.left
                            leftMargin: 24
                        }
                        height: 1
                        width: 651
                        color: "#EAEAEA"
                    }
                    QTextInputBoxTypeE {
                        anchors{
                            left: parent.left
                            leftMargin: 24
                        }
                        width: 651
                        height: 84
                        label.text: STR.STR_QML_851
                        input.text: planInfo.buffer_period.id === "" ? STR.STR_QML_921 : planInfo.buffer_period.display_name
                        input.backgroundColor: "#F5F5F5"
                        input.height: 52
                        input.readOnly: true
                        textColor: "#031F2B"
                        onTextEditClicked: {
                            QMLHandle.sendEvent(EVT.EVT_EDIT_YOUR_INHERITANCE_PLAN_REQUEST, ServiceType.IE_BUFFER_PERIOD)
                        }
                    }
                    Rectangle {
                        anchors{
                            left: parent.left
                            leftMargin: 24
                        }
                        height: 1
                        width: 651
                        color: "#EAEAEA"
                    }
                    QServiceInheritanceNotificationPreferencesOffchain {
                        anchors{
                            left: parent.left
                            leftMargin: 24
                        }
                        width: 651
                        inheritance: planInfo
                        onTextEditClicked: {
                            QMLHandle.sendEvent(EVT.EVT_EDIT_YOUR_INHERITANCE_PLAN_REQUEST, ServiceType.IE_NOTIFICATION)
                        }
                    }
                    Item {
                        width: 651
                        height: 80
                    }
                }
            }
        }
    }
    Connections {
        target: inheritancePlanInfo
        onSecurityQuestionClosed: {
            if (ServiceSetting.optionIndex === _VIEW_INHERITANCE_PLANING) {
                _Security.close()
            }
        }
        onInheritanceDummyTransactionAlert: {
            if (ServiceSetting.optionIndex === _VIEW_INHERITANCE_PLANING) {
                QMLHandle.sendEvent(EVT.EVT_HEALTH_CHECK_STARTING_REQUEST)
            }
        }
    }
    ViewInheritancePlanViewModel {
        id: vm
    }
}
