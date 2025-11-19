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
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.1
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.12
import Qt.labs.platform 1.1
import HMIEVENTS 1.0
import EWARNING 1.0
import NUNCHUCKTYPE 1.0
import DataPool 1.0
import "../../origins"
import "../../customizes"
import "../../customizes/Texts"
import "../../../../localization/STR_QML.js" as STR

Item {
    property var planInfo: inheritancePlanInfo.planInfo
    property var walletInfo: ServiceSetting.walletInfo
    property string title: STR.STR_QML_887
    property string subtitle1: STR.STR_QML_888
    property string subtitle2: STR.STR_QML_889
    property string warning: STR.STR_QML_890
    width: 346
    height: 512
    Column {
        width: parent.width
        spacing: 16
        QLato {
            width: parent.width
            text: title
            lineHeightMode: Text.FixedHeight
            lineHeight: 28
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
        }
        Row {
            spacing: 12
            QIcon {
                iconSize: 24
                source: "qrc:/Images/Images/1.Active.svg"
            }
            Column {
                width: 274
                spacing: 10
                QLato {
                    width: 310
                    height: 60
                    text: subtitle1
                    wrapMode: Text.WordWrap
                    horizontalAlignment: Text.AlignLeft
                }
                Rectangle {
                    width: 274
                    height: 48
                    color: "#F5F5F5"
                    radius: 12
                    QLato {
                        width: parent.width
                        text: planInfo.magic
                        anchors.centerIn: parent
                        horizontalAlignment: Text.AlignHCenter
                    }
                }
                Rectangle {
                    width: 274
                    height: 48
                    color: "#F5F5F5"
                    radius: 12
                    QLato {
                        width: parent.width
                        text: STR.STR_QML_2037
                        anchors.centerIn: parent
                        horizontalAlignment: Text.AlignHCenter
                    }
                    QIcon {
                        iconSize: 24
                        source: "qrc:/Images/Images/download_black.svg"
                        anchors {
                            right: parent.right
                            rightMargin: 12
                            verticalCenter: parent.verticalCenter
                        }
                        MouseArea {
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            onClicked: {
                                exportwalletDialog.currentFile = StandardPaths.writableLocation(StandardPaths.DocumentsLocation) + "/"
                                        + RoomWalletData.getValidFilename(walletInfo.walletName)
                                        + ".bsms"
                                exportwalletDialog.open()
                            }
                        }
                    }
                }
           }
        }
        Row {
            width: 346
            spacing: 12
            QIcon {
                iconSize: 24
                source: "qrc:/Images/Images/2.Active.svg"
            }
            QLato {
                width: 310
                text: subtitle2
                lineHeightMode: Text.FixedHeight
                lineHeight: 28
                wrapMode: Text.WordWrap
                horizontalAlignment: Text.AlignLeft
                anchors.verticalCenter: parent.verticalCenter
            }
        }
        Rectangle {
            width: warningRow.implicitWidth + 24
            height: warningRow.implicitHeight + 24
            color: "#FDEBD2"
            radius: 8
            Row {
                id: warningRow
                anchors.centerIn: parent
                spacing: 12
                QIcon {
                    iconSize: 24
                    source: "qrc:/Images/Images/warning-dark.svg"
                    anchors.verticalCenter: parent.verticalCenter
                }
                QLato {
                    width: 274
                    text: warning
                    font.weight: Font.Bold
                    lineHeightMode: Text.FixedHeight
                    font.pixelSize: 12
                    lineHeight: 16
                    wrapMode: Text.WordWrap
                    horizontalAlignment: Text.AlignLeft
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
        }
    }
    FileDialog {
        id: exportwalletDialog
        fileMode: FileDialog.SaveFile
        onAccepted: {
            walletInfo.requestExportWalletViaBSMS(exportwalletDialog.currentFile)
        }
    }
}
