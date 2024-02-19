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
import QtQuick.Controls 1.4
import QtGraphicalEffects 1.12
import QtQuick.Controls.Styles 1.4
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
    id: _Period
    property var planInfo
    property string period_name: planInfo.buffer_period.display_name
    property string period_id: planInfo.buffer_period.id
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_862
    onCloseClicked: closeTo(NUNCHUCKTYPE.SERVICE_TAB)
    content: Item {
        Column {
            anchors.fill: parent
            anchors.top: parent.top
            anchors.topMargin: -16
            spacing: 24
            QLato {
                width: 539
                height: 140
                text: STR.STR_QML_863
                anchors.left: parent.left
                font.weight: Font.Normal
                lineHeight: 28
                lineHeightMode: Text.FixedHeight
                wrapMode: Text.WrapAnywhere
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
            }
            Item {
                width: 539
                height: 212
                Column {
                    anchors.fill: parent
                    spacing: 16
                    QListView {
                        id: questions
                        width: 539
                        height: childrenRect.height
                        spacing: 8
                        clip: true
                        model: inheritancePlanInfo.periods
                        interactive: false
                        delegate: QRadioButtonTypeE {
                            id: btn
                            width: 539
                            height: 60
                            label: modelData.display_name
                            fontFamily: "Lato"
                            fontPixelSize: 16
                            fontWeight: btn.selected ? Font.ExtraBold : Font.DemiBold
                            selected: period_id === modelData.id
                            textBadge: modelData.is_recommended ? STR.STR_QML_879 : ""
                            onButtonClicked: {
                                period_name = modelData.display_name
                                period_id = modelData.id
                            }
                        }
                    }
                }
            }
        }
    }
    bottomRight: QTextButton {
        width: 195
        height: 48
        label.text: STR.STR_QML_864
        label.font.pixelSize: 16
        type: eTypeE
        onButtonClicked: {
            _Period.nextClicked()
        }
    }
    onPrevClicked: closeTo(NUNCHUCKTYPE.SERVICE_TAB)
    onNextClicked: {
        var buffer_period = {
            "id" : period_id,
            "display_name" : period_name
        }
        var _edit = {
            "buffer_period": buffer_period
        }
        inheritancePlanInfo.editPlanInfo(_edit)
        closeTo(NUNCHUCKTYPE.SERVICE_TAB)
    }
}

