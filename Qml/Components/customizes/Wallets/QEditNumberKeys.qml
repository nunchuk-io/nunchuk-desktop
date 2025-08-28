//*************************************************************************
// * This file is part of the Nunchuk software (https://nunchuk.io/)        *
// * Copyright (C) 2020-2022 Enigmo								          *
// * Copyright (C) 2022 Nunchuk								              *
// *                                                                        *
// * This program is free software; you can redistribute it and/or          *
// * modify it under the terms of the GNU General Public License            *
// * as published by the Free Software Foundation; either version 3         *
// * of the License, or (at your option) any later version.                 *
// *                                                                        *
// * This program is distributed in the hope that it will be useful,        *
// * but WITHOUT ANY WARRANTY; without even the implied warranty of         *
// * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
// * GNU General Public License for more details.                           *
// *                                                                        *
// * You should have received a copy of the GNU General Public License      *
// * along with this program.  If not, see <http://www.gnu.org/licenses/>.  *
// *                                                                        *
// *************************************************************************
// Qt imports

// Application-specific imports
import QtQuick 2.4
import QtQuick.Controls 2.3
import Qt.labs.platform 1.1
import QtGraphicalEffects 1.12

// Application-specific imports
import "../../../../localization/STR_QML.js" as STR
import "../../customizes"
import "../../customizes/Buttons"
import "../../customizes/Popups"
import "../../customizes/Signers"
import "../../customizes/Texts"
import "../../origins"

// Third-party or additional module imports
import DataPool 1.0
import EWARNING 1.0
import HMIEVENTS 1.0
import NUNCHUCKTYPE 1.0

Column {
    width: 350
    spacing: 17
    property string headline: STR.STR_QML_1652
    property string description: STR.STR_QML_1653

    Column {
        width: 350
        spacing: 4

        QLato {
            height: 20
            text: headline
            font.weight: Font.Bold
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignLeft
        }

        QLato {
            height: 16
            text: description
            font.weight: Font.Medium
            font.pixelSize: 12
            color: "#757575"
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignLeft
        }

    }

    Item {
        width: 350
        height: 48

        Row {
            height: 48
            spacing: 16

            Rectangle {
                width: 36
                height: 36
                radius: 36
                anchors.verticalCenter: parent.verticalCenter
                color: "transparent"
                border.width: 2
                border.color: minus.pressed ? "#595959" : "#031F2B"

                Rectangle {
                    width: 12
                    height: 2
                    color: minus.pressed ? "#595959" : "#031F2B"
                    anchors.centerIn: parent
                }

                MouseArea {
                    id: minus

                    anchors.fill: parent
                    onClicked: {
                        if (newWalletInfo.walletM > 0)
                            newWalletInfo.walletM--;

                    }
                }

            }

            Rectangle {
                id: root

                width: 48
                height: 48
                radius: 8
                color: "#FFFFFF"
                border.color: "#DEDEDE"
                border.width: 1

                QLato {
                    anchors.centerIn: parent
                    text: newWalletInfo.walletM
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

            }

            Rectangle {
                width: 36
                height: 36
                radius: 36
                anchors.verticalCenter: parent.verticalCenter
                color: "transparent"
                border.width: 2
                border.color: plusbtn.pressed ? "#595959" : "#031F2B"

                Rectangle {
                    width: 12
                    height: 2
                    color: plusbtn.pressed ? "#595959" : "#031F2B"
                    anchors.centerIn: parent
                }

                Rectangle {
                    width: 2
                    height: 12
                    color: plusbtn.pressed ? "#595959" : "#031F2B"
                    anchors.centerIn: parent
                }

                MouseArea {
                    id: plusbtn

                    anchors.fill: parent
                    onClicked: {
                        var count = flickerRight.updateVisibleCount();
                        newWalletInfo.walletN = count;
                        if (newWalletInfo.walletM < count)
                            newWalletInfo.walletM++;

                    }
                }

            }

        }

    }

}
