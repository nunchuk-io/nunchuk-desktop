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

QScreenAdd {
    anchors.fill: parent
    QOnScreenContentTypeA {
        visible: AppModel.addSignerWizard === eADD_INFORMATION
        width: popupWidth
        height: popupHeight
        anchors.centerIn: parent
        label.text: STR.STR_QML_923
        onCloseClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
        content: Item {
            Row {
                spacing: 36
                Rectangle {
                    width: 346
                    height: 512
                    radius: 24
                    color: "#D0E2FF"
                    QPicture {
                        width: 346
                        height: 300
                        anchors.verticalCenter: parent.verticalCenter
                        source: "qrc:/Images/Images/BitBox-illustration.svg"
                    }
                }
                Item {
                    width: 346
                    height: 512
                    Column {
                        width: parent.width
                        spacing: 24
                        QLato {
                            width: parent.width
                            text: STR.STR_QML_817
                            lineHeightMode: Text.FixedHeight
                            lineHeight: 28
                            wrapMode: Text.WordWrap
                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter
                        }
                        Repeater {
                            id: _guide
                            width: parent.width
                            readonly property var content_map: [
                                {height: 84, headline:STR.STR_QML_818, content: STR.STR_QML_926, icon: "qrc:/Images/Images/1.Active.svg" },
                                {height: 112, headline:STR.STR_QML_095, content: STR.STR_QML_927, icon: "qrc:/Images/Images/2.Active.svg" },
                            ]
                            model: content_map.length
                            Rectangle {
                                property var _item: _guide.content_map[index]
                                width: 346
                                height: _item.height
                                Row {
                                    spacing: 12
                                    QIcon {
                                        iconSize: 24
                                        id: _ico
                                        source: _item.icon
                                    }
                                    Column {
                                        width: 310
                                        height: _item.height
                                        spacing: 4
                                        QText {
                                            width: 310
                                            text: _item.headline
                                            color: "#031F2B"
                                            font.family: "Lato"
                                            font.pixelSize: 16
                                            font.weight: Font.DemiBold
                                            horizontalAlignment: Text.AlignLeft
                                            verticalAlignment: Text.AlignVCenter
                                        }
                                        QText {
                                            width: 310
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

        onPrevClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
        onNextClicked: {
            AppModel.addSignerWizard = eADD_REFRESH_DEVICE
        }
    }
    QOnScreenContentTypeA {
        id: _refresh
        visible: AppModel.addSignerWizard === eADD_REFRESH_DEVICE
        width: popupWidth
        height: popupHeight
        anchors.centerIn: parent
        label.text: STR.STR_QML_923
        onCloseClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
        content: QAddKeyRefreshDevices {
            title: STR.STR_QML_929
            state_id: EVT.STATE_ID_SCR_ADD_HARDWARE
        }
        onPrevClicked: {
            AppModel.addSignerWizard = eADD_INFORMATION
        }
        bottomRight: QTextButton {
            width: 120
            height: 48
            label.text: STR.STR_QML_265
            label.font.pixelSize: 16
            type: eTypeE
            enabled: _refresh.contentItem.isEnable()
            onButtonClicked: {
                _refresh.contentItem.addDevice()
            }
        }
    }

    QOnScreenContent {
        visible: AppModel.addSignerWizard === eADD_LOADING
        width: popupWidth
        height: popupHeight
        anchors.centerIn: parent
        enableHeader: false
        onCloseClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
        content: Item {
            Column {
                width: 400
                height: 56
                anchors.centerIn: parent
                spacing: 16
                QProgressbarTypeA {
                    id: progresBar
                    percentage: AppModel.addSignerPercentage
                }
                QLato{
                    font.weight: Font.Bold
                    font.pixelSize: 20
                    text: STR.STR_QML_930
                    anchors.horizontalCenter: parent.horizontalCenter
                    horizontalAlignment: Text.AlignHCenter
                }
            }
        }
    }

    QOnScreenContent {
        visible: AppModel.addSignerWizard === eADD_SUCCESSFULLY || AppModel.addSignerWizard === eADD_ERROR
        width: popupWidth
        height: popupHeight
        anchors.centerIn: parent
        label.text: ""
        onCloseClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
        Column {
            anchors.fill: parent
            anchors.margins: 36
            spacing: 24
            Rectangle {
                width: 96;height: 96;
                radius: 48
                color: AppModel.addSignerWizard === eADD_SUCCESSFULLY ? "#A7F0BA" : "#FFD7D9"
                QIcon {
                    iconSize: 60
                    anchors.centerIn: parent
                    source: AppModel.addSignerWizard === eADD_SUCCESSFULLY ? "qrc:/Images/Images/check-dark.svg" : "qrc:/Images/Images/error_outline_24px.png"
                }
            }
            QLato {
                width: parent.width
                height: 40
                text: AppModel.addSignerWizard === eADD_SUCCESSFULLY ? STR.STR_QML_931 : STR.STR_QML_965
                font.pixelSize: 32
                font.weight: Font.DemiBold
                verticalAlignment: Text.AlignVCenter
            }
            QLato {
                visible: AppModel.addSignerWizard === eADD_SUCCESSFULLY
                width: parent.width
                height: 28
                text: STR.STR_QML_828
                verticalAlignment: Text.AlignVCenter
                lineHeightMode: Text.FixedHeight
                lineHeight: 28
                wrapMode: Text.WordWrap
            }
        }
        bottomRight: Row {
            spacing: 12
            QTextButton {
                width: 120
                height: 48
                label.text: STR.STR_QML_777
                label.font.pixelSize: 16
                type: eTypeE
                onButtonClicked: doneOrTryAgainAddHardwareKey(AppModel.addSignerWizard === eADD_SUCCESSFULLY)
            }
        }
    }
}
