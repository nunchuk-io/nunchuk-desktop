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
import "../../Components/origins"
import "../../Components/customizes"
import "../../Components/customizes/Chats"
import "../../Components/customizes/Texts"
import "../../Components/customizes/Buttons"
import "../../../localization/STR_QML.js" as STR

QScreen {
    function fclose() {
        QMLHandle.sendEvent(EVT.EVT_CLOSE_TO_SERVICE_SETTINGS_REQUEST, EVT.STATE_ID_SCR_EDIT_YOUR_INHERITANCE_PLAN)
    }
    property string dateString: ServiceSetting.qInheritanceActivationDate !== "" ? ServiceSetting.qInheritanceActivationDate : Qt.formatDateTime(new Date(), "MM/dd/yyyy")
    property string message: ServiceSetting.qInheritanceNote
    property string period: ""
    property string period_id: ServiceSetting.qInheritancePeriodId
    property string email: ServiceSetting.qInheritanceEmail
    property bool isNotify: ServiceSetting.qInheritanceIsNotify

    QOnScreenContentTypeB {
        id: _Activation
        visible: ServiceSetting.qInheritancePlan === ServiceType.IE_ACTIVATION_DATE
        width: popupWidth
        height: popupHeight
        anchors.centerIn: parent
        label.text: STR.STR_QML_854
        onCloseClicked: fclose()
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
                        textInputted: dateString
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
            Popup {
                id: _calendar
                width: cal.width
                height: cal.height
                modal: true
                focus: true
                closePolicy: Popup.CloseOnReleaseOutside | Popup.CloseOnEscape
                background: Item{}
                anchors.centerIn: parent
                Calendar {
                    id: cal
                    anchors.centerIn: parent
                    onClicked: {
                        dateString = Qt.formatDateTime(date, "MM/dd/yyyy")
                        _calendar.close()
                    }
                    style: CalendarStyle {
                        gridVisible: false
                        dayDelegate: Rectangle {
                            gradient: Gradient {
                                GradientStop {
                                    position: 0.00
                                    color: styleData.selected ? "#111" : (styleData.visibleMonth && styleData.valid ? "#444" : "#666");
                                }
                                GradientStop {
                                    position: 1.00
                                    color: styleData.selected ? "#444" : (styleData.visibleMonth && styleData.valid ? "#111" : "#666");
                                }
                                GradientStop {
                                    position: 1.00
                                    color: styleData.selected ? "#777" : (styleData.visibleMonth && styleData.valid ? "#111" : "#666");
                                }
                            }

                            Label {
                                text: styleData.date.getDate()
                                anchors.centerIn: parent
                                color: styleData.valid ? "white" : "grey"
                            }

                            Rectangle {
                                width: parent.width
                                height: 1
                                color: "#555"
                                anchors.bottom: parent.bottom
                            }

                            Rectangle {
                                width: 1
                                height: parent.height
                                color: "#555"
                                anchors.right: parent.right
                            }
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
                    QImage {
                        anchors.verticalCenter: parent.verticalCenter
                        width: 36; height: 36;
                        source: "qrc:/Images/Images/info-60px.png"
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
        onPrevClicked: fclose()
        onNextClicked: {
            ServiceSetting.qInheritanceActivationDate = dateString
            fclose()
        }
    }

    QOnScreenContentTypeB {
        id: _Message
        visible: ServiceSetting.qInheritancePlan === ServiceType.IE_LEAVE_MESSAGE
        width: popupWidth
        height: popupHeight
        anchors.centerIn: parent
        label.text: STR.STR_QML_920
        onCloseClicked: fclose()
        content: Item {
            Column {
                anchors.fill: parent
                anchors.top: parent.top
                anchors.topMargin: -16
                spacing: 24
                QLato {
                    width: 539
                    height: 56
                    text: STR.STR_QML_859
                    anchors.left: parent.left
                    font.weight: Font.Normal
                    wrapMode: Text.WrapAnywhere
                    lineHeight: 28
                    lineHeightMode: Text.FixedHeight
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }
                QTextAreaBoxTypeA {
                    label: STR.STR_QML_860
                    optional: " (optional)"
                    boxWidth: 539
                    boxHeight: 276
                    isValid: true
                    textInputted: message
                    onTextInputtedChanged: {
                        message = textInputted
                    }

                    input.verticalAlignment: Text.AlignTop
                    input.wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                }
            }
        }
        bottomRight: QTextButton {
            width: 195
            height: 48
            label.text: STR.STR_QML_861
            label.font.pixelSize: 16
            type: eTypeE
            onButtonClicked: {
                _Message.nextClicked()
            }
        }
        onPrevClicked: fclose()
        onNextClicked: {
            ServiceSetting.qInheritanceNote = message
            fclose()
        }
    }

    QOnScreenContentTypeB {
        id: _Period
        visible: ServiceSetting.qInheritancePlan === ServiceType.IE_BUFFER_PERIOD
        width: popupWidth
        height: popupHeight
        anchors.centerIn: parent
        label.text: STR.STR_QML_862
        onCloseClicked: fclose()
        content: Item {
            Column {
                anchors.fill: parent
                anchors.top: parent.top
                anchors.topMargin: -16
                spacing: 24
                QLato {
                    width: 539
                    height: 140
                    text: STR.STR_QML_863
                    anchors.left: parent.left
                    font.weight: Font.Normal
                    lineHeight: 28
                    lineHeightMode: Text.FixedHeight
                    wrapMode: Text.WrapAnywhere
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }
                Item {
                    width: 539
                    height: 212
                    Column {
                        anchors.fill: parent
                        spacing: 16
                        QListView {
                            id: questions
                            width: 539
                            height: childrenRect.height
                            spacing: 8
                            clip: true
                            model: UserWallet.periods
                            interactive: false
                            delegate: QRadioButtonTypeE {
                                id: btn
                                width: 539
                                height: 60
                                label: modelData.display_name
                                fontFamily: "Lato"
                                fontPixelSize: 16
                                fontWeight: btn.selected ? Font.ExtraBold : Font.DemiBold
                                selected: period_id === modelData.id
                                textBadge: modelData.is_recommended ? STR.STR_QML_879 : ""
                                onButtonClicked: {
                                    period = modelData.display_name
                                    period_id = modelData.id
                                }
                            }
                        }
                    }
                }
            }
        }
        bottomRight: QTextButton {
            width: 195
            height: 48
            label.text: STR.STR_QML_864
            label.font.pixelSize: 16
            type: eTypeE
            onButtonClicked: {
                _Period.nextClicked()
            }
        }
        onPrevClicked: fclose()
        onNextClicked: {
            ServiceSetting.qInheritancePeriod = period
            ServiceSetting.qInheritancePeriodId = period_id
            fclose()
        }
    }

    QOnScreenContentTypeB {
        id: _Notification
        visible: ServiceSetting.qInheritancePlan === ServiceType.IE_NOTIFICATION
        width: popupWidth
        height: popupHeight
        anchors.centerIn: parent
        label.text: STR.STR_QML_865
        onCloseClicked: fclose()
        content: Item {
            Column {
                anchors.fill: parent
                anchors.top: parent.top
                anchors.topMargin: -16
                spacing: 24
                QLato {
                    width: 539
                    height: 56
                    text: STR.STR_QML_866
                    anchors.left: parent.left
                    font.weight: Font.Normal
                    wrapMode: Text.WrapAnywhere
                    lineHeight: 28
                    lineHeightMode: Text.FixedHeight
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }
                QTextInputBoxTypeB {
                    label: STR.STR_QML_867
                    boxWidth: 539
                    boxHeight: 116
                    isValid: true
                    textInputted: email
                    onTextInputtedChanged: {
                        email = textInputted
                    }
                    input.verticalAlignment: Text.AlignTop
                    input.wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                }
                QCheckBoxButton{
                    id: notify
                    width: 539
                    height: 24
                    label: STR.STR_QML_868
                    checked: ServiceSetting.qInheritanceIsNotify
                    onButtonClicked: {
                        isNotify = checked
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
                color:"#FDEBD2"
                radius: 8
                Row {
                    anchors.fill: parent
                    anchors.margins: 12
                    spacing: 8
                    QImage {
                        anchors.verticalCenter: parent.verticalCenter
                        width: 36; height: 36;
                        source: "qrc:/Images/Images/warning-dark.svg"
                    }
                    QLato {
                        anchors.verticalCenter: parent.verticalCenter
                        text: STR.STR_QML_869
                        width: 660
                        height: 56
                        wrapMode: Text.WordWrap
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                    }
                }
            }
        }

        bottomRight: Row {
            spacing: 12
            QTextButton {
                width: 239
                height: 48
                label.text: STR.STR_QML_871
                label.font.pixelSize: 16
                type: eTypeB
                onButtonClicked: {
                    ServiceSetting.qInheritanceIsNotify = false
                    ServiceSetting.qInheritanceEmail = ""
                    fclose()
                }
            }
            QTextButton {
                width: 259
                height: 48
                label.text: STR.STR_QML_870
                label.font.pixelSize: 16
                type: eTypeE
                onButtonClicked: {
                    _Notification.nextClicked()
                }
            }
        }
        onPrevClicked: fclose()
        onNextClicked: {
            ServiceSetting.qInheritanceIsNotify = isNotify
            ServiceSetting.qInheritanceEmail = email
            fclose()
        }
    }
}
