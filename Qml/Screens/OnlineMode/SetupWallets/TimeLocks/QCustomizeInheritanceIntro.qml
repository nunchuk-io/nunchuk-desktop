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
import QtQuick
import QtQuick.Controls
import NUNCHUCKTYPE 1.0
import "../../../../Components/origins"
import "../../../../Components/customizes"
import "../../../../Components/customizes/Texts"
import "../../../../Components/customizes/Buttons"
import "../../../../../localization/STR_QML.js" as STR

QOnScreenContentTypeB {
    id: _intro
    width: popupWidth
    height: popupHeight
    anchors.centerIn: parent
    property string titleText: STR.STR_QML_2381
    property string introText: STR.STR_QML_2382
    property string stepsHeading: STR.STR_QML_2383
    property var steps: [
        { title: STR.STR_QML_015, description: STR.STR_QML_2384 },
        { title: STR.STR_QML_2385, description: STR.STR_QML_2386 }
    ]
    property string footerText: STR.STR_QML_2387
    property var infoTexts: [STR.STR_QML_2388, STR.STR_QML_2389]
    property string secondaryLabel: STR.STR_QML_2390
    property string primaryLabel: STR.STR_QML_265
    label.text: titleText
    signal keepCurrentClicked()

    content: Item {
        Column {
            width: parent.width
            spacing: 24
            QLato {
                width: parent.width
                text: _intro.introText
                lineHeightMode: Text.FixedHeight
                lineHeight: 20
                wrapMode: Text.WordWrap
                horizontalAlignment: Text.AlignLeft
            }
            QLato {
                width: parent.width
                text: _intro.stepsHeading
                lineHeightMode: Text.FixedHeight
                lineHeight: 20
                wrapMode: Text.WordWrap
                horizontalAlignment: Text.AlignLeft
            }
            Repeater {
                model: _intro.steps
                Row {
                    width: parent.width
                    spacing: 12
                    Rectangle {
                        width: 24
                        height: 24
                        radius: 12
                        color: "transparent"
                        border.width: 1.5
                        border.color: "#031F2B"
                        QLato {
                            anchors.centerIn: parent
                            text: index + 1
                            font.pixelSize: 12
                        }
                    }
                    Column {
                        width: parent.width - 36
                        spacing: 4
                        QLato {
                            width: parent.width
                            text: modelData.title
                            font.weight: Font.Bold
                            horizontalAlignment: Text.AlignLeft
                        }
                        QLato {
                            width: parent.width
                            text: modelData.description
                            lineHeightMode: Text.FixedHeight
                            lineHeight: 20
                            wrapMode: Text.WordWrap
                            horizontalAlignment: Text.AlignLeft
                        }
                    }
                }
            }
            QLato {
                width: parent.width
                visible: _intro.footerText !== ""
                text: _intro.footerText
                lineHeightMode: Text.FixedHeight
                lineHeight: 20
                wrapMode: Text.WordWrap
                horizontalAlignment: Text.AlignLeft
            }
            Repeater {
                model: _intro.infoTexts
                QWarningBgMulti {
                    width: parent.width
                    icon: "qrc:/Images/Images/info-60px.svg"
                    txt.text: modelData
                }
            }
        }
    }

    bottomLeft: QTextButton {
        width: 80
        height: 48
        label.text: STR.STR_QML_059
        label.font.pixelSize: 16
        type: eTypeF
        onButtonClicked: _intro.prevClicked()
    }
    bottomRight: Row {
        spacing: 12
        QTextButton {
            width: label.paintedWidth + 2*20
            height: 48
            label.text: _intro.secondaryLabel
            label.font.pixelSize: 16
            type: eTypeJ
            onButtonClicked: _intro.keepCurrentClicked()
        }
        QTextButton {
            width: label.paintedWidth + 2*20
            height: 48
            label.text: _intro.primaryLabel
            label.font.pixelSize: 16
            type: eTypeE
            onButtonClicked: _intro.nextClicked()
        }
    }
}
