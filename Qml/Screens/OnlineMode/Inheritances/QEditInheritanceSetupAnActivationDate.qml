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
import QtQuick.Controls 1.4
import QtGraphicalEffects 1.12
import QtQuick.Controls.Styles 1.4
import HMIEVENTS 1.0
import EWARNING 1.0
import NUNCHUCKTYPE 1.0
import DataPool 1.0
import "../../../Components/origins"
import "../../../Components/customizes"
import "../../../Components/customizes/Chats"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../Components/customizes/Popups"
import "../../../../localization/STR_QML.js" as STR

QOnScreenContentTypeB {
    id: _Activation
    property var planInfo
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_854
    onCloseClicked: closeTo(NUNCHUCKTYPE.SERVICE_TAB)
    content: Item {
        Column {
            anchors.fill: parent
            anchors.top: parent.top
            anchors.topMargin: -16
            spacing: 24
            QLato {
                width: 539
                height: 56
                text: STR.STR_QML_855
                anchors.left: parent.left
                font.weight: Font.Normal
                wrapMode: Text.WrapAnywhere
                lineHeight: 28
                lineHeightMode: Text.FixedHeight
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
            }
            Item {
                width: 539
                height: 72
                QTextInputBoxTypeB {
                    label: STR.STR_QML_848
                    width: 539
                    height: 72
                    boxWidth: 539
                    boxHeight: 48
                    isValid: true
                    textInputted: _calendar.dateString
                }
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor
                    propagateComposedEvents: true
                    onClicked: {
                        _calendar.open()
                    }
                }
            }
        }
        Rectangle {
            width: parent.width
            height: 80
            anchors {
                bottom: parent.bottom
                bottomMargin: 36
            }
            color:"#EAEAEA"
            radius: 8
            Row {
                anchors.fill: parent
                anchors.margins: 12
                spacing: 8
                QIcon {
                    iconSize: 36
                    anchors.verticalCenter: parent.verticalCenter
                    source: "qrc:/Images/Images/info-60px.svg"
                }
                QLato {
                    anchors.verticalCenter: parent.verticalCenter
                    text: STR.STR_QML_856
                    width: 660
                    height: 56
                    wrapMode: Text.WordWrap
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }
    }
    bottomRight: QTextButton {
        width: 195
        height: 48
        label.text: STR.STR_QML_857
        label.font.pixelSize: 16
        type: eTypeE
        onButtonClicked: {
            _Activation.nextClicked()
        }
    }
    onPrevClicked: closeTo(NUNCHUCKTYPE.SERVICE_TAB)
    onNextClicked: {
        var _edit = {
            "activation_date": _calendar.dateString
        }
        inheritancePlanInfo.editPlanInfo(_edit)
        closeTo(NUNCHUCKTYPE.SERVICE_TAB)
    }
    QPopupCalendar {
        id: _calendar
        dateString: planInfo.activation_date
    }
}

