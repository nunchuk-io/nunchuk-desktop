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
import QtQuick.Controls 1.4
import QtQuick.Controls 2.3
import QtQuick.Controls.Styles 1.4
import QtGraphicalEffects 1.12
import HMIEVENTS 1.0
import EWARNING 1.0
import NUNCHUCKTYPE 1.0
import "../../Components/origins"
import "../../Components/customizes"
import "../../../localization/STR_QML.js" as STR

QScreen {
    id: rootAddsignerToWallet
    QOnScreenContent {
        width: popupWidth
        height: popupHeight
        anchors.centerIn: parent
        label.text: STR.STR_QML_142
        onCloseClicked: {
            if(NUNCHUCKTYPE.CHAT_TAB === AppModel.tabIndex){
                QMLHandle.sendEvent(EVT.EVT_ONLINE_ONS_CLOSE_REQUEST, EVT.STATE_ID_SCR_ADD_NEW_SOFTWARE_SIGNER)
            }
            else{
                if(NUNCHUCKTYPE.FLOW_ADD_WALLET === QMLHandle.currentFlow){
                    QMLHandle.sendEvent(EVT.EVT_NEW_SOFTWARE_SIGNER_BACK_TO_WALLET_SIGNER_CONFIGURATION)
                }
                else {
                    QMLHandle.sendEvent(EVT.EVT_ONS_CLOSE_REQUEST, EVT.STATE_ID_SCR_ADD_NEW_SOFTWARE_SIGNER)
                }
            }
        }
        QText {
            width: 540
            height: 56
            text: STR.STR_QML_143
            anchors {
                left: parent.left
                leftMargin: 36
                top: parent.top
                topMargin: 65
            }
            verticalAlignment: Text.AlignVCenter
            color: "#031F2B"
            font.family: "Lato"
            font.pixelSize: 16
            wrapMode: Text.WordWrap
            lineHeightMode: Text.FixedHeight
            lineHeight: 28
        }
        Grid {
            id: gridmmonic
            width: 730
            height: 500
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: 150
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
                        QText {
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
        Rectangle {
            width: 728
            height: 60
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 120
            anchors.horizontalCenter: parent.horizontalCenter
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
                QText {
                    height: parent.height
                    text: STR.STR_QML_144
                    color: "#031F2B"
                    font.family: "Lato"
                }
            }
        }
        QButtonTextLink {
            height: 24
            label: STR.STR_QML_059
            anchors {
                left: parent.left
                leftMargin: 40
                bottom: parent.bottom
                bottomMargin: 40
            }
            onButtonClicked: {
                if(NUNCHUCKTYPE.FLOW_ADD_WALLET === QMLHandle.currentFlow){
                    QMLHandle.sendEvent(EVT.EVT_ADD_NEW_SOFTWARE_SIGNER_TO_WALLET_BACK)
                }
                else {
                    QMLHandle.sendEvent(EVT.EVT_ADD_NEW_SOFTWARE_SIGNER_BACK)
                }
            }
        }
        QTextButton {
            width: 200
            height: 48
            label.text: STR.STR_QML_097
            label.font.pixelSize: 16
            type: eTypeE
            enabled: (textMnemonic !== "")
            anchors {
                right: parent.right
                rightMargin: 40
                bottom: parent.bottom
                bottomMargin: 32
            }
            onButtonClicked: {
                QMLHandle.sendEvent(EVT.EVT_CREATE_NEW_SEED)
            }
        }
    }
    property string textMnemonic: AppModel.mnemonic
}
