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
import Features.Inheritance.OnChain.ViewModels 1.0
import "../../../origins"
import "../../../customizes"
import "../../../customizes/Chats"
import "../../../customizes/Texts"
import "../../../customizes/Buttons"
import "../../../customizes/services"
import "../Common"

Item {
    id: viewByzantineinheritancePlanRoot
    property var    inheritancePlanInfo: ServiceSetting.walletInfo.inheritancePlanInfo
    property var    planInfo: inheritancePlanInfo.planInfo
    property string walletName: ServiceSetting.walletInfo.walletName
    property string myRole: ServiceSetting.walletInfo.myRole
    property bool   isEdit: myRole === "MASTER" || myRole === "ADMIN"
    QContextMenu {
        id: optionMenu
        menuWidth: 300
        icons: {
            var ls = []
            if(myRole === "MASTER" || myRole === "ADMIN"){
                ls.push("qrc:/Images/Images/close-24px.svg")
            }
            return ls
        }
        labels: {
            var ls = []
            if(myRole === "MASTER" || myRole === "ADMIN"){
                ls.push(STR.STR_QML_844)
            }
            return ls
        }
        colors: {
            var ls = []
            if(myRole === "MASTER" || myRole === "ADMIN"){
                ls.push("#CF4018")
            }
            return ls
        }
        onItemClicked: {
            switch(index){
            case 0:
                var _input = {
                    option : "cancel-inheritance-plan"
                }
                QMLHandle.sendEvent(EVT.EVT_INHERITANCE_PLAN_FINALIZE_REQUEST, _input)
                break;
            default:
                break;
            }
        }
    }

    Column {
        id: columnHeader
        anchors.fill: parent
        spacing: 0
        Rectangle {
            id: _img_bg
            width: parent.width
            height: 598
            color: "#D0E2FF"
            Column {
                anchors.fill: parent
                anchors.margins: 24
                spacing: 24
                
                Loader {
                    sourceComponent: headerplanOnchain
                    width: parent.width
                    height: 478
                }
                Item {
                    width: parent.width
                    height: 48
                    QText {
                        font.family: "Lato"
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
            }
        }

        Item {
            width: parent.width
            height: parent.height - _img_bg.height - normalRect.height
            Loader {
                sourceComponent: bodyplanOnchain
                anchors.fill: parent
                anchors.margins: 24
            }
        }
    }

    Rectangle {
        id: normalRect
        height: 80
        anchors{
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }
        layer.enabled: true
        layer.effect: DropShadow {
            radius: 4
            samples: radius * 2
            source: normalRect
            color: Qt.rgba(0, 0, 0, 0.5)
        }
        Row {
            anchors{
                right: parent.right
                rightMargin: 24
                bottom: parent.bottom
                bottomMargin: 16
            }
            spacing: 12
            layoutDirection: Qt.RightToLeft
            QTextButton {
                id: _save
                width: 66
                height: 48
                label.text: STR.STR_QML_835
                label.font.pixelSize: 16
                type: eTypeE
                enabled: planInfo.edit_isChanged
                onButtonClicked: {
                    var _input = {
                        option : "finalize-change"
                    }
                    QMLHandle.sendEvent(EVT.EVT_INHERITANCE_PLAN_FINALIZE_REQUEST, _input)
                }
            }
            QTextButton {
                width: 148
                height: 48
                label.text: STR.STR_QML_805
                label.font.pixelSize: 16
                type: eTypeF
                enabled: planInfo.edit_isChanged
                onButtonClicked: {
                    var _input = {
                        option : "discard-change"
                    }
                    QMLHandle.sendEvent(EVT.EVT_INHERITANCE_PLAN_FINALIZE_REQUEST, _input)
                }
            }
        }
    }

    Component {
        id: headerplanOnchain
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
                    Column {
                        width: parent.width
                        spacing: 12
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
                                    source: "qrc:/Images/Images/star-dark.png"
                                }
                                Column {
                                    spacing: 12
                                    QLato {
                                        font.pixelSize: 16
                                        color: "#1C1C1C"
                                        text: STR.STR_QML_749
                                        font.weight: Font.Bold
                                    }
                                    QLato {
                                        font.pixelSize: 16
                                        color: "#1C1C1C"
                                        text: planInfo.magic !== null ? planInfo.magic : "null null null"
                                    }
                                }
                            }
                        }
                        Flickable {
                            width: parent.width
                            height: row_key.height
                            contentWidth: row_key.width
                            clip: true
                            ScrollBar.vertical: ScrollBar { active: true }
                            flickDeceleration : Flickable.HorizontalFlick
                            Row {
                                id: row_key
                                spacing: 12
                                Repeater {
                                    model: planInfo.inheritance_keys.length
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
                                                source: "qrc:/Images/Images/key-dark.svg"
                                            }
                                            Column {
                                                spacing: 12
                                                QLato {
                                                    font.pixelSize: 16
                                                    color: "#1C1C1C"
                                                    text:  planInfo.inheritance_keys.length > 1 ? STR.STR_QML_2038.arg(index+1).arg(planInfo.inheritance_keys[index].xfp.toUpperCase()) : STR.STR_QML_1984.arg(planInfo.inheritance_keys[0].xfp.toUpperCase())
                                                    textFormat: Text.RichText
                                                }
                                                QLato {
                                                    font.pixelSize: 16
                                                    color: "#1C1C1C"
                                                    text: planInfo.inheritance_keys.length > 1 ? STR.STR_QML_2039.arg(index+1) : STR.STR_QML_1986
                                                    width: 290
                                                    wrapMode: Text.WordWrap
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    Column {
                        spacing: 12
                        QLato {
                            font.pixelSize: 16
                            color: "#FFFFFF"
                            text: STR.STR_QML_2101
                        }
                        Rectangle {
                            width: 393
                            height: 64
                            color: "#2A3F61"
                            radius: 12
                            Row {
                                spacing: 24
                                anchors.fill: parent
                                anchors.margins: 12
                                Column {
                                    spacing: 2
                                    visible: vm.blockHeight > 0
                                    QLato {
                                        font.pixelSize: 16
                                        color: "#FFFFFF"
                                        text: qsTr("%1").arg(utils.formatAmount(qsTr("%1").arg(vm.blockHeight)))
                                    }
                                    QLato {
                                        font.pixelSize: 12
                                        color: "#FFFFFF"
                                        text: STR.STR_QML_188
                                    }
                                }
                                Rectangle {
                                    width: 1
                                    height: 40
                                    color: "#DEDEDE"
                                    visible: vm.blockHeight > 0
                                    anchors.verticalCenter: parent.verticalCenter
                                }
                                Column {
                                    spacing: 2
                                    Row {
                                        spacing: 4
                                        height: 20
                                        QLato {
                                            font.pixelSize: 16
                                            color: "#FFFFFF"
                                            text: qsTr("Est. %1 %2").arg(vm.valueDate).arg(vm.valueTime)
                                            anchors.verticalCenter: parent.verticalCenter
                                            verticalAlignment: Text.AlignVCenter
                                        }
                                        QLato {
                                            font.pixelSize: 13
                                            color: "#A6A6A6"
                                            text: "(Estimated)"
                                            anchors.verticalCenter: parent.verticalCenter
                                            verticalAlignment: Text.AlignVCenter
                                        }
                                    }
                                    
                                    QLato {
                                        font.pixelSize: 12
                                        color: "#FFFFFF"
                                        text: vm.valueTimezone
                                    }
                                }
                            }
                        }
                    }
                }
            }
            
        }
    }
    QmlUtils {
        id: utils
    }
    Component {
        id: bodyplanOnchain
        Item {
            clip: true
            Flickable {
                width: 661
                height: parent.height
                contentWidth: width
                contentHeight: _colum.childrenRect.height + 48
                ScrollBar.vertical: ScrollBar { active: true }
                Column {
                    id: _colum
                    width: parent.width
                    spacing: 24
                    QTextAreaBoxTypeB {
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
                            if(isEdit) {
                                QMLHandle.sendEvent(EVT.EVT_EDIT_YOUR_INHERITANCE_PLAN_REQUEST, ServiceType.IE_LEAVE_MESSAGE)
                            }
                        }
                    }
                    Rectangle {
                        height: 1
                        width: 651
                        color: "#EAEAEA"
                    }
                    QServiceInheritanceNotificationPreferencesOnchain {
                        width: 651
                        inheritance: planInfo
                        onTextEditClicked: {
                            if(isEdit) {
                                QMLHandle.sendEvent(EVT.EVT_EDIT_YOUR_INHERITANCE_PLAN_REQUEST, ServiceType.IE_NOTIFICATION)
                            }
                        }
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
        Component.onCompleted: {
            vm.attachContext(vmContext)
        }
    }
}
