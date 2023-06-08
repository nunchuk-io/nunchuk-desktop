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
import "../Components/customizes"
import "../Components/customizes/Texts"
import "../Components/origins"

QScreen {
    id: toast
    background.color: "transparent"
    function closeToast() { QMLHandle.sendEvent(EVT.EVT_CLOSE_TOAST_MESSAGE)}
    MouseArea { anchors.fill: parent ; onClicked: { toast.closeToast()} }

    readonly property int    warningType: AppModel.warningMessage.type      // EWARNING.NONE_MSG,
                                                                            // EWARNING.SUCCESS_MSG,
                                                                            // EWARNING.WARNING_MSG,
                                                                            // EWARNING.ERROR_MSG,
                                                                            // EWARNING.EXCEPTION_MSG
    readonly property int    warningCode: AppModel.warningMessage.code
    readonly property string warningWhat: AppModel.warningMessage.what
    readonly property string warningContent: AppModel.warningMessage.contentDisplay
    readonly property string warningExplain: AppModel.warningMessage.explaination
    readonly property int popupType: AppModel.warningMessage.popupType

    readonly property var warningColor: [
        "#EAEAEA",
        "#A7F0BA",
        "#FDEBD2",
        "#CF4018",
        "#CF4018"
    ]

    readonly property var warningTextColor: [
        "#031F2B",
        "#031F2B",
        "#031F2B",
        "#FFFFFF",
        "#FFFFFF"
    ]

    readonly property var warningIcon: [
        "qrc:/Images/Images/error-dark.svg",
        "qrc:/Images/Images/check-outline-dark.svg",
        "qrc:/Images/Images/warning-dark.svg",
        "qrc:/Images/Images/error-light.svg",
        "qrc:/Images/Images/error-light.svg",
    ]

    readonly property var closeIcon: [
        "qrc:/Images/Images/close-dark.svg",
        "qrc:/Images/Images/close-dark.svg",
        "qrc:/Images/Images/close-dark.svg",
        "qrc:/Images/Images/close-light.svg",
        "qrc:/Images/Images/close-light.svg",
    ]

    Loader{
        anchors.fill: parent
        sourceComponent:{
            switch(popupType){
            case Popup_t.PopupTop: return topComponent
            case Popup_t.PopupCenter: return centerComponent
            case Popup_t.PopupBottom: return bottomComponent
            default: return null
            }
        }
    }
    Component{
        id: topComponent
        Item {
            anchors.centerIn: parent
            MouseArea { anchors.fill: parent ; onClicked: { toast.closeToast()} }
            Rectangle {
                id: content
                width: contentRow.width + 24
                height: contentRow.height + 24
                radius: 8
                color: warningColor[warningType]
                anchors.right: parent.right
                anchors.rightMargin: 88
                anchors.top: parent.top
                anchors.topMargin: 104
                MouseArea { anchors.fill: parent ; onClicked: {}}
                Row {
                    id: contentRow
                    anchors.centerIn: parent
                    spacing: 8
                    QImage {
                        width: 36
                        height: 36
                        source: warningIcon[warningType]
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    QText {
                        width: 600
                        anchors.verticalCenter: parent.verticalCenter
                        lineHeightMode: Text.FixedHeight
                        lineHeight: 28
                        wrapMode: Text.WrapAnywhere
                        color: warningTextColor[warningType]
                        text: warningType === EWARNING.SUCCESS_MSG || warningType === EWARNING.WARNING_MSG ? warningExplain : warningContent
                    }
                    Rectangle {
                        width: 48
                        height: 48
                        color: !closemouse.containsMouse ? "transparent" : Qt.rgba(0, 0, 0, 0.1)
                        anchors.verticalCenter: parent.verticalCenter
                        radius: 48
                        QImage {
                            width: 24
                            height: 24
                            source: closeIcon[warningType]
                            anchors.centerIn: parent
                        }
                        MouseArea {
                            id: closemouse
                            anchors.fill: parent
                            hoverEnabled: true
                            cursorShape: Qt.PointingHandCursor
                            onClicked: { QMLHandle.sendEvent(EVT.EVT_CLOSE_TOAST_MESSAGE) }
                        }
                    }
                }
            }
            DropShadow {
                anchors.fill: content
                horizontalOffset: 3
                verticalOffset: 3
                radius: 8.0
                samples: 17
                color: "#80000000"
                source: content
            }
        }
    }

    Component{
        id: centerComponent
        Item {
            anchors.centerIn: parent
            MouseArea { anchors.fill: parent ; onClicked: { toast.closeToast()} }
            Rectangle {
                id: content
                width: contentRow.width + 24
                height: contentRow.height + 24
                radius: 8
                color: warningColor[warningType]
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                anchors.topMargin: 625
                MouseArea { anchors.fill: parent ; onClicked: {}}
                Row {
                    id: contentRow
                    anchors.centerIn: parent
                    spacing: 8
                    QImage {
                        width: 36
                        height: 36
                        source: warningIcon[warningType]
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    QText {
                        width: 600
                        anchors.verticalCenter: parent.verticalCenter
                        lineHeightMode: Text.FixedHeight
                        lineHeight: 28
                        wrapMode: Text.WrapAnywhere
                        color: warningTextColor[warningType]
                        text: warningType === EWARNING.SUCCESS_MSG || warningType === EWARNING.WARNING_MSG ? warningExplain : warningContent
                    }
                    Rectangle {
                        width: 48
                        height: 48
                        color: !closemouse.containsMouse ? "transparent" : Qt.rgba(0, 0, 0, 0.1)
                        anchors.verticalCenter: parent.verticalCenter
                        radius: 48
                        QImage {
                            width: 24
                            height: 24
                            source: closeIcon[warningType]
                            anchors.centerIn: parent
                        }
                        MouseArea {
                            id: closemouse
                            anchors.fill: parent
                            hoverEnabled: true
                            cursorShape: Qt.PointingHandCursor
                            onClicked: { QMLHandle.sendEvent(EVT.EVT_CLOSE_TOAST_MESSAGE) }
                        }
                    }
                }
            }
            DropShadow {
                anchors.fill: content
                horizontalOffset: 3
                verticalOffset: 3
                radius: 8.0
                samples: 17
                color: "#80000000"
                source: content
            }
        }
    }

    Component{
        id: bottomComponent
        Item {
            anchors.centerIn: parent
            MouseArea { anchors.fill: parent ; onClicked: { toast.closeToast()} }
            Rectangle {
                id: content
                width: contentRow.width + 24
                height: contentRow.height + 24
                radius: 8
                color: warningColor[warningType]
                anchors.right: parent.right
                anchors.rightMargin: 88
                anchors.top: parent.top
                anchors.topMargin: 760
                MouseArea { anchors.fill: parent ; onClicked: {}}
                Row {
                    id: contentRow
                    anchors.centerIn: parent
                    spacing: 8
                    QImage {
                        width: 36
                        height: 36
                        source: warningIcon[warningType]
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    QText {
                        width: 600
                        anchors.verticalCenter: parent.verticalCenter
                        lineHeightMode: Text.FixedHeight
                        lineHeight: 28
                        wrapMode: Text.WrapAnywhere
                        color: warningTextColor[warningType]
                        text: warningType === EWARNING.SUCCESS_MSG || warningType === EWARNING.WARNING_MSG ? warningExplain : warningContent
                    }
                    Rectangle {
                        width: 48
                        height: 48
                        color: !closemouse.containsMouse ? "transparent" : Qt.rgba(0, 0, 0, 0.1)
                        anchors.verticalCenter: parent.verticalCenter
                        radius: 48
                        QImage {
                            width: 24
                            height: 24
                            source: closeIcon[warningType]
                            anchors.centerIn: parent
                        }
                        MouseArea {
                            id: closemouse
                            anchors.fill: parent
                            hoverEnabled: true
                            cursorShape: Qt.PointingHandCursor
                            onClicked: { QMLHandle.sendEvent(EVT.EVT_CLOSE_TOAST_MESSAGE) }
                        }
                    }
                }
            }
            DropShadow {
                anchors.fill: content
                horizontalOffset: 3
                verticalOffset: 3
                radius: 8.0
                samples: 17
                color: "#80000000"
                source: content
            }
        }
    }
}
