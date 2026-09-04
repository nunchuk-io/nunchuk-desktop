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

import QtQuick
import QtQuick.Controls
import DRACO_CODE 1.0
import DataPool 1.0
import EWARNING 1.0
import Features.Claiming.ViewModels 1.0
import HMIEVENTS 1.0
import NUNCHUCKTYPE 1.0
import QRCodeItem 1.0
import Qt.labs.platform 1.1
import Qt5Compat.GraphicalEffects
import "./../../../origins"
import "./../../../customizes"
import "./../../../customizes/Buttons"
import "./../../../customizes/Chats"
import "./../../../customizes/Texts"

Item {
    property var list: [
        {id: "hardware",         description: QSTR.STR_QML_2080},
        {id: "backupPassword",   description: QSTR.STR_QML_2081},
    ]
    Column {
        id: _claim
        anchors.fill: parent
        spacing: 24

        anchors {
            left: parent.left
            leftMargin: 24
            top: parent.top
            topMargin: 24
        }

        Rectangle {
            id: banner

            width: parent.width - 24
            height: parent.height * 0.42
            radius: 24
            color: "#D0E2FF"

            QPicture {
                anchors.fill: parent
                fillMode: Image.PreserveAspectFit
                source: "qrc:/Images/Images/claim-inheritance-illustration.svg"
            }

        }

        QMontserrat {
            id: title
            text: QSTR.STR_QML_747
            font.pixelSize: 32
            font.weight: Font.Medium
        }

        Item {
            width: parent.width
            height: parent.height - banner.height - title.height - 48
            Column {
                width: parent.width
                spacing: 16

                QText {
                    text: QSTR.STR_QML_2079
                    color: "#031F2B"
                    font.family: "Lato"
                    font.pixelSize: 16
                    font.weight: Font.Normal
                }

                Row {
                    height: 76
                    spacing: 12
                    Repeater {
                        model: list
                        QRadioButtonTypeE {
                            width: 343
                            height: 76
                            fontWeight: Font.Bold
                            label: modelData.description
                            selected: (vm.isBackupPasswordUsed ? "backupPassword" : "hardware") == modelData.id
                            onButtonClicked: { 
                                vm.isBackupPasswordUsed = (modelData.id == "backupPassword")
                            }
                        }
                    }
                }

            }

            Rectangle {
                width: parent.width + 24
                height: 80
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                color: "#FFFFFF"

                Rectangle {
                    width: parent.width
                    height: 1
                    color: "#DEDEDE"
                }

                QTextButton {
                    width: label.paintedWidth + 2 * 16
                    height: 48
                    label.text: QSTR.STR_QML_265
                    label.font.pixelSize: 16
                    type: eTypeE
                    onButtonClicked: {
                        vm.confirmProceed()
                    }

                    anchors {
                        right: parent.right
                        rightMargin: 24
                        verticalCenter: parent.verticalCenter
                    }
                }
            }
        }
    }

    ProceedOptionsViewModel {
        id: vm
    }
}
