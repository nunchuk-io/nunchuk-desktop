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
import "../../../Components/customizes/Popups"
import "../../../../localization/STR_QML.js" as STR

QOnScreenContentTypeA {
    id:_content
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    label.text: STR.STR_QML_1068
    extraHeader: Item {}
    onCloseClicked: closeTo(NUNCHUCKTYPE.WALLET_TAB)
    content: Item {
        property bool anyChanged: _repeat.option !== "" || _date.start_date !== "" || _date.end_date !== ""
        Column {
            anchors {
                top: parent.top
                topMargin: -8
            }
            spacing: 24
            QLato {
                width: 539
                height: 28
                text: STR.STR_QML_1095
                anchors.left: parent.left
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
            }
            Item {
                width: 728
                height: 308
                Row {
                    spacing: 36
                    QFrequencyRepeat {
                        id: _repeat
                        width: 346
                        height: 308
                    }
                    QFrequencyDateRange {
                        id: _date
                        width: 346
                        height: 308
                    }
                }
                Connections {
                    target: _content
                    onSignalFrequencyApply: {
                        if (AppModel.walletInfo.recurringPayment.isValidDate(_date.start_date, _date.end_date, _date.noEndate, _repeat.option))
                        {
                            var _input = {
                                type: "setup-frequency-of-payment",
                                frequency: _repeat.option,
                                start_date: _date.start_date,
                                end_date: _date.noEndate ? {} : _date.end_date
                            }
                            QMLHandle.sendEvent(EVT.EVT_RECURRING_PAYMENTS_ENTER, _input)
                        }
                    }
                    onSignalBeginApply: {
                        if (_date.start_date === "") {
                            AppModel.showToast(-1, STR.STR_QML_1168, EWARNING.ERROR_MSG);
                        }
                        else
                        {
                            var recurring = AppModel.walletInfo.recurringPayment
                            if (recurring.isValidDate(_date.start_date)) {
                                _info.open()
                            }
                            else
                            {
                                signalFrequencyApply()
                            }
                        }
                    }
                    onSignalChangeDate: {
                        _date.needChangeDate()
                    }
                }
            }
        }
    }
    onPrevClicked: requestBack()
    nextEnable: _content.contentItem.anyChanged
    onNextClicked: {
        signalBeginApply()
    }
    signal signalBeginApply()
    signal signalFrequencyApply()
    signal signalChangeDate()
    QPopupInfoTwoButtons {
        id: _info
        property string link:"https://nunchuk.io/"
        title: STR.STR_QML_339
        contentText: STR.STR_QML_1115
        labels: [STR.STR_QML_341,STR.STR_QML_1114]
        isVertical: false
        funcs: [
            function() {},
            function() {
                _content.signalChangeDate()
            }
        ]
    }
}
