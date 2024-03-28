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
import "../../../Components/origins"
import "../../../Components/customizes"
import "../../../Components/customizes/Chats"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../../localization/STR_QML.js" as STR

QOnScreenContentTypeA {
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_1266
    onCloseClicked: closeTo(NUNCHUCKTYPE.WALLET_TAB)
    property string mnemonicstr: ""
    content: Item {
        height: 496
        Column {
            spacing: 24
            QLato {
                width: 646
                text: STR.STR_QML_1269
                anchors.left: parent.left
                lineHeightMode: Text.FixedHeight
                lineHeight: 28
                wrapMode: Text.WordWrap
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft
            }
            Grid {
                id: gridmmonic
                width: 730
                height: 500
                columns: 6
                spacing: 12
                Repeater {
                    model: 24
                    Item {
                        width: (gridmmonic.width - gridmmonic.spacing*(gridmmonic.columns-1)) / gridmmonic.columns
                        height: 48
                        y: 100
                        Rectangle {
                            id: itembg
                            height: 48
                            width: parent.width - 4
                            radius: 8
                            color: "#FFFFFF"
                            visible: false
                        }
                        DropShadow {
                            anchors.fill: itembg
                            horizontalOffset: 3
                            verticalOffset: 3
                            radius: 8.0
                            samples: 17
                            color: "#80000000"
                            source: itembg
                            QLato {
                                id: nemonictext
                                anchors {
                                    fill: parent
                                    topMargin: 5
                                    bottomMargin: 5
                                    leftMargin: 10
                                    rightMargin: 5
                                }
                                verticalAlignment: Text.AlignVCenter
                                text: ((index+1) > 9 ? (index+1) : "0"+(index+1)) + ". " + textMnemonic.split(' ')[index];
                            }
                        }
                    }
                }
            }
        }
        Rectangle {
            width: 728
            height: 60
            anchors {
                bottom: parent.bottom
                bottomMargin: 16
                horizontalCenter: parent.horizontalCenter
            }
            radius: 8
            color: "#EAEAEA"
            Row {
                anchors.fill: parent
                anchors.margins: 12
                spacing: 8
                QImage {
                    height: parent.height
                    width: height
                    source: "qrc:/Images/Images/info-60px.png"
                }
                QLato {
                    height: parent.height
                    text: STR.STR_QML_144
                }
            }
        }
    }

    onPrevClicked: closeTo(NUNCHUCKTYPE.WALLET_TAB)
    bottomRight: Row {
        spacing: 12
        QTextButton {
            width: 99
            height: 48
            label.text: STR.STR_QML_097
            label.font.pixelSize: 16
            type: eTypeE
            enabled: (textMnemonic !== "")
            onButtonClicked: {
                OnBoarding.state = "confirmSeedPhrase"
            }
        }
    }
    property string textMnemonic: AppModel.mnemonic
}
