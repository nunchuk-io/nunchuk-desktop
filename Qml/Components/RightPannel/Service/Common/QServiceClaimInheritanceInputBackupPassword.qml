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
import DRACO_CODE 1.0
import DataPool 1.0
import EWARNING 1.0
import Features.Claiming.ViewModels 1.0
import HMIEVENTS 1.0
import NUNCHUCKTYPE 1.0
import QRCodeItem 1.0
import Qt.labs.platform 1.1
import QtGraphicalEffects 1.0
import "./../../../origins"
import "./../../../customizes"
import "./../../../customizes/Buttons"
import "./../../../customizes/Chats"
import "./../../../customizes/Texts"

Item {
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
                anchors.centerIn: parent
                source: "qrc:/Images/Images/claim-inheritance-illustration.svg"
            }

        }

        QMontserrat {
            id: title
            text: STR.STR_QML_747
            font.pixelSize: 32
            font.weight: Font.Medium
        }

        Item {
            width: parent.width
            height: parent.height - banner.height - title.height - 48

            function backuppwdClear() {
                backupPassword.textInputted = "";
                backupPassword_two.textInputted = "";
            }

            Component.onCompleted: {
                backuppwdClear();
            }

            Column {
                width: parent.width
                spacing: 16

                QText {
                    text: STR.STR_QML_1987
                    color: "#031F2B"
                    font.family: "Lato"
                    font.pixelSize: 16
                    font.weight: Font.Normal
                }

                QTextInputBoxTypeB {
                    id: backupPassword
                    label: STR.STR_QML_727
                    boxWidth: 537
                    boxHeight: 48
                    isValid: true
                    visible: vm.remainingCount > 0
                    onTextInputtedChanged: {
                        if (!backupPassword.isValid) {
                            backupPassword.isValid = true;
                            backupPassword.errorText = "";
                        }
                        backupPassword.showError = false;
                        vm.backupPwd = backupPassword.textInputted;
                    }
                }

                QTextInputBoxTypeB {
                    id: backupPassword_two
                    label: STR.STR_QML_727_optinal
                    boxWidth: 537
                    boxHeight: 48
                    isValid: true
                    visible: vm.remainingCount > 1
                    onTextInputtedChanged: {
                        if (!backupPassword_two.isValid) {
                            backupPassword_two.isValid = true;
                            backupPassword_two.errorText = "";
                        }
                        backupPassword_two.showError = false;
                        vm.backupPwdTwo = backupPassword_two.textInputted;
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
                    label.text: STR.STR_QML_265
                    label.font.pixelSize: 16
                    type: eTypeE
                    enabled: vm.backupPwd !== ""
                    onButtonClicked: {
                        vm.next()
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

    BackupPasswordViewModel {
        id: vm
    }
}
