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
import "../../../localization/STR_QML.js" as STR

Item {
    property string keyType: ""
    property string fingerPrint: ""
    property string notice: ""
    Column {
        spacing: 24
        QLato {
            text: notice
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
        }
        Rectangle {
            width: 539
            height: 464
            border.width: 1
            border.color: "#EAEAEA"
            radius: 8
            Column
            {
                anchors.fill: parent
                Repeater {
                    model: AppModel.masterSignerList
                    Item {
                        width: 539 - 12
                        visible: keyType === model.master_signer_deviceType
                        height: visible ? 92 : 0
                        anchors{
                            left: parent.left
                            leftMargin: 12
                        }
                        Row{
                            anchors.fill: parent
                            spacing: 8
                            Rectangle {
                                width: 48
                                height: 48
                                radius: width
                                color: "#F5F5F5"
                                anchors.verticalCenter: parent.verticalCenter
                                QImage {
                                    width: 24
                                    height: 24
                                    anchors.centerIn: parent
                                    source: GlobalData.iconTypes(keyType, model.master_signer_type)
                                    sourceSize.width: 100
                                    sourceSize.height: 100
                                }
                            }
                            Item{
                                width: 146
                                height: 60
                                anchors.verticalCenter: parent.verticalCenter
                                Column{
                                    spacing: 4
                                    QText {
                                        width: 146
                                        height: 20
                                        text: model.master_signer_name
                                        color: "#031F2B"
                                        font.weight: Font.Normal
                                        font.family: "Lato"
                                        font.pixelSize: 16
                                    }
                                    QBadge {
                                        text: STR.STR_QML_898
                                        color: "#EAEAEA"
                                    }
                                    QText {
                                        width: 146
                                        height: 20
                                        text: "XFP: " + model.master_signer_fingerPrint
                                        color: "#595959"
                                        font.weight: Font.Normal
                                        font.capitalization: Font.AllUppercase
                                        font.family: "Lato"
                                        font.pixelSize: 12
                                    }
                                }
                            }
                        }
                        QImage {
                            width: 24
                            height: 24
                            source: fingerPrint === model.master_signer_fingerPrint ? "qrc:/Images/Images/radio-selected-dark.svg" : "qrc:/Images/Images/radio-dark.svg"
                            scale: primaryKeyMouse.containsMouse ? 1.1 : 1.0
                            anchors{
                                right: parent.right
                                rightMargin: 12
                                verticalCenter: parent.verticalCenter
                            }
                        }
                        MouseArea {
                            id: primaryKeyMouse
                            hoverEnabled: true
                            cursorShape: Qt.PointingHandCursor
                            anchors.fill: parent
                            onClicked: {
                                fingerPrint = model.master_signer_fingerPrint
                            }
                        }
                    }
                }
            }
        }
    }
}
