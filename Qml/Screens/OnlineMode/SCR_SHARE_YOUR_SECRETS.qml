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
import QtQuick 2.4
import QtQuick.Controls 2.3
import QtGraphicalEffects 1.12
import HMIEVENTS 1.0
import EWARNING 1.0
import NUNCHUCKTYPE 1.0
import DataPool 1.0
import "../../Components/origins"
import "../../Components/customizes"
import "../../Components/customizes/Chats"
import "../../Components/customizes/Texts"
import "../../Components/customizes/Buttons"
import "../../Components/customizes/services"
import "../../Components/customizes/Popups"
import "../../../localization/STR_QML.js" as STR

QScreen {
    property int flow: AppModel.walletInfo.flow
    property var inheritancePlanInfo: flow === AlertType.CREATE_INHERITANCE_PLAN_SUCCESS ? AppModel.walletInfo.inheritancePlanInfo : ServiceSetting.walletInfo.inheritancePlanInfo
    property var walletType: flow === AlertType.CREATE_INHERITANCE_PLAN_SUCCESS ?  AppModel.walletInfo.walletType : ServiceSetting.walletInfo.walletType
    readonly property int eDIRECT_INHERITANCE: 0
    readonly property int eINDIRECT_INHERITANCE: 1
    readonly property int eJOINT_CONTROL: 2

    property int  selectedOption: eDIRECT_INHERITANCE

    function findObj(obj, _property) {
        if (obj.hasOwnProperty(_property)) return obj
        else return findObj(obj.parent, _property)
    }
    Loader {
        id: _loader
        width: popupWidth
        height: popupHeight
        anchors.centerIn: parent
        sourceComponent: walletType === NUNCHUCKTYPE.MINISCRIPT ? _onChain : _offChain
    }

    Component {
        id: _offChain
        QOnScreenContentTypeB {
            id: _Option
            width: popupWidth
            height: popupHeight
            anchors.centerIn: parent
            label.text: STR.STR_QML_847
            onCloseClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
            content: Item {
                Column {
                    anchors.fill: parent
                    anchors.top: parent.top
                    anchors.topMargin: -16
                    spacing: 24
                    QLato {
                        width: 539
                        height: 56
                        text: STR.STR_QML_880
                        anchors.left: parent.left
                        font.weight: Font.Normal
                        lineHeight: 28
                        lineHeightMode: Text.FixedHeight
                        wrapMode: Text.WrapAnywhere
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                    }
                    Flickable {
                        width: 549
                        height: 440
                        contentWidth: width
                        contentHeight: _colum.childrenRect.height
                        ScrollBar.vertical: ScrollBar { active: true }
                        clip: true
                        Column {
                            id: _colum
                            anchors.fill: parent
                            spacing: 16
                            QRadioButtonTypeF {
                                width: 539
                                height: 116
                                selected: selectedOption === eDIRECT_INHERITANCE
                                labelTop: STR.STR_QML_881
                                labelBottom: STR.STR_QML_882
                                onButtonClicked: { selectedOption = eDIRECT_INHERITANCE; }
                            }
                            QRadioButtonTypeF {
                                width: 539
                                height: 144
                                selected: selectedOption === eINDIRECT_INHERITANCE
                                labelTop: STR.STR_QML_883
                                labelBottom: STR.STR_QML_884
                                onButtonClicked: { selectedOption = eINDIRECT_INHERITANCE }
                            }
                            QRadioButtonTypeF {
                                width: 539
                                height: 172
                                selected: selectedOption === eJOINT_CONTROL
                                labelTop: STR.STR_QML_885
                                labelBottom: STR.STR_QML_886
                                onButtonClicked: { selectedOption = eJOINT_CONTROL; }
                            }
                        }
                    }
                }
            }
            bottomRight: QTextButton {
                width: 98
                height: 48
                label.text: STR.STR_QML_097
                label.font.pixelSize: 16
                type: eTypeE
                onButtonClicked: {
                    _Option.nextClicked()
                }
            }
            onPrevClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
            onNextClicked: {
                shareSecretPopup.open()
            }
        }
    }
    Component {
        id: _onChain
        QOnScreenContentTypeB {
            id: _Option
            width: popupWidth
            height: popupHeight
            anchors.centerIn: parent
            label.text: STR.STR_QML_847
            onCloseClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
            content: Item {
                Column {
                    anchors.fill: parent
                    anchors.top: parent.top
                    anchors.topMargin: -16
                    spacing: 24
                    QLato {
                        width: 539
                        height: 56
                        text: STR.STR_QML_2033
                        anchors.left: parent.left
                        font.weight: Font.Normal
                        lineHeight: 28
                        lineHeightMode: Text.FixedHeight
                        wrapMode: Text.WrapAnywhere
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                    }
                    Flickable {
                        width: 549
                        height: 440
                        contentWidth: width
                        contentHeight: _colum.childrenRect.height
                        ScrollBar.vertical: ScrollBar { active: true }
                        clip: true
                        Column {
                            id: _colum
                            anchors.fill: parent
                            spacing: 16
                            QRadioButtonTypeF {
                                width: 539
                                height: 116
                                selected: selectedOption === eDIRECT_INHERITANCE
                                labelTop: STR.STR_QML_881
                                labelBottom: STR.STR_QML_2034
                                onButtonClicked: { selectedOption = eDIRECT_INHERITANCE; }
                            }
                            QRadioButtonTypeF {
                                width: 539
                                height: 116
                                selected: selectedOption === eINDIRECT_INHERITANCE
                                labelTop: STR.STR_QML_883
                                labelBottom: STR.STR_QML_2035
                                onButtonClicked: { selectedOption = eINDIRECT_INHERITANCE }
                            }
                            QRadioButtonTypeF {
                                width: 539
                                height: 116
                                selected: selectedOption === eJOINT_CONTROL
                                labelTop: STR.STR_QML_885
                                labelBottom: STR.STR_QML_2036
                                onButtonClicked: { selectedOption = eJOINT_CONTROL; }
                            }
                        }
                    }
                }
            }
            bottomRight: QTextButton {
                width: 98
                height: 48
                label.text: STR.STR_QML_097
                label.font.pixelSize: 16
                type: eTypeE
                onButtonClicked: {
                    _Option.nextClicked()
                }
            }
            onPrevClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
            onNextClicked: {
                shareSecretPopup.open()
            }
        }
    }

    QPopupOverlayScreen {
        id: shareSecretPopup
        content: {
            // Build a lookup table to avoid duplicate switch blocks
            var onChainMap = {
                [eDIRECT_INHERITANCE]: _directInheritanceHBOnchain,
                [eINDIRECT_INHERITANCE]: _indirectInheritanceHBOnchain,
                [eJOINT_CONTROL]: _jointControlHBOnchain
            }
            var offChainMap = {
                [eDIRECT_INHERITANCE]: _directInheritance,
                [eINDIRECT_INHERITANCE]: _indirectInheritance,
                [eJOINT_CONTROL]: _jointControl
            }
            var map = walletType === NUNCHUCKTYPE.MINISCRIPT ? onChainMap : offChainMap
            // Fallback to direct inheritance if key missing
            return map[selectedOption] || (walletType === NUNCHUCKTYPE.MINISCRIPT ? _directInheritanceHBOnchain : _directInheritance)
        }
    }

    Component {
        id: _directInheritance
        QOnScreenContentTypeB {
            width: popupWidth
            height: popupHeight
            anchors.centerIn: parent
            label.text: STR.STR_QML_881
            onCloseClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
            content: Item {
                Row {
                    spacing: 36
                    Rectangle {
                        width: 346
                        height: 512
                        radius: 24
                        color: "#D0E2FF"
                        QPicture {
                            width: 153
                            height: 214
                            anchors.centerIn: parent
                            source: "qrc:/Images/Images/inheritance_backup_password.svg"
                        }
                    }
                    QInheritanceDetailsOffChain {
                        width: 346
                        height: 512
                        title: STR.STR_QML_887
                        warning: STR.STR_QML_890
                    }
                }
            }
            bottomRight: QTextButton {
                width: 71
                height: 48
                label.text: STR.STR_QML_777
                label.font.pixelSize: 16
                type: eTypeE
                onButtonClicked: {
                    findObj(parent, "nextClicked").nextClicked()
                    QMLHandle.sendEvent(EVT.EVT_UPDATE_YOUR_SECRET_REQUEST)
                }
            }
            bottomLeft: Item {}
            onPrevClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
            onNextClicked: {
                inheritancePlanInfo.secret = selectedOption
            }
        }
    }
    Component {
        id: _directInheritanceHBOnchain
        Item {
            width: popupWidth
            height: popupHeight
            anchors.centerIn: parent
            QOnScreenContentTypeB {
                anchors.fill: parent
                label.text: STR.STR_QML_881
                onCloseClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
                content: Item {
                    Row {
                        spacing: 36
                        Rectangle {
                            width: 346
                            height: 512
                            radius: 24
                            color: "#D0E2FF"
                            QPicture {
                                width: 228
                                height: 140
                                anchors.centerIn: parent
                                source: "qrc:/Images/Images/shareyoursecrets.svg"
                            }
                        }
                        QInheritanceDetailsOnChain {
                            width: 346
                            height: 512
                            title: STR.STR_QML_1968
                            subtitle1: STR.STR_QML_1971
                            subtitle2: STR.STR_QML_1972
                            warning: STR.STR_QML_1974
                        }
                    }
                }
                bottomRight: QTextButton {
                    width: 98
                    height: 48
                    label.text: STR.STR_QML_1977
                    label.font.pixelSize: 16
                    type: eTypeE
                    onButtonClicked: {
                        findObj(parent, "nextClicked").nextClicked()
                    }
                }
                bottomLeft:  Item {}
                onPrevClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
                onNextClicked: {
                    inheritancePlanInfo.secret = selectedOption
                    howitworksDirectInheritance.visible = true
                }
            }
            QOnScreenContentTypeB {
                id: howitworksDirectInheritance
                anchors.fill: parent
                label.text: STR.STR_QML_1976
                onCloseClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
                visible: false
                content: Item {
                    Row {
                        spacing: 36
                        Rectangle {
                            width: 346
                            height: 512
                            radius: 24
                            color: "#D0E2FF"
                            QPicture {
                                width: 134
                                height: 134
                                anchors.centerIn: parent
                                source: "qrc:/Images/Images/howitworks.svg"
                            }
                        }
                        QInheritanceHowItWorks {
                            width: 346
                            height: 512
                            title: STR.STR_QML_1978
                            subtitle1: STR.STR_QML_1979
                            subtitle2: STR.STR_QML_1980
                        }
                    }
                }
                bottomRight: QTextButton {
                    width: 71
                    height: 48
                    label.text: STR.STR_QML_777
                    label.font.pixelSize: 16
                    type: eTypeE
                    onButtonClicked: {
                        findObj(parent, "nextClicked").nextClicked()
                    }
                }
                bottomLeft:  QButtonTextLink {
                    height: 48
                    label: STR.STR_QML_059
                    onButtonClicked: {
                        howitworksDirectInheritance.visible = false
                    }
                }
                onPrevClicked: {
                    QMLHandle.sendEvent(EVT.EVT_UPDATE_YOUR_SECRET_REQUEST)
                }
                onNextClicked: {
                    QMLHandle.sendEvent(EVT.EVT_UPDATE_YOUR_SECRET_REQUEST)
                }
            }
        }
    }

    Component {
        id: _indirectInheritance
        QOnScreenContentTypeB {
            width: popupWidth
            height: popupHeight
            anchors.centerIn: parent
            label.text: STR.STR_QML_883
            onCloseClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
            content: Item {
                Row {
                    spacing: 36
                    Rectangle {
                        width: 346
                        height: 512
                        radius: 24
                        color: "#D0E2FF"
                        QPicture {
                            width: 153
                            height: 214
                            anchors.centerIn: parent
                            source: "qrc:/Images/Images/inheritance_backup_password.svg"
                        }
                    }
                    QInheritanceDetailsOffChain {
                        width: 346
                        height: 512
                        title: STR.STR_QML_891
                        warning: STR.STR_QML_893
                    }
                }
            }
            bottomRight: QTextButton {
                width: 71
                height: 48
                label.text: STR.STR_QML_777
                label.font.pixelSize: 16
                type: eTypeE
                onButtonClicked: {
                    findObj(parent, "nextClicked").nextClicked()
                    QMLHandle.sendEvent(EVT.EVT_UPDATE_YOUR_SECRET_REQUEST)
                }
            }
            bottomLeft: Item {}
            onPrevClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
            onNextClicked: {
                inheritancePlanInfo.secret = selectedOption
            }
        }
    }
    Component {
        id: _indirectInheritanceHBOnchain
        Item {
            width: popupWidth
            height: popupHeight
            anchors.centerIn: parent
            QOnScreenContentTypeB {
                anchors.fill: parent
                label.text: STR.STR_QML_883
                onCloseClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
                content: Item {
                    Row {
                        spacing: 36
                        Rectangle {
                            width: 346
                            height: 512
                            radius: 24
                            color: "#D0E2FF"
                            QPicture {
                                width: 228
                                height: 140
                                anchors.centerIn: parent
                                source: "qrc:/Images/Images/shareyoursecrets.svg"
                            }
                        }
                        QInheritanceDetailsOnChain {
                            width: 346
                            height: 512
                            title: STR.STR_QML_1969
                            subtitle1: STR.STR_QML_1971
                            subtitle2: STR.STR_QML_1972
                            warning: STR.STR_QML_1974
                        }
                    }
                }
                bottomRight: QTextButton {
                    width: 98
                    height: 48
                    label.text: STR.STR_QML_1977
                    label.font.pixelSize: 16
                    type: eTypeE
                    onButtonClicked: {
                        findObj(parent, "nextClicked").nextClicked()
                    }
                }
                bottomLeft: Item {}
                onPrevClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
                onNextClicked: {
                    inheritancePlanInfo.secret = selectedOption
                    howitworksInDirectInheritance.visible = true
                }
            }

            QOnScreenContentTypeB {
                id: howitworksInDirectInheritance
                anchors.fill: parent
                label.text: STR.STR_QML_1976
                onCloseClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
                visible: false
                content: Item {
                    Row {
                        spacing: 36
                        Rectangle {
                            width: 346
                            height: 512
                            radius: 24
                            color: "#D0E2FF"
                            QPicture {
                                width: 134
                                height: 134
                                anchors.centerIn: parent
                                source: "qrc:/Images/Images/howitworks.svg"
                            }
                        }
                        QInheritanceHowItWorks {
                            width: 346
                            height: 512
                            title: STR.STR_QML_1978
                            subtitle1: STR.STR_QML_1979
                            subtitle2: STR.STR_QML_1981
                        }
                    }
                }
                bottomRight: QTextButton {
                    width: 71
                    height: 48
                    label.text: STR.STR_QML_777
                    label.font.pixelSize: 16
                    type: eTypeE
                    onButtonClicked: {
                        findObj(parent, "nextClicked").nextClicked()
                    }
                }
                bottomLeft:  QButtonTextLink {
                    height: 48
                    label: STR.STR_QML_059
                    onButtonClicked: {
                        howitworksInDirectInheritance.visible = false
                    }
                }
                onPrevClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
                onNextClicked: {
                    QMLHandle.sendEvent(EVT.EVT_UPDATE_YOUR_SECRET_REQUEST)
                }
            }
        }
    }

    Component {
        id: _jointControl
        QOnScreenContentTypeB {
            width: popupWidth
            height: popupHeight
            anchors.centerIn: parent
            label.text: STR.STR_QML_885
            onCloseClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
            content: Item {
                Row {
                    spacing: 36
                    Rectangle {
                        width: 346
                        height: 512
                        radius: 24
                        color: "#D0E2FF"
                        QPicture {
                            width: 153
                            height: 214
                            anchors.centerIn: parent
                            source: "qrc:/Images/Images/inheritance_backup_password.svg"
                        }
                    }
                    QInheritanceDetailsOffChain {
                        width: 346
                        height: 512
                        title: STR.STR_QML_892
                        warning: STR.STR_QML_894
                    }
                }
            }
            bottomRight: QTextButton {
                width: 71
                height: 48
                label.text: STR.STR_QML_777
                label.font.pixelSize: 16
                type: eTypeE
                onButtonClicked: {
                    findObj(parent, "nextClicked").nextClicked()
                    QMLHandle.sendEvent(EVT.EVT_UPDATE_YOUR_SECRET_REQUEST)
                }
            }
            bottomLeft: Item {}
            onPrevClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
            onNextClicked: {
                inheritancePlanInfo.secret = selectedOption
            }
        }
    }
    Component {
        id: _jointControlHBOnchain
        Item {
            width: popupWidth
            height: popupHeight
            anchors.centerIn: parent
            QOnScreenContentTypeB {
                anchors.fill: parent
                label.text: STR.STR_QML_885
                onCloseClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
                content: Item {
                    Row {
                        spacing: 36
                        Rectangle {
                            width: 346
                            height: 512
                            radius: 24
                            color: "#D0E2FF"
                            QPicture {
                                width: 228
                                height: 140
                                anchors.centerIn: parent
                                source: "qrc:/Images/Images/shareyoursecrets.svg"
                            }
                        }
                        QInheritanceDetailsOnChain {
                            width: 346
                            height: 512
                            title: STR.STR_QML_1970
                            subtitle1: STR.STR_QML_1973
                            subtitle2: STR.STR_QML_1975
                            warning: STR.STR_QML_1974
                        }
                    }
                }
                bottomRight: QTextButton {
                    width: 98
                    height: 48
                    label.text: STR.STR_QML_1977
                    label.font.pixelSize: 16
                    type: eTypeE
                    onButtonClicked: {
                        findObj(parent, "nextClicked").nextClicked()
                    }
                }
                bottomLeft: Item {}
                onPrevClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
                onNextClicked: {
                    inheritancePlanInfo.secret = selectedOption
                    howitworksJointControl.visible = true
                }
            }
            QOnScreenContentTypeB {
                id: howitworksJointControl
                anchors.fill: parent
                label.text: STR.STR_QML_1976
                onCloseClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
                visible: false
                content: Item {
                    Row {
                        spacing: 36
                        Rectangle {
                            width: 346
                            height: 512
                            radius: 24
                            color: "#D0E2FF"
                            QPicture {
                                width: 134
                                height: 134
                                anchors.centerIn: parent
                                source: "qrc:/Images/Images/howitworks.svg"
                            }
                        }
                        QInheritanceHowItWorks {
                            width: 346
                            height: 512
                            title: STR.STR_QML_1978
                            subtitle1: STR.STR_QML_1979
                            subtitle2: STR.STR_QML_1982
                        }
                    }
                }
                bottomRight: QTextButton {
                    width: 71
                    height: 48
                    label.text: STR.STR_QML_777
                    label.font.pixelSize: 16
                    type: eTypeE
                    onButtonClicked: {
                        findObj(parent, "nextClicked").nextClicked()
                    }
                }
                bottomLeft:  QButtonTextLink {
                    height: 48
                    label: STR.STR_QML_059
                    onButtonClicked: {
                        howitworksJointControl.visible = false
                    }
                }
                onPrevClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
                onNextClicked: {
                    QMLHandle.sendEvent(EVT.EVT_UPDATE_YOUR_SECRET_REQUEST)
                }
            }
        }
    }
}
