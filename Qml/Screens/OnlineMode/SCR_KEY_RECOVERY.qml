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
import Qt.labs.platform 1.1
import HMIEVENTS 1.0
import EWARNING 1.0
import NUNCHUCKTYPE 1.0
import DataPool 1.0
import "../../Components/origins"
import "../../Components/customizes"
import "../../Components/customizes/Texts"
import "../../Components/customizes/Buttons"
import "../../Components/customizes/Popups"
import "../../../localization/STR_QML.js" as STR

QScreen {
    id: _period
    property var keyRecovery: ServiceSetting.servicesTag.keyRecovery
    property int questionSelected: 0
    QOnScreenContentTypeA {
        width: popupWidth
        height: popupHeight
        anchors.centerIn: parent
        label.text: STR.STR_QML_698
        onCloseClicked: closeTo(NUNCHUCKTYPE.SERVICE_TAB)
        Column {
            anchors {
                top: parent.top
                topMargin: 84
                left: parent.left
                leftMargin: 36
            }

            width: 646
            spacing: 16
            Repeater {
                id: _description
                width: parent.width
                readonly property var content_map: [
                    {width: 646, height: 84, title:STR.STR_QML_722, icon: ""   },
                    {width: 539, height: 84, title:STR.STR_QML_723, icon: "qrc:/Images/Images/1.Active.svg"   },
                    {width: 539, height: 56, title:STR.STR_QML_724, icon: "qrc:/Images/Images/2.Active.svg"   },
                ]
                model: content_map.length
                Rectangle {
                    id: _rect
                    width: _description.content_map[index].width
                    height: _description.content_map[index].height
                    Row {
                        spacing: 8
                        QIcon {
                            iconSize: 24
                            id:_numicon
                            source: _description.content_map[index].icon
                            visible: source != ""
                        }
                        QText {
                            id:_numText
                            width: _rect.width - _numicon.width
                            height: _description.content_map[index].height
                            anchors.verticalCenter: parent.verticalCenter
                            text: _description.content_map[index].title
                            color: "#031F2B"
                            font.family: "Lato"
                            font.pixelSize: 16
                            lineHeightMode: Text.FixedHeight
                            lineHeight: 28
                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter
                        }
                    }
                }
            }

        }

        Rectangle {
            width: 718
            height: 60
            radius: 8
            color: "#EAEAEA"
            anchors {
                horizontalCenter: parent.horizontalCenter
                bottom: parent.bottom
                bottomMargin: 120
            }

            Row {
                anchors.fill: parent
                anchors.margins: 15
                spacing: 11
                QIcon {
                    iconSize: 30
                    anchors.verticalCenter: parent.verticalCenter
                    source: "qrc:/Images/Images/error_outline_24px_n.png"
                }
                QText {
                    width: 650
                    height: 28
                    anchors.verticalCenter: parent.verticalCenter
                    text: STR.STR_QML_728
                    color: "#031F2B"
                    font.family: "Lato"
                    font.pixelSize: 16
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }

        onPrevClicked: closeTo(NUNCHUCKTYPE.SERVICE_TAB)

        onNextClicked: {
            if (keyRecovery.tapsigners.length > 0) {
                signers.currentIndex = 0
                _chose.open()
            } else {
                _info1.open()
            }
        }
        Popup {
            id: _chose
            width: parent.width
            height: parent.height
            modal: true
            focus: true
            background: Item{}
            QOnScreenContentTypeB {
                width: 600
                height: 292
                anchors {
                    top:parent.top
                    topMargin: 150
                    horizontalCenter: parent.horizontalCenter
                }
                label.text: ""
                onCloseClicked: {
                    _chose.close()
                }

                QText {
                    width: 528
                    height: 20
                    anchors {
                        horizontalCenter: parent.horizontalCenter
                        bottom: signers.top
                        bottomMargin: 4
                    }
                    text: STR.STR_QML_729
                    color: "#031F2B"
                    font.family: "Lato"
                    font.pixelSize: 16
                    font.weight: Font.DemiBold
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }


                QComboBoxIndex {
                    id: signers
                    width: 528
                    height: 48
                    anchors {
                        top:parent.top
                        topMargin: 124
                        horizontalCenter: parent.horizontalCenter
                    }
                    textRole: "displayName"
                    model: keyRecovery.tapsigners
                }

                onPrevClicked: {
                    _chose.close()
                }

                onNextClicked: {
                    var item = keyRecovery.tapsigners[signers.currentIndex]
                    QMLHandle.sendEvent(EVT.EVT_INPUT_TAPSIGNER_SELECT_REQUEST,item)
                }
            }
        }
    }
    QPopupInfo{
        id:_info1
        contentText: STR.STR_QML_744
    }
    Connections {
        target: keyRecovery
        onKeyRecoveryPendingApproval: {
            errorConfirm.open()
        }
    }
    QPopupInfo{
        id: errorConfirm
        title: STR.STR_QML_339
        contentText: STR.STR_QML_1056
        btnLabel: STR.STR_QML_341
    }
}
