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

QOnScreenContentTypeB {
    id:_content
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_970
    extraHeader: Item {}
    onCloseClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
    property var dashboardInfo: GroupWallet.dashboardInfo
    property string select_period_id: dashboardInfo.historyPeriodId
    content: Item {
        Column {
            anchors.fill: parent
            spacing: 24
            QLato {
                width: 539
                height: paintedHeight
                text: STR.STR_QML_1300
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
                wrapMode: Text.WordWrap
                font.bold: true
            }
            QListView {
                id: _historyPeriods
                width: 539
                height: childrenRect.height
                spacing: 8
                clip: true
                model: GroupWallet.historyPeriods
                interactive: false
                delegate: Item {
                    width: 539
                    height: 48
                    QLato {
                        id: text
                        text: modelData.display_name
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    QIcon {
                        id: icon
                        iconSize: 24
                        source: select_period_id === modelData.id ? "qrc:/Images/Images/radio-selected-dark.svg" : "qrc:/Images/Images/radio-dark.svg"
                        anchors{
                            verticalCenter: parent.verticalCenter
                            right: parent.right
                        }
                    }
                    MouseArea {
                        id: mouse
                        anchors.fill: parent
                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor
                        onClicked: {
                            select_period_id = modelData.id
                        }
                    }
                }
            }
        }
    }
    onPrevClicked: closeTo(NUNCHUCKTYPE.CURRENT_TAB)
    nextEnable: select_period_id !== dashboardInfo.historyPeriodId
    bottomRight: QTextButton {
        width: 66
        height: 48
        label.text: STR.STR_QML_835
        label.font.pixelSize: 16
        type: eTypeE
        onButtonClicked: {
            var _input = {
                type: "select-period-history",
                history_period_id: select_period_id
            }
            QMLHandle.sendEvent(EVT.EVT_DASHBOARD_ALERT_INFO_ENTER, _input)
            closeTo(NUNCHUCKTYPE.CURRENT_TAB)
        }
    }
}
