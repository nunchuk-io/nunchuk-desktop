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
import "../../../Components/origins"
import "../../../Components/customizes/Buttons"
import "../../../Components/customizes/Signers"
import "../../../Components/customizes/Texts"

QSingleSignerBaseDelegate {
    id: del
    width: 350
    height: 84
    property bool isSigned: signerData.single_signed_status
    signal signRequest()
    Loader {
        width: 57
        height: 36
        anchors {
            verticalCenter: parent.verticalCenter
            right: parent.right
            rightMargin: 0
        }
        sourceComponent: isSigned ? signedComp : requiredSignature
    }
    Component {
        id: requiredSignature
        Item {
            QTextButton {
                width: 57
                height: 36
                label.text: STR.STR_QML_509
                label.font.pixelSize: 12
                type: eTypeE
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                onButtonClicked: { signRequest() }
            }
        }
    }
    Component {
        id: signedComp
        Item {
            Row {
                spacing: 4
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right

                QLato {
                    text: STR.STR_QML_2108
                    font.pixelSize: 12
                    color: "#031F2B"
                    anchors.verticalCenter: parent.verticalCenter
                }

                QIcon {
                    iconSize: 24
                    source: "qrc:/Images/Images/check_circle_24px_n.png"
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
        }
    }
}
