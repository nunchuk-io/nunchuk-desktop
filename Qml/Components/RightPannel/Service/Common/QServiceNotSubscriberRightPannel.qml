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
import DataPool 1.0
import DRACO_CODE 1.0
import EWARNING 1.0
import "../../../origins"
import "../../../customizes"
import "../../../customizes/Chats"
import "../../../customizes/Texts"
import "../../../customizes/Buttons"
import "../../../../../localization/STR_QML.js" as STR

Item {
    id:_clainNot
    readonly property var option_map: [
        {visible: true,        height: 136, headline:STR.STR_QML_756, content: STR.STR_QML_757 , icon: "qrc:/Images/Images/mulitsig-dark.svg"             },
        {visible: true,        height: 136, headline:STR.STR_QML_736, content: STR.STR_QML_759 , icon: "qrc:/Images/Images/inheritance-dark.svg"          },
        {visible: true,        height: 108, headline:STR.STR_QML_1393,content: STR.STR_QML_1394, icon: "qrc:/Images/Images/groups-24px.svg"               },
        {visible: true,        height: 108, headline:STR.STR_QML_697, content: STR.STR_QML_762 , icon: "qrc:/Images/Images/emergency-lockdown-dark.svg"   },
        {visible: true,        height: 108, headline:STR.STR_QML_763, content: STR.STR_QML_764 , icon: "qrc:/Images/Images/signing-policy-dark.svg"       },
        {visible: true,        height: 108, headline:STR.STR_QML_765, content: STR.STR_QML_766 , icon: "qrc:/Images/Images/key-recovery-dark.svg"         },
        {visible: true,        height: 108, headline:STR.STR_QML_767, content: STR.STR_QML_768 , icon: "qrc:/Images/Images/contact-support-dark.svg"      },
        {visible: isLoggedIn,  height: 108, headline:STR.STR_QML_790, content: STR.STR_QML_791 , icon: "qrc:/Images/Images/member-discount-dark.svg"      },
    ]
    property bool isByzantine: ClientController.user.isByzantineUser || ClientController.user.isFinneyUser
    property bool isLoggedIn: ClientController.isNunchukLoggedIn
    Rectangle {
        width: parent.width
        height: 60
        color: "#D0E2FF"
        visible: ClientController.isNunchukLoggedIn
        QLato {
            text: STR.STR_QML_737
            font.weight: Font.Bold
            anchors{
                left: parent.left
                leftMargin: 24
                verticalCenter: parent.verticalCenter
            }
        }
        QIconButton {
            iconSize: 24
            anchors {
                right: parent.right
                rightMargin: 24
                verticalCenter: parent.verticalCenter
            }
            icon: "qrc:/Images/Images/right-arrow-dark.svg"
            onButtonClicked: {
                if (ServiceSetting.servicesTag.inheritanceCheck()) {
                    ServiceSetting.optionIndex = _CLAIM_AN_INHERITANCE
                } else {
                    _clainNot.showPopup()
                }
            }
            bgColor: "transparent"
        }
    }
    Column {
        anchors.fill: parent
        spacing: 24
        anchors{
            left: parent.left
            leftMargin: 24
            top: parent.top
            topMargin: 24 + (ClientController.isNunchukLoggedIn ? 60 : 0)
        }
        Rectangle {
            width: parent.width - 24
            height: 300
            radius: 24
            color: "#D0E2FF"
            QImage {
                anchors.centerIn: parent
                width: sourceSize.width
                height: sourceSize.height
                source: "qrc:/Images/Images/assisted-wallet.svg"
            }
        }
        Item {
            width: parent.width - 24
            height: parent.height * 0.45
            Column {
                width: parent.width
                spacing: 16
                QText {
                    text: STR.STR_QML_754
                    color: "#031F2B"
                    font.family: "Montserrat"
                    font.pixelSize: 32
                    font.weight: Font.DemiBold
                }
                QText {
                    text: STR.STR_QML_755
                    color: "#031F2B"
                    font.family: "Montserrat"
                    font.pixelSize: 16
                    font.weight: Font.Normal
                }
                Item {
                    width: parent.width
                    height: 322
                    Flickable {
                        id: _flick
                        anchors.fill: parent
                        contentWidth: parent.width; contentHeight: 480
                        clip: true
                        ScrollBar.vertical: ScrollBar { active: true }
                        Flow {
                            width: parent.width
                            spacing: 16
                            Repeater {
                                width: parent.width
                                model: option_map.length
                                Rectangle {
                                    property var _item: option_map[index]
                                    width: 414
                                    height: _item.height
                                    visible: _item.visible
                                    Row {
                                        spacing: 12
                                        Rectangle {
                                            width: 48
                                            height: 48
                                            radius: 48
                                            color: "#F5F5F5"
                                            visible: _ico.source != ""
                                            QIcon {
                                                iconSize: 24
                                                id: _ico
                                                anchors.centerIn: parent
                                                source: _item.icon
                                            }
                                        }
                                        Column {
                                            width: 354
                                            height: _item.height
                                            spacing: 4
                                            QText {
                                                width: 354
                                                text: _item.headline
                                                color: "#031F2B"
                                                font.family: "Lato"
                                                font.pixelSize: 16
                                                font.weight: Font.DemiBold
                                                horizontalAlignment: Text.AlignLeft
                                                verticalAlignment: Text.AlignVCenter
                                            }
                                            QText {
                                                width: 354
                                                text: _item.content
                                                color: "#031F2B"
                                                font.family: "Lato"
                                                font.pixelSize: 16
                                                lineHeightMode: Text.FixedHeight
                                                lineHeight: 28
                                                wrapMode: Text.WordWrap
                                                horizontalAlignment: Text.AlignLeft
                                                verticalAlignment: Text.AlignVCenter
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
    }
    QTextButton {
        width: 123
        height: 48
        anchors{
            right: parent.right
            rightMargin: 24
            bottom: parent.bottom
            bottomMargin: 16
        }
        label.text: STR.STR_QML_770
        label.font.pixelSize: 16
        type: eTypeE
        onButtonClicked: {
            _clainNot.showPopup()
        }
    }
    function showPopup() {
        _InfoVer.link = "https://nunchuk.io/claim"
        _InfoVer.linkTop = true
        _InfoVer.contentText = STR.STR_QML_771
        _InfoVer.labels = [STR.STR_QML_772,STR.STR_QML_341]
        _InfoVer.open();
    }
}
