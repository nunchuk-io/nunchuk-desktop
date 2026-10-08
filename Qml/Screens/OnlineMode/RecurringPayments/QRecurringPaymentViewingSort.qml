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
import QtGraphicalEffects 1.12
import QtQuick.Controls 2.5
import QRCodeItem 1.0
import HMIEVENTS 1.0
import EWARNING 1.0
import NUNCHUCKTYPE 1.0
import DataPool 1.0
import "../../../Components/origins"
import "../../../Components/customizes/Texts"
import "../../../Components/customizes/Buttons"
import "../../../../localization/STR_QML.js" as STR

Popup {
    id: _sort
    width: parent.width
    height: parent.height
    modal: true
    focus: true
    closePolicy: Popup.CloseOnEscape
    background: Item{}
    property var recurringPayment: AppModel.walletInfo.recurringPayment
    property string option: recurringPayment.sortOption
    Connections {
        target: recurringPayment
        onSortOptionChanged: {
            option = recurringPayment.sortOption
        }
    }

    property var sorts: [
        {id: "newest"},
        {id: "oldest"},
        {id: "A-Z"},
        {id: "Z-A"}
    ]
    Rectangle {
        id: qrmask
        width: 600
        height: 548
        radius: 24
        color: "#FFFFFF"
        anchors.centerIn: parent
        layer.enabled: true
        layer.effect: OpacityMask {
            maskSource: Rectangle {
                width: 600
                height: 548
                radius: 24
            }
        }
        QCloseButton {
            anchors {
                right: parent.right
                rightMargin: 16
                top: parent.top
                topMargin: 16
            }
            onClicked: _sort.close()
        }
        QMontserrat {
            anchors {
                top: parent.top
                topMargin: 36
                left: parent.left
                leftMargin: 36
            }
            font.pixelSize: 32
            text: STR.STR_QML_1137
        }
        Item {
            anchors {
                top: parent.top
                topMargin: 107
                left: parent.left
                leftMargin: 36
            }
            Column {
                spacing: 24
                Column {
                    spacing: 16
                    QLato {
                        font.weight: Font.DemiBold
                        font.pixelSize: 16
                        text: STR.STR_QML_1139
                    }
                    Column {
                        QRadioButtonTypeA {
                            width: 528
                            height: 48
                            label: STR.STR_QML_1140
                            selected: option === sorts[0].id
                            onButtonClicked: { option = sorts[0].id; }
                        }
                        QRadioButtonTypeA {
                            width: 528
                            height: 48
                            label: STR.STR_QML_1141
                            selected: option === sorts[1].id
                            onButtonClicked: { option = sorts[1].id; }
                        }
                    }
                }
                Column {
                    spacing: 16
                    QLato {
                        font.weight: Font.DemiBold
                        font.pixelSize: 16
                        text: STR.STR_QML_1142
                    }
                    Column {
                        QRadioButtonTypeA {
                            width: 528
                            height: 48
                            label: STR.STR_QML_1143
                            selected: option === sorts[2].id
                            onButtonClicked: { option = sorts[2].id; }
                        }
                        QRadioButtonTypeA {
                            width: 528
                            height: 48
                            label: STR.STR_QML_1144
                            selected: option === sorts[3].id
                            onButtonClicked: { option = sorts[3].id; }
                        }
                    }
                }
            }
        }

        Row {
            anchors {
                bottom: parent.bottom
                bottomMargin: 36
                left: parent.left
                leftMargin: 36
            }
            spacing: 12
            QTextButton {
                width: 80
                height: 48
                label.text: STR.STR_QML_245
                label.font.pixelSize: 16
                type: eTypeF
                onButtonClicked: {
                    _sort.close()
                }
            }
            QTextButton {
                width: 89
                height: 48
                label.text: STR.STR_QML_1138
                label.font.pixelSize: 16
                type: eTypeB
                onButtonClicked: {
                    var _input = {
                        type: "recurring-payment-clear-all"
                    }
                    QMLHandle.sendEvent(EVT.EVT_RECURRING_PAYMENTS_ENTER, _input)
                    _sort.close()
                }
            }
        }

        QTextButton {
            anchors {
                bottom: parent.bottom
                bottomMargin: 36
                right: parent.right
                rightMargin: 36
            }
            width: 74
            height: 48
            label.text: STR.STR_QML_534
            label.font.pixelSize: 16
            type: eTypeE
            enabled: option !== recurringPayment.sortOption
            onButtonClicked: {
                var _input = {
                    type: "recurring-payment-sort",
                    sortOption: option
                }
                QMLHandle.sendEvent(EVT.EVT_RECURRING_PAYMENTS_ENTER, _input)
                _sort.close()
            }
        }
    }
    DropShadow {
        anchors.fill: qrmask
        horizontalOffset: 3
        verticalOffset: 3
        radius: 8.0
        samples: 17
        color: "#80000000"
        source: qrmask
    }
}
